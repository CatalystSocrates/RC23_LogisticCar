#ifndef __TCS34725_H
#define __TCS34725_H

#define TCS3472_ADDRESS          (0x29)

#define TCS3472_COMMAND_BIT      (0x80)

#define TCS3472_ENABLE           (0x00)
#define TCS3472_ENABLE_AIEN      (0x10)    /* RGBC Interrupt Enable */
#define TCS3472_ENABLE_WEN       (0x08)    /* Wait enable - Writing 1 activates the wait timer */
#define TCS3472_ENABLE_AEN       (0x02)    /* RGBC Enable - Writing 1 actives the ADC, 0 disables it */
#define TCS3472_ENABLE_PON       (0x01)    /* Power on - Writing 1 activates the internal oscillator, 0 disables it */
#define TCS3472_ATIME            (0x01)    /* Integration time */
#define TCS3472_WTIME            (0x03)    /* Wait time (if TCS34725_ENABLE_WEN is asserted) */
#define TCS3472_WTIME_2_4MS      (0xFF)    /* WLONG0 = 2.4ms   WLONG1 = 0.029s */
#define TCS3472_WTIME_204MS      (0xAB)    /* WLONG0 = 204ms   WLONG1 = 2.45s  */
#define TCS3472_WTIME_614MS      (0x00)    /* WLONG0 = 614ms   WLONG1 = 7.4s   */
#define TCS3472_AILTL            (0x04)    /* Clear channel lower interrupt threshold */
#define TCS3472_AILTH            (0x05)
#define TCS3472_AIHTL            (0x06)    /* Clear channel upper interrupt threshold */
#define TCS3472_AIHTH            (0x07)
#define TCS3472_PERS             (0x0C)    /* Persistence register - basic SW filtering mechanism for interrupts */
#define TCS3472_PERS_NONE        (0b0000)  /* Every RGBC cycle generates an interrupt                                */
#define TCS3472_PERS_1_CYCLE     (0b0001)  /* 1 clean channel value outside threshold range generates an interrupt   */
#define TCS3472_PERS_2_CYCLE     (0b0010)  /* 2 clean channel values outside threshold range generates an interrupt  */
#define TCS3472_PERS_3_CYCLE     (0b0011)  /* 3 clean channel values outside threshold range generates an interrupt  */
#define TCS3472_PERS_5_CYCLE     (0b0100)  /* 5 clean channel values outside threshold range generates an interrupt  */
#define TCS3472_PERS_10_CYCLE    (0b0101)  /* 10 clean channel values outside threshold range generates an interrupt */
#define TCS3472_PERS_15_CYCLE    (0b0110)  /* 15 clean channel values outside threshold range generates an interrupt */
#define TCS3472_PERS_20_CYCLE    (0b0111)  /* 20 clean channel values outside threshold range generates an interrupt */
#define TCS3472_PERS_25_CYCLE    (0b1000)  /* 25 clean channel values outside threshold range generates an interrupt */
#define TCS3472_PERS_30_CYCLE    (0b1001)  /* 30 clean channel values outside threshold range generates an interrupt */
#define TCS3472_PERS_35_CYCLE    (0b1010)  /* 35 clean channel values outside threshold range generates an interrupt */
#define TCS3472_PERS_40_CYCLE    (0b1011)  /* 40 clean channel values outside threshold range generates an interrupt */
#define TCS3472_PERS_45_CYCLE    (0b1100)  /* 45 clean channel values outside threshold range generates an interrupt */
#define TCS3472_PERS_50_CYCLE    (0b1101)  /* 50 clean channel values outside threshold range generates an interrupt */
#define TCS3472_PERS_55_CYCLE    (0b1110)  /* 55 clean channel values outside threshold range generates an interrupt */
#define TCS3472_PERS_60_CYCLE    (0b1111)  /* 60 clean channel values outside threshold range generates an interrupt */
#define TCS3472_CONFIG           (0x0D)
#define TCS3472_CONFIG_WLONG     (0x02)    /* Choose between short and long (12x) wait times via TCS34725_WTIME */
#define TCS3472_CONTROL          (0x0F)    /* Set the gain level for the sensor */
#define TCS3472_ID               (0x12)    /* 0x44 = TCS34721/TCS34725, 0x4D = TCS34723/TCS34727 */
#define TCS3472_STATUS           (0x13)
#define TCS3472_STATUS_AINT      (0x10)    /* RGBC Clean channel interrupt */
#define TCS3472_STATUS_AVALID    (0x01)    /* Indicates that the RGBC channels have completed an integration cycle */
#define TCS3472_CDATAL           (0x14)    /* Clear channel data */
#define TCS3472_CDATAH           (0x15)
#define TCS3472_RDATAL           (0x16)    /* Red channel data */
#define TCS3472_RDATAH           (0x17)
#define TCS3472_GDATAL           (0x18)    /* Green channel data */
#define TCS3472_GDATAH           (0x19)
#define TCS3472_BDATAL           (0x1A)    /* Blue channel data */
#define TCS3472_BDATAH           (0x1B)

