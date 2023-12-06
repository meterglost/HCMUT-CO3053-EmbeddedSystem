# Embedded system course with ESP32

## Programming workspace

### Prerequisites

-   Docker and docker compose
-   VSCode with devcontainer extension
-   [ESP RFC server](https://github.com/espressif/esptool/releases/latest)

### Workspace

1.  Open VSCode Command Palette with `Ctrl+Shift+P`
2.  Select `Dev Containers: Reopen in Container`
3.  Choose the workspace you want to open

### Workspace config

1.  Open VSCode Command Palette with `Ctrl+Shift+P`
2.  Select `ESP-IDF: Add vscode configuration folder`
3.  In `.vscode/c_cpp_properties.json`, `includePath` and `path` property, add `"${workspaceFolder}/build/config"`

## Remote serial port

1.  Check which USB port the ESP32 is binded to

    > **NOTE**\
    > Check on host machine, not inside devcontainer

    On Ubuntu

    ```bash
    ls -la /sys/bus/usb-serial/devices/
    ```

    On Windows
    ```powershell
    Get-PnpDevice -PresentOnly -Class Ports | Select-Object Name, Description, DeviceID | Format-List
    ```

2.  Run RFC server to redirect network connection to that USB port

    ```shell
    sudo ./esp_rfc2217_server /dev/ttyUSB0
    ```

    Now the ESP32 can be connected to flash firmware through network at

    ```
    rfc2217://<host_address>:2217?ign_set_control
    ```

    > **NOTE**\
    > Workspace is pre-configured to use `host.docker.internal` as `<host_address>`, which means connect to ESP32 on the same host machine

## Flash firmware

Workspace has been configured so that you can build/flash/monitor the ESP32 using button on VSCode status bar or by calling action in VSCode command pallete.

Manually, you can use this command to flash your ESP32

```shell
idf.py -p rfc2217://host.docker.internal:2217?ign_set_control flash
```
