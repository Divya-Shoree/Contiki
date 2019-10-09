#include <stdio.h>
#include "contiki.h"
#include "dev/button-sensor.h"
#include "dev/light-sensor.h"
#include "dev/leds.h"
#include <stdio.h>

/*-------------------------------------------------------------------------*/
PROCESS(my_first_app_process, "My First App");
AUTOSTART_PROCESSES(&my_first_app_process);
/*--------------------------------------------------------------------------*/
static uint8_t active;
PROCESS_THREAD(my_first_app_process, ev, data)
{
  /* Declare variables required */
  static int i=652;
  
  PROCESS_BEGIN();
  printf("EE-%d is an awesome course at USC\n",i);
  active = 0;
  SENSORS_ACTIVATE(button_sensor);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event &&
                             data == &button_sensor);
    leds_toggle(LEDS_ALL);
    if(!active) {
      /* activate light sensor */
      SENSORS_ACTIVATE(light_sensor);
      printf("Light: %d\n", light_sensor.value(0));
    } else {
      /* deactivate light sensor */
      printf("Light: %d\n", light_sensor.value(0));
      SENSORS_DEACTIVATE(light_sensor);
    }
    active ^= 1;
    leds_toggle(LEDS_ALL);
  }
  PROCESS_END();
}
