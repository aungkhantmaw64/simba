#include "sx126x_board.h"

#include <string.h>

#include "hal_gpio.h"
#include "hal_spi.h"
#include "include/sx126x_board.h"
#include "osal_task.h"

#define SX126X_SPI_TX_BUFFER_MAX_SIZE \
  (256 + 9)  // Largest commands (9 bytes) + (256) bytes data buffer

static sx126x_board_config_t g_board_config = {0};

void sx126x_board_init(sx126x_board_config_t* config) {
  memcpy(&g_board_config, config, sizeof(g_board_config));
}

sx126x_cmd_status_t sx126x_write_command(uint8_t* cmd, size_t cmd_size,
                                         uint8_t* data, size_t data_size) {
  uint8_t tx_buf[SX126X_SPI_TX_BUFFER_MAX_SIZE] = {0};

  sx126x_cmd_status_t ret = SX126X_CMD_STATUS_DATA_READY;
  // Wait on busy
  while (1 == hal_gpio_get_level(g_board_config.busy_io_num)) {
    osal_task_sleep_ms(1);
  }

  memcpy(tx_buf, cmd, cmd_size);

  if (NULL != data && 0 < data_size) {
    memcpy(tx_buf + cmd_size, data, data_size);
  }

  hal_spi_transaction_t transaction = {
      .tx_buf = tx_buf,
      .tx_buf_size = cmd_size + data_size,
  };

  hal_spi_transfer(g_board_config.spi, &transaction);

  return ret;
}
