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
| .github/workflow | Workflow configuration files for GitHub Actions                       |
| build/           | Build outputs (object files + executable)                             |
| docs/            | Documentation (e.g., coding guidelines)                               |
| lib/             | External dependencies                                                 |
| src/             | Source files (.c/.h)                                                  |
| .clang-format    | Configuration file for `clang-format` tool to define code style rules |
| .dockerignore    | List of files to exclude from the Docker build context                |
| .gitignore       | List of files to exclude from Git repository                          |
| Dockerfile       | Script used to build the Docker image for this project                |
| LICENSE          | Tells others what they can and cannot do with the repository          |
| Makefile         | Script that automates the build process                               |
| README.md        | This markdown file                                                    |

## Prerequisites

### Hardware

- **B-L4S5I-IOT01A** development board or equivalent STM32L4 microcontroller board

### Software

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

## Continuous Integration

A simple continuous integration (CI) system is implemented to have some protection against erroneous code changes and formatting. This system is developed using GitHub Actions, Docker, and some of the aforementioned tools. The CI system's configuration is written in `ci.yml` located in [.github/workflows](.github/workflows).

The GitHub Action performs static analysis, checks formatting, and build capability on each commit pushed to GitHub. The GitHub Action blocks the commits from being merged until they pass. While a CI system is in place to demonstrate the principle, it only provides some weak protection against erroneous code changes and formatting. A more rigourous CI system is suggested in a professional environment that also performs unit testing, performance testing, integration testing, and hardware/software testing on the real target.

The GitHub Action implemented in this repository runs inside a Docker container which has the required software preinstalled. The Docker container is generated using a Docker image and it is available in a [repository on Docker Hub](https://hub.docker.com/repository/docker/meetkumarp/stm32cubeclt-1.18.0).

## Coding Guidelines

The coding guidelines are described in [docs/coding_guidelines.md](docs/coding_guidelines.md). Some guidelines are enforced by the code formatter (clang-format). There is a phony target in the `Makefile` to format all source files with `clang-format`.

```bash
make format
```

## Static Analysis

A static analyzer is used to detect and flag the types of bugs that a compiler might fail to catch. In this project, `cppcheck` is used as the static analyzer. There is a phony target in the `Makefile` to analyze all files with `cppcheck`.

```bash
make cppcheck
```

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
