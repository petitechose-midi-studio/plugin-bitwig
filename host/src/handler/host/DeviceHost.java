package handler.host;

import com.bitwig.extension.controller.api.*;
import protocol.Protocol;
import protocol.struct.DeviceChangeHeaderMessage;
import protocol.struct.DeviceChildrenMessage;
import protocol.struct.DeviceListWindowMessage;
import protocol.struct.DevicePageChangeMessage;
import config.BitwigConfig;
import util.DeviceTypeUtils;
import java.util.List;
import java.util.ArrayList;
import handler.controller.DeviceController;

/**
 * DeviceHost - Observes Bitwig Device and sends updates TO controller
 *
 * RESPONSIBILITIES:
 * 1. Observe device changes (name, enabled, parameters) → send protocol messages
 * 2. Send device lists/children on request
 * 3. Manage parameter echo suppression (via DeviceController)
 *
 * DELEGATES TO:
 * - DeviceNavigator: Hierarchical navigation (slots/layers/drums)
 */
public class DeviceHost {
    private final ControllerHost host;
    private final Protocol protocol;
    private final CursorTrack cursorTrack;
    private final CursorDevice cursorDevice;
    private final CursorRemoteControlsPage remoteControls;
    private final DeviceBank deviceBank;

    private DeviceLayerBank layerBank;
    private DrumPadBank drumPadBank;
    private DeviceController deviceController;
    private DeviceNavigator navigator;

    private String lastDeviceName = "";
    private int lastPageIndex = -1;

    // Prevent duplicate calls during transitions
    private boolean deviceChangePending = false;  // Ignores individual param messages during device change
    private boolean pageChangePending = false;  // Debounce for sendPageChange
    private boolean selectorRequestActive = false; // Pause all value/modulation sends during selector requests

    // View state tracking (from controller VIEW_STATE_CHANGE message)
    // viewType: 0=REMOTE_CONTROLS, 1=MIX, 2=CLIP
    private int controllerViewType = 0;
    private boolean controllerSelectorActive = false;

    // Combined batch for values + modulated values (single synchronized update)
    private static final int BATCH_INTERVAL_MS = 1  ; // ~66Hz batch rate (15ms = imperceptible for UI)
    private final float[] modulatedValues = new float[BitwigConfig.MAX_PARAMETERS];
    private final float[] parameterValues = new float[BitwigConfig.MAX_PARAMETERS];
    private final float[] pendingValues = new float[BitwigConfig.MAX_PARAMETERS];
    private final String[] pendingDisplayValues = new String[BitwigConfig.MAX_PARAMETERS];
    private final boolean[] previousIsModulated = new boolean[BitwigConfig.MAX_PARAMETERS];
    private final boolean[] hasAutomationState = new boolean[BitwigConfig.MAX_PARAMETERS];
    private int valuesDirtyMask = 0;  // Bit mask: which parameter values changed
    private int valuesEchoMask = 0;   // Bit mask: which parameters are echoes
    private boolean batchDirty = false; // True if any value or modulated value changed

    // Pre-allocated String array for display values (avoid null in message)
    private final String[] batchDisplayValues = new String[BitwigConfig.MAX_PARAMETERS];

    public DeviceHost(
        ControllerHost host,
        Protocol protocol,
        CursorTrack cursorTrack,
        CursorDevice cursorDevice,
        CursorRemoteControlsPage remoteControls,
        DeviceBank deviceBank
    ) {
        this.host = host;
        this.protocol = protocol;
        this.cursorTrack = cursorTrack;
        this.cursorDevice = cursorDevice;
        this.remoteControls = remoteControls;
        this.deviceBank = deviceBank;
    }

    public void setupObservers() {
        // Create banks
        layerBank = cursorDevice.createLayerBank(BitwigConfig.MAX_CHILDREN);
        drumPadBank = cursorDevice.createDrumPadBank(BitwigConfig.MAX_CHILDREN);

        // Create navigator (needs banks)
        navigator = new DeviceNavigator(host, cursorDevice, deviceBank, layerBank);

        // Mark interested + add observers by domain
        setupCursorTrackObservables();
        setupCursorDeviceObservables();
        setupRemoteControlsObservables();
        setupDeviceBankObservables();
        setupLayerAndDrumBanksObservables();

        // Start combined batch timer
        startBatchTimer();
    }

