#include "contiki.h"
#include "dev/leds.h"
#include <stdio.h>
char hello[] = "hello from the mote!";
//---------------------------------------------------------------
PROCESS(led_process, "led process");
AUTOSTART_PROCESSES(&led_process);
//---------------------------------------------------------------
PROCESS_THREAD(led_process, ev, data)
{       
        PROCESS_BEGIN();
        leds_on(LEDS_RED);
	printf("%s\n",hello);
	printf("The LED %u is %u\n", LEDS_RED, leds_get());
        PROCESS_END();
}         
