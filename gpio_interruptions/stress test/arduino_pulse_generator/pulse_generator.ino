#define PIN_OUTPUT_1 LED_BUILTIN
#define PIN_OUTPUT_2 9

/* Macro to be used ina 10ms loop */
#define TIME_MS(_ms) (_ms * 16)

typedef struct {
    unsigned int start_delay;
    unsigned int pulses;
    unsigned int pulse_time;
    unsigned int pin;
} pin_config_t;

/* Pulse Connfigurations
                    Start Delay  # Pulses    Pulse Duration      PIN        */
pin_config_t out1 = {TIME_MS(0),    10,      TIME_MS(0.1), PIN_OUTPUT_1};
pin_config_t out2 = {TIME_MS(0),    10,      TIME_MS(0.1), PIN_OUTPUT_2};

unsigned int counter = 0;
unsigned int cycle_time = TIME_MS(1000);

#define DEBUG 0

#if DEBUG == 1
#define DEBUG(_c, _pin) Serial.print(_c); Serial.println(_pin)
#else
#define DEBUG(_c, _pin)
#endif

void pulse_generator(pin_config_t *cfg)
{
    if (counter < cfg->start_delay)
    {
      digitalWrite(cfg->pin, LOW);
    }
    else
    {
      if (counter >= (cfg->pulse_time * cfg->pulses) + cfg->start_delay)
      {
        digitalWrite(cfg->pin, LOW);
      }
      else
      {
        if ((counter % cfg->pulse_time) == 0)
        {
          digitalWrite(cfg->pin, !digitalRead(cfg->pin));
        }
      }
      
    }  
}
/***************************************************************************************/
/* Configuration */
/***************************************************************************************/

void setup() {

  /* PIN init */
  pinMode(PIN_OUTPUT_1, OUTPUT);
  pinMode(PIN_OUTPUT_2, OUTPUT);

  /* Serial init */
  Serial.begin(9600);

  noInterrupts();           // disable all interrupts

  TCCR1A = 0;

  TCCR1B = 0;

  TCNT1  = 0;


  OCR1A = (unsigned int)(16000000/256)/16000;            // compare match register 16MHz/256/2Hz

  TCCR1B |= (1 << WGM12);   // CTC mode

  TCCR1B |= (1 << CS12);    // 256 prescaler 

  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt

  interrupts();             // enable all interrupts
  
  delay(10000);
}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{

  pulse_generator(&out1);
  pulse_generator(&out2);
  if (counter >= cycle_time)counter = 0;
  counter++;

}

/***************************************************************************************/
/* Main Loop */
/***************************************************************************************/
void loop() {

    //pulse_generator(&out1);
    //pulse_generator(&out2);
    //DEBUG(counter, digitalRead(PIN_OUTPUT_1));
    //if (counter >= cycle_time)counter = 0;
    //counter++;
    delay(1);
}
