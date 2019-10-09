#ifndef COLLECT_DATA_H_
#define COLLECT_DATA_H_

#include "net/linkaddr.h"
#include "lib/list.h"

struct neighbor_data_list{
  LIST_STRUCT(list);
  struct ctimer periodic;
};
struct neighbor_data{
    struct neighbor_data *next;
    linkaddr_t addr;
    uint16_t rssi, lqi;
    uint16_t age;
};

void
neighbor_data_list_new(struct neighbor_data_list *data_list);

struct neighbor_data *
neighbor_data_list_find(struct neighbor_data_list *data_list,
                           const linkaddr_t *addr);

void
neighbor_data_list_add(struct neighbor_data_list *data_list, const linkaddr_t
                *addr, uint16_t received_rssi, uint16_t received_lqi);

void
neighbor_data_list_remove(struct neighbor_data_list *data_list,
                             const linkaddr_t *addr);

#endif /* COLLECT_DATA_H_ */
/** @} */
/** @} */

