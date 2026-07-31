/* =========================================================================
    Ceedling - Test-Centered Build System for C
    ThrowTheSwitch.org
    Copyright (c) 2010-26 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

/* #include "mock_sx126x_board.h" */
#include <stdint.h>
#include <string.h>

#include "mock_sx126x_board.h"
#include "sx126x.h"
#include "unity.h"

#define TEST_SX126X_MAX_CAPTURED_WRITE_CMD_BUFFER_SIZE (100)

static uint8_t
    g_capture_command[TEST_SX126X_MAX_CAPTURED_WRITE_CMD_BUFFER_SIZE];
static uint8_t g_capture_data[TEST_SX126X_MAX_CAPTURED_WRITE_CMD_BUFFER_SIZE];

static sx126x_cmd_status_t capture_write_command(uint8_t* cmd, size_t cmd_size,
                                                 uint8_t* data,
                                                 size_t data_size) {
  memcpy(g_capture_command, cmd, cmd_size);
  memcpy(g_capture_data, data, data_size);
}

void setUp(void) {
  sx126x_write_command_fake.custom_fake = capture_write_command;
}

void tearDown(void) {
  memset(g_capture_command, 0, sizeof(g_capture_command));
  memset(g_capture_data, 0, sizeof(g_capture_command));
}

void test_sx126x_set_standby_rc(void) {
  // ARRANGE
  // ACT
  sx126x_set_standby(SX126X_STBY_RC);

  // ASSERT
  TEST_ASSERT_EQUAL(1, sx126x_write_command_fake.call_count);
  TEST_ASSERT_EQUAL_HEX8(0x80, g_capture_command[0]);
  TEST_ASSERT_EQUAL_HEX8(0x00, g_capture_command[1]);  // STBY_RC
  TEST_ASSERT_EQUAL(2, sx126x_write_command_fake.arg1_val);
}

void test_sx126x_set_standby_xosc(void) {
  // ARRANGE
  // ACT
  sx126x_set_standby(SX126X_STBY_XOSC);

  // ASSERT
  TEST_ASSERT_EQUAL(1, sx126x_write_command_fake.call_count);
  TEST_ASSERT_EQUAL_HEX8(0x80, g_capture_command[0]);
  TEST_ASSERT_EQUAL_HEX8(0x01, g_capture_command[1]);  // STBY_XOSC
  TEST_ASSERT_EQUAL(2, sx126x_write_command_fake.arg1_val);
}

void test_sx126x_set_packet_type_gfsk(void) {
  // ARRANGE
  // ACT
  sx126x_set_packet_type(SX126X_PACKET_TYPE_GFSK);
  // ASSERT
  TEST_ASSERT_EQUAL(1, sx126x_write_command_fake.call_count);
  TEST_ASSERT_EQUAL_HEX8(0x8A, g_capture_command[0]);
  TEST_ASSERT_EQUAL_HEX8(0x00, g_capture_command[1]);
  TEST_ASSERT_EQUAL(2, sx126x_write_command_fake.arg1_val);
}

void test_sx126x_set_packet_type_lora(void) {
  // ARRANGE
  // ACT
  sx126x_set_packet_type(SX126X_PACKET_TYPE_LORA);
  // ASSERT
  TEST_ASSERT_EQUAL(1, sx126x_write_command_fake.call_count);
  TEST_ASSERT_EQUAL_HEX8(0x8A, g_capture_command[0]);
  TEST_ASSERT_EQUAL_HEX8(0x01, g_capture_command[1]);
  TEST_ASSERT_EQUAL(2, sx126x_write_command_fake.arg1_val);
}

void test_sx126x_set_rf_frequency(void) {
  // ARRANGE
  // ACT
  sx126x_set_rf_frequency(92000000);  // 92 Mhz
  // ASSERT
  TEST_ASSERT_EQUAL(1, sx126x_write_command_fake.call_count);
  TEST_ASSERT_EQUAL_HEX8(0x86, g_capture_command[0]);
  TEST_ASSERT_EQUAL_HEX8(0x05, g_capture_command[1]);
  TEST_ASSERT_EQUAL_HEX8(0xc0, g_capture_command[2]);
  TEST_ASSERT_EQUAL_HEX8(0x00, g_capture_command[3]);
  TEST_ASSERT_EQUAL_HEX8(0x00, g_capture_command[4]);
  TEST_ASSERT_EQUAL(5, sx126x_write_command_fake.arg1_val);
}

void test_sx126x_set_pa_config(void) {
  // ARANGE
  sx126x_pa_config_t pa_config = {
      .pa_duty_cycle = 0x04,
      .hp_max = 0x07,
      .device_sel = 0x00,
      .pa_lut = 0x01,  // Reserved & Must always be 0x01
  };

  // ACT
  sx126x_set_pa_config(&pa_config);
  // ASSERT
  TEST_ASSERT_EQUAL(1, sx126x_write_command_fake.call_count);
  TEST_ASSERT_EQUAL_HEX8(0x95, g_capture_command[0]);
  TEST_ASSERT_EQUAL_HEX8(0x04, g_capture_command[1]);
  TEST_ASSERT_EQUAL_HEX8(0x07, g_capture_command[2]);
  TEST_ASSERT_EQUAL_HEX8(0x00, g_capture_command[3]);
  TEST_ASSERT_EQUAL_HEX8(0x01, g_capture_command[4]);
  TEST_ASSERT_EQUAL(5, sx126x_write_command_fake.arg1_val);
}

