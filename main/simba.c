#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <esp_check.h>
#include <esp_err.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdbool.h>

#define SX126X_MOSI_IO_NUM (10)
#define SX126X_MISO_IO_NUM (11)
#define SX126X_SCLK_IO_NUM (9)
#define SX126X_CS_IO_NUM (8)
#define SX126X_BUSY_IO_NUM (13)
#define SX126X_DIO1_IO_NUM (14)

#define SX126X_MAX_TRANSFER_SIZE (4092)

const char* TAG = "main";

static void sx126x_init(spi_device_handle_t spi) {
  // Init BUSY line GPIO as input
  // Init DIO1 line GPIO as input
  gpio_config_t io_conf = {0};
}

void app_main(void) {
  spi_bus_config_t spi_bus_config = {
      .mosi_io_num = SX126X_MOSI_IO_NUM,
      .miso_io_num = SX126X_MISO_IO_NUM,
      .sclk_io_num = SX126X_SCLK_IO_NUM,
      .max_transfer_sz = SX126X_MAX_TRANSFER_SIZE,
  };
  ESP_ERROR_CHECK(
      spi_bus_initialize(SPI2_HOST, &spi_bus_config, SPI_DMA_CH_AUTO));

  spi_device_handle_t sx126x_spi_handle = {0};
  spi_device_interface_config_t sx126x_spi_dev_config = {
      .mode = 0,                              // MODE=0 (CPOL=0, CPOL=1)
      .clock_speed_hz = SPI_MASTER_FREQ_10M,  // 10 Mhz
      .spics_io_num = SX126X_CS_IO_NUM,
      .queue_size = 7,
  };
  ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &sx126x_spi_dev_config,
                                     &sx126x_spi_handle));
  ESP_LOGI(TAG, "Initialized SPI device successfully.");

  sx126x_init(sx126x_spi_handle);

  while (true) {
    vTaskDelay(1000);
  }
}
