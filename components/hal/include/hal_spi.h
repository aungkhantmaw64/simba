#ifndef __HAL_SPI_H__
#define __HAL_SPI_H__

#include <stddef.h>
#include <stdint.h>

typedef void* hal_spi_handle_t;

typedef struct hal_spi_transaction {
  uint8_t* tx_buf;
  size_t tx_buf_size;
  uint8_t* rx_buf;
  size_t rx_buf_size;
} hal_spi_transaction_t;

uint8_t hal_spi_transfer(hal_spi_handle_t handle, hal_spi_transaction_t* trans);

#endif