    /**
     * Check if isModulated state changed and send granular message if it did.
     * isModulated = hasAutomation || (modulatedValue != value)
     */
    private void checkAndSendIsModulatedChange(int paramIndex) {
        boolean hasAutomation = hasAutomationState[paramIndex];
        float value = parameterValues[paramIndex];
        float modulatedValue = modulatedValues[paramIndex];
        boolean isModulated = hasAutomation || Math.abs(modulatedValue - value) > 0.001f;

        if (isModulated != previousIsModulated[paramIndex]) {
            previousIsModulated[paramIndex] = isModulated;
            protocol.deviceRemoteControlIsModulatedChange(paramIndex, isModulated);
        }
    }

    /**
     * Start the combined batch timer.
     * Sends values and modulated values in a single synchronized message.
     */
    private void startBatchTimer() {
        host.scheduleTask(this::batchTick, BATCH_INTERVAL_MS);
    }

    /**
     * Combined batch tick: send all values and modulated values in one message.
     * This ensures perfect synchronization between value and modulation display.
     */
    private void batchTick() {
        // Reschedule for next tick
        host.scheduleTask(this::batchTick, BATCH_INTERVAL_MS);

        // Skip if not on RemoteControls view or selector is open
        if (controllerViewType != 0 || controllerSelectorActive) return;
        if (deviceChangePending || selectorRequestActive) return;

        // Only send if something changed
        if (!batchDirty) return;
        batchDirty = false;

        // Build display values array (replace nulls with empty strings)
        // Build hasAutomationMask from hasAutomationState array
        int hasAutomationMask = 0;
        for (int i = 0; i < BitwigConfig.MAX_PARAMETERS; i++) {
            batchDisplayValues[i] = pendingDisplayValues[i] != null ? pendingDisplayValues[i] : "";
            if (hasAutomationState[i]) {
                hasAutomationMask |= (1 << i);
            }
        }

        // Send single combined message - zero allocation (arrays passed directly)
        protocol.deviceRemoteControlsBatch(0, valuesDirtyMask, valuesEchoMask, hasAutomationMask, pendingValues, modulatedValues, batchDisplayValues);

        // Reset masks
        valuesDirtyMask = 0;
        valuesEchoMask = 0;

        // Check isModulated state for all parameters
        for (int i = 0; i < BitwigConfig.MAX_PARAMETERS; i++) {
            checkAndSendIsModulatedChange(i);
        }
    }

    private void setupCursorTrackObservables() {
        cursorTrack.name().markInterested();

        // Reset device name cache when track changes, so device observer will trigger sendDeviceChange
        // Note: TrackHost now handles TrackChangeMessage (name, color, position, type)
        cursorTrack.name().addValueObserver(trackName -> {
            lastDeviceName = "";  // Reset so device observer will trigger sendDeviceChange
        });
    }

    private void setupCursorDeviceObservables() {
        cursorDevice.exists().markInterested();
        cursorDevice.name().markInterested();
        cursorDevice.isEnabled().markInterested();
        cursorDevice.position().markInterested();
        cursorDevice.deviceChain().name().markInterested();
        cursorDevice.isNested().markInterested();
        cursorDevice.hasSlots().markInterested();
        cursorDevice.slotNames().markInterested();
        cursorDevice.hasLayers().markInterested();
        cursorDevice.hasDrumPads().markInterested();
        cursorDevice.deviceType().markInterested();

        // Single observer for device changes - name() is triggered for both new devices and device switches
        cursorDevice.name().addValueObserver(deviceName -> {
            boolean exists = cursorDevice.exists().get();

            if (!exists) {
                // Device cleared
                lastDeviceName = "";
                sendDeviceCleared();
            } else if (!deviceName.equals(lastDeviceName)) {
                // New or different device
                lastDeviceName = deviceName;
                sendDeviceChange();
            }
        });

        // Observer for device selection changes (focus moved to different device in chain)
        // Updates device list with new activeDeviceIndex for DeviceSelector sync
        cursorDevice.position().addValueObserver(position -> {
            if (cursorDevice.exists().get()) {
                sendDeviceListWindow(0);
            }
        });

        // Observer for device existence changes
        // Handles: 0→1 device (send device info) and 1→0 device (send "No Device")
        cursorDevice.exists().addValueObserver(exists -> {
            if (exists) {
                lastDeviceName = cursorDevice.name().get();
                sendDeviceChange();
                sendDeviceListWindow(0);
            } else {
                // Device no longer exists (switched to empty track or last device deleted)
                lastDeviceName = "";
                sendDeviceCleared();
            }
        });
    }

