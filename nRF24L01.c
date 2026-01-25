#include <stdint.h>
#include "stm32l4xx_hal.h" 
#include "nrf24l01.h"

// GPIO FUNCTIONS
static inline void CSN_LOW(nrf24_handle_t *d)
{
    HAL_GPIO_WritePin(d->csn_port, d->csn_pin, GPIO_PIN_RESET);
}

static inline void CSN_HIGH(nrf24_handle_t *d)
{
    HAL_GPIO_WritePin(d->csn_port, d->csn_pin, GPIO_PIN_SET);
}

static inline void CE_LOW(nrf24_handle_t *d)
{
    HAL_GPIO_WritePin(d->ce_port, d->ce_pin, GPIO_PIN_RESET);
}

static inline void CE_HIGH(nrf24_handle_t *d)
{
    HAL_GPIO_WritePin(d->ce_port, d->ce_pin, GPIO_PIN_SET);
}

// SPI FUNCTIONS
static uint8_t nrf24_read_reg(nrf24_handle_t *d, uint8_t reg)
{
    uint8_t tx[2] = { CMD_R_REGISTER | reg, CMD_NOP };
    uint8_t rx[2];

    CSN_LOW(d);
    HAL_SPI_TransmitReceive(d->hspi, tx, rx, 2, HAL_MAX_DELAY);
    CSN_HIGH(d);

    return rx[1];
}

static void nrf24_write_reg(nrf24_handle_t *d, uint8_t reg, uint8_t val)
{
    uint8_t tx[2] = { CMD_W_REGISTER | reg, val };

    CSN_LOW(d);
    HAL_SPI_Transmit(d->hspi, tx, 2, HAL_MAX_DELAY);
    CSN_HIGH(d);
}

static void nrf24_write_buf(nrf24_handle_t *d, uint8_t reg,
                            uint8_t *buf, uint8_t len)
{
    uint8_t cmd = CMD_W_REGISTER | reg;

    CSN_LOW(d);
    HAL_SPI_Transmit(d->hspi, &cmd, 1, HAL_MAX_DELAY);
    HAL_SPI_Transmit(d->hspi, buf, len, HAL_MAX_DELAY);
    CSN_HIGH(d);
}

static void nrf24_read_payload(nrf24_handle_t *d, uint8_t *buf, uint8_t len)
{
    uint8_t cmd = CMD_R_RX_PAYLOAD;

    CSN_LOW(d);
    HAL_SPI_Transmit(d->hspi, &cmd, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(d->hspi, buf, len, HAL_MAX_DELAY);
    CSN_HIGH(d);
}

static void nrf24_flush_tx(nrf24_handle_t *d)
{
    uint8_t cmd = CMD_FLUSH_TX;
    CSN_LOW(d);
    HAL_SPI_Transmit(d->hspi, &cmd, 1, HAL_MAX_DELAY);
    CSN_HIGH(d);
}

static void nrf24_flush_rx(nrf24_handle_t *d)
{
    uint8_t cmd = CMD_FLUSH_RX;
    CSN_LOW(d);
    HAL_SPI_Transmit(d->hspi, &cmd, 1, HAL_MAX_DELAY);
    CSN_HIGH(d);
}

// PUBLIC FUNCTIONS
void NRF24_Init(nrf24_handle_t *d)
{
    CE_LOW(d);
    HAL_Delay(5);

    nrf24_write_reg(d, CONFIG_REGISTER,
                    CONFIG_EN_CRC |
                    CONFIG_CRCO |
                    CONFIG_PWR_UP);

    nrf24_write_reg(d, EN_AA_REGISTER, EN_P0);
    nrf24_write_reg(d, EN_RXADDR_REGISTER, EN_P0);
    nrf24_write_reg(d, SETUP_AW_REGISTER, SETUP_AW_5B);
    nrf24_write_reg(d, SETUP_RETR_REGISTER, NRF24_RETR_500US_3);
    nrf24_write_reg(d, RF_CH_REGISTER, NRF24_CH_40);
    nrf24_write_reg(d, RF_SETUP_REGISTER, RF_DR_1MBPS | RF_PWR_NEG6DBM);
    nrf24_write_reg(d, RX_PW_P0_REGISTER, NRF24_DEFAULT_PAYLOAD);

    uint8_t addr[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};
    nrf24_write_buf(d, RX_ADDR_P0_REGISTER, addr, 5);
    nrf24_write_buf(d, TX_ADDR_REGISTER, addr, 5);

    nrf24_flush_tx(d);
    nrf24_flush_rx(d);

    CE_LOW(d);
}

void NRF24_SetTxMode(nrf24_handle_t *d)
{
    CE_LOW(d);
    uint8_t cfg = nrf24_read_reg(d, CONFIG_REGISTER);
    cfg &= ~CONFIG_PRIM_RX;
    cfg |= CONFIG_PWR_UP;
    nrf24_write_reg(d, CONFIG_REGISTER, cfg);
    HAL_Delay(1);
}

void NRF24_SetRxMode(nrf24_handle_t *d)
{
    CE_LOW(d);
    uint8_t cfg = nrf24_read_reg(d, CONFIG_REGISTER);
    cfg |= CONFIG_PRIM_RX | CONFIG_PWR_UP;
    nrf24_write_reg(d, CONFIG_REGISTER, cfg);
    nrf24_flush_rx(d);
    CE_HIGH(d);
    HAL_Delay(1);
}

void NRF24_Send(nrf24_handle_t *d, uint8_t *data, uint8_t len)
{
    NRF24_SetTxMode(d);
    nrf24_flush_tx(d);

    uint8_t cmd = CMD_W_TX_PAYLOAD;
    CSN_LOW(d);
    HAL_SPI_Transmit(d->hspi, &cmd, 1, HAL_MAX_DELAY);
    HAL_SPI_Transmit(d->hspi, data, len, HAL_MAX_DELAY);
    CSN_HIGH(d);

    CE_HIGH(d);
    HAL_Delay(1);
    CE_LOW(d);
}

uint8_t NRF24_DataAvailable(nrf24_handle_t *d)
{
    return (nrf24_read_reg(d, STATUS_REGISTER) & CONFIG_MASK_RX_DR);
}

void NRF24_Read(nrf24_handle_t *d, uint8_t *data, uint8_t len)
{
    nrf24_read_payload(d, data, len);
    nrf24_write_reg(d, STATUS_REGISTER, CONFIG_MASK_RX_DR);
}
