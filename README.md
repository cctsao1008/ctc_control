# Instrument Automation Control

Instrument Automation Control is a Windows-based C/C++ automation and equipment-orchestration system for coordinating heterogeneous instruments, actuators, communication interfaces, scripting logic, and data services within a multi-stage automated workflow.

The repository contains a Visual Studio Win32 application together with equipment-control modules, scheduling and command infrastructure, Python integration, MQTT, Modbus/RS-485 communication, MariaDB access, configuration handling, logging, and machine-specific interfaces.

## Overview

The software is structured as a supervisory automation system rather than a single device driver.

Higher-level orchestration is separated from equipment-specific control. Core services coordinate commands, scheduling, system state, configuration, logging, database access, and automation logic, while modular device and communication components expose lower-level machine capabilities.

The codebase includes support for:

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

Equipment-oriented modules include components for:

- washer control
- syringe pump control
- microscope XY positioning
- scanner control
- servo control
- RS-485-connected equipment
- centrifuge bindings
- gripper bindings
- guideway / motion bindings

## System architecture

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

The architecture follows a layered automation model:

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

## Repository layout

```text
instrument-automation-control/
├── config/                  Runtime and configuration examples
├── core/                    Core orchestration and service infrastructure
├── lib/                     Bundled third-party libraries and binaries
├── module/                  Protocol, interpreter, and equipment modules
├── script/                  Python and control scripts
├── ctc_control.vcxproj      Visual Studio C++ project
├── ctc_control.vcxproj.filters
└── ctc_control.vcxproj.user
```

Historical internal project and source names such as `ctc_control.vcxproj` are preserved in the source tree.

## Core layer

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

`core_common.h` brings together Windows APIs, serial communication, pthreads, Modbus, MQTT / Mosquitto, JSON, LCM-lite, configuration support, Python, database access, and equipment modules.

## Equipment and support modules

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

## Interpreter and automation

Interpreter-related sources expose equipment functions above device-level control. Bindings or components exist for equipment such as centrifuge, gripper, guideway, microscope XY stage, syringe pump, washer, and MariaDB.

The `script/` directory contains Python-side automation and control scripts.

```text
Device primitives
      ↓
C/C++ control interfaces
      ↓
Interpreter bindings
      ↓
Scripted automation / workflow logic
```

## Communication and integration

| Interface | Evidence in repository |
|---|---|
| Serial / RS-485 | `cserial`, `rs485`, equipment modules |
| Modbus | libmodbus headers and libraries |
| MQTT | Mosquitto and `module/mqtt` |
| LCM-lite | `module/lcm-lite` |
| Database | MariaDB client integration |
| Python | CPython embedding / interpreter modules |
| Windows APIs | Win32 headers and synchronization primitives |

## Configuration

The `config/` directory contains INI-style configuration files, including:

```text
config/default.ini
config/output.ini
config/rs485p1.ini
```

Some code also references JSON-based runtime configuration. For example, `core_control.cpp` reads:

```text
Debug\ctc-config.json
```

## Build environment

The checked-in Visual Studio project identifies:

- **Application type:** Win32 console application
- **Visual Studio toolset:** `v120`
- **Configurations:** `Debug|Win32` and `Release|Win32`

The project file references dependencies including:

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

The Visual Studio project also contains pre-build copy commands for required DLLs, Python runtime files, scripts, and configuration files.

## Design characteristics

The repository captures several architectural patterns:

- supervisory orchestration above heterogeneous equipment
- separation between core coordination and device-specific modules
- separation between device primitives and higher-level scripted automation
- centralized command and scheduling infrastructure
- reusable communication abstractions
- configuration-driven runtime behavior
- database and messaging integration
- interpreter bindings above machine-control interfaces
- modular equipment integration rather than a single monolithic driver

## Project history

This repository preserves a historical engineering implementation of a modular instrument-automation and equipment-orchestration system.

Source files identify **Ricardo Tsao** as the original author on multiple core components.

## License

No explicit top-level license is identified. Bundled third-party components may have their own licenses and terms.
