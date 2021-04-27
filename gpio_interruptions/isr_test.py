import time
import RPi.GPIO as GPIO

# Global variables
pin5_prevState = 0
pin5_counter = 0
pin5_prevCounter = 0
pin5_time = time.time()

pin12_prevState = 0
pin12_counter = 0
pin12_prevCounter = 0
pin12_time = time.time()

'''
1 - Handle all interruption in the same callback function
2 - Do your own border detection
3 - Check for all pins on every interruption
4 - Do your own debouce for buttons
'''
def pin_handler(pin):
	global pin5_counter, pin5_time, pin5_prevState
	global pin12_counter, pin12_time, pin12_prevState
	
	pin5_state = GPIO.input(5)
	pin12_state = GPIO.input(12)
	
	if (pin5_state != pin5_prevState):
		pin5_prevState = pin5_state
		
		pin5_time = time.time()
		pin5_counter += 1
		print("Pin5 - State:", pin5_state, " Counter: ", pin5_counter)
	
	if (pin12_state != pin12_prevState):
		pin12_prevState = pin12_state
		
		pin12_time = time.time()
		pin12_counter += 1
		print("Pin12 - State:", pin12_state, " Counter: ", pin12_counter)


# GPIO configuration
GPIO.setmode(GPIO.BCM)

GPIO.setup(5, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.add_event_detect(5, GPIO.BOTH, pin_handler)

GPIO.setup(12, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.add_event_detect(12, GPIO.BOTH, pin_handler)


while True:
	# Debounce
	#if (pin5_counter != pin5_prevCounter) and (time.time()-pin5_time > 0.2):
	#	   pin5_prevCounter = pin5_counter
	#	   print("Switch event:", GPIO.input(5))
		   
	time.sleep(0.1)

