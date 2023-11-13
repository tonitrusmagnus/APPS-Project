
#include "adc.hpp"

float adc::read() {
        int raw_value = adc1_get_raw(adc_channel_);
        float avg = adc_queue.average();

        if (abs(raw_value-avg) > 1) {
            adc_queue.add(raw_value);
            avg = adc_queue.average();
        }

        float voltage = (avg / 4095.0) * DEFAULT_VREF;
        
        return voltage;
    }
adc::adc(gpio_num_t adc_pin) : adc_queue(5)
{
    adc_pin_ = adc_pin;
    adc_channel_ = gpiotoadc1(adc_pin);
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(adc_channel_, ADC_ATTEN_DB_11);

    adc_queue.setLength(5);
}

adc::~adc()
{
}

adc1_channel_t gpiotoadc1(gpio_num_t number){
    adc1_channel_t output;
        switch(number)
    {
    case GPIO_NUM_32:
        output = ADC1_CHANNEL_4;
        break;
    case GPIO_NUM_33:
        output = ADC1_CHANNEL_5;
        break;
    case GPIO_NUM_34:
        output = ADC1_CHANNEL_6;
        break;
    case GPIO_NUM_35:
        output = ADC1_CHANNEL_7;
        break;
    default:
        output = ADC1_CHANNEL_2;
        break;
    }
    return output;
}