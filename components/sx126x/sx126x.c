#include "sx126x.h"

#include <math.h>
#include <stdint.h>

#include "include/sx126x.h"
#include "include/sx126x_board.h"
#include "sx126x_board.h"

#define SX126X_FREQ_DIV (double)pow(2.0, 25.0)
#define SX126X_XTAL_FREQ (32000000)  // 32 Mhz
#define SX126X_FREQ_STEP (double)(SX126X_XTAL_FREQ / SX126X_FREQ_DIV)
#define SX126X_TX_TIMEOUT_DIV ((double)15.625)

void sx126x_set_standby(sx126x_standby_mode_t mode) {
  uint8_t cmd[2] = {
      [0] = SX126X_CMD_OPCODE_SET_STANDBY,
      [1] = (uint8_t)mode,
  };
  sx126x_write_command(cmd, sizeof(cmd), NULL, 0);
}

void sx126x_set_packet_type(sx126x_packet_type_t type) {
  uint8_t cmd[2] = {
      [0] = SX126X_CMD_OPCODE_SET_PACKET_TYPE,
      [1] = (uint8_t)type,
  };
  sx126x_write_command(cmd, sizeof(cmd), NULL, 0);
}

void sx126x_set_rf_frequency(uint32_t freq_hz) {
  uint8_t buffer[4] = {0};
  uint32_t Freq = (uint32_t)(freq_hz / SX126X_FREQ_STEP);
  uint8_t cmd[5] = {
      [0] = SX126X_CMD_OPCODE_SET_RF_FREQUENCY,
      [1] = (uint8_t)((Freq >> 24) & 0xFF),
      [2] = (uint8_t)((Freq >> 16) & 0xFF),
      [3] = (uint8_t)((Freq >> 8) & 0xFF),
      [4] = (uint8_t)((Freq) & 0xFF),
  };
  sx126x_write_command(cmd, sizeof(cmd), NULL, 0);
}

void sx126x_set_pa_config(sx126x_pa_config_t* config) {
  uint8_t cmd[5] = {
      [0] = SX126X_CMD_OPCODE_SET_PA_CONFIG,
      [1] = config->pa_duty_cycle,
      [2] = config->hp_max,
      [3] = config->device_sel,
      [4] = config->pa_lut,
  };
  sx126x_write_command(cmd, sizeof(cmd), NULL, 0);
}

void sx126x_set_buffer_base_address(sx126x_buffer_base_addr_t* addr) {
  uint8_t cmd[3] = {
      [0] = SX126X_CMD_OPCODE_SET_BUFFER_BASE_ADDRESS,
      [1] = addr->tx_base_addr,
      [2] = addr->rx_base_addr,
  };
  sx126x_write_command(cmd, sizeof(cmd), NULL, 0);
}

void sx126x_write_buffer(uint8_t offset, uint8_t* data, size_t size) {
  uint8_t cmd = SX126X_CMD_OPCODE_WRITE_BUFFER;
  sx126x_write_command(&cmd, 1, data, size);
}

void sx126x_set_modulation_params(uint8_t* p_params, size_t num) {
  uint8_t cmd = SX126X_CMD_OPCODE_SET_MODULATION_PARAMS;
  sx126x_write_command(&cmd, 1, p_params, num);
}

void sx126x_set_packet_params(uint8_t* p_params, size_t num) {
  uint8_t cmd = SX126X_CMD_OPCODE_SET_PACKET_PARAMS;
  sx126x_write_command(&cmd, 1, p_params, num);
}

void sx126x_set_dio_irq_params(uint16_t irq_params, uint16_t dio1_params,
                               uint16_t dio2_params, uint16_t dio3_params) {
  uint8_t cmd[9] = {
      [0] = SX126X_CMD_OPCODE_SET_DIO_IRQ_PARAMS,
      [1] = ((irq_params >> 8) & 0xFF),
      [2] = ((irq_params) & 0xFF),
      [3] = ((dio1_params >> 8) & 0xFF),
      [4] = ((dio1_params) & 0xFf),
      [5] = ((dio2_params >> 8) & 0xFF),
      [6] = ((dio2_params) & 0xFF),
      [7] = ((dio3_params >> 8) & 0xFF),
      [8] = ((dio3_params) & 0xFF),
  };
  sx126x_write_command(cmd, sizeof(cmd), NULL, 0);
}

void sx126x_write_register(uint16_t offset, uint8_t* data, size_t size) {
  uint8_t cmd[3] = {
      [0] = SX126X_CMD_OPCODE_WRITE_REGISTER,
      [1] = ((offset >> 8) & 0xFF),
      [2] = ((offset) & 0xFF),
  };
  sx126x_write_command(cmd, sizeof(cmd), data, size);
}

void sx126x_set_tx(uint32_t timeout_us) {
  double timeout = timeout_us / SX126X_TX_TIMEOUT_DIV;
  uint8_t cmd[4] = {
      [0] = SX126X_CMD_OPCODE_SET_TX,
      [1] = (((uint32_t)timeout >> 16) & 0xFF),
      [2] = (((uint32_t)timeout >> 8) & 0xFF),
      [3] = (((uint32_t)timeout) & 0xFF),
  };
  sx126x_write_command(cmd, sizeof(cmd), NULL, 0);
}

void sx126x_clear_irq_status(uint16_t irq_params) {
  uint8_t cmd[3] = {
      [0] = SX126X_CMD_OPCODE_CLEAR_IRQ_PARAMS,
      [1] = ((irq_params >> 8) & 0xFF),
      [2] = ((irq_params) & 0xFF),
  };
  sx126x_write_command(cmd, sizeof(cmd), NULL, 0);
}
