idf_component_register(SRCS "Adafruit_SHT4x.cpp"
                    REQUIRES Adafruit_Sensor
                    REQUIRES esp_driver_i2c esp_timer
                    INCLUDE_DIRS "include")
