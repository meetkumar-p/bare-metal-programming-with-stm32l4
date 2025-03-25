# Bare Metal Programming with STM32L4

This repository is inspired by [bare-metal-programming-guide](https://github.com/cpq/bare-metal-programming-guide), a GitHub repository by **cpq**. The main difference is that this project will use the **B-L4S5I-IOT01A** development board.

This project will follow many of the steps outlined in the *bare-metal-programming-guide* repository, including:

- Blinky
- Blinky with SysTick Interrupt
- UART
- `printf()` over UART
- Vendor CMSIS Headers
- Clocks Configuration

## Directory Structure

| Directory        | Description                                                           |
|------------------|-----------------------------------------------------------------------|
| docs/            | Documentation (e.g., coding guidelines)                               |
| .gitignore       | List of files to exclude from Git repository                          |
| LICENSE          | Tells others what they can and cannot do with the repository          |
| README.md        | This markdown file                                                    |

## Prerequisites

### Hardware

- **B-L4S5I-IOT01A** development board or equivalent STM32L4 microcontroller board

## Software

| Tool                  | Description                        | Link                                                                                                                   |
|-----------------------|------------------------------------|------------------------------------------------------------------------------------------------------------------------|
| Git                   | distributed version control system | [http://git-scm.com/](http://git-scm.com/)                                                                             |
| GNU make              | build automation tool              | [http://www.gnu.org/software/make/](http://www.gnu.org/software/make/)                                                 |
| PuTTY (or equivalent) | serial console                     | [https://www.putty.org/](https://www.putty.org/)                                                                       |
| STM32CubeCLT          | STM32 CLI toolset                  | [https://www.st.com/en/development-tools/stm32cubeclt.html](https://www.st.com/en/development-tools/stm32cubeclt.html) |

## Installation

1. Clone the repository and its submodules

    ```bash
    git clone --recurse-submodules https://github.com/meetkumar-p/bare-metal-programming-with-stm32l4.git
    ```

2. Install the prerequisites (if necessary)

3. Build the project with `make build`

4. Flash the generated binary file to a development board with `make flash`

5. Use a serial console to monitor UART output (if applicable)

## Coding Guidelines

The coding guidelines are described in [docs/coding_guidelines.md](docs/coding_guidelines.md).

## Version Control

### Commit Messages

- Use **Conventional Commits** for commit message format
  - [https://www.conventionalcommits.org/](https://www.conventionalcommits.org/)
  - Commit message structure:

    ```text
    <type>(<optional scope>): <description>

    <optional body>

    <optional footer(s)>
    ```

### Branch Naming

- Use descriptive names for branches, including the issue number if applicable.
  - For example: `feat/gpio`, `hotfix/gpio-read-write-bug`, `feat/lsm6dsl`

## References

- bare-metal-programming-guide [repository](https://github.com/cpq/bare-metal-programming-guide)
- STM32L4+ microcontroller series [reference manual](https://www.st.com/resource/en/reference_manual/rm0432-stm32l4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
- STM32L4Sxxx microcontroller [datasheet](https://www.st.com/resource/en/datasheet/stm32l4s5vi.pdf)
- B-L4S5I-IOT01A discovery kit [user manual](https://www.st.com/resource/en/user_manual/um2708-discovery-kit-for-iot-node-multichannel-communication-with-stm32l4-series-stmicroelectronics.pdf)
