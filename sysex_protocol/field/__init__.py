"""
Field Categories - Auto-Discovery

This module automatically discovers and loads all field category modules
in the field/ directory. Each category module (e.g., transport.py, parameter.py)
defines reusable field instances.

ARCHITECTURE:
- Auto-discovery: All .py files in field/ are automatically imported
- Centralized registry: ALL_FIELDS contains all fields from all categories
- DRY principle: Define fields once, use in multiple messages

ADDING A NEW CATEGORY:
1. Create field/mycategory.py
2. Define fields using Field instances (e.g., my_field = Field('name', Type.UINT8))
3. That's it! The module is auto-discovered and loaded

USAGE:
    # Import specific fields
    from field.transport import transport_play

    # Or import all fields (useful for inspection)
    from field import ALL_FIELDS
"""

import importlib
from pathlib import Path

# Global field registry (will be populated by imports)
ALL_FIELDS = {}

# Auto-discovery: Import all .py files in this directory
message_dir = Path(__file__).parent
current_package = __package__  # e.g., 'bitwig.sysex_protocol.field' or None

if current_package:
    # We're being imported as a package, use relative imports
    for file in message_dir.glob('*.py'):
        # Skip __init__.py
        if file.stem != '__init__':
            try:
                # Import the module (e.g., .transport from bitwig.sysex_protocol.field)
                module = importlib.import_module(f'.{file.stem}', package=current_package)

                # Collect all FieldBase instances from the module
                from protocol_codegen.core.field import FieldBase
                for attr_name in dir(module):
                    attr = getattr(module, attr_name)
                    if isinstance(attr, FieldBase):
                        ALL_FIELDS[attr_name] = attr

            except Exception as e:
                # Log error but don't crash (allows partial development)
                print(f"Warning: Failed to load field category '{file.stem}': {e}")
else:
    # Fallback: not imported as package, skip auto-discovery
    print("Warning: field package loaded without package context, skipping auto-discovery")

__all__ = ['ALL_FIELDS']
