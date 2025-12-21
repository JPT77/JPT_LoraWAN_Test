/**
 * @file hdc2080.c
 * @brief HDC2080 API. Low Power Humidity and Temperature Digital Sensor.
 */

#include "hdc2080.h"
#include "i2c.h"

uint8_t rx_buffer[5] = {0};
uint8_t tx_buffer[5] = {0};

static uint8_t cfg = HDC2080_NOT_CONFIGURED;

/*!
 * @brief Structure that contains board specific functions.
 */
static hdc2080_low_level_functions_TypeDef funcptr;

/*!
 * @brief Initialize HDC2080. Low level and configuration.
 */
void hdc2080_init(void)
{
  hdc2080_init_low_level();
}

/*!
 * @brief Deinitialize HDC2080.
 */
void hdc2080_deinit(void)
{
  hdc2080_deinit_low_level();
}

/*!
 * @brief Set low level functions.
 */
void hdc2080_set_low_level_functions(hdc2080_low_level_functions_TypeDef low_level_funcptr)
{
  funcptr = low_level_funcptr;
}

/*!
 * @brief Configure HDC2080 for single acquisition.
 */
void hdc2080_conf_single(void)
{
  hdc2080_reset();

  // Normal operation mode
  // Auto measurement mode
  // Heater off
  // DRDY/INT_EN = High Z
  // Interrupt polarity = 0
  // Interrupt mode = level sensitive
  hdc2080_write_register(HDC2080_RESET_DRDY_INT_CONF, 0x00);

  // 14 bit temperature resolution
  // 14 bit humidity resolution
  // Measure temperature and humidity
  // Measurement trigger = no action
  hdc2080_write_register(HDC2080_MEASUREMENT_CONF, 0x00);

  cfg = HDC2080_CONFIGURED_SINGLE;
}

void hdc2080_conf_continuous(uint8_t sample_rate)
{
  uint8_t reg;

  hdc2080_reset();

  // Data ready interrupt enable
  hdc2080_write_register(HDC2080_INTERRUPT_ENABLE, 0x80);

  hdc2080_set_sample_rate(sample_rate);

  // DRDY/INT_EN = enable
  // Interrupt polarity = 0
  reg = hdc2080_read_register(HDC2080_RESET_DRDY_INT_CONF);
  reg &= 0xF9;
  reg |= 0x04;    // Enable and active low
  hdc2080_write_register(HDC2080_RESET_DRDY_INT_CONF, reg);

  // 14 bit temperature resolution
  // 14 bit humidity resolution
  // Measure temperature and humidity
  // Measurement trigger = no action
  hdc2080_write_register(HDC2080_MEASUREMENT_CONF, 0x00);

  funcptr.en_drdy();
  funcptr.en_int();

  cfg = HDC2080_CONFIGURED_CONTINUOUS;
}

/*!
 * @brief Set interrupt thresholds for temperature and humidity.
 * @param temp_hum Upper 16 bits are temperature threshold
 * lower 16 bits are for humidity.
 */
void hdc2080_set_int_config(uint32_t temp_hum)
{
  hdc2080_set_temperature_thres(hdc2080_decode_temperature((uint16_t) (temp_hum >> 16)));
  hdc2080_set_humidity_thres(hdc2080_decode_humidity((uint16_t) (temp_hum)));
  hdc2080_write_register(HDC2080_INTERRUPT_ENABLE, 0x7F);	//0x60
}

/*!
 * @brief Set desired sample rate.
 * @param sample_rate The interval the sensor sample at.
 */
void hdc2080_set_sample_rate(uint8_t sample_rate)
{
  volatile uint8_t reg_contents;
  reg_contents = hdc2080_read_register(HDC2080_RESET_DRDY_INT_CONF);
  reg_contents &= HDC2080_SAMPLE_RATE_DEL_MASK;	// default = manual

  switch (sample_rate)
  {
    case HDC2080_MANUAL:
    case HDC2080_TWO_MINS:
    case HDC2080_ONE_MINS:
    case HDC2080_TEN_SECONDS:
      reg_contents |= sample_rate;
      break;
    default:
      break;
  }

  hdc2080_write_register(HDC2080_RESET_DRDY_INT_CONF, reg_contents);
}

