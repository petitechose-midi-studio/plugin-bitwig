from protocol import PrimitiveField, Type

# ============================================================================
# COLOR FIELDS (Generic)
# ============================================================================

# Color as 24-bit RGB hex value (0xRRGGBB)
# Compatible with LVGL lv_color_hex() format
# Range: 0x000000 (black) to 0xFFFFFF (white)
# Format: (R << 16) | (G << 8) | B where R, G, B are in [0, 255]
color_rgb = PrimitiveField('color', type_name=Type.UINT32)
