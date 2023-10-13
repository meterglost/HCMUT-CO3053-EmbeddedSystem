# Embedded system course with ESP32

## Programming workspace with VSCode devcontainer

### Prerequisites

-   Docker and docker compose
-   VSCode with devcontainer extension
-   [ESP RFC server](https://github.com/espressif/esptool/releases/latest)

### Create workspace with devcontainer

Open `.devcontainer/devcontainer.json` and modify the line

```json
...
	"workspaceFolder": "/workspaces",
...
```

to match the lab you want to build, for example

```json
...
	"workspaceFolder": "/workspaces/lab1",
...
```

Then open VSCode Command Palette with `Ctrl+Shift+P` and select `Dev Containers: Rebuild and Reopen in Container`

### Run RFC server for flashing firmware

-   First you need to check which "port" the ESP32 is binded to, `/dev/ttyUSB0` for example.

-   Then run an RFC server to redirect network connection to that "port"

    > [!NOTE]\
    > Run with superuser privileges

    ```shell
    sudo ./esp_rfc2217_server /dev/ttyUSB0
    ```

-   Now you can connect to the device to flash firmware through `rfc2217://<host_address>:2217?ign_set_control`

### Build/Flash/Monitor

Workspace has been config so that you can use build/flash/monitor button from esp-idf extension

#### Build manually

```shell
idf.py build
```

#### Flash manually

To flash from inside devcontainer, use `host.docker.internal` as `<host_address>`

```shell
idf.py -p rfc2217://host.docker.internal:2217?ign_set_control flash
```
