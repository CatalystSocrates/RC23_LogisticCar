## 引脚接口定义

```
//USART1
//USART1_RX -> PA10
//USART1_TX -> PA9
//USART2
//USART2_RX -> PD6
//USART2_TX -> PD5
//MPU6050
//MPU6050_SCL -> PD11
//MPU6050_SDA -> PD10
//TCS34725
//TCS34725_SCL -> PE3
//TCS34725_SDA -> PE4
//TCS34725_Exit -> PD9
//CAN
//CAN_TX -> PD1
//CAN_RX -> PD0
//ST7735
//SPI2_RST -> PD8
//SPI2_MOSI -> PB15
//SPI2_DC -> PB14
//SPI2_SCK -> PB13
//SPI2_CS -> PB12
//Encoder
//EncoderA1 -> PA6
//EncoderA2 -> PA7
//EncoderB1 -> PA15
//EncoderB2 -> PB3
//EncoderC1 -> PD12
//EncoderC2 -> PD13
//Motor
//AIN1 -> PB0
//AIN2 -> PB1
//BIN1 -> PC0
//BIN2 -> PC1
//CIN1 -> PC2
//CIN2 -> PC3
//DIN1 -> PC4
//DIN2 -> PC5
//PWM
//PWMA -> PE9
//PWMB -> PE11
//PWMC -> PE13
//PWMD -> PE14
//PWME -> PA0
//PWMF -> PA1
//PWMG -> PA2
//PWMH -> PA3
//PWMI -> PC6
//PWMJ -> PC7
//PWMK -> PC8
//PWML -> PC9
```

## 自定义库部分函数使用方法

```
//这里将会有一堆程序
```

## 整机大体逻辑

STM32F103VET6负责整体的运动逻辑控制，树莓派识别二维码与红绿灯发送到STM32中改变小车运行状态。
首先，识别小车底下色块，为黄色的话进入物料放置模式，识别二维码后打开物料盒，发送语言提醒，识别到放入物品后自动启动。
后面过程与要求一致。
