CONTIKI = ../..
ifndef TARGET
TARGET=sky
endif

all: blink sky-collect #rt-leds test-button test-cfs tcprudolph0 TemperatureSky

%.tgz: %.ihex
	mkdir $(basename $<) ; \
	mv $< $(basename $<) ; \
	echo $(basename $<)/$(basename $<).ihex 600 > $(basename $<)/runfile ; \
	tar czf $@ $(basename $<)

CONTIKI_WITH_IPV4 = 1
CONTIKI_WITH_RIME = 1
include $(CONTIKI)/Makefile.include
