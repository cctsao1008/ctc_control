# Instrument Automation Control

Instrument Automation Control is a legacy Windows-based C/C++ automation and equipment-orchestration system for coordinating heterogeneous instruments, actuators, communication interfaces, scripting logic, and data services within a multi-stage automated workflow.

The repository preserves a Visual Studio Win32 application together with equipment-control modules, scheduling and command infrastructure, Python integration, MQTT, Modbus/RS-485 communication, MariaDB access, configuration handling, logging, and machine-specific interfaces.

> **Project status:** Legacy / reference project. The codebase reflects its original Windows development environment and bundled third-party dependencies. Some toolchains, libraries, paths, and runtime assumptions may require restoration or modernization before the project can be built and used on a current system.

## Overview

This project is structured as a supervisory automation system rather than a single device driver.

The software separates higher-level orchestration from equipment-specific control. Core services coordinate commands, scheduling, system state, configuration, logging, database access, and automation logic, while modular device and communication components expose lower-level machine capabilities.

This allows individual device operations to be composed into higher-level automated procedures through a combination of native C/C++ control logic and scripting.

The checked-in project contains support for areas including:

- equipment control and actuation
- command and service handling
- workflow scheduling
- serial / RS-485 communication
- Modbus
- MQTT
- MariaDB
- configuration parsing
- logging
- Python / embedded interpreter integration
- memory and data handling

Equipment-oriented modules visible in the repository include components for:

- washer control
- syringe pump control
- microscope XY positioning
- scanner control
- servo control
- RS-485-connected equipment
- centrifuge bindings
- gripper bindings
- guideway / motion bindings

## System Architecture

```text
                 Automated Workflow / Scripts
                           |
                 Command & Scheduling Layer
                           |
                  Core Orchestration
                           |
        +------------------+------------------+
        |                  |                  |
   Equipment          Motion / I/O       Infrastructure
    Modules           & Communication        Services
        |                  |                  |
   washer              serial / RS-485      config
   syringe pump        Modbus               logging
   microscope XY       servo                database
   scanner             MQTT                 Python
   gripper             LCM-lite             interpreter
   guideway                                 memory/data
        |                  |                  |
        +------------------+------------------+
                           |
                   Physical Equipment
```

The architecture reflects a layered automation model:

```text
Workflow / script
      ↓
Command and scheduling
      ↓
Core orchestration
      ↓
Device / motion / communication modules
      ↓
Physical equipment and external services
```

## Repository Layout

```text
instrument-automation-control/
├── config/                  Runtime and configuration examples
├── core/                    Core orchestration and service infrastructure
├── lib/                     Bundled third-party libraries and binaries
├── module/                  Protocol, interpreter, and equipment modules
├── script/                  Python and control scripts
├── ctc_control.vcxproj      Historical Visual Studio C++ project
├── ctc_control.vcxproj.filters
└── ctc_control.vcxproj.user
```

The repository has been renamed for clearer public positioning, but historical internal project and source names such as `ctc_control.vcxproj` are intentionally preserved.

## Core Layer

The `core/` layer provides shared system-level services and coordination.

Representative files include:

- `core_control.cpp` — central control logic and parameter handling
- `core_system.cpp` — system facilities and timing helpers
- `core_service.cpp` — service handling
- `core_commander.cpp` — command handling
- `core_actuator.cpp` — actuator-related control
- `core_scheduler.cpp` — scheduling support
- `core_mariadb.cpp` — MariaDB integration
- `core_memorywriter.cpp` — memory / data handling
- `core_rsh.cpp` — remote-shell-related functionality

`core_common.h` acts as a central integration header and brings together Windows APIs, serial communication, pthreads, Modbus, MQTT / Mosquitto, JSON, LCM-lite, configuration support, Python, database access, and equipment modules.

## Equipment and Support Modules

The `module/` directory contains both reusable support components and equipment-specific modules.

Examples include:

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

The separation between the core layer and equipment modules allows hardware-specific operations to remain distinct from higher-level orchestration and workflow logic.

## Interpreter and Automation

The repository contains an interpreter layer and Python integration used to expose and coordinate machine functions above device-level control.

Interpreter-related sources include bindings or components for equipment such as:

- centrifuge
- gripper
- guideway
- microscope XY stage
- syringe pump
- washer
- MariaDB

The `script/` directory contains Python-side automation and control scripts.

This structure reflects a separation between:

```text
Device primitives
      ↓
C/C++ control interfaces
      ↓
Interpreter bindings
      ↓
Scripted automation / workflow logic
```

