/**
 * Logger.hpp - Protocol Logging Utilities
 *
 * AUTO-GENERATED - DO NOT EDIT
 *
 * Provides utility functions for converting protocol messages to human-readable
 * YAML format for debugging via Serial USB.
 *
 * Key Features:
 * - floatToString(): Hybrid snprintf approach for precise float formatting
 * - g_logBuffer: Shared 32KB buffer for all toString() methods
 * - Handles edge cases: NaN, Inf, -Inf
 * - 4 decimal places precision
 * - Optimized for embedded systems (no dynamic allocation)
 */

#pragma once

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cmath>

namespace Protocol {

// ============================================================================
// Shared Log Buffer (used by all message toString() methods)
// ============================================================================

/// Size of the shared log buffer
constexpr size_t LOG_BUFFER_SIZE = 32768;

/// Shared buffer for toString() output - WARNING: not thread-safe!
/// All toString() calls share this buffer. Log immediately after calling.
#ifdef EXTMEM
inline EXTMEM char g_logBuffer[LOG_BUFFER_SIZE];
#else
inline char g_logBuffer[LOG_BUFFER_SIZE];
#endif

/**
 * Convert float to string with 4 decimal places using hybrid approach.
 *
 * Edge Cases Handled:
 * - NaN → "NaN"
 * - +Infinity → "Inf"
 * - -Infinity → "-Inf"
 * - Normal values → "123.4567" format
 *
 * Hybrid Algorithm:
 * 1. Use snprintf for integer part and formatting (reliable, fast)
 * 2. Manual extraction of 4 decimals with proper rounding
 * 3. Handle rounding overflow (e.g., 9.9999 → 10.0000)
 *
 * @param buffer Output buffer (must have at least 16 bytes)
 * @param bufferSize Size of output buffer
 * @param value Float value to convert
 * @return Number of characters written (excluding null terminator)
 *
 * Examples:
 *   3.14159 → "3.1416" (rounded)
 *   -0.5    → "-0.5000"
 *   0.0     → "0.0000"
 *   NaN     → "NaN"
 */
static inline int floatToString(char* buffer, size_t bufferSize, float value) {
    // Edge case 1: NaN (Not a Number)
    if (std::isnan(value)) {
        return snprintf(buffer, bufferSize, "NaN");
    }

    // Edge case 2: Infinity (positive or negative)
    if (std::isinf(value)) {
        return snprintf(buffer, bufferSize, value > 0 ? "Inf" : "-Inf");
    }

    // Handle negative numbers
    bool negative = value < 0;
    if (negative) {
        value = -value;  // Work with absolute value
    }

    // Extract integer part
    int32_t intPart = (int32_t)value;

    // Extract 4 decimal places with rounding
    // Multiply by 10000 to shift 4 decimals to integer range
    // Add 0.5 for proper rounding (banker's rounding)
    int32_t fracPart = (int32_t)((value - intPart) * 10000.0f + 0.5f);

    // Handle rounding overflow
    // Example: 9.99995 → intPart=9, fracPart=10000 → should become 10.0000
    if (fracPart >= 10000) {
        intPart++;
        fracPart = 0;
    }

    // Use snprintf for final formatting
    // Format: [sign]integer.decimal (with leading zeros on decimal)
    // Use %ld for portability (int32_t may be long on some platforms)
    return snprintf(buffer, bufferSize, "%s%ld.%04ld",
                    negative ? "-" : "",  // Add negative sign if needed
                    (long)intPart,        // Integer part (cast for portability)
                    (long)fracPart);      // Fractional part (4 digits, zero-padded)
}

}  // namespace Protocol
