#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

void usart_init(void)
{
/* Enable clocks for GPIO port A (for GPIO_USART2_TX) and USART1. */
rcc_periph_clock_enable(RCC_USART1);
rcc_periph_clock_enable(RCC_GPIOA);

/* Setup GPIO pin GPIO_USART1_TX/GPIO9 on GPIO port A for transmit. */
gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9);
gpio_set_af(GPIOA, GPIO_AF1, GPIO9);

/* Setup UART parameters. */
usart_set_baudrate(USART1, 115200);
usart_set_databits(USART1, 8);
usart_set_stopbits(USART1, USART_CR2_STOP_1_0BIT);
usart_set_mode(USART1, USART_MODE_TX);
usart_set_parity(USART1, USART_PARITY_NONE);
usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

/* Finally enable the USART. */
usart_enable(USART1);
}