void test_sx126x_set_buffer_base_address(void) {
  // ARANGE
  sx126x_buffer_base_addr_t addr = {
      .tx_base_addr = 0x80,
      .rx_base_addr = 0x10,
  };

  // ACT
  sx126x_set_buffer_base_address(&addr);

  // ASSERT
  TEST_ASSERT_EQUAL(1, sx126x_write_command_fake.call_count);
  TEST_ASSERT_EQUAL_HEX8(0x8F, g_capture_command[0]);
  TEST_ASSERT_EQUAL_HEX8(0x80, g_capture_command[1]);
  TEST_ASSERT_EQUAL_HEX8(0x10, g_capture_command[2]);
  TEST_ASSERT_EQUAL(3, sx126x_write_command_fake.arg1_val);
}

void test_sx126x_write_buffer(void) {
  // ARANGE

  uint8_t offset = 0x80;
  uint8_t buffer[4] = {0x00, 0x01, 0x02, 0x03};
  size_t size = sizeof(buffer);

  // ACT
  sx126x_write_buffer(offset, buffer, size);

  // ASSERT
  TEST_ASSERT_EQUAL(1, sx126x_write_command_fake.call_count);
  TEST_ASSERT_EQUAL_HEX8(0x0E, g_capture_command[0]);
  TEST_ASSERT_EQUAL(1, sx126x_write_command_fake.arg1_val);

  TEST_ASSERT_EQUAL_HEX8(0x00, g_capture_data[0]);
  TEST_ASSERT_EQUAL_HEX8(0x01, g_capture_data[1]);
  TEST_ASSERT_EQUAL_HEX8(0x02, g_capture_data[2]);
  TEST_ASSERT_EQUAL_HEX8(0x03, g_capture_data[3]);
  TEST_ASSERT_EQUAL(4, sx126x_write_command_fake.arg3_val);
}

void test_sx126x_set_modulation_params(void) {
  // ARANGE

  // ACT
  uint8_t mod_params[7] = {
      [0] = 0x01, [1] = 0x02, [2] = 0x03, [3] = 0x04,
      [4] = 0x05, [5] = 0x06, [6] = 0x07,
  };
  sx126x_set_modulation_params(mod_params, sizeof(mod_params));

  // ASSERT
  TEST_ASSERT_EQUAL(1, sx126x_write_command_fake.call_count);
  TEST_ASSERT_EQUAL_HEX8(0x8B, g_capture_command[0]);
  TEST_ASSERT_EQUAL(1, sx126x_write_command_fake.arg1_val);

  TEST_ASSERT_EQUAL_HEX8(0x01, g_capture_data[0]);
  TEST_ASSERT_EQUAL_HEX8(0x02, g_capture_data[1]);
  TEST_ASSERT_EQUAL_HEX8(0x03, g_capture_data[2]);
  TEST_ASSERT_EQUAL_HEX8(0x04, g_capture_data[3]);
  TEST_ASSERT_EQUAL_HEX8(0x05, g_capture_data[4]);
  TEST_ASSERT_EQUAL_HEX8(0x06, g_capture_data[5]);
  TEST_ASSERT_EQUAL_HEX8(0x07, g_capture_data[6]);
  TEST_ASSERT_EQUAL(7, sx126x_write_command_fake.arg3_val);
}

/* // More useful interfaces / APIs */
/* // GFSK params */
/* // 1. bitrate */
/* // 2. pulse_shape */
/* // 3. frequency_deviation / f_dev */
/* // */
/* // Lora Params */
/* // */
/* // */
/* // 1. SF */
/* // 2. BW */
/* // 3. CR */
/* // 4. low_data_rate_optimize */
void test_sx126x_set_packet_params(void) {
  // ARANGE

  // ACT
  uint8_t packet_params[8] = {
      [0] = 0x01, [1] = 0x02, [2] = 0x03, [3] = 0x04,
      [4] = 0x05, [5] = 0x06, [6] = 0x07, [7] = 0x08,
  };
  sx126x_set_packet_params(packet_params, sizeof(packet_params));

  // ASSERT
  TEST_ASSERT_EQUAL(1, sx126x_write_command_fake.call_count);
  TEST_ASSERT_EQUAL_HEX8(0x8C, g_capture_command[0]);
  TEST_ASSERT_EQUAL(1, sx126x_write_command_fake.arg1_val);

  TEST_ASSERT_EQUAL_HEX8(0x01, g_capture_data[0]);
  TEST_ASSERT_EQUAL_HEX8(0x02, g_capture_data[1]);
  TEST_ASSERT_EQUAL_HEX8(0x03, g_capture_data[2]);
  TEST_ASSERT_EQUAL_HEX8(0x04, g_capture_data[3]);
  TEST_ASSERT_EQUAL_HEX8(0x05, g_capture_data[4]);
  TEST_ASSERT_EQUAL_HEX8(0x06, g_capture_data[5]);
  TEST_ASSERT_EQUAL_HEX8(0x07, g_capture_data[6]);
  TEST_ASSERT_EQUAL_HEX8(0x08, g_capture_data[7]);
  TEST_ASSERT_EQUAL(8, sx126x_write_command_fake.arg3_val);
}

