// This will be the header for the file
#ifndef NRF24L01_H
#define NRF24L01_H           

// Register addresses and data
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
#define FIFO_STATUS_REGISTER             0x17   //read
#define DYNPD_REGISTER                   0x1C
#define FEATURE_REGISTER                 0x1D

// SPI commands
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

// Settings for the registers
#define CONFIG_MASK_RX_DR (1 << 6)
#define CONFIG_MASK_TX_DS (1 << 5)
#define CONFIG_MASK_MAX_RT (1 << 4)
#define CONFIG_EN_CRC (1 << 3)
#define CONFIG_CRCO (1 << 2)
#define CONFIG_PWR_UP (1 << 1)
#define CONFIG_PRIM_RX (1 << 0)

// bit masks for the EN_AA Data pipe Register
#define EN_P0 (1 << 0)
#define EN_P1 (1 << 1)
#define EN_P2 (1 << 2)
#define EN_P3 (1 << 3)
#define EN_P4 (1 << 4)
#define EN_P5 (1 << 5)

#define SETUP_AW_3B 0x01
#define SETUP_AW_4B 0x02
#define SETUP_AW_5B 0x03

// 250us or 500us and 1-3 retransmissions
typedef enum {
    SETUP_RETR_DISABLED = 0x00,
    SETUP_RETR_250us_1T = 0x01,
    SETUP_RETR_250us_2T = 0x02,
    SETUP_RETR_250us_3T = 0x03,
    SETUP_RETR_500us_1T = 0x11,
    SETUP_RETR_500us_2T = 0x12,  
    SETUP_RETR_500us_3T = 0x13
} SETUP_RETR;

// working on channels 40, 41 or 42
typedef enum {
    RF_CH_40 = 0x28,
    RF_CH_41 = 0x29,
    RF_CH_42 = 0x2A
} RF_CH_SETTING;

// bitmasks for RF_SETUP Register
#define RF_PLL_LOCK   (1 << 4)
#define RF_DR         (1 << 3) //1 for 2Mbps and 0 for 1Mbps
#define RF_PWR_1      (1 << 2) //00 for -18dBm, 01 for -12dBm, 10 for -6dBm, 11 for 0dBm
#define RF_PWR_0      (1 << 1)
#define LNA_HCURR     (1 << 0)

// Just the first pipe is set to 4B as default
typedef enum {
    RX_PW_P0_SIZE = 0x04,
    RX_PW_P1_SIZE = 0x00,
    RX_PW_P2_SIZE = 0x00,
    RX_PW_P3_SIZE = 0x00,
    RX_PW_P4_SIZE = 0x00,
    RX_PW_P5_SIZE = 0x00
} RX_PW_PX_SETTING;

// Bitmasks for the FEATURE register
#define FEATURE_EN_DYN_ACK (1 << 0)
#define FEATURE_EN_ACK_PAY (1 << 1)
#define FEATURE_EN_DPL     (1 << 2)


// Public Functions
// nRF24L01_init()

// Private Functions

#endif