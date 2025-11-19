# MIDI Studio - Bitwig Plugin

Bidirectional Bitwig Studio extension with Teensy 4.1 hardware controller.

## Overview

MIDI Studio is a bidirectional system for advanced DAW control:

- **Bitwig Extension (Host)**: Java plugin using Bitwig Extension API
- **Teensy Firmware (Device)**: C++ embedded application with LVGL interface
- **Custom SysEx Protocol**: Auto-generated bidirectional communication

### Key Features

- Transport control (Play, Stop, Record)
- Device/track navigation with visual feedback
- 8 encoder macro control (Remote Controls)
- ILI9341 display with custom LVGL interface
- Real-time Host ↔ Device synchronization

## Prerequisites

### Host Development

- **JDK 25+**: [Oracle JDK](https://www.oracle.com/java/technologies/downloads/) or [Temurin](https://adoptium.net/)
- **Maven 3.9+**: [Installation](https://maven.apache.org/install.html)
- **Bitwig Studio 6 Beta**: Currently in development for Bitwig 6
  - Uses latest API features (text value lists, etc.)
  - Bitwig 5 backport may be considered if needed

### Device Development

- **PlatformIO**: [Installation](https://platformio.org/install)
- **Python 3.11+** with **uv**: `pip install uv`

### Hardware

- **Teensy 4.1** (600 MHz ARM Cortex-M7)
- **ILI9341 Display** (320x240, SPI)
- **8 Rotary Encoders** with push-buttons
- **CD74HC4067 Multiplexer** (optional)

## Project Structure

```
plugin-bitwig/
├── host/                    # Bitwig Extension (Java)
│   ├── src/main/java/com/midi_studio/
│   │   ├── MidiStudioExtension.java
│   │   ├── handler/
│   │   │   ├── controller/  # Device → Bitwig
│   │   │   └── host/        # Bitwig → Device
│   │   └── protocol/        # Generated SysEx code
│   └── pom.xml
│
├── src/                     # Teensy Firmware (C++)
│   ├── main.cpp
│   ├── handler/             # Protocol handlers
│   ├── protocol/            # Generated SysEx code
│   └── ui/                  # LVGL interface
│
├── protocol/                # Protocol generator (Python)
│   ├── sysex_messages.py
│   └── field/
│
└── script/bash/extension/   # Build scripts
```

## Quick Start

### Install dependencies

```bash
# Python
uv sync

# PlatformIO
pio lib install
```

### Build Extension

```bash
./script/bash/extension/bitwig-package.sh
# or
cd host && mvn package
```

Extension is auto-deployed to:
- **Windows**: `%USERPROFILE%\Documents\Bitwig Studio\Extensions`
- **macOS**: `~/Documents/Bitwig Studio/Extensions`
- **Linux**: `~/Bitwig Studio/Extensions`

### Build & Upload Firmware

```bash
# Production
pio run -e prod -t upload

# Debug (with logs)
pio run -e debug -t upload
pio device monitor
```

### Activate in Bitwig

1. Settings → Controllers → Add controller
2. Search "MIDI Studio"
3. Select MIDI ports
4. Extension shows "MIDI Studio : Connected"

## Development

### Architecture

```
Bitwig Studio (API)
        ↕ MIDI SysEx
Protocol Layer (C++)
        ↕
    Handlers
        ↕
   UI (LVGL)
```

**Handler Pattern**: Fire-and-forget
- Handlers register with Bitwig API and stay alive via API references
- Extension doesn't hold references (avoids memory leaks)

**Separation**:
- `handler/controller/`: Device → Bitwig commands
- `handler/host/`: Bitwig → Device observations

### Add New Message

1. Define in `protocol/message/`:
```python
class MyMessage(Message):
    message_id: int = 0x42
    value: int
```

2. Generate code:
```bash
./script/bash/protocol/generate_protocol.sh
```

3. Implement handlers (Java & C++):
```java
protocol.onMyMessage = msg -> device.setSomething(msg.value);
```
```cpp
protocol.onMyMessage = [](const MyMessage& msg) { /* ... */ };
```

### Debugging

**Bitwig logs**: `~/Documents/Bitwig Studio/log/BitwigStudio.log`

**Teensy logs**:
```bash
pio run -e debug -t upload
pio device monitor
```

### Build Scripts

```bash
# Extension
./script/bash/extension/bitwig-compile.sh   # Compile only
./script/bash/extension/bitwig-package.sh   # Build & deploy
./script/bash/extension/bitwig-clean.sh     # Clean

# Protocol
./script/bash/protocol/generate_protocol.sh # Regenerate messages
```

## Guidelines

### Commits

Follow [Conventional Commits](https://www.conventionalcommits.org/):

```
feat: add device bypass toggle
fix: correct parameter scaling
refactor: simplify host initialization
docs: update README
chore: upgrade to Java 25
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

Check Java version: `java -version` (must be ≥ 25)

### No Host ↔ Device communication

1. Check MIDI ports in Bitwig Settings
2. Check device logs: `pio device monitor`
3. Regenerate protocol: `./script/bash/protocol/generate_protocol.sh`
4. Rebuild both: `mvn package && pio run -e prod -t upload`

### Maven "Guice/Unsafe" warnings

Already handled in scripts via:
```bash
export MAVEN_OPTS="--sun-misc-unsafe-memory-access=allow"
```

## Resources

- [Bitwig Extension API](https://github.com/bitwig/bitwig-extensions)
- [PlatformIO Teensy](https://docs.platformio.org/en/latest/platforms/teensy.html)
- [LVGL Documentation](https://docs.lvgl.io/)
- [DrivenByMoss](https://github.com/git-moss/DrivenByMoss) - Reference extension framework

---

**Status**: Active development for Bitwig 6
**Version**: 0.1.0 (Alpha)
**Author**: Petitechose Audio
