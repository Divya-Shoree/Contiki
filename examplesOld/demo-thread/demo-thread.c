#include "contiki.h"
#include "sys/mt.h"
#include <stdio.h>

PROCESS(mythread, "demo thread");
AUTOSTART_PROCESSES(&mythread);

static int count;

void thread_compute(void *data)
{
	printf("calling thread_compute()\n");
	int num = *((int*)data);
	printf("num: %d\n", num);
	int i;
	int val = 0;
	for(i = 0; i < num; i++){
		val += 2;
	}
	printf("num: %d, total: %d\n", num, val);
	count++;
	mt_yield();
	mt_exit();
}

PROCESS_THREAD(mythread, ev, data)
{
	PROCESS_BEGIN();
	printf("demo threading\n");

	static struct mt_thread thread1;
	static struct mt_thread thread2;

	int n1=10, n2=20;

	mt_init();
	mt_start(&thread1, thread_compute, &n1);
	mt_start(&thread2, thread_compute, &n2);

	mt_exec(&thread1);
	mt_exec(&thread2);

	while(1){
		if(count>1){
			mt_stop(&thread1);
			mt_stop(&thread2);
			break;
		}
	}

	printf("thread exit\n");
	mt_remove();

	PROCESS_END();
}
