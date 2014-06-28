
#include "pwr.h"
#include "timer.h"
#include "apps.h"

int main(void)
{
	//sleep a bit. this helps the voltage of the radio to settle. also you are not yet locked out of JTAG (which could happen if the chip goes immediately into standby mode)
	if(!PWR_WAS_I_IN_STANDBY)
		timer_sleep_ms(1500);


	//the ifdefs could be replaced by the ID resistor measurement... someday
#ifdef APP_TEMP
	app_temperature_sensor();
#endif

#ifdef APP_UART_GATEWAY
	app_uart_gateway();
#endif

#ifdef APP_WALLSOCKET
	app_wallsocket();
#endif


	return 0;
}
