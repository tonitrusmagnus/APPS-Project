#include "buttons.hpp"

// #define GPIO_INPUT_IO_0     CONFIG_GPIO_INPUT_0
// #define GPIO_INPUT_IO_1     CONFIG_GPIO_INPUT_1
#define GPIO_INPUT_PIN_SEL  ((1ULL<<GPIO_INPUT_IO_0) | (1ULL<<GPIO_INPUT_IO_1))

void IRAM_ATTR Button::gpio_isr_handler(void* arg)
{
    Button* instance = reinterpret_cast<Button*>(arg);
    if (instance) {
        instance->onButtonPress();
    }
}

// Initialize the static member variable
bool Button::isrServiceInstalled = false;

void Button::onButtonPress() {
    flag = true;
}

void Button::init(gpio_num_t buttonpin, edge interruptedge){

    gpio_config_t io_conf = {};
    io_conf.intr_type = (interruptedge==rising) ? GPIO_INTR_POSEDGE: GPIO_INTR_NEGEDGE,
    io_conf.pin_bit_mask = (1ULL<<buttonpin),
    io_conf.mode = GPIO_MODE_INPUT,
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);

    if(isrServiceInstalled == false){
        gpio_install_isr_service(0); //install gpio isr service
        isrServiceInstalled = true;
    }
    
    gpio_isr_handler_add(buttonpin, Button::gpio_isr_handler, (void*) this); //hook isr handler for specific gpio pin

}
Button::Button(){

}

 
Button::~Button() {
}