#ifndef __SX126X_BOARD_H__
#define __SX126X_BOARD_H__

#include <stddef.h>
#include <stdint.h>

#include "hal_spi.h"
#include "sx126x.h"

typedef struct sx126x_board_config {
  uint8_t busy_io_num;
  hal_spi_handle_t spi;
} sx126x_board_config_t;

typedef enum sx126x_cmd_status {
  SX126X_CMD_STATUS_DATA_READY = 0x2,
  SX126X_CMD_STATUS_TIMEOUT = 0x3,
  SX126X_CMD_STATUS_PROCESSING_ERROR = 0x4,
  SX126X_CMD_STATUS_FAILED_TO_EXECUTE = 0x5,
  SX126X_CMD_STATUS_TX_DONE = 0x6,
} sx126x_cmd_status_t;

typedef enum sx126x_cmd_opcode {
  SX126X_CMD_OPCODE_SET_STANDBY = 0x80,
  SX126X_CMD_OPCODE_SET_PACKET_TYPE = 0x8A,
  SX126X_CMD_OPCODE_SET_RF_FREQUENCY = 0x86,
  SX126X_CMD_OPCODE_SET_PA_CONFIG = 0x95,
  SX126X_CMD_OPCODE_SET_BUFFER_BASE_ADDRESS = 0x8F,
  SX126X_CMD_OPCODE_WRITE_BUFFER = 0x0E,
  SX126X_CMD_OPCODE_SET_MODULATION_PARAMS = 0x8B,
  SX126X_CMD_OPCODE_SET_PACKET_PARAMS = 0x8C,
  SX126X_CMD_OPCODE_SET_DIO_IRQ_PARAMS = 0x08,
  SX126X_CMD_OPCODE_WRITE_REGISTER = 0x0D,
  SX126X_CMD_OPCODE_SET_TX = 0x83,
  SX126X_CMD_OPCODE_CLEAR_IRQ_PARAMS = 0x02,
} sx126x_cmd_opcode_t;

void sx126x_board_init(sx126x_board_config_t* config);

sx126x_cmd_status_t sx126x_write_command(uint8_t* cmd, size_t cmd_size,
                                         uint8_t* data, size_t data_size);

#endif
