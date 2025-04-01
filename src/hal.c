#include "hal.h"

static GPIO_TypeDef *gpio_bank(uint16_t pin)
{
    return GPIO(PINBANK(pin));
}

void gpio_init(const uint16_t pin, const GPIO_Modes mode, const GPIO_Output_Types output_type,
               const GPIO_Output_Speeds output_speed, const GPIO_Pull_Type pull_type,
               const uint8_t alt_func_num)
{
    GPIO_TypeDef *gpio = gpio_bank(pin);
    uint8_t pin_num = (uint8_t)PINNO(pin);

    // enable GPIO clock for pin
    RCC->AHB2ENR |= BIT(PINBANK(pin));

    SETBITS(gpio->MODER, 0x3UL << (pin_num * 2), ((uint32_t)mode) << (pin_num * 2));
    SETBITS(gpio->OTYPER, 0x1UL << pin_num, ((uint32_t)output_type) << pin_num);
    SETBITS(gpio->OSPEEDR, 0x3UL << (pin_num * 2), ((uint32_t)output_speed) << (pin_num * 2));
    SETBITS(gpio->PUPDR, 0x3UL << (pin_num * 2), ((uint32_t)pull_type) << (pin_num * 2));
    SETBITS(gpio->AFR[pin_num >> 3], 0xFUL << ((pin_num & 7) * 4),
            ((uint32_t)alt_func_num) << ((pin_num & 7) * 4));
}

void gpio_input(const uint16_t pin)
{
    gpio_init(pin, GPIO_MODE_INPUT, GPIO_OTYPE_PUSH_PULL, GPIO_OSPEED_HIGH, GPIO_PUPDR_NONE, 0);
}

void gpio_output(const uint16_t pin)
{
    gpio_init(pin, GPIO_MODE_OUTPUT, GPIO_OTYPE_PUSH_PULL, GPIO_OSPEED_HIGH, GPIO_PUPDR_NONE, 0);
}

bool gpio_read(const uint16_t pin)
{
    return gpio_bank(pin)->IDR & BIT(PINNO(pin)) ? true : false;
}

void gpio_toggle(const uint16_t pin)
{
    GPIO_TypeDef *gpio = gpio_bank(pin);
    uint32_t mask = BIT(PINNO(pin));

    gpio->BSRR = mask << (gpio->ODR & mask ? 16 : 0);
}

void gpio_write(const uint16_t pin, const bool val)
{
    gpio_bank(pin)->BSRR = BIT(PINNO(pin)) << (val ? 0 : 16);
}

void usart_init(USART_TypeDef *const usart, const uint32_t baud)
{
    uint8_t alt_func_num = 0;
    uint16_t tx_pin = 0, rx_pin = 0;
    uint32_t bus_frequency = 0;

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
        alt_func_num = 7;
    }
    else if(UART4 == usart)
    {
        alt_func_num = 8;
    }

    // define bus frequency
    if(USART1 == usart)
    {
        bus_frequency = APB2_FREQUENCY;
    }
    else if(UART4 == usart)
    {
        bus_frequency = APB1_FREQUENCY;
    }

    // initialize GPIO pins for USART
    gpio_init(rx_pin, GPIO_MODE_AF, GPIO_OTYPE_PUSH_PULL, GPIO_OSPEED_HIGH, GPIO_PUPDR_NONE,
              alt_func_num);
    gpio_init(tx_pin, GPIO_MODE_AF, GPIO_OTYPE_PUSH_PULL, GPIO_OSPEED_HIGH, GPIO_PUPDR_NONE,
              alt_func_num);

    // disable the parameterized USART
    usart->CR1 &= 0xC0000000;

    // clear and set new baud rate
    SETBITS(usart->BRR, USART_BRR_DIV_FRACTION | USART_BRR_DIV_MANTISSA, (bus_frequency / baud));

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
