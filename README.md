# CTC Control

CTC Control is a Windows-based C/C++ control application organized around a modular core and a collection of equipment, communication, scripting, and infrastructure modules.

The repository contains a Visual Studio Win32 console application together with device-control components, protocol libraries, configuration files, Python integration, MQTT, Modbus/RS-485 communication, MariaDB access, and equipment-specific modules.

> **Project status:** Legacy / reference project. The codebase reflects the original Windows development environment and bundled third-party dependencies. Some toolchains, libraries, paths, and runtime assumptions may require restoration or modernization before the project can be built and used on a current system.

## Overview

The code is structured as a control framework rather than a single monolithic device driver. The `core/` layer provides common services and coordination, while `module/` contains device-specific and reusable components.

The Visual Studio project currently builds a Win32 console application and references modules for:

- equipment control and actuation
- command and service handling
- RS-485 / serial communication
- Modbus
- MQTT
- MariaDB
- configuration parsing
- logging
- Python / embedded interpreter integration
- scheduling and memory/data handling

Equipment-oriented modules visible in the project include components for a washer, syringe pump, microscope XY stage, scanner, RS-485 devices, servo control, and other machine-specific interfaces.

## Architecture

```text
                     +----------------------+
                     |     CTC Control      |
                     |   Win32 Application  |
                     +----------+-----------+
                                |
                 +--------------v--------------+
                 |            core/            |
                 | system / service / command  |
                 | scheduler / DB / logging    |
                 +--------------+--------------+
                                |
          +---------------------+---------------------+
          |                     |                     |
+---------v---------+ +---------v---------+ +---------v---------+
| Device Modules    | | Communications    | | Runtime / Support |
| washer            | | serial / RS-485   | | config           |
| syringe pump      | | Modbus            | | logging          |
| microscope XY     | | MQTT              | | Python           |
| scanner / servo   | | LCM-lite          | | interpreter      |
+-------------------+ +-------------------+ +-------------------+
                                |
                     +----------v-----------+
                     | External equipment / |
                     | services / database  |
                     +----------------------+
```

## Repository Layout

```text
ctc_control/
├── config/                  Runtime/configuration examples
├── core/                    Core control and service infrastructure
├── lib/                     Bundled third-party libraries and binaries
├── module/                  Protocol, interpreter, and equipment modules
├── script/                  Python/control scripts
├── ctc_control.vcxproj      Visual Studio C++ project
├── ctc_control.vcxproj.filters
└── ctc_control.vcxproj.user
```

### `core/`

The core layer includes components such as:

- `core_control.cpp` — central control logic and parameter handling
- `core_system.cpp` — system-level facilities and timing helpers
- `core_service.cpp` — service handling
- `core_commander.cpp` — command handling
- `core_actuator.cpp` — actuator-related control
- `core_scheduler.cpp` — scheduling support
- `core_mariadb.cpp` — MariaDB integration
- `core_memorywriter.cpp` — memory/data handling
- `core_rsh.cpp` — remote-shell-related functionality

`core_common.h` acts as a central integration header and brings together Windows APIs, serial communication, pthreads, Modbus, MQTT/Mosquitto, JSON, LCM-lite, configuration support, Python, database access, and equipment modules.

### `module/`

The repository contains both reusable third-party/support modules and project-specific equipment modules. Examples include:

- `washer`
- `syringepump`
- `microscope_xy`
- `scanner`
- `rs485`
- `bianneng`
- `mqtt`
- `interpreter`
- `tiny-json`
- `cserial`
- `lcm-lite`
- `config`
- `log.c`
- `getopt` / `wingetopt`
- `micropython`

## Original Build Environment

The checked-in Visual Studio project identifies the original build target as:

- **Application type:** Win32 console application
- **Visual Studio toolset:** `v120`
- **Configurations:** `Debug|Win32` and `Release|Win32`

`v120` corresponds to the Visual Studio 2013-era MSVC toolchain.

