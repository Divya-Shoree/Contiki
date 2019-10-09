#include "contiki.h"
#include "dev/sht11/sht11-sensor.h"
#include "dev/light-sensor.h"
#include "dev/leds.h"
#include <stdio.h>

/*--------------------------------------------------------------------------*/
PROCESS(sense_temperature, "Test temperature sensing");
AUTOSTART_PROCESSES(&sense_temperature);
/*--------------------------------------------------------------------------*/

PROCESS_THREAD(sense_temperature, ev, data)
{
	static struct etimer et;
	static int temperature;
	static int humidity;

	PROCESS_BEGIN();

	SENSORS_ACTIVATE(sht11_sensor);
	etimer_set(&et, CLOCK_SECOND * 30);
	temperature = sht11_sensor.value(SHT11_SENSOR_TEMP);
	humidity = sht11_sensor.value(SHT11_SENSOR_HUMIDITY);
	while(1){
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		printf("Temperature is %d\n, Humidity is %d\n", temperature, humidity);
		etimer_set(&et, CLOCK_SECOND * 30);

	}
	SENSORS_DEACTIVATE(sht11_sensor);
	PROCESS_END();
}
