#include <string.h>

#include "mock_hal_gpio.h"
#include "mock_hal_spi.h"
#include "mock_osal_task.h"
#include "sx126x_board.h"
#include "unity.h"

static hal_spi_transaction_t g_capture_spi_transaction = {0};

static int custom_hal_spi_transfer(hal_spi_handle_t spi,
                                   hal_spi_transaction_t* transaction) {
  memcpy(&g_capture_spi_transaction, transaction,
         sizeof(hal_spi_transaction_t));
}

void setUp(void) {
  RESET_FAKE(hal_gpio_get_level);
  RESET_FAKE(hal_spi_transfer);
  RESET_FAKE(osal_task_sleep_ms);

  hal_spi_transfer_fake.custom_fake = custom_hal_spi_transfer;

  memset(&g_capture_spi_transaction, 0, sizeof(g_capture_spi_transaction));
}

void tearDown(void) {}

void test_sx126x_wait_before_write_command_when_busy(void) {
  // ARRANGE
  int gpio_get_level_ret_val_seq[2] = {
      1,
      0,
  };
  SET_RETURN_SEQ(hal_gpio_get_level, gpio_get_level_ret_val_seq, 2);

  uint8_t cmd[3] = {
      [0] = 0xab,
      [1] = 0x93,
      [2] = 0xfb,
  };
  uint8_t data[3] = {
      [0] = 0xab,
      [1] = 0xbc,
      [2] = 0x12,
  };

  // ACT
  sx126x_board_config_t board_config = {
      .busy_io_num = 13,
  };

  sx126x_board_init(&board_config);

  sx126x_cmd_status_t ret =
      sx126x_write_command(cmd, sizeof(cmd), data, sizeof(data));

  // ASSSERT
  TEST_ASSERT_EQUAL(2, hal_gpio_get_level_fake.call_count);
  TEST_ASSERT_EQUAL(board_config.busy_io_num, hal_gpio_get_level_fake.arg0_val);
  TEST_ASSERT_EQUAL(1, osal_task_sleep_ms_fake.arg0_val);

  TEST_ASSERT_EQUAL_PTR(hal_gpio_get_level, fff.call_history[0]);
  TEST_ASSERT_EQUAL_PTR(osal_task_sleep_ms, fff.call_history[1]);
  TEST_ASSERT_EQUAL_PTR(hal_gpio_get_level, fff.call_history[2]);

  TEST_ASSERT_EQUAL(SX126X_CMD_STATUS_DATA_READY, ret);
}

void test_sx126x_write_command_only_and_no_data(void) {
  // ARRANGE
  uint8_t cmd[3] = {
      [0] = 0xab,
      [1] = 0x93,
      [2] = 0xfb,
  };
  // ACT
  sx126x_board_config_t board_config = {
      .busy_io_num = 13,
      .spi = (hal_spi_handle_t)0x5f,  // dummy spi
  };

  sx126x_board_init(&board_config);

  sx126x_cmd_status_t ret = sx126x_write_command(cmd, sizeof(cmd), NULL, 0);

  // ASSERT
  TEST_ASSERT_EQUAL_PTR(board_config.spi, hal_spi_transfer_fake.arg0_val);

  TEST_ASSERT_EQUAL_HEX8(3, g_capture_spi_transaction.tx_buf_size);
  TEST_ASSERT_EQUAL_HEX8(cmd[0], g_capture_spi_transaction.tx_buf[0]);
  TEST_ASSERT_EQUAL_HEX8(cmd[1], g_capture_spi_transaction.tx_buf[1]);
  TEST_ASSERT_EQUAL_HEX8(cmd[2], g_capture_spi_transaction.tx_buf[2]);

  TEST_ASSERT_EQUAL(SX126X_CMD_STATUS_DATA_READY, ret);
}

void test_sx126x_write_command_with_data(void) {
  // ARRANGE
  uint8_t cmd[3] = {
      [0] = 0xab,
      [1] = 0x93,
      [2] = 0xfb,
  };
  uint8_t data[4] = {
      [0] = 0x12,
      [1] = 0x34,
      [2] = 0x56,
      [3] = 0x78,
  };

  // ACT
  sx126x_board_config_t board_config = {
      .busy_io_num = 13,
      .spi = (hal_spi_handle_t)0x5f,  // dummy spi
  };

  sx126x_board_init(&board_config);

  sx126x_cmd_status_t ret =
      sx126x_write_command(cmd, sizeof(cmd), data, sizeof(data));

  // ASSERT
  TEST_ASSERT_EQUAL_PTR(board_config.spi, hal_spi_transfer_fake.arg0_val);

  TEST_ASSERT_EQUAL_HEX8(7, g_capture_spi_transaction.tx_buf_size);
  TEST_ASSERT_EQUAL_HEX8(cmd[0], g_capture_spi_transaction.tx_buf[0]);
  TEST_ASSERT_EQUAL_HEX8(cmd[1], g_capture_spi_transaction.tx_buf[1]);
  TEST_ASSERT_EQUAL_HEX8(cmd[2], g_capture_spi_transaction.tx_buf[2]);
  TEST_ASSERT_EQUAL_HEX8(data[0], g_capture_spi_transaction.tx_buf[3]);
  TEST_ASSERT_EQUAL_HEX8(data[1], g_capture_spi_transaction.tx_buf[4]);
  TEST_ASSERT_EQUAL_HEX8(data[2], g_capture_spi_transaction.tx_buf[5]);
  TEST_ASSERT_EQUAL_HEX8(data[3], g_capture_spi_transaction.tx_buf[6]);

  TEST_ASSERT_EQUAL(SX126X_CMD_STATUS_DATA_READY, ret);
}