#define TCS3472_INTEGRATIONTIME_2_4MS   0xFF   /**<  2.4ms - 1 cycle    - Max Count: 1024  */
#define TCS3472_INTEGRATIONTIME_24MS    0xF6   /**<  24ms  - 10 cycles  - Max Count: 10240 */
#define TCS3472_INTEGRATIONTIME_50MS    0xEB   /**<  50ms  - 20 cycles  - Max Count: 20480 */
#define TCS3472_INTEGRATIONTIME_101MS   0xD5   /**<  101ms - 42 cycles  - Max Count: 43008 */
#define TCS3472_INTEGRATIONTIME_154MS   0xC0   /**<  154ms - 64 cycles  - Max Count: 65535 */
#define TCS3472_INTEGRATIONTIME_240MS   0x9C   /**<  240ms - 100 cycles - Max Count: 65535 */
#define TCS3472_INTEGRATIONTIME_700MS   0x00   /**<  700ms - 256 cycles - Max Count: 65535 */

#define TCS3472_GAIN_1X                 0x00   /**<  No gain  */
#define TCS3472_GAIN_4X                 0x01   /**<  4x gain  */
#define TCS3472_GAIN_16X                0x02   /**<  16x gain */
#define TCS3472_GAIN_60X                0x03   /**<  60x gain */





#include "main.h"
#include "delayus.h"

#define max3v(v1, v2, v3)   ((v1)<(v2)? ((v2)<(v3)?(v3):(v2)):((v1)<(v3)?(v3):(v1)))
#define min3v(v1, v2, v3)   ((v1)>(v2)? ((v2)>(v3)?(v3):(v2)):((v1)>(v3)?(v3):(v1)))
//#define TCS3472_ATIME            (0x01)
//#define TCS3472_WTIME_2_4MS      (0xFF)    /* WLONG0 = 2.4ms   WLONG1 = 0.029s */
//#define TCS3472_WTIME_204MS      (0xAB)    /* WLONG0 = 204ms   WLONG1 = 2.45s  */
//#define TCS3472_WTIME_614MS      (0x00)



typedef struct{
	unsigned short h;
	unsigned short s;
	unsigned short l;
}COLOR_HSL;

typedef struct{
	unsigned short  c;      //[0-65536]
	unsigned short  r;
	unsigned short  g;
	unsigned short  b;
}COLOR_RGBC;




void I2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Write_Ack(void);
uint8_t I2C_Read_Ack(void);
void I2C_Send_Byte(uint8_t txd);
uint8_t I2C_Read_Byte(unsigned char rdack);
void TCS3472_WRITE( uint8_t WrAddr, uint8_t data);
uint8_t TCS3472_READ_1Byte( uint8_t RdAddr);
uint16_t TCS3472_READ_2Byte( uint8_t RdAddr);
void TCS3472_PON_Enable(void);
void TCS3472_PON_Disable(void);
void TCS3472_AEN_Enable(void);
void TCS3472_AEN_Disable(void);
void TCS3472_WEN_Enable(void);
void TCS3472_WEN_Disable(void);
void TCS3472_AIEN_Enable(void);
void TCS3472_AIEN_Disable(void);
uint8_t TCS3472_ID_Verification(void);
void TCS34725_Init(void);
void TCS34725_GET_RGB(COLOR_RGBC *Rgb);
const char* mapRGBCtoColor(float hue, float saturation, float Lightness);

void RGBtoHSL(COLOR_RGBC *Rgb, COLOR_HSL *Hsl);
//void RGBtoHSL(COLOR_RGBC *Rgb, COLOR_HSL *Hsl);

#endif