## Communication and Integration

The codebase contains integration points for multiple communication and service mechanisms.

| Interface | Evidence in repository |
|---|---|
| Serial / RS-485 | `cserial`, `rs485`, equipment modules |
| Modbus | libmodbus headers and libraries |
| MQTT | Mosquitto and `module/mqtt` |
| LCM-lite | `module/lcm-lite` |
| Database | MariaDB client integration |
| Python | CPython embedding / interpreter modules |
| Windows APIs | Win32 headers and synchronization primitives |

These interfaces support communication between the orchestration layer, physical equipment, scripts, and external data or messaging services.

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

This path is part of the historical runtime environment and may need adjustment depending on the execution directory.

## Original Build Environment

The checked-in Visual Studio project identifies the historical build target as:

- **Application type:** Win32 console application
- **Visual Studio toolset:** `v120`
- **Configurations:** `Debug|Win32` and `Release|Win32`

`v120` corresponds to the Visual Studio 2013-era MSVC toolchain.

The project file contains direct include and library references to bundled or expected dependencies such as:

- pthreads-win32
- libmodbus 3.1.4
- Mosquitto 1.4.14
- Python 3.6.x
- MariaDB 10.1 client libraries

The Debug configuration links libraries including:

```text
mosquitto.lib
pthreadVC2.lib
modbus.lib
libmysql.lib
```

The Visual Studio project also uses pre-build copy commands to place required DLLs, Python runtime files, scripts, and configuration files into the output directory.

## Building

The repository preserves an older Windows / Visual Studio environment. The most faithful starting point is therefore the checked-in project:

```text
ctc_control.vcxproj
```

A historical build would require the corresponding Win32 MSVC toolchain and the dependency layout expected by the project file.

For a modern rebuild, review at least:

1. Visual Studio platform toolset migration
2. 32-bit versus 64-bit dependency compatibility
3. Python embedding API and version compatibility
4. Mosquitto, MariaDB, libmodbus, and pthread library versions
5. hard-coded relative runtime paths
6. DLL deployment and configuration-file locations

No claim is made that the repository currently builds unmodified with a modern Visual Studio installation.

## Design Characteristics

The repository demonstrates several architectural patterns that remain useful as a reference:

- supervisory orchestration above heterogeneous equipment
- separation between core coordination and device-specific modules
- separation between device primitives and higher-level scripted automation
- centralized command and scheduling infrastructure
- reusable communication abstractions
- configuration-driven runtime behavior
- database and messaging integration
- interpreter bindings above machine-control interfaces
- modular equipment integration rather than a single monolithic driver

The design also reflects the constraints of its original environment and is tightly coupled in places to Win32 APIs, bundled binaries, legacy dependency versions, and historical filesystem assumptions.

## Modernization Notes

If the project is revived, a practical modernization path would be:

```text
1. Reproduce the original Win32 build
          ↓
2. Document runtime dependencies and equipment interfaces
          ↓
3. Establish a known-good behavioral baseline
          ↓
4. Separate third-party libraries from project source
          ↓
5. Replace hard-coded paths with runtime configuration
          ↓
6. Upgrade toolchain and dependencies incrementally
          ↓
7. Add repeatable build and smoke-test procedures
```

The first priority should be reproducibility and behavioral understanding rather than immediately rewriting the architecture. Once the original system behavior and device boundaries are documented, individual modules can be replaced or modernized with lower risk.

## Getting the Source

```bash
git clone https://github.com/cctsao1008/instrument-automation-control.git
cd instrument-automation-control
```

## Project History

This is a historical engineering project retained as a reference implementation of a modular instrument-automation and equipment-orchestration system.

Source files identify **Ricardo Tsao** as the original author on multiple core components.

The repository is useful primarily as a record of system architecture and integration patterns involving:

- multi-device coordination
- hardware abstraction
- communication protocols
- scheduling
- scripting
- database integration
- higher-level automation

## Scope and Naming

The current repository name, **Instrument Automation Control**, describes the software architecture and engineering scope visible in the source tree.

Historical internal identifiers and filenames are preserved where changing them would alter or obscure the original source history.

The README intentionally describes only functionality and architecture that can be supported by the repository itself.

## License

No explicit top-level license was identified while preparing this README. Unless a license is added, normal copyright rules apply to the original project code.

Bundled third-party components may have their own licenses and terms. Consult their respective source directories before reuse or redistribution.
