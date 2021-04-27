#include <stdio.h>
#include <time.h>
#include <chrono>
#include <wiringPi.h>

using namespace std;

int pin5_counter = 0;
int pin5_lastState = 0;
int pin5_lastCounter = 0;
chrono::time_point<chrono::system_clock> pin5_time;
int pin12_counter = 0;
int pin12_lastState = 0;

void pin_isr_callback()
{
	int pin5_state = digitalRead(5);
	int pin12_state = digitalRead(12);
	
	if(pin5_lastState != pin5_state)
	{
		pin5_counter += 1;
		pin5_lastState = pin5_state;
		pin5_time = chrono::high_resolution_clock::now();
		printf("Pin 5: status %d, counter %d\r\n", digitalRead(5), pin5_counter);
	}

	if(pin12_lastState != pin12_state)
	{
		pin12_counter += 1;
		pin12_lastState = pin12_state;
		printf("Pin 12: status %d, counter %d\r\n", digitalRead(12), pin12_counter);
	}
}

int main(int arc, char **argv)
{
	wiringPiSetupGpio(); /* init broadcom pin numbers */
	pinMode(5, INPUT);
	pinMode(12, INPUT);
	
	pullUpDnControl(5, PUD_UP);
	pullUpDnControl(12, PUD_UP);
	
	wiringPiISR(5, INT_EDGE_BOTH, pin_isr_callback);
	wiringPiISR(12, INT_EDGE_BOTH, pin_isr_callback);
	
	while(1)
	{
		auto t_now = chrono::high_resolution_clock::now();
		if(pin5_counter != pin5_lastCounter)
		{
			auto diff = chrono::duration_cast<chrono::milliseconds>(t_now - pin5_time).count();
			
			if(diff > 50)
			{
				printf("Pin 5 debounce event: %d\r\n", digitalRead(5));
				pin5_lastCounter = pin5_counter;
			}
		}
			
		delay(50);
	}
}
