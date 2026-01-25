#ifndef NRF24L01_H
#define NRF24L01_H

#include <stdint.h>

// REGISTER MAP
#define CONFIG_REGISTER                  0x00
#define EN_AA_REGISTER                   0x01
#define EN_RXADDR_REGISTER               0x02
#define SETUP_AW_REGISTER                0x03
#define SETUP_RETR_REGISTER              0x04
#define RF_CH_REGISTER                   0x05
#define RF_SETUP_REGISTER                0x06
#define STATUS_REGISTER                  0x07
#define OBSERVE_TX_REGISTER              0x08
#define CD_REGISTER                      0x09
#define RX_ADDR_P0_REGISTER              0x0A
#define RX_ADDR_P1_REGISTER              0x0B
#define RX_ADDR_P2_REGISTER              0x0C
#define RX_ADDR_P3_REGISTER              0x0D
#define RX_ADDR_P4_REGISTER              0x0E
#define RX_ADDR_P5_REGISTER              0x0F
#define TX_ADDR_REGISTER                 0x10
#define RX_PW_P0_REGISTER                0x11
#define RX_PW_P1_REGISTER                0x12
#define RX_PW_P2_REGISTER                0x13
#define RX_PW_P3_REGISTER                0x14
#define RX_PW_P4_REGISTER                0x15
#define RX_PW_P5_REGISTER                0x16
#define FIFO_STATUS_REGISTER             0x17
#define DYNPD_REGISTER                   0x1C
#define FEATURE_REGISTER                 0x1D

// SPI COMMANDS
#define CMD_R_REGISTER                   0x00
#define CMD_W_REGISTER                   0x20
#define CMD_R_RX_PAYLOAD                 0x61
#define CMD_W_TX_PAYLOAD                 0xA0
#define CMD_FLUSH_TX                     0xE1
#define CMD_FLUSH_RX                     0xE2
#define CMD_REUSE_TX_PL                  0xE3
#define CMD_ACTIVATE                     0x50
#define CMD_R_RX_PL_WID                  0x60
#define CMD_W_ACK_PAYLOAD                0xA8
#define CMD_W_TX_PAYLOAD_NOACK           0xB0
#define CMD_NOP                          0xFF

// BITWISE FOR CONFIG REGISTER
#define CONFIG_MASK_RX_DR                (1 << 6)
#define CONFIG_MASK_TX_DS                (1 << 5)
#define CONFIG_MASK_MAX_RT               (1 << 4)
#define CONFIG_EN_CRC                    (1 << 3)
#define CONFIG_CRCO                      (1 << 2)
#define CONFIG_PWR_UP                    (1 << 1)
#define CONFIG_PRIM_RX                   (1 << 0)

// ENABLED DATA PIPES
#define EN_P0                            (1 << 0)
#define EN_P1                            (1 << 1)
#define EN_P2                            (1 << 2)
#define EN_P3                            (1 << 3)
#define EN_P4                            (1 << 4)
#define EN_P5                            (1 << 5)

// ADDRESS WIDTH
#define SETUP_AW_3B                      0x01
#define SETUP_AW_4B                      0x02
#define SETUP_AW_5B                      0x03

// RETRANSMISSION SETTINGS
typedef enum {
    NRF24_RETR_DISABLED = 0x00,
    NRF24_RETR_250US_1  = 0x01,
    NRF24_RETR_250US_2  = 0x02,
    NRF24_RETR_250US_3  = 0x03,
    NRF24_RETR_500US_1  = 0x11,
    NRF24_RETR_500US_2  = 0x12,
    NRF24_RETR_500US_3  = 0x13
} nrf24_retr_t;

// RF CHANNELS
typedef enum {
    NRF24_CH_40 = 40,
    NRF24_CH_41 = 41,
    NRF24_CH_42 = 42
} nrf24_channel_t;

// RF SETUP BITS
#define RF_DR_1MBPS                      0
#define RF_DR_2MBPS                      (1 << 3)

#define RF_PWR_NEG18DBM                  0
#define RF_PWR_NEG12DBM                  (1 << 1)
#define RF_PWR_NEG6DBM                   (1 << 2)
#define RF_PWR_0DBM                      ((1 << 2) | (1 << 1))

// PAYLOAD
#define NRF24_MAX_PAYLOAD                32
#define NRF24_DEFAULT_PAYLOAD            4

// DRIVER HANDLE
typedef struct __SPI_HandleTypeDef SPI_HandleTypeDef;
typedef struct GPIO_TypeDef GPIO_TypeDef;

typedef struct
{
    SPI_HandleTypeDef *hspi;

    GPIO_TypeDef *ce_port;
    uint16_t ce_pin;

    GPIO_TypeDef *csn_port;
    uint16_t csn_pin;

} nrf24_handle_t;

// PUBLIC FUNCTIONS
void NRF24_Init(nrf24_handle_t *dev);

void NRF24_SetTxMode(nrf24_handle_t *dev);
void NRF24_SetRxMode(nrf24_handle_t *dev);

void NRF24_Send(nrf24_handle_t *dev, uint8_t *data, uint8_t len);
uint8_t NRF24_DataAvailable(nrf24_handle_t *dev);
void NRF24_Read(nrf24_handle_t *dev, uint8_t *data, uint8_t len);

#endif /* NRF24L01_H */