#include "hal.h"

void gpio_set_mode(const uint16_t pin, const GPIO_Modes mode)
{
    GPIO_TypeDef *gpio_bank = GPIO(PINBANK(pin));
    int pin_number = PINNO(pin);

    // enable GPIO clock for pin
    RCC->AHB2ENR |= BIT(PINBANK(pin));

    // clear existing pin mode
    gpio_bank->MODER &= ~(3U << (pin_number * 2));

    // set new pin mode
    gpio_bank->MODER |= (mode & 3U) << (pin_number * 2);
}

void gpio_set_af(const uint16_t pin, const uint8_t af_num)
{
    GPIO_TypeDef *gpio_bank = GPIO(PINBANK(pin));
    int pin_number = PINNO(pin);

    // clear and set new alternate function number
    gpio_bank->AFR[pin_number >> 3] &= ~(15UL << ((pin_number & 7) * 4));
    gpio_bank->AFR[pin_number >> 3] |= ((uint32_t)af_num) << ((pin_number & 7) * 4);
}

void gpio_write(const uint16_t pin, const bool val)
{
    GPIO_TypeDef *gpio_bank = GPIO(PINBANK(pin));
    int pin_number = PINNO(pin);

    // write pin state
    gpio_bank->BSRR = (1U << pin_number) << (val ? 0 : 16);
}

void usart_init(USART_TypeDef *const usart, uint32_t baud)
{
    uint8_t af = 0;
    uint16_t tx_pin = 0, rx_pin = 0;

    // enable clock for USART peripheral
    if(USART1 == usart)
    {
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    }
    else if(UART4 == usart)
    {
        RCC->APB1ENR1 |= RCC_APB1ENR1_UART4EN;
    }

    // create Tx/Rx pins for USART peripheral
    if(USART1 == usart)
    {
        tx_pin = PIN('B', 6);
        rx_pin = PIN('B', 7);
    }
    else if(UART4 == usart)
    {
        tx_pin = PIN('A', 0);
        rx_pin = PIN('A', 1);
    }

    // define the alternate function number
    if(USART1 == usart)
    {
        af = 7;
    }
    else if(UART4 == usart)
    {
        af = 8;
    }

    // set mode and function for Tx/Rx pins
    gpio_set_mode(tx_pin, GPIO_MODE_AF);
    gpio_set_af(tx_pin, af);
    gpio_set_mode(rx_pin, GPIO_MODE_AF);
    gpio_set_af(rx_pin, af);

    // disable the parameterized USART
    usart->CR1 &= 0xC0000000;

    // clear and set new baud rate
    usart->BRR &= 0xFFFF0000;
    usart->BRR |= (uint16_t)(SYSCLK_FREQ / baud);

    // enable the parameterized USART and its transmitter and receiver
    usart->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

void spin(volatile uint32_t count)
{
    // execute NOP instruction a given number of times
    while(count--)
    {
        asm("nop");
    }
}
