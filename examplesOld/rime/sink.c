/*
 * Copyright (c) 2007, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */
 
/**
 * \file
 *         Example of sink receiving messages from a single node
 * \author
 *         Sergio Diaz
 */
 
#include "contiki.h"
#include "net/rime/rime.h"
 
#include "dev/button-sensor.h"
 
#include "dev/leds.h"
 
#include <stdio.h>
 
#include "example-uni-temp.h" // Declares the struct environment
 
/*---------------------------------------------------------------------------*/
PROCESS(sink_receive, "Sink Receive"); //Through this process the Sink receives messages
  
AUTOSTART_PROCESSES(&sink_receive); //Start the process when the node boots
 
/*---------------------------------------------------------------------------*/
static void
recv_uc(struct unicast_conn *c, const linkaddr_t *from) //Receives the messages from the nodes and prints them
{
 
  printf("unicast message received from %d.%d\n",
  from->u8[0], from->u8[1]); // The sink informs from which node the message comes from
 
  struct environment envirRX; // Declare the structure envirRX
 
  packetbuf_copyto( &envirRX ); // Copy the message from the packet buffer to the structure called envirRX
 
  printf("Data\t"); // Print the string "Data"
  printf("%d\t", envirRX.sequence);  // Print the sequence number
  printf("%d\t", envirRX.temp); // Print the temperature value
  printf("%d\n", envirRX.light);  // Print the light value
 
}
static const struct unicast_callbacks unicast_callbacks = {recv_uc}; //Every time a packet arrives the function recv_uc is called
static struct unicast_conn uc; // Declares the unicast connection uc
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(sink_receive, ev, data) //Declares the protothread called sink_receive
{
  PROCESS_EXITHANDLER(unicast_close(&uc);) // Specify an action when a process exits
     
  PROCESS_BEGIN(); // Says where the process starts
 
  unicast_open(&uc, 146, &unicast_callbacks); //Opens a unicast connection
 
  while(1) {
      PROCESS_WAIT_EVENT(); //Wait for an event to be posted to the process
                            //Wait for the event of receiving a message
  }
 
  PROCESS_END(); //Says where the process ends
}
/*---------------------------------------------------------------------------*/
 
 
 