    private void setupRemoteControlsObservables() {
        remoteControls.selectedPageIndex().markInterested();
        remoteControls.pageCount().markInterested();
        remoteControls.getName().markInterested();
        remoteControls.pageNames().markInterested();

        // Parameters
        for (int i = 0; i < BitwigConfig.MAX_PARAMETERS; i++) {
            markParameterInterested(remoteControls.getParameter(i));
        }

        // Page change observer - debounced, skip if device change will handle it
        remoteControls.selectedPageIndex().addValueObserver(pageIndex -> {
            if (pageIndex != lastPageIndex) {
                lastPageIndex = pageIndex;
                if (!deviceChangePending && !pageChangePending) {
                    pageChangePending = true;
                    host.scheduleTask(() -> {
                        pageChangePending = false;
                        if (!deviceChangePending) {
                            sendPageChange();
                        }
                    }, BitwigConfig.PAGE_CHANGE_MS);
                }
            }
        });

        // Parameter observers - skip during device transitions (batch message handles it)
        for (int i = 0; i < BitwigConfig.MAX_PARAMETERS; i++) {
            final int paramIndex = i;
            RemoteControl param = remoteControls.getParameter(i);

            param.value().displayedValue().addValueObserver(displayValue -> {
                if (deviceChangePending || selectorRequestActive)
                    return; // Skip during transitions or selector requests

                float value = (float) param.value().get();
                parameterValues[paramIndex] = value; // Track for isModulated detection
                pendingValues[paramIndex] = value;   // Store for batch
                pendingDisplayValues[paramIndex] = displayValue; // Store display value for batch
                valuesDirtyMask |= (1 << paramIndex); // Mark value as changed
                batchDirty = true;  // Mark batch for sending

                // Check if this is an echo from controller
                if (deviceController != null && deviceController.consumeEcho(paramIndex)) {
                    valuesEchoMask |= (1 << paramIndex);
                }
            });

            param.name().addValueObserver(name -> {
                if (deviceChangePending) return;  // Skip - DevicePageChangeMessage will contain all names
                protocol.deviceRemoteControlNameChange(paramIndex, name);
            });

            // Automation state observer - updates hasAutomationMask in batch
            param.hasAutomation().addValueObserver(hasAutomation -> {
                if (deviceChangePending) return;  // Skip - DevicePageChangeMessage will contain automation state
                hasAutomationState[paramIndex] = hasAutomation; // Track for batch mask
                batchDirty = true;  // Ensure batch is sent with updated mask
                // Check isModulated immediately (automation changes are rare)
                checkAndSendIsModulatedChange(paramIndex);
            });

            param.modulatedValue().addValueObserver(modulatedValue -> {
                if (deviceChangePending) return;  // Skip - DevicePageChangeMessage will contain modulated values
                modulatedValues[paramIndex] = (float) modulatedValue;
                batchDirty = true;  // Mark for next batch tick
            });

            // Origin observer - sends lightweight origin-only message
            param.value().getOrigin().addValueObserver(origin -> {
                if (deviceChangePending) return;  // Skip - DevicePageChangeMessage will contain origin
                protocol.deviceRemoteControlOriginChange(paramIndex, (float) origin);
            });
        }
    }