/*!
 * @brief Set temperature threshold.
 * @param current_temperature Currently measured temperature.
 */
void hdc2080_set_temperature_thres(double current_temperature)
{
  if (current_temperature >= -40.0 && current_temperature < 125.0)
    {
      uint8_t temperature_8bit = (uint8_t)( ( ( current_temperature + 40 ) * 256.0 / 165.0 ) + 0.5 );
      uint8_t thres[2];
      thres[0] = (uint8_t) (((current_temperature - HDC2080_TEMP_DELTA_MIN + 40) * 256.0 / 165.0) + 0.5);
      if (thres[0] >= temperature_8bit)
        {
          thres[0] = temperature_8bit - 1;
        }
      thres[1] = (uint8_t) (((current_temperature + HDC2080_TEMP_DELTA_MIN + 40) * 256.0 / 165.0) + 0.5);
      if (thres[1] <= temperature_8bit)
        {
          thres[1] = temperature_8bit + 1;
        }
      hdc2080_write_burst(HDC2080_TEMP_THR_LOW, thres, 2);
    }
}

/*!
 * @brief Set humidity threshold.
 * @param current_humidity Currently measured humidity.
 */
void hdc2080_set_humidity_thres(double current_humidity)
{
  if (current_humidity >= 0.0 && current_humidity < 100.0)
    {
      uint8_t humidity_8bit = (uint8_t) (((current_humidity) / 100.0 * 256.0) + 0.5);
      uint8_t thres[2];
      thres[0] = (uint8_t) (((current_humidity - HDC2080_HUM_DELTA_MIN) / 100.0 * 256.0) + 0.5);
      if (thres[0] >= humidity_8bit)
        {
          thres[0] = humidity_8bit - 1;
        }
      thres[1] = (uint8_t) (((current_humidity + HDC2080_HUM_DELTA_MIN) / 100.0 * 256.0) + 0.5);
      if (thres[1] <= humidity_8bit)
        {
          thres[1] = humidity_8bit - 1;
        }
      hdc2080_write_burst(HDC2080_RH_THR_LOW, thres, 2);
    }
}

/*!
 * @brief Start sensor conversion with previously configured sample rate.
 */
void hdc2080_start_conversion(void)
{
  uint8_t reg_contents;
  reg_contents = hdc2080_read_register(HDC2080_MEASUREMENT_CONF);
  reg_contents |= 0x01;
  hdc2080_write_register(HDC2080_MEASUREMENT_CONF, reg_contents);
}

/*!
 * @brief Stop sensor conversion.
 */
void hdc2080_end_conversion(void)
{
  uint8_t reg_contents;
  reg_contents = hdc2080_read_register(HDC2080_MEASUREMENT_CONF);
  reg_contents &= 0xFE;
  hdc2080_write_register(HDC2080_MEASUREMENT_CONF, reg_contents);
}

/*!
 * @brief Wait for available data.
 */
void hdc2080_wait_for_data(bool *init)
{
  if (cfg == HDC2080_CONFIGURED_SINGLE)
    {
      funcptr.delay_ms(2);
    }
  else if (cfg == HDC2080_CONFIGURED_CONTINUOUS)
    {
      funcptr.drdy(init);
    }
}

/*!
 * @brief Get combined temperature and humidity (register value).
 * @return Temperature and humidity.
 */
uint32_t hdc2080_get_temperature_humidity(void)
{
  hdc2080_read_burst(HDC2080_TEMPERATURE_LOW, rx_buffer, 4);

  if (cfg == HDC2080_CONFIGURED_CONTINUOUS)
    {
      funcptr.en_int();
    }

  return ((rx_buffer[1] << 24) | (rx_buffer[0] << 16) | (rx_buffer[3] << 8) | (rx_buffer[2]));
}

