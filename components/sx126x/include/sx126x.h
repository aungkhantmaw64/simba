#ifndef __SX126X_H__
#define __SX126X_H__

#include <stddef.h>
#include <stdint.h>

typedef enum sx126x_standby_mode {
  SX126X_STBY_RC = 0,
  SX126X_STBY_XOSC = 1,
} sx126x_standby_mode_t;

typedef enum sx126x_packet_type {
  SX126X_PACKET_TYPE_GFSK = 0x00,
  SX126X_PACKET_TYPE_LORA = 0x01,
} sx126x_packet_type_t;

typedef struct sx126x_pa_config {
  uint8_t pa_duty_cycle;
  uint8_t hp_max;
  uint8_t device_sel;
  uint8_t pa_lut;
} sx126x_pa_config_t;

typedef struct sx126x_buffer_base_addr {
  uint8_t tx_base_addr;
  uint8_t rx_base_addr;
} sx126x_buffer_base_addr_t;

typedef enum sx126x_ramp_time {
  SX126X_SET_RAMP_10U = 0x00,    // 10 us
  SX126X_SET_RAMP_20U = 0x01,    // 20 us
  SX126X_SET_RAMP_40U = 0x02,    // 40 us
  SX126X_SET_RAMP_80U = 0x03,    // 80 us
  SX126X_SET_RAMP_200U = 0x04,   // 200 us
  SX126X_SET_RAMP_800U = 0x05,   // 800 us
  SX126X_SET_RAMP_1700U = 0x06,  // 1700 us
  SX126X_SET_RAMP_3400U = 0x07,  // 3400 us
} sx126x_ramp_time_t;

typedef struct sx126x_tx_params {
  int8_t power;
  sx126x_ramp_time_t ramp_time;
} sx126x_tx_params_t;

void sx126x_set_standby(sx126x_standby_mode_t mode);
void sx126x_set_packet_type(sx126x_packet_type_t type);
void sx126x_set_rf_frequency(uint32_t freq_hz);
void sx126x_set_pa_config(sx126x_pa_config_t* config);
void sx126x_set_tx_params(sx126x_tx_params_t* params);
void sx126x_set_buffer_base_address(sx126x_buffer_base_addr_t* addr);
void sx126x_write_buffer(uint8_t offset, uint8_t* data, size_t size);
void sx126x_set_modulation_params(uint8_t* p_params, size_t num);
void sx126x_set_packet_params(uint8_t* p_params, size_t num);
void sx126x_set_dio_irq_params(uint16_t irq_params, uint16_t dio1_params,
                               uint16_t dio2_params, uint16_t dio3_params);
void sx126x_write_register(uint16_t offset, uint8_t* data, size_t size);
void sx126x_set_tx(uint32_t timeout_ms);
void sx126x_clear_irq_status(uint16_t irq_params);

#endif