    private void setupDeviceBankObservables() {
        deviceBank.itemCount().markInterested();
        deviceBank.scrollPosition().markInterested();
        deviceBank.canScrollBackwards().markInterested();
        deviceBank.canScrollForwards().markInterested();

        // Observer for device count changes (add/remove device)
        deviceBank.itemCount().addValueObserver(count -> {
            sendDeviceListWindow(0);  // Use windowed loading
        });

        // All devices in bank
        for (int i = 0; i < BitwigConfig.MAX_BANK_SIZE; i++) {
            final int deviceIndex = i;
            Device device = deviceBank.getItemAt(i);
            markDeviceInterested(device);

            // Device enabled state observer - sends notification to controller
            device.isEnabled().addValueObserver(isEnabled -> {
                if (device.exists().get()) {
                    protocol.deviceEnabledState(deviceIndex, isEnabled);
                }
            });
        }
    }

    private void setupLayerAndDrumBanksObservables() {
        layerBank.itemCount().markInterested();
        layerBank.scrollPosition().markInterested();
        drumPadBank.scrollPosition().markInterested();
        drumPadBank.canScrollBackwards().markInterested();
        drumPadBank.canScrollForwards().markInterested();

        for (int i = 0; i < BitwigConfig.MAX_CHILDREN; i++) {
            markLayerInterested(layerBank.getItemAt(i));
            markDrumPadInterested(drumPadBank.getItemAt(i));
        }
    }

    /**
     * Send current device state (called at startup)
     * Note: TrackHost now handles TrackChangeMessage
     */
    public void sendInitialState() {
        // Delay to let Bitwig API initialize values
        host.scheduleTask(() -> {
            sendDeviceChange();
            sendDeviceListWindow(0);
        }, BitwigConfig.DEVICE_CHANGE_HEADER_MS);
    }

    public void setDeviceController(DeviceController deviceController) {
        this.deviceController = deviceController;
    }

    /**
     * Update controller view state (from VIEW_STATE_CHANGE message).
     * Controls whether modulated values batch is sent.
     *
     * @param viewType       0=REMOTE_CONTROLS, 1=MIX, 2=CLIP
     * @param selectorActive true if any selector/overlay is open
     */
    public void setControllerViewState(int viewType, boolean selectorActive) {
        this.controllerViewType = viewType;
        this.controllerSelectorActive = selectorActive;
    }

    /**
     * Schedule action after device bank update delay.
     * Used by TrackController when selecting track to auto-select first device.
     */
    public void scheduleAfterDeviceBankUpdate(Runnable action) {
        host.scheduleTask(action, BitwigConfig.DEVICE_ENTER_CHILD_MS);
    }

    private static final int PAGE_WINDOW_SIZE = 16;

    /**
     * Send windowed page names starting at requestedStartIndex.
     * Implements lazy-loading pattern: sends 16 items per request.
     * Clamps startIndex if out of range.
     *
     * @param requestedStartIndex Start index requested by controller
     */
    public void sendPageNamesWindow(int requestedStartIndex) {
        // Pause value/modulation sends to free bandwidth for selector response
        selectorRequestActive = true;

        host.scheduleTask(() -> {
            final int totalCount = remoteControls.pageCount().get();
            final int currentIndex = remoteControls.selectedPageIndex().get();
            final String[] pageNamesArray = remoteControls.pageNames().get();

            // Clamp startIndex if out of range
            int startIndex = requestedStartIndex;
            if (startIndex >= totalCount) {
                startIndex = Math.max(0, totalCount - PAGE_WINDOW_SIZE);
            }
            if (startIndex < 0) {
                startIndex = 0;
            }

            // Build window of up to 16 items (protocol encodes count prefix)
            int windowSize = Math.min(PAGE_WINDOW_SIZE, totalCount - startIndex);
            final String[] windowNames = new String[windowSize];
            for (int i = 0; i < windowSize; i++) {
                int idx = startIndex + i;
                if (pageNamesArray != null && idx < pageNamesArray.length) {
                    windowNames[i] = pageNamesArray[idx];
                } else {
                    windowNames[i] = "";
                }
            }

            protocol.devicePageNamesWindow(
                totalCount,     // Total pages (absolute)
                startIndex,     // Actual start index (may be clamped)
                currentIndex,   // Currently selected page
                windowNames     // This window's page names (up to 16)
            );

            // Resume value/modulation sends after response sent
            selectorRequestActive = false;
        }, BitwigConfig.DEVICE_ENTER_CHILD_MS);
    }

