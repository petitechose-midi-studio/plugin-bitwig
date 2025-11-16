"""
Message Categories - Auto-Discovery with Introspection

This module automatically discovers and loads all message category modules
in the message/ directory. Each category module (e.g., transport.py, parameter.py)
defines messages as pure Message instances.

ARCHITECTURE:
- Auto-discovery: All .py files in message/ are automatically imported
- Introspection: Message instances collected via isinstance() check
- Pure data: Messages are simple dataclass instances (no side effects)
- Name injection: Variable names automatically injected into Message.name

ADDING A NEW CATEGORY:
1. Create message/mycategory.py
2. Define messages: TRANSPORT_PLAY = Message(description='...', fields=[...])
3. That's it! The module is auto-discovered and messages collected

USAGE:
    from message import ALL_MESSAGES
    # ALL_MESSAGES contains all Message instances from all categories
"""

import importlib
from pathlib import Path
from typing import List
from protocol.message import Message

# Global message registry (populated by introspection)
ALL_MESSAGES: List[Message] = []

# Auto-discovery: Import all .py files in this directory
message_dir = Path(__file__).parent
current_package = __package__  # e.g., 'bitwig.sysex_protocol.message' or None

if current_package:
    # We're being imported as a package, use relative imports
    for file in message_dir.glob('*.py'):
        # Skip __init__.py
        if file.stem != '__init__':
            try:
                # Import the module (e.g., .transport from bitwig.sysex_protocol.message)
                module = importlib.import_module(f'.{file.stem}', package=current_package)

                # Collect all Message instances via introspection
                for attr_name in dir(module):
                    # Skip private/dunder attributes
                    if attr_name.startswith('_'):
                        continue

                    attr = getattr(module, attr_name)

                    # Check if it's a Message instance
                    if isinstance(attr, Message):
                        # Inject the variable name into the message
                        attr.name = attr_name
                        ALL_MESSAGES.append(attr)

            except Exception as e:
                # Log error but don't crash (allows partial development)
                print(f"Warning: Failed to load message category '{file.stem}': {e}")
else:
    # Fallback: not imported as package, skip auto-discovery
    print("Warning: message package loaded without package context, skipping auto-discovery")

__all__ = ['ALL_MESSAGES']
