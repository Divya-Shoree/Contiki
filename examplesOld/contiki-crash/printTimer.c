#include "contiki.h"
#include "sys/node-id.h" //Included to be able to set node's ID.
#include "dev/button-sensor.h"
#include <stdio.h>

//Print hello world if the button is pressed or if the timer ticks off..

/*-------------------------------------------------------------------*/
PROCESS(hello_world_process, "Hello world process");
AUTOSTART_PROCESSES(&hello_world_process);
/*-------------------------------------------------------------------*/
static struct etimer et;
static uint8_t counter;
PROCESS_THREAD(hello_world_process, ev, data)
{
	PROCESS_BEGIN();
	counter = 0;
	SENSORS_ACTIVATE(button_sensor);
	etimer_set(&et, CLOCK_SECOND * 5);
	while(1)
	{
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et) ||ev == sensors_event);// && data == &button_sensor);
		printf("counter = %d\t",++counter);
		if(ev == sensors_event){
			printf("Button pressed");
		}
		else{
			printf("Hello World!");
		}
		etimer_set(&et, CLOCK_SECOND * 5);
	}
	PROCESS_END();
}
/*-------------------------------------------------------------------*/
