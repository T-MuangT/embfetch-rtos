# Embfetch RTOS

Embfetch RTOS is a C library for displaying a system information in summary along with an ASCII logo, akin to Neofetch, but for real-time operating systems such as Zephyr and FreeRTOS.

## Installation methods

Depending on which system you may have, the library may be installed in various real-time operating systems.
Currently, the supported systems are as follows:

### Scripts for file copying

There are three scripts to choose from, each doing the exact same thing, but for different shell environments.
| Script Name | Environment | Command |
| :--- | :--- | :--- |
| `package.py` | Python | `python ./scripts/package.py <platform> <destination_dir>` |
| `package.sh` | Bash/Zsh/POSIX | `./scripts/package.sh <platform> <destination_dir>` |
| `package.ps1` | PowerShell | `.\scripts\package.ps1 <platform> <destination_dir>` |

### Prerequisites and installation

| OS / Environment | Prerequisites & Config | Installation & Build Method |
| :--- | :--- | :--- |
| **FreeRTOS** | Shell library (FreeRTOS+CLI, vendor, or custom) set up. | Copy files to kernel source directory. Register `embfetch` command. Add to `Makefile` `SRCS`. Run `make`. |
| **Zephyr** | `CONFIG_SHELL=y` or evaluate `prj.conf.example`. | Copy files to `src` directory. Register `embfetch` command. Add to `CMakeLists.txt` `target_sources()`. Run `west build`. |
| **RT-Thread** | FinSH configured. | Copy files to `applications` directory. Build via RT-Thread Studio (Rebuild) or run `scons -j$(nproc)`. |
| **ThreadX** (Azure RTOS) | Shell library (vendor/custom) set up. | Copy files to `src` directory. Register `embfetch` command. Run `cmake -B build` & `cmake --build build`. |
| **NuttX** (Apache) | Native `nsh` environment. | Create `apps/examples/embfetch` directory and copy files. Add to `Kconfig`. Run `make`. |
| **Keil RTX5** | `CMSIS/Core` `CMSIS/RTOS2/Keil RTX5` selected; shell library set up; `USING_KEIL_RTX` defined. | Add files to project. Register `embfetch` command. Rebuild Project. |
| **Keil RTX4** | `CMSIS/Core` `CMSIS/RTOS/Keil RTX` selected; shell library set up; `USING_KEIL_RTX` defined. | Add files to project. Register `embfetch` command. Rebuild Project. |
| **ESP-IDF** | `system/console` template (basic/advanced) in Espressif IDE. | Copy files to `main` directory. Register `embfetch` command. Update `main/CMakeLists.txt` (`SRCS` and `REQUIRES`). Rebuild Project. |
| **nRF-Connect** | `CONFIG_SHELL=y` or evaluate `prj.conf.example`. | Copy files to `src` directory. Register `embfetch` command. Add to `CMakeLists.txt`  `target_sources()`. Run `west build`. |
