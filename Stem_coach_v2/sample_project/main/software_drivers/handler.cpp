// #include "handler.hpp"


// #define tag "SSD1306"
// oled screen;

// adc pot_Hz(GPIO_NUM_34);
// adc pot_dB(GPIO_NUM_35);

// Led_Driver leds(LedPins,LED_AMOUNT);
// audio_driver Audio_driver;
// //Servo servo(27);

// handler::handler(/* args */)
// {
//     Audio_driver.driver_init();  
// }

// handler::~handler()
// {
// }

// void handler::init() {
//     screen.oled_init();
    
//     //screen.settext("Testing testing testing test test test",1);
// }

// void handler::handle(int Db, int Hz, int DbAverage, int HzAverage){
//     if((gpio_get_level(DBONLEDBUTTON) == 0) && (previousstate1 == 1)){
//         Mode = mode_frequency;
//         Audio_driver._driver(frequence_select);
//         skipbool = true;
//         vTaskDelay(50 / portTICK_PERIOD_MS);
//         previousstate1 = 0;
//     }
//     else if((gpio_get_level(DBONLEDBUTTON) == 1) && (previousstate1 == 0)){
//         previousstate1 = 1;
//     }
//     else if((gpio_get_level(HzOnLedButton) == 0) && (previousstate2 == 1)){
//         Mode = mode_volume;
//         Audio_driver._driver(volume_select);
//         skipbool = true;
//         vTaskDelay(50 / portTICK_PERIOD_MS);
//         previousstate2 = 0;
//     } 
//     else if((gpio_get_level(HzOnLedButton) == 1) && (previousstate2 == 0)){
//         previousstate2 = 1;
//     }

//     datatransfer tester;
//     pot_Hz_value = pot_Hz.read();
//     pot_dB_value = pot_dB.read();

//     thres_Hz = map(pot_Hz_value, POT_MIN, POT_MAX, HZ_THRES_MIN, HZ_THRES_MAX-1);
//     thres_dB = map(pot_dB_value, POT_MIN, POT_MAX, DB_THRES_MIN+1, DB_THRES_MAX);

//     int ledlevel = 0;
//     //int temp = 0;

//     if(Mode == mode_volume){
//         // Calculate how many of 8 leds turn on
//         //ledlevel = constrain(8 + floor((Db-thres_dB)/4), 1, 8);
        
//         // Calculate fraction
//         float dB_fraction = (float)(constrain(Db,DB_THRES_MIN,thres_dB) - DB_THRES_MIN) / (float)(thres_dB - DB_THRES_MIN);

//         //tester.uartTest(dB_fraction*100);

//         ledlevel = dB_fraction * 8;
        

//         if(HzAverage < thres_Hz){//frequency low enough
//             Audio_driver._driver(frequence_good);
//         }
//         else{//frequency to high
//             Audio_driver._driver(frequence_down);
//         }
//     }
//     else{ //mode_frequency
//         // Calculate how many of 8 leds turn on
//         //ledlevel = constrain(8 - floor((Hz-thres_Hz)/10), 1, 8);
//         float Hz_fraction = (float)(HZ_THRES_MAX - (constrain(Hz,thres_Hz,HZ_THRES_MAX) )) / (float)(HZ_THRES_MAX - thres_Hz);
        
//         //tester.uartTest(Hz_fraction*100);
        
//         ledlevel = constrain( Hz_fraction*8, 0, 8);

//         if(DbAverage < thres_dB){
//             Audio_driver._driver(volume_up);
//         }
//         else{
//             Audio_driver._driver(volume_good);
//         }      
//     }

//     char _String[100] = "";
//     sprintf(_String,"Hz: %d,\tdB: %d\n",thres_Hz, thres_dB);
//     screen.settext(_String,1);

//     leds.setLevel(ledlevel);
// }