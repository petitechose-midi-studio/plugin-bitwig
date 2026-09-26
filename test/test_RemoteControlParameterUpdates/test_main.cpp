#include <array>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "../../src/state/ParameterState.hpp"
#include "../../src/protocol/ProtocolCallbacks.hpp"
#include "../../src/handler/host/RemoteControlHostHandler.cpp"

namespace {

using bitwig::handler::ParameterEncoderPort;
using bitwig::state::ParameterSlot;
using bitwig::state::ParameterState;

// ProtocolCallbacks keeps a protected constructor: it is a base class only.
struct TestProtocolCallbacks : Protocol::ProtocolCallbacks {};

void require(bool condition, const char* message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

void requireNear(float actual, float expected, const char* message) {
    if (std::fabs(actual - expected) > 1e-6f) {
        throw std::runtime_error(message);
    }
}

struct ConfigureCall {
    uint8_t index = 0;
    ParameterType type = ParameterType::KNOB;
    uint8_t discreteCount = 0;
    float value = 0.0f;
};

struct PositionCall {
    uint8_t index = 0;
    float value = 0.0f;
};

class RecordingParameterEncoderPort final : public ParameterEncoderPort {
public:
    void configure(uint8_t parameterIndex,
                   ParameterType type,
                   uint8_t discreteCount,
                   float value) override {
        configureCalls.push_back(ConfigureCall{parameterIndex, type, discreteCount, value});
    }

    void setPosition(uint8_t parameterIndex, float value) override {
        positionCalls.push_back(PositionCall{parameterIndex, value});
    }

    std::vector<ConfigureCall> configureCalls;
    std::vector<PositionCall> positionCalls;
};

struct ParameterUpdateFixture {
    ParameterState parameters;
    TestProtocolCallbacks callbacks;
    RecordingParameterEncoderPort encoders;
    bitwig::handler::RemoteControlHostHandler handler{parameters, callbacks, encoders};
};

Protocol::DeviceRemoteControlUpdateMessage makeUpdate(uint8_t index,
                                                      ParameterType type,
                                                      float value,
                                                      int16_t discreteCount) {
    Protocol::DeviceRemoteControlUpdateMessage message{};
    message.remoteControlIndex = index;
    message.parameterName = "Param";
    message.parameterValue = value;
    message.displayValue = "";
    message.parameterOrigin = 0.0f;
    message.parameterExists = true;
    message.parameterType = type;
    message.discreteValueCount = discreteCount;
    message.currentValueIndex = 0;
    message.hasAutomation = false;
    message.modulatedValue = value;
    return message;
}

Protocol::DeviceRemoteControlsBatchMessage makeBatch() {
    Protocol::DeviceRemoteControlsBatchMessage message{};
    message.sequenceNumber = 1;
    message.dirtyMask = 0;
    message.echoMask = 0;
    message.hasAutomationMask = 0;
    message.values.fill(0.0f);
    message.modulatedValues.fill(0.0f);
    for (auto& display : message.displayValues) {
        display.clear();
    }
    return message;
}

struct SlotSnapshot {
    float value = 0.0f;
    float origin = 0.0f;
    float modulationOffset = 0.0f;
    bool isModulated = false;
    bool hasAutomation = false;
    bool visible = true;
    bool loading = false;
    bool metadataSet = false;
    std::string name;
    std::string displayValue;
    ParameterType type = ParameterType::KNOB;
    int16_t discreteCount = -1;
    uint8_t currentValueIndex = 0;
};

SlotSnapshot snapshot(const ParameterSlot& slot) {
    SlotSnapshot state;
    state.value = slot.value.get();
    state.origin = slot.origin.get();
    state.modulationOffset = slot.modulationOffset.get();
    state.isModulated = slot.isModulated.get();
    state.hasAutomation = slot.hasAutomation.get();
    state.visible = slot.visible.get();
    state.loading = slot.loading.get();
    state.metadataSet = slot.metadataSet.get();
    state.name = slot.name.get();
    state.displayValue = slot.displayValue.get();
    state.type = slot.type.get();
    state.discreteCount = slot.discreteCount.get();
    state.currentValueIndex = slot.currentValueIndex.get();
    return state;
}

bool sameSnapshot(const SlotSnapshot& a, const SlotSnapshot& b) {
    return a.value == b.value && a.origin == b.origin &&
           a.modulationOffset == b.modulationOffset &&
           a.isModulated == b.isModulated && a.hasAutomation == b.hasAutomation &&
           a.visible == b.visible && a.loading == b.loading &&
           a.metadataSet == b.metadataSet && a.name == b.name &&
           a.displayValue == b.displayValue && a.type == b.type &&
           a.discreteCount == b.discreteCount &&
           a.currentValueIndex == b.currentValueIndex;
}

std::array<SlotSnapshot, bitwig::state::PARAMETER_COUNT> snapshotAll(
    const ParameterState& parameters) {
    std::array<SlotSnapshot, bitwig::state::PARAMETER_COUNT> snapshots;
    for (uint8_t i = 0; i < bitwig::state::PARAMETER_COUNT; ++i) {
        snapshots[i] = snapshot(parameters.slots[i]);
    }
    return snapshots;
}

void requireSlotsUnchanged(
    const std::array<SlotSnapshot, bitwig::state::PARAMETER_COUNT>& before,
    const ParameterState& parameters,
    const char* message) {
    for (uint8_t i = 0; i < bitwig::state::PARAMETER_COUNT; ++i) {
        require(sameSnapshot(before[i], snapshot(parameters.slots[i])), message);
    }
}

// ---------------------------------------------------------------------------
// Scenario 1: a valid update affects the expected slot only.
// ---------------------------------------------------------------------------
void test_valid_update_affects_expected_slot_only() {
    ParameterUpdateFixture fixture;

    // Seed slot 1 so "untouched" is distinguishable from "default".
    fixture.callbacks.onDeviceRemoteControlUpdate(
        makeUpdate(1, ParameterType::KNOB, 0.75f, -1));

    const auto before = snapshotAll(fixture.parameters);
    const size_t configureCountBefore = fixture.encoders.configureCalls.size();

    Protocol::DeviceRemoteControlUpdateMessage message =
        makeUpdate(3, ParameterType::LIST, 0.5f, 4);
    message.parameterName = "Cutoff";
    message.displayValue = "50 %";
    message.parameterOrigin = 0.25f;
    message.hasAutomation = true;
    message.currentValueIndex = 2;

    fixture.callbacks.onDeviceRemoteControlUpdate(message);

    const auto& updated = fixture.parameters.slots[3];
    requireNear(updated.value.get(), 0.5f, "updated slot value must match the message");
    require(std::string(updated.name.get()) == "Cutoff",
            "updated slot name must match the message");
    require(std::string(updated.displayValue.get()) == "50 %",
            "updated slot display value must match the message");
    requireNear(updated.origin.get(), 0.25f, "updated slot origin must match the message");
    require(updated.hasAutomation.get(), "updated slot automation flag must be set");
    require(updated.type.get() == ParameterType::LIST, "updated slot type must match the message");
    require(updated.discreteCount.get() == 4, "updated slot discrete count must match");
    require(updated.currentValueIndex.get() == 2, "updated slot index must match the message");
    require(updated.visible.get(), "updated slot must stay visible when the parameter exists");
    require(!updated.loading.get(), "updated slot must leave the loading state");
    require(updated.metadataSet.get() == before[3].metadataSet,
            "single updates must not change metadataSet");

    for (uint8_t i = 0; i < bitwig::state::PARAMETER_COUNT; ++i) {
        if (i == 3) continue;
        require(sameSnapshot(before[i], snapshot(fixture.parameters.slots[i])),
                "slots other than the message index must stay unchanged");
    }

    require(fixture.encoders.configureCalls.size() == configureCountBefore + 1,
            "a valid update must configure exactly one encoder");
    const auto& call = fixture.encoders.configureCalls.back();
    require(call.index == 3, "encoder configure must target the message index");
    require(call.type == ParameterType::LIST, "encoder configure must carry the parameter type");
    require(call.discreteCount == 4, "encoder configure must carry the discrete count");
    requireNear(call.value, 0.5f, "encoder configure must carry the parameter value");
    require(fixture.encoders.positionCalls.empty(),
            "single updates must not move the encoder directly");

    std::cout << "[PASS] test_valid_update_affects_expected_slot_only\n";
}

// ---------------------------------------------------------------------------
// Scenario 2: an invalid index writes to no slot and no encoder.
// ---------------------------------------------------------------------------
void test_invalid_index_writes_nowhere() {
    ParameterUpdateFixture fixture;
    fixture.callbacks.onDeviceRemoteControlUpdate(
        makeUpdate(0, ParameterType::KNOB, 0.25f, -1));

    const auto before = snapshotAll(fixture.parameters);
    const size_t configureCount = fixture.encoders.configureCalls.size();
    const size_t positionCount = fixture.encoders.positionCalls.size();

    fixture.callbacks.onDeviceRemoteControlUpdate(
        makeUpdate(9, ParameterType::KNOB, 0.9f, -1));

    Protocol::DeviceRemoteControlNameChangeMessage nameChange{};
    nameChange.remoteControlIndex = 9;
    nameChange.parameterName = "Out of range";
    fixture.callbacks.onDeviceRemoteControlNameChange(nameChange);

    Protocol::DeviceRemoteControlOriginChangeMessage originChange{};
    originChange.remoteControlIndex = 200;
    originChange.parameterOrigin = 1.0f;
    fixture.callbacks.onDeviceRemoteControlOriginChange(originChange);

    Protocol::DeviceRemoteControlDiscreteValuesMessage discreteValues{};
    discreteValues.remoteControlIndex = 9;
    discreteValues.discreteValueNames = {"A", "B"};
    discreteValues.currentValueIndex = 1;
    fixture.callbacks.onDeviceRemoteControlDiscreteValues(discreteValues);

    Protocol::DeviceRemoteControlIsModulatedChangeMessage modulatedChange{};
    modulatedChange.remoteControlIndex = 9;
    modulatedChange.isModulated = true;
    fixture.callbacks.onDeviceRemoteControlIsModulatedChange(modulatedChange);

    Protocol::RemoteControlValueStateMessage valueState{};
    valueState.remoteControlIndex = 9;
    valueState.parameterValue = 0.9f;
    valueState.displayValue = "90 %";
    fixture.callbacks.onRemoteControlValueState(valueState);

    requireSlotsUnchanged(before, fixture.parameters,
                          "invalid indices must not write to any parameter slot");
    require(fixture.encoders.configureCalls.size() == configureCount,
            "invalid indices must not configure an encoder");
    require(fixture.encoders.positionCalls.size() == positionCount,
            "invalid indices must not move an encoder");

    std::cout << "[PASS] test_invalid_index_writes_nowhere\n";
}

// ---------------------------------------------------------------------------
// Scenario 3: a partial batch respects masks and keeps value, modulation and
// display coherent.
//
// Characterization note: the modulation offset is recomputed from the value
// stored *before* this batch applies its dirty updates. The tests below pin
// that implemented ordering; whether it should use the post-update value is a
// product decision to confirm before changing it.
// ---------------------------------------------------------------------------
void test_partial_batch_respects_masks() {
    ParameterUpdateFixture fixture;
    fixture.callbacks.onDeviceRemoteControlUpdate(
        makeUpdate(2, ParameterType::KNOB, 0.4f, -1));
    fixture.callbacks.onDeviceRemoteControlUpdate(
        makeUpdate(5, ParameterType::LIST, 0.0f, 4));

    const auto before = snapshotAll(fixture.parameters);
    const size_t positionCount = fixture.encoders.positionCalls.size();

    Protocol::DeviceRemoteControlsBatchMessage batch = makeBatch();
    batch.sequenceNumber = 7;
    batch.dirtyMask = (1 << 2) | (1 << 5);
    batch.echoMask = (1 << 2);
    batch.hasAutomationMask = (1 << 5);
    batch.values[2] = 0.9f;
    batch.values[5] = 1.0f;
    batch.modulatedValues[0] = 0.3f;
    batch.modulatedValues[2] = 0.5f;
    batch.modulatedValues[5] = 0.75f;
    batch.displayValues[2] = "42 %";

    fixture.callbacks.onDeviceRemoteControlsBatch(batch);

    const auto& knob = fixture.parameters.slots[2];
    requireNear(knob.value.get(), 0.4f, "KNOB echoes must keep the optimistic value");
    require(std::string(knob.displayValue.get()) == "42 %",
            "KNOB echoes must still take the authoritative display value");
    requireNear(knob.modulationOffset.get(), 0.1f,
                "dirty slot modulation offset uses the pre-update value");
    require(!knob.hasAutomation.get(), "automation flags outside the mask stay cleared");

    const auto& list = fixture.parameters.slots[5];
    requireNear(list.value.get(), 1.0f,
                "LIST updates must apply the batch value even when echoed");
    require(list.currentValueIndex.get() == 3,
            "LIST updates must recompute the discrete index from the value");
    require(list.hasAutomation.get(), "automation flags inside the mask must apply");
    requireNear(list.modulationOffset.get(), 0.75f,
                "dirty slot modulation offset uses the pre-update value");

    requireNear(fixture.parameters.slots[0].modulationOffset.get(), 0.3f,
                "modulation offsets update for non-dirty slots as well");
    requireNear(fixture.parameters.slots[1].value.get(), before[1].value,
                "non-dirty slots must keep their value");
    requireNear(fixture.parameters.slots[1].modulationOffset.get(), -before[1].value,
                "non-dirty slots recompute the modulation offset from their value");
    require(std::string(fixture.parameters.slots[1].name.get()) == before[1].name,
            "non-dirty slots must keep their name");

    require(fixture.encoders.positionCalls.size() == positionCount + 1,
            "only the non-echo dirty slot may move an encoder");
    const auto& position = fixture.encoders.positionCalls.back();
    require(position.index == 5, "the moved encoder must be the non-echo dirty slot");
    requireNear(position.value, 1.0f, "the moved encoder must carry the batch value");

    std::cout << "[PASS] test_partial_batch_respects_masks\n";
}

// ---------------------------------------------------------------------------
// Scenario 3b: a LIST echo applies the value and skips the encoder position.
// ---------------------------------------------------------------------------
void test_list_echo_applies_value_without_moving_encoder() {
    ParameterUpdateFixture fixture;
    fixture.callbacks.onDeviceRemoteControlUpdate(
        makeUpdate(4, ParameterType::LIST, 0.0f, 4));

    const size_t positionCount = fixture.encoders.positionCalls.size();

    Protocol::DeviceRemoteControlsBatchMessage batch = makeBatch();
    batch.dirtyMask = (1 << 4);
    batch.echoMask = (1 << 4);
    batch.values[4] = 0.5f;

    fixture.callbacks.onDeviceRemoteControlsBatch(batch);

    requireNear(fixture.parameters.slots[4].value.get(), 0.5f,
                "LIST echoes must apply the incoming value");
    require(fixture.parameters.slots[4].currentValueIndex.get() == 2,
            "LIST echoes must recompute the discrete index");
    require(fixture.encoders.positionCalls.size() == positionCount,
            "LIST echoes must not move the encoder position");

    std::cout << "[PASS] test_list_echo_applies_value_without_moving_encoder\n";
}

// ---------------------------------------------------------------------------
// Value state confirmations move the encoder without touching other slots.
// ---------------------------------------------------------------------------
void test_value_state_moves_expected_encoder_only() {
    ParameterUpdateFixture fixture;
    fixture.callbacks.onDeviceRemoteControlUpdate(
        makeUpdate(6, ParameterType::KNOB, 0.2f, -1));

    const auto before = snapshotAll(fixture.parameters);
    const size_t positionCount = fixture.encoders.positionCalls.size();

    Protocol::RemoteControlValueStateMessage message{};
    message.remoteControlIndex = 6;
    message.parameterValue = 0.65f;
    message.displayValue = "65 %";
    fixture.callbacks.onRemoteControlValueState(message);

    requireNear(fixture.parameters.slots[6].value.get(), 0.65f,
                "value state must update the slot value");
    require(std::string(fixture.parameters.slots[6].displayValue.get()) == "65 %",
            "value state must update the display value");

    require(fixture.encoders.positionCalls.size() == positionCount + 1,
            "value state must add one encoder position call");
    const auto& position = fixture.encoders.positionCalls.back();
    require(position.index == 6, "value state must target the message index");
    requireNear(position.value, 0.65f, "value state must carry the message value");

    for (uint8_t i = 0; i < bitwig::state::PARAMETER_COUNT; ++i) {
        if (i == 6) continue;
        require(sameSnapshot(before[i], snapshot(fixture.parameters.slots[i])),
                "value state must not touch other slots");
    }

    std::cout << "[PASS] test_value_state_moves_expected_encoder_only\n";
}

}  // namespace

int main() {
    try {
        test_valid_update_affects_expected_slot_only();
        test_invalid_index_writes_nowhere();
        test_partial_batch_respects_masks();
        test_list_echo_applies_value_without_moving_encoder();
        test_value_state_moves_expected_encoder_only();
    } catch (const std::exception& error) {
        std::cerr << "[FAIL] " << error.what() << "\n";
        return 1;
    }

    std::cout << "All RemoteControlParameterUpdates tests passed\n";
    return 0;
}
