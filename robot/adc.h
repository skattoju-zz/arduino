void adc_init();
void adc_clear_interrupt_flag();
int adc_conversion_in_progress();
int get_adc();

void adc_channel_select(int);
int get_avg_adc(); 

float get_distance();