    /**
     * Send a windowed portion of the device list (16 items max per window).
     * Used for lazy-loading large device chains.
     *
     * @param requestedStartIndex The starting index requested by the controller
     */
    public void sendDeviceListWindow(int requestedStartIndex) {
        // Pause value/modulation sends to free bandwidth for selector response
        selectorRequestActive = true;

        host.scheduleTask(() -> {
            final int totalDeviceCount = deviceBank.itemCount().get();
            final int currentDevicePosition = cursorDevice.position().get();
            final boolean isNested = cursorDevice.isNested().get();
            final String parentName = isNested ? cursorDevice.deviceChain().name().get() : "";

            // Clamp startIndex if out of range
            int startIndex = requestedStartIndex;
            if (startIndex >= totalDeviceCount) {
                startIndex = Math.max(0, totalDeviceCount - BitwigConfig.LIST_WINDOW_SIZE);
            }
            if (startIndex < 0) {
                startIndex = 0;
            }

            // Build windowed device list
            final DeviceListWindowMessage.Devices[] windowDevices = buildDevicesListWindow(startIndex);

            protocol.deviceListWindow(
                totalDeviceCount,
                startIndex,
                currentDevicePosition,
                isNested,
                parentName,
                windowDevices
            );

            // Resume value/modulation sends after response sent
            selectorRequestActive = false;
        }, BitwigConfig.DEVICE_ENTER_CHILD_MS);
    }

    private DeviceListWindowMessage.Devices[] buildDevicesListWindow(int startIndex) {
        List<DeviceListWindowMessage.Devices> list = new ArrayList<>();
        int bankSize = deviceBank.getSizeOfBank();
        int endIndex = Math.min(startIndex + BitwigConfig.LIST_WINDOW_SIZE, bankSize);

        for (int i = startIndex; i < endIndex; i++) {
            Device device = deviceBank.getItemAt(i);
            if (!device.exists().get()) continue;

            String deviceTypeRaw = device.deviceType().get();
            int deviceTypeInt = DeviceTypeUtils.toInt(deviceTypeRaw);

            list.add(new DeviceListWindowMessage.Devices(
                device.position().get(),
                device.name().get(),
                device.isEnabled().get(),
                deviceTypeInt,
                getDeviceChildrenTypes(device)
            ));
        }

        return list.toArray(new DeviceListWindowMessage.Devices[0]);
    }

    private int[] getDeviceChildrenTypes(Device device) {
        int[] types = new int[4];
        int index = 0;
        if (device.hasSlots().get() && index < 4) types[index++] = 1;
        if (device.hasLayers().get() && index < 4) types[index++] = 2;
        if (device.hasDrumPads().get() && index < 4) types[index++] = 4;
        return types;
    }

    public void sendDeviceChildren(int deviceIndex, int childType) {
        // Delay to let Bitwig API update values
        host.scheduleTask(() -> {
            final Device device = deviceBank.getItemAt(deviceIndex);
            if (!device.exists().get()) {
                protocol.deviceChildren(deviceIndex, 0, 0, new DeviceChildrenMessage.Children[0]);
                return;
            }

            // Build flat list of ALL children (slots + layers OR drums)
            final List<DeviceChildrenMessage.Children> allChildren = new ArrayList<>();

            // Add slots first (itemType=0)
            for (DeviceChildrenMessage.Children slot : getSlots(device)) {
                if (allChildren.size() >= BitwigConfig.MAX_CHILDREN) break;
                allChildren.add(slot);
            }

            // Get drum pads first to check if this is a drum device
            List<DeviceChildrenMessage.Children> drumPads = getAllDrumPads(device);
            boolean hasDrumPads = !drumPads.isEmpty();

            // Add layers only if NOT a drum device (layers duplicate drum pads)
            if (!hasDrumPads) {
                for (DeviceChildrenMessage.Children layer : getLayers(device)) {
                    if (allChildren.size() >= BitwigConfig.MAX_CHILDREN) break;
                    allChildren.add(layer);
                }
            }

            // Add drum pads (itemType=2)
            for (DeviceChildrenMessage.Children drum : drumPads) {
                if (allChildren.size() >= BitwigConfig.MAX_CHILDREN) break;
                allChildren.add(drum);
            }

            protocol.deviceChildren(deviceIndex, 0, allChildren.size(), allChildren.toArray(new DeviceChildrenMessage.Children[0]));
        }, BitwigConfig.DEVICE_ENTER_CHILD_MS);
    }

