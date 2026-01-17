# MIDI Studio - Bitwig Plugin

Bitwig Studio extension with Teensy 4.1 hardware controller for advanced DAW control.

## Overview

MIDI Studio connects Bitwig with custom hardware:

- **Bitwig Extension**: Java plugin using Bitwig Extension API v25
- **Teensy Firmware**: C++ embedded application with LVGL interface
- **High-Bandwidth Protocol**: Serial8 (8-bit binary) via [oc-bridge](https://github.com/open-control/bridge)

### Key Features

- Transport control (Play, Stop, Record)
- Device/track navigation with visual feedback
- 8 encoder Remote Controls with automation indicators
- Multiple views (RemoteControls, Mix, Clip)
- Stacked overlays with virtualized lists
- ILI9341 display with custom LVGL interface
- Real-time Host ↔ Device synchronization (~50Hz batch updates)

## Architecture

```
┌──────────────┐    USB Serial     ┌────────────┐      UDP       ┌─────────────┐
│   Teensy     │◄─────────────────►│  oc-bridge │◄──────────────►│   Bitwig    │
│  Controller  │   COBS framing    │  (Rust)    │   :9000        │  Extension  │
└──────────────┘                   └────────────┘                └─────────────┘
```

**Protocol Layer**: Messages defined in Python, auto-generated to C++ and Java using [protocol-codegen](https://github.com/open-control/protocol-codegen).

## Prerequisites

### Host Development

#### Windows
```powershell
# OpenJDK 21
winget install Microsoft.OpenJDK.21

# Maven
winget install Apache.Maven
```

#### Linux (Fedora/RHEL)
```bash
# OpenJDK 21
sudo dnf install java-21-openjdk.x86_64

# Maven
sudo dnf install maven
```

#### Linux (Ubuntu/Debian)
```bash
# OpenJDK 21
sudo apt update
sudo apt install openjdk-21-jdk

# Maven
sudo apt install maven
```

#### Verify Installation
```bash
java -version   # Should show OpenJDK 21
mvn -version    # Should show Maven 3.9+
```

### Bridge

Download [oc-bridge](https://github.com/open-control/bridge/releases) for your platform:
- `oc-bridge-windows.exe`
- `oc-bridge-linux`

Or build from source: `cargo build --release`

### Device Development

See [open-control/framework](https://github.com/open-control/framework) for:
- **PlatformIO** installation
- **Python 3.13+** with **uv**
- **Teensy board support**

Quick summary:
```bash
# Python + uv
pip install uv

# PlatformIO (recommended via VS Code extension)
# Or: pip install platformio
```

### Hardware

See [midi-studio/core](../core/) for hardware requirements (Teensy 4.1, display, encoders, etc.)

### Bitwig Studio

- **Bitwig Studio 6**: Uses latest API features (API v25)

## Project Structure

```
plugin-bitwig/
├── host/src/               # Bitwig Extension (Java)
│   ├── midistudio/         # Main extension code
│   ├── handler/            # Message handlers
│   │   ├── controller/     # Hardware → Bitwig
│   │   └── host/           # Bitwig → Hardware
│   ├── protocol/           # Generated protocol (package: protocol)
│   └── util/               # Utilities
│
├── src/                    # Teensy Firmware (C++)
│   ├── main.cpp            # Entry point
│   ├── context/            # BitwigContext
│   ├── handler/            # Protocol handlers
│   │   ├── host/           # Host → Controller
│   │   └── input/          # User → Bitwig
│   ├── state/              # State management
│   │   ├── BitwigState.hpp
│   │   ├── OverlayManager.hpp
│   │   └── ViewManager.hpp
│   ├── protocol/           # Generated protocol (namespace: Protocol)
│   └── ui/                 # LVGL interface
│       ├── remotecontrols/ # RemoteControlsView
│       ├── device/         # DeviceSelector
│       ├── track/          # TrackSelector
│       ├── view/           # ViewSelector
│       └── widget/         # Shared widgets
│
├── protocol/               # Protocol Definitions (Python)
│   ├── message/            # Message definitions
│   ├── field/              # Field definitions
│   ├── plugin_paths.py     # Output configuration
│   └── binary_protocol_config.py   # Binary config
│
└── script/                 # Build & generation scripts
    ├── extension/          # Java build scripts
    └── protocol/           # Protocol generator
```

## Quick Start

### 1. Install dependencies

```bash
# Python
uv sync

# PlatformIO
pio lib install
```

### 2. Build Extension

```bash
./script/extension/bitwig-package.sh
# or
cd host && mvn package
```

Extension is auto-deployed to:
- **Windows**: `%USERPROFILE%\Documents\Bitwig Studio\Extensions`
- **macOS**: `~/Documents/Bitwig Studio/Extensions`
- **Linux**: `~/Bitwig Studio/Extensions`

### 3. Build & Upload Firmware

```bash
# Production
pio run -e prod -t upload
```

### 4. Start Bridge

```bash
# Launch with TUI (auto-detects Teensy)
oc-bridge

# Or headless
oc-bridge --headless
```

### 5. Activate in Bitwig

1. Settings → Controllers → Add controller
2. Search "petitechose.audio" vendor and select "MIDI Studio"

   ![Extension Selection](asset/extension_select.png)

3. Extension shows "MIDI Studio : Connected"

## Protocol

Messages are defined in Python and auto-generated to C++ and Java.

### Message Types

| Category | Examples |
|----------|----------|
| Device | `DEVICE_CHANGE_HEADER`, `DEVICE_REMOTE_CONTROL_UPDATE` |
| Remote Controls | `DEVICE_REMOTE_CONTROLS_BATCH`, `DEVICE_REMOTE_CONTROL_TOUCH` |
| Navigation | `DEVICE_LIST_WINDOW`, `DEVICE_PAGE_NAMES_WINDOW` |
| Track | `TRACK_*` messages |
| Transport | `TRANSPORT_*` messages |
| View State | `VIEW_STATE_CHANGE` |

### Optimized Types

| Type | Usage |
|------|-------|
| `norm8` | Parameter values (1 byte, ~0.8% precision) |
| `norm16` | High-precision values (2 bytes) |

### Batch Updates

Remote control values are sent at ~50Hz using `DEVICE_REMOTE_CONTROLS_BATCH`:
- `parameter_values_batch`: 8× norm8 values
- `parameter_modulated_values_batch`: 8× norm8 modulated values
- `parameter_display_values_batch`: 8× strings (for LIST/BUTTON types)

### Generate Protocol

```bash
./script/protocol/generate_protocol.sh
```

## UI Architecture

### Views

| View | Description |
|------|-------------|
| `RemoteControlsView` | 8 parameter widgets with automation indicators |
| `MixView` | (Stub) Channel strip view |
| `ClipView` | (Stub) Clip launching view |

### State Management

Uses reactive `Signal<T>` and `SignalVector<T>` from open-control/framework:

```cpp
// In state struct
Signal<std::string> deviceName{""};
SignalVector<std::string, 32> pageNames;

// Subscribe in view
watcher_.watchAll([this]() { updateUI(); }, state.deviceName, state.pageNames);
```

### Overlay System

`OverlayManager` handles stacked overlays with event ownership:
- DeviceSelector
- TrackSelector
- PageSelector
- ListOverlay (for parameter discrete values)

## Development

### Add New Message

1. Define in `protocol/message/`:
```python
MY_NEW_MESSAGE = Message(
    description='Description of message',
    fields=[field1, field2]
)
```

2. Register in `protocol/message/__init__.py`

3. Generate code:
```bash
./script/protocol/generate_protocol.sh
```

4. Implement handlers (Java & C++):
```java
// Java (host/)
callbacks.onMyNewMessage = msg -> { /* handle */ };
```
```cpp
// C++ (src/)
protocol.onMyNewMessage = [](const MyNewMessageMessage& msg) { /* handle */ };
```

### Debugging

**Bitwig logs**: `~/Documents/Bitwig Studio/log/BitwigStudio.log`

**Bridge logs**: Run `oc-bridge` with TUI, use filters (1/2/3 keys)

**Teensy logs**: Enable `OC_LOG` in platformio.ini, view via bridge TUI

### Build Scripts

```bash
# Extension
./script/extension/bitwig-compile.sh   # Compile only
./script/extension/bitwig-package.sh   # Build & deploy
./script/extension/bitwig-clean.sh     # Clean

# Protocol
./script/protocol/generate_protocol.sh # Regenerate messages
```

## Guidelines

### Commits

Follow [Conventional Commits](https://www.conventionalcommits.org/):

```
feat: add automation indicator support
fix: correct parameter scaling
refactor: extract handler to SRP modules
docs: update README
chore: upgrade dependencies
```

### Code Style

**Java**: Google Java Style, 3 spaces (Bitwig convention), max 120 chars

**C++**: C++17, PascalCase classes, camelCase methods, 4 spaces

## Troubleshooting

### Extension not in Bitwig

```bash
# Check deployment
ls ~/Documents/Bitwig\ Studio/Extensions/

# Check logs
tail -f ~/Documents/Bitwig\ Studio/log/BitwigStudio.log

# Rebuild
cd host && mvn clean package
```

### "Unsupported class file major version"

Check Java version: `java -version` (should be 21)

### No Host ↔ Device communication

1. Check bridge is running: `oc-bridge`
2. Check bridge detects Teensy (should show in TUI)
3. Check Bitwig extension is active (logs show "Connected")
4. Regenerate protocol: `./script/protocol/generate_protocol.sh`
5. Rebuild both: `mvn package && pio run -t upload`

### Maven "Guice/Unsafe" warnings

Already handled in scripts via:
```bash
export MAVEN_OPTS="--sun-misc-unsafe-memory-access=allow"
```

## Resources

- [Bitwig Extension API](https://github.com/bitwig/bitwig-extensions)
- [open-control/framework](https://github.com/open-control/framework) - Embedded framework
- [open-control/bridge](https://github.com/open-control/bridge) - Serial-to-UDP bridge
- [protocol-codegen](https://github.com/open-control/protocol-codegen) - Protocol generator
- [LVGL Documentation](https://docs.lvgl.io/)
- [DrivenByMoss](https://github.com/git-moss/DrivenByMoss) - Reference extension

---

**Status**: Active development for Bitwig 6
**Version**: 0.1.0 (Alpha)
**Author**: Petitechose Audio

## Icon Font System

Custom icons are generated as a font for efficient rendering with LVGL.

### Workflow

```
asset/icon/*.svg → asset/font/bitwig_icons.ttf → src/ui/font/data/bitwig_icons_14.*
```

### Generate Icons

```bash
# 1. Generate TTF font from SVGs
bash script/font/generate_bitwig_icons.sh

# 2. Convert to LVGL binary format
bash script/lvgl/font/convert_font.sh
```

### Use in Code

```cpp
#include "ui/font/icon.hpp"

Icon::set(label, Icon::TRANSPORT_PLAY);
Icon::set(label, Icon::TRANSPORT_PLAY, Icon::S18);  // with size
```

See [script/font/README.md](script/font/README.md) for details.
