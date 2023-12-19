
#include "adc.hpp"

float adc::read() {
    int raw_value = adc1_get_raw(adc_channel_);
    float avg = adc_queue.average();

    if (abs(raw_value-avg) > 1) {
        adc_queue.add(raw_value);
        avg = adc_queue.average();
    }

    float voltage = (avg / 4095.0) * default_Vref;
    
    return voltage;
}

adc::adc(adc1_channel_t adc1_channel) : adc_queue(5)
{
    adc_channel_ = adc1_channel;
    ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_12));
    ESP_ERROR_CHECK(adc1_config_channel_atten(adc_channel_, ADC_ATTEN_DB_11));

    adc_queue.setLength(5);
}

adc::~adc()
{
}
