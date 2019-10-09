#include "contiki.h"
#include "dev/sht11/sht11-sensor.h"
#include "dev/light-sensor.h"
#include "dev/leds.h"
#include <stdio.h>
//Declare the process
PROCESS(send_sensor_info_process, "Print the Sensors Information");
 
//Make the process start when the module is loaded
AUTOSTART_PROCESSES(&send_sensor_info_process);
 
/*---------------------------------------------------------------------------*/
static int
get_light(void) //Get the light value from sensor
{
  return 10 * light_sensor.value(LIGHT_SENSOR_PHOTOSYNTHETIC) / 7;
}
/*---------------------------------------------------------------------------*/
static int
get_temp(void) //Get the temperature value from sensor
{
  return ((sht11_sensor.value(SHT11_SENSOR_TEMP) / 10) - 396) / 10;
}
/*---------------------------------------------------------------------------*/
 
int sequence = 0; // A sequence number that enumerates the data from 0 and increases in 1.
 
//Define the process code
PROCESS_THREAD(send_sensor_info_process, ev, data)
{
 
 
  static struct etimer et; // Struct used for the timer
 
  PROCESS_BEGIN(); 
 
  
  while(1){
  
  etimer_set(&et, CLOCK_SECOND * 5); // Configure timer to expire in 5 seconds
 
  SENSORS_ACTIVATE(light_sensor); // Activate light sensor
  SENSORS_ACTIVATE(sht11_sensor); // Activate temperature sensor
 
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et)); // Wait until timer expires 
 
  printf("Data\t", get_temp()); // Print the string "Data"
  printf("%d\t", sequence++);  // Print the sequence number
  printf("%d\t", get_temp()); // Print the temperature value
  printf("%d\n", get_light());  // Print the light value

  etimer_reset(&et); // Reset timer
 
  SENSORS_DEACTIVATE(light_sensor); // Deactivate light sensor
  SENSORS_DEACTIVATE(sht11_sensor);  // Deactivate temperature sensor
 
  }
   
  PROCESS_END();
}
 
