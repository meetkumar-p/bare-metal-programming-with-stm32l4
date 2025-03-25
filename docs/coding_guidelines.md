# Coding Guidelines

## Table of Contents

- [Introduction](#introduction)
- [General Guidelines](#general-guidelines)
- [Naming Conventions](#naming-conventions)
- [Code Formatting](#code-formatting)
- [Commenting](#commenting)
- [Error Handling](#error-handling)

## Introduction

This document outlines the coding standards and best practices to follow when contributing to the project. Adhering to these guidelines helps maintain code quality, readability, and consistency across the project. Some guidelines are enforced by the code formatter (clang-format).

## General Guidelines

- Follow the **principle of least surprise**
  - The code should be intuitive and easy to follow
- Keep functions small and focused on a single task
- Strive for **readability** and **maintainability** of the code
- Stick to the **DRY** (Don't Repeat Yourself) principle

## Naming Conventions

### Variables

- Use **snake_case** for variable names
  - e.g., `user_name`, `total_amount`

### Functions

- Use **snake_case** for function names
  - e.g., `calculate_total()`, `fetch_user_data()`

### Struct and Enum

- Use **Pascal_Case** with underscore for struct and enum names
  - For example:

    ```c
    typedef struct GPIO_Registers
    {
        volatile uint32_t MODER;
        volatile uint32_t OTYPER;
        volatile uint32_t OSPEEDR;
        volatile uint32_t PUPDR;
        volatile uint32_t IDR;
        volatile uint32_t ODR;
        volatile uint32_t BSRR;
        volatile uint32_t LCKR;
        volatile uint32_t AFR[2];
        volatile uint32_t BRR;
    } GPIO_Registers;
    
    typedef enum
    {
        SPI_STATE_UNINIT,
        SPI_STATE_SUCCESS,
        SPI_STATE_FAIL
    } Spi_State;
    ```

### Constants

- Use **UPPERCASE** with underscores for constant names
  - e.g., `MAX_RETRY_COUNT`, `API_KEY`

### File Names

- Use **snake_case** for file names
  - e.g., `user_service.c`, `order_controller.c`
- One code module per header and source file
  - A module is something that makes sense as a single idea
  - e.g., io.h/io.c, uart.h/uart.c, i2c.h/i2c.c
- Prefix function names with module name
  - For example:

    ```c
    void mcu_init(void)
    void uart_init(void)
    void gpio_write(const uint16_t pin, const bool val)
    ```

## Code Formatting

### Indentation

- Use **4 spaces** for indentation
- Do not use tabs

### Line Length

- Keep lines of code under **100 characters**

### Curly Brackets

- Always use curly brackets `{}` for control structures, even for one-liners
  - For example:

    ```c
    if (true == success)
    {
        gpio_write(21, true);
    }
    ```

### Defines

- Define all constants and comment if it is unclear why
- Always use parenthesis (even for constants) to avoid unexpected macro expansion
  - For example:

    ```c
    // This constant ...
    #define CONSTANT_NUMBER                 (21U)
    ```

- Use `do{} while(0)` for multi-statement macros to increase readibility
  - For example:

    ```c
    #define ASSERT(expression)              \
      do                                    \
      {                                     \
          if(!(expression))                 \
          {                                 \
              assert_handler(pc);           \
          }                                 \
      } while(0)
    ```

  - Avoid macros when possible as they could lead to bugs and maintenance challenges
    - Common pitfalls with macros include:
      - Lack of type safety
      - Unintended multiple evaluations
      - Complex debugging from macro expansions

### Header Files

- Use `#include` guards in every header file to avoid duplicated and recursive inclusions
  - For example:

    ```c
    #ifndef HEADER_FILE_NAME_H_
    #define HEADER_FILE_NAME_H_

    // Your header file content here

    #endif // HEADER_FILE_NAME_H_
    ```

- Include header files in order from local to global
  - For example:

    ```c
    #include "drivers/i2c.h"
    #include "drivers/spi.h"

    #include "stm32l4s5xx.h"

    #include <stdio.h>
    ```

- Minimize the `#include` preprocessor directives in header files

### Switch Statements

- When using switch statement with an enum, handle all cases and skip the default case
  - Without the default case, the compiler warning will require the switch statement to be updated when a new enum is added
  - For example:

    ```c
    switch(spi_state)
    {
        case SPI_STATE_UNINIT:
            // ...
            break;
        case SPI_STATE_SUCCESS:
            // ...
            break;
        case SPI_STATE_FAIL:
            // ...
            break;
    }
    ```

### Functions

- Minimize scope of identifiers when possible
  - Define functions as `static` if they are used only inside a single translation unit (.c file)
- For functions that do not need parameter(s), use `void` as the parameter because C functions with empty parenthesis can be called with any number of parameters

### Structs

- For struct, use pass by pointer where possible to reduce stack usage

### Typedef

- Always use `typedef` for enum and struct to be more concise when using them
- Enum values should be `UPPER_SNAKE_CASE` and prefixed with enum name
  - For example:

    ```c
    typedef enum Spi_State
    {
        SPI_STATE_UNINIT,
        SPI_STATE_SUCCESS,
        SPI_STATE_FAIL
    } Spi_State;
    ```

- Do not use **_t** suffix as it is reserved for POSIX

### Fixed-Width Integers (stdint.h)

- Use fixed-width integers (uint8_t, int8_t, uint16_t, int16_t, uint32_t, etc.)
- Makes memory usage clear and porting easier

### Const Correctness

- All variables that should not change should be `const`
- Protects variables against accidental modification
- Makes code more explicit and readable
  - For example:

    ```c
    void gpio_write(const uint16_t pin, const bool val)
    ```

## Commenting

### General Guidelines

- Write comments only when the code is not self-explanatory
- Use comments to describe "why", not "how" and "what"
- Use `//` for single line comments and `/* */` for multi-line comments

### Documentation

- For enum, struct, and functions, use Doxygen comments to explain their purpose and usage
  - For example:

    ```c
    /**
     * @brief Calculate the sum of two numbers.
     *
     * @param a             First addend.
     * @param b             Second addend.
     *
     * @return uint32_t     Sum of the parameters.
     */
    uint32_t sum(const uint32_t a, const uint32_t b);
    ```

## Error Handling

- Handle errors gracefully
- Log meaningful error messages, when appropriate

### Asserts

- Asserts are used for catching obvious mistakes during development
- Use for detecting invalid function arguments, return value, and invalid variable content
- Do not use assertion for checking sensor readings
