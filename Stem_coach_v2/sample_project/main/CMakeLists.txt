idf_component_register(SRCS 
                        "hardware_drivers/servo_driver.cpp"
                        "hardware_drivers/Led_Driver.cpp"
                        "hardware_drivers/leds.cpp"
                        "hardware_drivers/oled.cpp"
                        "hardware_drivers/adc.cpp" 
                        "hardware_drivers/buttons.cpp" 
                        
                        "microphone/microphone_i2s.cpp"
                        "microphone/signalprocessing.cpp"

                        "helper_functions/queue.cpp"
                        "helper_functions/helper_functions.cpp"
                        "helper_functions/timer.cpp"

                        "software_drivers/mp3_driver.cpp" 
                        "software_drivers/uart.cpp"

                        "Apps_main.cpp"  
                        "statemachine.cpp"
                        "main.cpp" 
                    INCLUDE_DIRS ".")