    /**
     * Execute device navigation: enter child (slot/layer/drum)
     * Delegates to DeviceNavigator
     */
    public void enterDeviceChild(int deviceIndex, int itemType, int childIndex) {
        navigator.enterDeviceChild(host, deviceIndex, itemType, childIndex);
    }

    /**
     * Execute device navigation: exit to parent
     * Delegates to DeviceNavigator
     */
    public void exitToParent() {
        navigator.exitToParent();
    }

    private List<DeviceChildrenMessage.Children> getSlots(Device device) {
        List<DeviceChildrenMessage.Children> slots = new ArrayList<>();
        String[] slotNames = device.slotNames().get();

        if (slotNames != null) {
            for (int i = 0; i < slotNames.length && i < BitwigConfig.MAX_CHILDREN; i++) {
                slots.add(new DeviceChildrenMessage.Children(i, slotNames[i], 0)); // itemType=0 for Slot
            }
        }

        return slots;
    }

    private List<DeviceChildrenMessage.Children> getLayers(Device device) {
        List<DeviceChildrenMessage.Children> layers = new ArrayList<>();

        // Limit layers max, only active ones
        for (int i = 0; i < BitwigConfig.MAX_CHILDREN; i++) {
            DeviceLayer layer = layerBank.getItemAt(i);
            if (layer.exists().get()) {
                layers.add(new DeviceChildrenMessage.Children(i, layer.name().get(), 1)); // itemType=1 for Layer
            }
        }

        return layers;
    }

    private List<DeviceChildrenMessage.Children> getAllDrumPads(Device device) {
        List<DeviceChildrenMessage.Children> allPads = new ArrayList<>();
        int scrollPos = drumPadBank.scrollPosition().get();

        // Simple iteration like layers - no async scroll operations
        for (int i = 0; i < BitwigConfig.MAX_CHILDREN; i++) {
            DrumPad pad = drumPadBank.getItemAt(i);
            if (pad.exists().get()) {
                int midiNote = scrollPos + i;
                String padName = pad.name().get() + " (" + getMidiNoteName(midiNote) + ")";
                allPads.add(new DeviceChildrenMessage.Children(midiNote, padName, 2)); // itemType=2 for DrumPad
            }
        }

        return allPads;
    }

    private static final String[] MIDI_NOTE_NAMES = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

    private String getMidiNoteName(int midiNote) {
        int octave = (midiNote / 12) - 1;
        int noteIndex = midiNote % 12;
        return MIDI_NOTE_NAMES[noteIndex] + octave;
    }

    private void sendDeviceChange() {
        // Block individual observers - batch message will contain everything
        deviceChangePending = true;

        // Delay all reads to let Bitwig API update hasSlots/hasLayers/hasDrumPads
        // Uses longer delay (100ms) because child type properties populate slower than name/state
        host.scheduleTask(() -> {
            // Guard: if device was deleted during delay, don't send empty state
            // (sendDeviceCleared handles the "No Device" case separately)
            if (!cursorDevice.exists().get()) {
                deviceChangePending = false;
                return;
            }

            String deviceName = cursorDevice.name().get();
            boolean isEnabled = cursorDevice.isEnabled().get();
            String deviceTypeRaw = cursorDevice.deviceType().get();
            int deviceType = DeviceTypeUtils.toInt(deviceTypeRaw);
            int pageIndex = remoteControls.selectedPageIndex().get();
            int pageCount = remoteControls.pageCount().get();
            String pageName = getPageName(pageIndex, pageCount);
            int[] childrenTypes = getDeviceChildrenTypes(cursorDevice);

            sendDeviceChangeHeader(deviceName, isEnabled, deviceType, pageIndex, pageCount, pageName, childrenTypes);
            sendPageChange();
        }, BitwigConfig.DEVICE_CHANGE_HEADER_MS);
    }

