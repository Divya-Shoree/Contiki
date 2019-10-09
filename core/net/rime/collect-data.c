/*
 * Copyright (c) 2006, Swedish Institute of Computer Science.
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

#include <limits.h>
#include <stdio.h>

#include "contiki.h"
#include "lib/memb.h"
#include "lib/list.h"

#include "net/rime/collect-data.h"
#include "net/rime/collect.h"

#define MAX_COLLECT_NEIGHBORS 8
#define WORST_RSSI -70
#define WORST_LQI 50
#define MAX_AGE 180
#define PERIODIC_INTERVAL CLOCK_SECOND * 60


MEMB(neighbor_data_mem, struct neighbor_data, MAX_COLLECT_NEIGHBORS);

/*--------------------------------------------------------------------------*/
static void
periodic(void *ptr)
{
  struct neighbor_data_list *data_list;
  struct neighbor_data *d;

  data_list = ptr;
printf("peridoic ");
  /*Go through all collected neighbors and increade their age.*/
  for(d = list_head(data_list->list); d != NULL; d = list_item_next(d)){
    d->age++;
  }
 
  for(d = list_head(data_list->list); d != NULL; d = list_item_next(d)){ 
    if((d->age % 4) == 0){
      memb_free(&neighbor_data_mem, d);
      list_remove(data_list->list, d);
      d = list_head(data_list->list);
     }
  }
 
  ctimer_set(&data_list->periodic, PERIODIC_INTERVAL,
		  periodic, data_list);
}

/*---------------------------------------------------------------------------*/
void
neighbor_data_list_new(struct neighbor_data_list *data_list)
{
  LIST_STRUCT_INIT(data_list, list);
  list_init(data_list->list);
  ctimer_set(&data_list->periodic, CLOCK_SECOND, periodic, data_list);
}
/*---------------------------------------------------------------------------*/
struct neighbor_data *
neighbor_data_list_find(struct neighbor_data_list *data_list,
                           const linkaddr_t *addr)
{
  struct neighbor_data *d;
  if(data_list == NULL) {
    return NULL;
  }
  for(d = list_head(data_list->list); d != NULL; d = list_item_next(d)) {
    if(linkaddr_cmp(&d->addr, addr)) {
      return d;
    }
  }
  return NULL;
}

/*---------------------------------------------------------------------------*/
void
neighbor_data_list_add(struct neighbor_data_list *data_list, const linkaddr_t
	       	*addr, uint16_t received_rssi, uint16_t received_lqi)
{
  struct neighbor_data *d;
  if(addr == NULL){
    return 0;
  }
  if(data_list == NULL){
    return 0;
  }
  for(d = list_head(data_list->list); d != NULL; d = list_item_next(d)){
    if(linkaddr_cmp(&d->addr, addr)){
      //Already on the list and the link quality has degraded
      //remove it
      if(received_rssi < WORST_RSSI || received_lqi < WORST_LQI){
        //list_remove(data_list->list, d);
        //memb_free(&neighbor_data_mem, d);
      }
      break;
    }
  }
  /*If the neighbor was not on the list and it was good link estimates
   * we try to allocate memory to it.
   */
  if(d == NULL){
    d = memb_alloc(&neighbor_data_mem);
    if(d != NULL ){
      if(received_rssi > WORST_RSSI ){
        list_add(data_list->list, d);
       }
     }
  }

  /*If there is not enough memory we recycle an old Neighbor*/


  if(d != NULL){
    linkaddr_copy(&d->addr, addr);
    d->age = 0;
    d->rssi = received_rssi;
    d->lqi = received_lqi;
  }


}
/*---------------------------------------------------------------------------*/
void
neighbor_data_list_remove(struct neighbor_data_list *data_list,
                             const linkaddr_t *addr)
{
  struct neighbor_data *d;

  if(data_list == NULL) {
    return;
  }

  d = neighbor_data_list_find(data_list, addr);

  if(d != NULL && d->rssi < WORST_RSSI) {
      list_remove(data_list->list, d);
      memb_free(&neighbor_data_mem, d);
  }
}
/*---------------------------------------------------------------------------*/