The project file contains direct include/library references to bundled or expected dependencies such as:

- pthreads-win32
- libmodbus 3.1.4
- Mosquitto 1.4.14
- Python 3.6.x
- MariaDB 10.1 client libraries

The Debug configuration links against libraries including:

```text
mosquitto.lib
pthreadVC2.lib
modbus.lib
libmysql.lib
```

The project also uses pre-build copy commands to place required DLLs, Python runtime files, scripts, and configuration files into the output directory.

## Building

This repository preserves an older Windows/Visual Studio environment. The most faithful starting point is therefore the checked-in Visual Studio project:

```text
ctc_control.vcxproj
```

A historical build would require the corresponding Win32 MSVC toolchain and the dependency layout expected by the project file.

For a modern rebuild, expect to review at least:

1. Visual Studio platform toolset migration
2. 32-bit versus 64-bit dependency compatibility
3. Python embedding API/version compatibility
4. Mosquitto, MariaDB, libmodbus, and pthread library versions
5. hard-coded relative runtime paths
6. DLL deployment and configuration-file locations

No claim is made here that the current repository builds unmodified with a modern Visual Studio installation.

## Configuration

The `config/` directory contains INI-style configuration files, including:

```text
config/default.ini
config/output.ini
config/rs485p1.ini
```

`default.ini` demonstrates section/key-based configuration handling.

Some code also references JSON-based runtime configuration. For example, `core_control.cpp` contains logic that reads:

```text
Debug\ctc-config.json
```

This is a legacy runtime assumption and may need adjustment depending on the execution directory.

## Communication and Integration

The codebase includes support or integration points for multiple communication mechanisms:

| Interface | Evidence in project |
|---|---|
| Serial / RS-485 | `cserial`, `rs485`, equipment modules |
| Modbus | libmodbus headers/libraries |
| MQTT | Mosquitto and `module/mqtt` |
| LCM-lite | `module/lcm-lite` |
| Database | MariaDB client integration |
| Python | CPython embedding / interpreter modules |
| Windows IPC/system APIs | Win32 headers and synchronization primitives |

## Interpreter and Automation

The project contains an interpreter layer and Python integration intended to expose or coordinate machine functions from scripts. Interpreter sources include bindings/components for equipment such as:

- centrifuge
- gripper
- guideway
- microscope XY
- syringe pump
- washer
- MariaDB

The `script/` directory contains Python-side automation/control scripts.

## Design Characteristics

From the repository structure, the project follows several useful architectural patterns:

- separation between core orchestration and equipment-specific modules
- centralized shared system state/services
- protocol abstraction through reusable communication modules
- script/interpreter integration above device-level functions
- external database and messaging integration
- configuration-driven runtime behavior

At the same time, the project is tightly coupled to its original Windows and Visual Studio environment, especially through Win32 APIs, bundled binary dependencies, and legacy toolchain paths.

## Modernization Notes

If this project is revived, a practical modernization path would be:

```text
1. Reproduce the original Win32 build
          ↓
2. Document runtime dependencies and device interfaces
          ↓
3. Separate third-party libraries from project source
          ↓
4. Replace hard-coded paths with runtime configuration
          ↓
5. Upgrade toolchain and dependencies incrementally
          ↓
6. Add repeatable build and smoke-test procedures
```

The first priority should be reproducibility rather than immediately rewriting the architecture. Once the original behavior is understood, individual modules can be replaced or modernized with lower risk.

## Getting the Source

```bash
git clone https://github.com/cctsao1008/ctc_control.git
cd ctc_control
```

## Project History

The source files identify **Ricardo Tsao** as the original author on multiple core components.

This repository is retained as a reference implementation of a modular Windows-based equipment-control system integrating hardware interfaces, communications, scripting, and higher-level orchestration.

## License

No explicit top-level license was identified while preparing this README. Unless a license is added, normal copyright rules apply to the original project code. Individual bundled third-party components may have their own licenses and terms; consult their respective source directories before reuse or redistribution.