    private void sendDeviceChangeHeader(String deviceName, boolean isEnabled, int deviceType, int pageIndex, int pageCount, String pageName, int[] childrenTypes) {
        protocol.deviceChangeHeader(deviceName, isEnabled, deviceType, new DeviceChangeHeaderMessage.PageInfo(pageIndex, pageCount, pageName), childrenTypes);
    }

    private void sendDeviceCleared() {
        // Block individual observers during clear
        deviceChangePending = true;

        // Send "No Device" state to controller
        host.scheduleTask(() -> {
            // Send header with "No Device" and empty state
            protocol.deviceChangeHeader(
                "No Device",           // deviceName
                false,                 // isEnabled
                0,                     // deviceType (UNKNOWN)
                new DeviceChangeHeaderMessage.PageInfo(0, 0, ""),  // empty page info
                new int[]{0, 0, 0, 0}  // no children types
            );

            // Clear all parameters (mark as not visible)
            for (int i = 0; i < BitwigConfig.MAX_PARAMETERS; i++) {
                protocol.deviceRemoteControlUpdate(
                    i,                 // remoteControlIndex
                    "",                // parameterName
                    0.0f,              // parameterValue
                    "",                // displayValue
                    0.0f,              // parameterOrigin
                    false,             // parameterExists (NOT visible)
                    0,                 // parameterType
                    (short) 0,         // discreteValueCount
                    0,                 // currentValueIndex
                    false,             // hasAutomation
                    0.0f               // modulatedValue
                );
            }

            // Resume observers
            deviceChangePending = false;
        }, BitwigConfig.STANDARD_DELAY_MS);
    }

    private void sendPageChange() {
        final int pageIndex = remoteControls.selectedPageIndex().get();
        final int pageCount = remoteControls.pageCount().get();
        final String pageName = remoteControls.getName().get();
        final List<DevicePageChangeMessage.RemoteControls> remoteControlsList = buildRemoteControlsListForPageChange();

        protocol.devicePageChange(
            new DevicePageChangeMessage.PageInfo(pageIndex, pageCount, pageName),
            remoteControlsList.toArray(new DevicePageChangeMessage.RemoteControls[0])
        );

        // Ensure batch is sent with updated hasAutomationMask
        batchDirty = true;

        // Resume individual observers
        deviceChangePending = false;
    }

    private String getPageName(int pageIndex, int pageCount) {
        if (pageCount > 0 && pageIndex >= 0 && pageIndex < pageCount) {
            String[] names = remoteControls.pageNames().get();
            if (names != null && pageIndex < names.length) {
                return names[pageIndex];
            }
        }
        return "";
    }

    private List<DevicePageChangeMessage.RemoteControls> buildRemoteControlsListForPageChange() {
        List<DevicePageChangeMessage.RemoteControls> remoteControlsList = new ArrayList<>();
        for (int i = 0; i < BitwigConfig.MAX_PARAMETERS; i++) {
            RemoteControl param = remoteControls.getParameter(i);
            ParameterData data = captureParameterData(param, i);

            // Sync tracking arrays for throttled observers
            parameterValues[i] = data.value;
            modulatedValues[i] = data.modulatedValue;
            pendingValues[i] = data.value;
            hasAutomationState[i] = data.hasAutomation;
            previousIsModulated[i] = data.isModulated;

            remoteControlsList.add(new DevicePageChangeMessage.RemoteControls(
                i,
                data.value,
                data.name,
                data.origin,
                data.exists,
                (short) data.discreteCount,
                data.displayedValue,
                data.typeInfo.parameterType,
                data.typeInfo.discreteValueNames,
                data.typeInfo.currentValueIndex,
                data.hasAutomation,
                    data.modulatedValue,
                    data.isModulated
            ));
        }
        return remoteControlsList;
    }

    private static class ParameterTypeInfo {
        final int parameterType;
        final String[] discreteValueNames;
        final int currentValueIndex;

        ParameterTypeInfo(int parameterType, String[] discreteValueNames, int currentValueIndex) {
            this.parameterType = parameterType;
            this.discreteValueNames = discreteValueNames;
            this.currentValueIndex = currentValueIndex;
        }
    }

