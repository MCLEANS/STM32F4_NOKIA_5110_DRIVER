#include "stm32f4xx.h"
#include "clockconfig.h"
#include "NOKIA_5110.h"
#include <stdlib.h>

#define RST_PORT GPIOD
#define RST_PIN 0
#define CS_PORT GPIOD
#define CS_PIN 1
#define DC_PORT GPIOD
#define DC_PIN 2

custom_libraries::clock_config system_clock;
custom_libraries::NOKIA_5110 NOKIA(CS_PORT,
                                    CS_PIN,
                                    RST_PORT,
                                    RST_PIN,
                                    DC_PORT,
                                    DC_PIN,
                                    SPI1,
                                    GPIOA,
                                    5,
                                    7,
                                    6,
                                    64);

int main(void) {
  
  system_clock.initialize();

  int second = 0;
	int minute = 10;

  while(1){

    second++;
		if(second > 59){
			second = 0;
			minute++;
		}

		char ch_seconds[4];
		char ch_minutes[4];

		itoa(second,ch_seconds,10);
		itoa(minute,ch_minutes,10);

		NOKIA.print(ch_minutes,20,3);
		NOKIA.print(":",35,3);
		NOKIA.print(ch_seconds,45,3);
		
		for(volatile int i = 0; i < 5000000; i++){}
		NOKIA.clear();

  }
}
