/*************************************************** 
  This is an example for the SHT4x Humidity & Temp Sensor

  Designed specifically to work with the SHT4x sensor from Adafruit
  ----> https://www.adafruit.com/products/4885

  These sensors use I2C to communicate, 2 pins are required to  
  interface
 ****************************************************/

#include <stdio.h>
#include "freertos/FreeRTOS.h"

#include "driver/gpio.h"
#include "driver/i2c_master.h"

#include "Adafruit_SHT4x.h"

Adafruit_SHT4x sht4 = Adafruit_SHT4x();

i2c_master_bus_handle_t bus_handle;
#define I2C_MASTER_SCL_IO GPIO_NUM_20
#define I2C_MASTER_SDA_IO GPIO_NUM_19

extern "C" void app_main(void)
{
  printf("Adafruit SHT4x test!\n");

  printf("Initlise I2C bus.\n");

  i2c_master_bus_config_t i2c_bus_config = {
    .i2c_port = I2C_NUM_0,
    .sda_io_num = I2C_MASTER_SDA_IO,
    .scl_io_num = I2C_MASTER_SCL_IO,
    .clk_source = I2C_CLK_SRC_DEFAULT,
    .glitch_ignore_cnt = 7,
    .intr_priority = 0,
    .trans_queue_depth = 0,
    .flags = { 
        .enable_internal_pullup = true,
        .allow_pd = false
    }
  };

  ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_bus_config, &bus_handle));
  
  printf("Initlise SHT4x\n");
  if (! sht4.begin(&bus_handle)) 
  {
    printf("Couldn't find SHT4x\n");
    while (1) vTaskDelay(portTICK_PERIOD_MS);
  }

  printf("Found SHT4x sensor\n");
  printf("Serial number 0x%lX\n", sht4.readSerial() );

  // You can have 3 different precisions, higher precision takes longer
  sht4.setPrecision(SHT4X_HIGH_PRECISION);
  switch (sht4.getPrecision()) 
  {
    case SHT4X_HIGH_PRECISION: 
      printf("High precision\n");
      break;
    case SHT4X_MED_PRECISION: 
      printf("Med precision\n");
      break;
    case SHT4X_LOW_PRECISION: 
      printf("Low precision\n");
      break;
  }

  // You can have 6 different heater settings
  // higher heat and longer times uses more power
  // and reads will take longer too!
  sht4.setHeater(SHT4X_NO_HEATER);
  switch (sht4.getHeater()) 
  {
    case SHT4X_NO_HEATER: 
      printf("No heater\n");
      break;
    case SHT4X_HIGH_HEATER_1S: 
      printf("High heat for 1 second\n");
      break;
    case SHT4X_HIGH_HEATER_100MS: 
      printf("High heat for 0.1 second\n");
      break;
    case SHT4X_MED_HEATER_1S: 
      printf("Medium heat for 1 second\n");
      break;
    case SHT4X_MED_HEATER_100MS: 
      printf("Medium heat for 0.1 second\n");
      break;
    case SHT4X_LOW_HEATER_1S: 
      printf("Low heat for 1 second\n");
      break;
    case SHT4X_LOW_HEATER_100MS: 
      printf("Low heat for 0.1 second\n");
      break;
  };

  printf("Main Loop:\n");
  sensors_event_t humidity, temp;
  uint32_t timestamp;
  while(1)
  {
      timestamp = esp_timer_get_time();
      sht4.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
      timestamp = esp_timer_get_time() - timestamp;

      printf("Temperature: %f degrees C\n", temp.temperature);
      printf("Humidity: %f %% rH", humidity.relative_humidity); 

      printf("Read duration (ms): %lu\n", timestamp);
      vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