    private static class ParameterData {
        final String name;
        final float value;
        final String displayedValue;
        final float origin;
        final boolean exists;
        final int discreteCount;
        final ParameterTypeInfo typeInfo;
        final boolean hasAutomation;
        final float modulatedValue;
        final boolean isModulated;

        ParameterData(String name, float value, String displayedValue, float origin,
                     boolean exists, int discreteCount, ParameterTypeInfo typeInfo,
                boolean hasAutomation, float modulatedValue, boolean isModulated) {
            this.name = name;
            this.value = value;
            this.displayedValue = displayedValue;
            this.origin = origin;
            this.exists = exists;
            this.discreteCount = discreteCount;
            this.typeInfo = typeInfo;
            this.hasAutomation = hasAutomation;
            this.modulatedValue = modulatedValue;
            this.isModulated = isModulated;
        }
    }

    private ParameterData captureParameterData(RemoteControl param, int paramIndex) {
        int discreteCount = param.value().discreteValueCount().get();
        String name = param.name().get();
        float value = (float) param.value().get();
        String displayedValue = param.value().displayedValue().get();
        float origin = (float) param.value().getOrigin().get();
        boolean exists = param.exists().get();
        boolean hasAutomation = param.hasAutomation().get();
        float modulatedValue = (float) param.modulatedValue().get();

        // isModulated: true if hasAutomation or if modulatedValue differs from value
        // This indicates a modulation source is active (automation, LFO, envelope,
        // etc.)
        boolean isModulated = hasAutomation || Math.abs(modulatedValue - value) > 0.001f;

        if (deviceController != null) {
            deviceController.updateParameterMetadata(paramIndex, discreteCount);
        }

        ParameterTypeInfo typeInfo = detectParameterType(param, discreteCount);

        return new ParameterData(name, value, displayedValue, origin, exists, discreteCount, typeInfo, hasAutomation,
                modulatedValue, isModulated);
    }

    private ParameterTypeInfo detectParameterType(RemoteControl param, int discreteCount) {
        String[] discreteNames = param.value().discreteValueNames().get();
        if (discreteNames == null) discreteNames = new String[0];

        int currentIndex = 0;
        if (discreteCount > 0) {
            currentIndex = (int) Math.round(param.value().get() * (discreteCount - 1));
            currentIndex = Math.max(0, Math.min(currentIndex, discreteCount - 1));
        }

        int parameterType = 0;
        if (discreteCount == 2 && discreteNames.length >= 2) {
            String name0 = discreteNames[0].toLowerCase().trim();
            String name1 = discreteNames[1].toLowerCase().trim();
            boolean isButton = (name0.equals("off") && name1.equals("on")) ||
                               (name0.equals("disabled") && name1.equals("enabled")) ||
                               (name0.equals("no") && name1.equals("yes")) ||
                               (name0.equals("false") && name1.equals("true"));
            parameterType = isButton ? 1 : 2;
        } else if (discreteCount > 2) {
            parameterType = 2;
        }

        return new ParameterTypeInfo(parameterType, discreteNames, currentIndex);
    }

    private void markLayerInterested(DeviceLayer layer) {
        layer.exists().markInterested();
        layer.name().markInterested();
    }

    private void markDrumPadInterested(DrumPad pad) {
        pad.exists().markInterested();
        pad.name().markInterested();
    }

    private void markDeviceInterested(Device device) {
        device.exists().markInterested();
        device.name().markInterested();
        device.position().markInterested();
        device.isEnabled().markInterested();
        device.deviceType().markInterested();
        device.hasSlots().markInterested();
        device.hasLayers().markInterested();
        device.hasDrumPads().markInterested();
        device.slotNames().markInterested();
    }

    private void markParameterInterested(RemoteControl param) {
        param.exists().markInterested();
        param.value().markInterested();
        param.name().markInterested();
        param.value().getOrigin().markInterested();
        param.hasAutomation().markInterested();
        param.modulatedValue().markInterested();
        param.value().discreteValueCount().markInterested();
        param.value().displayedValue().markInterested();
        param.value().discreteValueNames().markInterested();
    }
}