void test_sx126x_set_dio_irq_params(void) {
  // ARRANGE
  uint16_t irq_params = 0x1245;
  uint16_t dio1_params = 0xbeaf;
  uint16_t dio2_params = 0xdead;
  uint16_t dio3_params = 0x79ac;

  // ACT
  sx126x_set_dio_irq_params(irq_params, dio1_params, dio2_params, dio3_params);
  // ASSERT
  TEST_ASSERT_EQUAL(1, sx126x_write_command_fake.call_count);
  TEST_ASSERT_EQUAL_HEX8(0x08, g_capture_command[0]);
  // irq_params
  TEST_ASSERT_EQUAL_HEX8(0x12, g_capture_command[1]);
  TEST_ASSERT_EQUAL_HEX8(0x45, g_capture_command[2]);

  // dio1_params
  TEST_ASSERT_EQUAL_HEX8(0xbe, g_capture_command[3]);
  TEST_ASSERT_EQUAL_HEX8(0xaf, g_capture_command[4]);
  // dio2_params
  TEST_ASSERT_EQUAL_HEX8(0xde, g_capture_command[5]);
  TEST_ASSERT_EQUAL_HEX8(0xad, g_capture_command[6]);
  // dio3_params
  TEST_ASSERT_EQUAL_HEX8(0x79, g_capture_command[7]);
  TEST_ASSERT_EQUAL_HEX8(0xac, g_capture_command[8]);
  TEST_ASSERT_EQUAL(9, sx126x_write_command_fake.arg1_val);
}

void test_sx126x_write_reg(void) {
  // ARRANGE
  uint16_t offset = 0x23ab;
  uint8_t data[3] = {
      0x12,
      0x34,
      0x56,
  };

  // ACT
  sx126x_write_register(offset, data, sizeof(data));

  // ASSERT
  TEST_ASSERT_EQUAL(1, sx126x_write_command_fake.call_count);
  TEST_ASSERT_EQUAL_HEX8(0x0D, g_capture_command[0]);
  TEST_ASSERT_EQUAL_HEX8(0x23, g_capture_command[1]);
  TEST_ASSERT_EQUAL_HEX8(0xab, g_capture_command[2]);
  TEST_ASSERT_EQUAL(3, sx126x_write_command_fake.arg1_val);
  // offset address
  // data
  TEST_ASSERT_EQUAL_HEX8(0x12, g_capture_data[0]);
  TEST_ASSERT_EQUAL_HEX8(0x34, g_capture_data[1]);
  TEST_ASSERT_EQUAL_HEX8(0x56, g_capture_data[2]);
  TEST_ASSERT_EQUAL(3, sx126x_write_command_fake.arg3_val);
}

void test_sx126x_set_tx(void) {
  // ARRANGE
  uint32_t timeout_us = 1000000;

  // ACT
  sx126x_set_tx(timeout_us);
  // ASSERT
  TEST_ASSERT_EQUAL(1, sx126x_write_command_fake.call_count);
  TEST_ASSERT_EQUAL_HEX8(0x83, g_capture_command[0]);
  // Ref. Page 68. Rev 1.2 - Timeout formula
  TEST_ASSERT_EQUAL_HEX8(0x00, g_capture_command[1]);
  TEST_ASSERT_EQUAL_HEX8(0xfa, g_capture_command[2]);
  TEST_ASSERT_EQUAL_HEX8(0x00, g_capture_command[3]);

  TEST_ASSERT_EQUAL(4, sx126x_write_command_fake.arg1_val);
}

void test_sx126x_clear_irq_flag(void) {
  // ARRANGE
  uint16_t irq_params = 0xabcd;
  // ACT
  sx126x_clear_irq_status(irq_params);
  // ASSERT
  TEST_ASSERT_EQUAL(1, sx126x_write_command_fake.call_count);
  TEST_ASSERT_EQUAL_HEX8(0x02, g_capture_command[0]);
  // Ref. Page 68. Rev 1.2 - Timeout formula
  TEST_ASSERT_EQUAL_HEX8(0xab, g_capture_command[1]);
  TEST_ASSERT_EQUAL_HEX8(0xcd, g_capture_command[2]);

  TEST_ASSERT_EQUAL(3, sx126x_write_command_fake.arg1_val);
}