/*!
 * @brief Decode register value of temperature to actual value.
 * @param temperature Register value of temperature.
 * @return Temperature value.
 */
double hdc2080_decode_temperature(uint16_t temperature)
{
  return ((((double) temperature / (double) HDC2080_2POW16) * 165.0) - 40.0);
}

/*!
 * @brief Decode register value of humidity to actual value.
 * @param humidity Register value of humidity.
 * @return Humidity value.
 */
double hdc2080_decode_humidity(uint16_t humidity)
{
  return ((double) humidity / (double) HDC2080_2POW16 * 100.0);
}

/*!
 * @brief Reset hdc2082 register contents.
 */
void hdc2080_reset(void)
{
  uint8_t reg_contents;
  reg_contents = hdc2080_read_register(HDC2080_RESET_DRDY_INT_CONF);

  reg_contents |= 0x80;
  hdc2080_write_register(HDC2080_RESET_DRDY_INT_CONF, reg_contents);
  funcptr.delay_ms(50);
}

/*!
 * @brief Check if HDC2080 decive ID is correct.
 */
bool hdc2080_validate_manufacturer_device_id(void)
{
  uint16_t u16_HDC2080_manufacturer_id;
  uint16_t u16_HDC2080_device_id;
  u16_HDC2080_manufacturer_id = hdc2080_get_manufacturer_id(  );
  u16_HDC2080_device_id = hdc2080_get_device_id(  );
  if( u16_HDC2080_manufacturer_id != 0x5449 || u16_HDC2080_device_id != 0x07D0)
  {
    return false;
  }
  return true;
}


/*!
 * @brief Get manufacturer id from the device
 * @return Manufacturer ID
 */
uint16_t hdc2080_get_manufacturer_id(void)
{
  hdc2080_read_burst(HDC2080_MANUFACTURER_ID_LOW, rx_buffer, 2);

  if (cfg == HDC2080_CONFIGURED_CONTINUOUS)
    {
      funcptr.en_int();
    }

  return ((rx_buffer[1] << 8) | (rx_buffer[0]));
}


/*!
 * @brief Get device id from the device
 * @return Device ID
 */
uint16_t hdc2080_get_device_id(void)
{
  hdc2080_read_burst(HDC2080_DEVICE_ID_LOW, rx_buffer, 2);

  if (cfg == HDC2080_CONFIGURED_CONTINUOUS)
    {
      funcptr.en_int();
    }

  return ((rx_buffer[1] << 8) | (rx_buffer[0]));
}


/*!
 * @brief Read one register.
 * @param reg Register to read.
 * @return Register value.
 */
uint8_t hdc2080_read_register(uint8_t reg)
{
  funcptr.i2c_read( HDC2080_ADDR_GND_CONNECT, reg, 1, rx_buffer, 1 );
  
  return rx_buffer[0];
}

/*!
 * @brief Read several registers at once.
 * @param reg Start register.
 * @param data Array containing data.
 * @param len How many registers to read.
 */
void hdc2080_read_burst(uint8_t reg, uint8_t *data, uint8_t len)
{
  funcptr.i2c_read( HDC2080_ADDR_GND_CONNECT, reg, 1, data, len );
}

/*!
 * @brief Write data to one register.
 * @param reg Register that should be written.
 * @param data Data to write.
 */
void hdc2080_write_register(uint8_t reg, uint8_t data)
{
  funcptr.i2c_write( HDC2080_ADDR_GND_CONNECT, reg, 1, &data, 1 );
}

/*!
 * @brief Write several registers at once.
 * @param reg Start register.
 * @param data Array containing data.
 * @param len How many registers to write.
 */
void hdc2080_write_burst(uint8_t reg, uint8_t *data, uint8_t len)
{
  funcptr.i2c_write( HDC2080_ADDR_GND_CONNECT, reg, 1, data, len );
}
