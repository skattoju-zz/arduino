#include <avr/io.h>
#include "adc.h"

/* Sensor Data */

uint8_t adc_data[5];
static float dist_data[2];
const float voltages[] = {0.365, 0.552, 0.883, 1.276, 1.636, 2.303};
const int distances[]= {80, 50, 30, 20, 15, 10};

/* initializes the ADC */
void adc_init(){
  DDRC = 0b00000000;
  ADCSRA = _BV(ADEN) | _BV(ADPS0); //enable adc
  DIDR0 = 0b00111111;
  ADMUX |= _BV(ADLAR);
}

/* clears the ADC interrupt flag */
void adc_clear_interrupt_flag(void)
{
  ADCSRA |= _BV(ADIF);
}

/* returns boolean value indicating if the ADC conversion is still in prgress */
int adc_conversion_in_progress(void){
  return !(ADCSRA & _BV(ADIF));
}

int get_adc(){
  uint8_t adc_value;        /* sensor ADC value */
  ADCSRA |= _BV(ADSC);      // start ADC conversion

  while(adc_conversion_in_progress());

  adc_value = ADCH;
  adc_clear_interrupt_flag();

  return(adc_value);
}

void adc_channel_select(int channel){
  if ((channel <= 5) && (channel >= 0)) {
    // set the channel in ADMUX register; left-align result
    ADMUX = channel | _BV(ADLAR);
    // discard the first conversion
    get_adc();
  }
}

/* get ADC Value */
int get_avg_adc(){

  int sum = 0;
  int avg;
  int i = 0;

  for(i=0; i < 10; i++){

    adc_data[i%5] = get_adc();
    sum += adc_data[i%5];
  }

  avg = sum/10;

  //_serial_write((unsigned char)(avg+0x30));

  return avg;
}

float get_distance(){

	//uint8_t distance;
	//uint8_t v1;
	//uint8_t d1;
	uint8_t adc_value = get_avg_adc();

	if (adc_value > voltages[0]) {
		int i =0;
		for (i=0; i < 5; i++) {

			if (adc_value < voltages[i]) {
				return (adc_value / voltages[i])*distances[i];
			}
		}
	}

	return 0;

}


