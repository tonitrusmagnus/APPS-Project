idf_component_register(SRCS "statemachine.cpp"
                            "Apps_main.cpp" 
                            "main.cpp"

                            "drivers/led_driver.cpp"
                            "drivers/leds.cpp"
                            "drivers/buttons.cpp" 
                            "drivers/max7312.cpp"
                            "drivers/mp3_driver.cpp" 

                            "microphone/microphone_i2s.cpp"
                            "microphone/signalprocessing.cpp"

                            "helper_functions/queue.cpp"
                            "helper_functions/helper_functions.cpp"
                            "helper_functions/timer.cpp"
                    INCLUDE_DIRS ".")
