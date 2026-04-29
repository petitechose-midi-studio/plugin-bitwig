#include <exception>
#include <iostream>
#include <stdexcept>

#include "../../src/handler/NestedIndexUtils.hpp"

namespace {

void require(bool condition, const char* message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

void test_nested_display_index_maps_to_raw_index() {
    require(bitwig::utils::toRawIndex(0) == -1, "nested display index 0 should map to back button");
    require(bitwig::utils::toRawIndex(1) == 0, "nested display index 1 should map to raw index 0");
    require(bitwig::utils::toRawIndex(7) == 6, "nested display index 7 should map to raw index 6");

    std::cout << "[PASS] test_nested_display_index_maps_to_raw_index\n";
}

void test_flat_display_index_stays_unchanged() {
    require(bitwig::utils::toRawIndex(0, false) == 0, "flat display index 0 should stay raw index 0");
    require(bitwig::utils::toRawIndex(3, false) == 3, "flat display index 3 should stay raw index 3");
    require(bitwig::utils::toDisplayIndex(4, false) == 4, "flat raw index 4 should stay display index 4");

    std::cout << "[PASS] test_flat_display_index_stays_unchanged\n";
}

void test_raw_index_maps_to_nested_display_index() {
    require(bitwig::utils::toDisplayIndex(0) == 1, "nested raw index 0 should map to display index 1");
    require(bitwig::utils::toDisplayIndex(4) == 5, "nested raw index 4 should map to display index 5");

    std::cout << "[PASS] test_raw_index_maps_to_nested_display_index\n";
}

void test_back_button_is_display_index_zero() {
    require(bitwig::utils::isBackButton(0), "display index 0 should be the back button");
    require(!bitwig::utils::isBackButton(1), "display index 1 should not be the back button");

    std::cout << "[PASS] test_back_button_is_display_index_zero\n";
}

}  // namespace

int main() {
    try {
        test_nested_display_index_maps_to_raw_index();
        test_flat_display_index_stays_unchanged();
        test_raw_index_maps_to_nested_display_index();
        test_back_button_is_display_index_zero();
    } catch (const std::exception& error) {
        std::cerr << "[FAIL] " << error.what() << "\n";
        return 1;
    }

    std::cout << "All NestedIndexUtils tests passed\n";
    return 0;
}
