#include "buttons.hpp"

void IRAM_ATTR Button::gpio_isr_handler(void *arg)
{
    // Get class instance 
    Button *instance = reinterpret_cast<Button *>(arg);
    if (instance) {
        instance->onButtonPress();
    }
}

// buttonpin_ should be a GPIO_NUM_xx
// interruptedge_ choose between GPIO_INTR_POSEDGE and GPIO_INTR_NEGEDGE
Button::Button(gpio_num_t buttonpin_, gpio_int_type_t interruptedge_)
{
    buttonpin = buttonpin_;
    interruptedge = interruptedge_;
}

void Button::onButtonPress()
{
    flag = true;
}

void Button::init() {
    gpio_config_t io_conf = {};
    io_conf.intr_type = interruptedge, 
    io_conf.pin_bit_mask = (1ULL << buttonpin),
    io_conf.mode = GPIO_MODE_INPUT,
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    ESP_ERROR_CHECK(gpio_config(&io_conf)); // Set configuration

    esp_err_t err = gpio_install_isr_service(0); // Install gpio isr service
    
    if (err != ESP_ERR_INVALID_STATE) { // Ignore the "ISR service is allready installed" error
        ESP_ERROR_CHECK(err);
    }

    // hook isr handler for specific gpio pin
    ESP_ERROR_CHECK(gpio_isr_handler_add(buttonpin, Button::gpio_isr_handler, (void *)this)); 
}

bool Button::getFlag() {
    return flag;
}

void Button::resetFlag() {
    flag = false;
}