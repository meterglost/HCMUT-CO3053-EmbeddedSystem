# Contributing guides

## Setup

Dev environment is pre-configured using [Devcontainer](https://code.visualstudio.com/docs/devcontainers/containers) and [VSCode workspace](https://code.visualstudio.com/docs/editor/workspaces). Each lab is a [ESP-IDF project](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/build-system.html#concepts) and has its own folder in workspace.

Open project inside devcontainer and then open workspace with `.devcontainer/.code-workspace` file.

## Build ESP-IDF project

ESP-IDF project can be built [directly with CMake](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/build-system.html#using-cmake-directly) or using [CLI tool wrapped around CMake](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/tools/idf-py.html) for convenience

### Config

```bash
cmake -S . -B build -G Ninja
```

-   Only need to run once to generate configuration files for the project, unless the cmake file is changed
-   If `sdkconfig.defaults` file is changed, `sdkconfig` file need to be removed so it can be re-generated and applied the new default values.

### Build

```bash
ninja -C build all
```

is equivalent to:

```bash
ninja -C build app bootloader partition-table
```

-   Include building for 3 parts of the firmware: [partition table](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/partition-tables.html), [bootloader](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/bootloader.html) and the main application
-   Builds are cached, only changed parts are re-built

### Flash

```bash
ninja -C build flash
```

-   Require `ESPPORT` and `ESPBAUD` environment variables
-   Can detect and re-build changed parts automatically

### Monitor

```bash
ninja -C build monitor
```

-   Require `ESPPORT` and `ESPBAUD` environment variables

## Check ESP32 serial port

### Linux

```bash
ls -la /sys/bus/usb-serial/devices/
```

### Windows

```powershell
Get-CimInstance -ClassName Win32_SerialPort | Select-Object Name, Description, DeviceID | Format-List
```

## Connect serial port in devcontainer

ESP-IDF provides [ESP RFC server](https://github.com/espressif/esptool/releases/latest) to create [remote serial port](https://docs.espressif.com/projects/esptool/en/latest/esp32/esptool/remote-serial-ports.html)

```bash
esp_rfc2217_server <USB_PORT>
```

And devcontainer is pre-configured to connect to that remote serial port with `ESPPORT` and `ESPBAUD` environment variables pre-defined. Check [devcontainer.json](.devcontainer/devcontainer.json) file for details.
