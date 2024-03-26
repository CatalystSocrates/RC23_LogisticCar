#include "stm32f1xx_hal.h"
#include "mpu6050.h"
#include <inttypes.h>
#include "system.h"
#include "stm32f1xx_hal.h"

/* 定义IIC总线连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */
#define MPU_GPIO_PORT_IIC     MPU6050_SDA_GPIO_Port                       /* GPIO端口 */
#define MPU_RCC_IIC_ENABLE    __HAL_RCC_GPIOB_CLK_ENABLE()       /* GPIO端口时钟 */
#define MPU_IIC_SCL_PIN       MPU6050_SCL_Pin                  /* 连接到SCL时钟线的GPIO */
#define MPU_IIC_SDA_PIN       MPU6050_SDA_Pin                  /* 连接到SDA数据线的GPIO */

/* 定义读写SCL和SDA的宏，已增加代码的可移植性和可阅读性 */
#if 1	/* 条件编译： 1 选择GPIO的库函数实现IO读写 */
#define MPU_IIC_SCL_1()  HAL_GPIO_WritePin(MPU_GPIO_PORT_IIC, MPU_IIC_SCL_PIN, GPIO_PIN_SET)		/* SCL = 1 */
#define MPU_IIC_SCL_0()  HAL_GPIO_WritePin(MPU_GPIO_PORT_IIC, MPU_IIC_SCL_PIN, GPIO_PIN_RESET)		/* SCL = 0 */

#define MPU_IIC_SDA_1()  HAL_GPIO_WritePin(MPU_GPIO_PORT_IIC, MPU_IIC_SDA_PIN, GPIO_PIN_SET)		/* SDA = 1 */
#define MPU_IIC_SDA_0()  HAL_GPIO_WritePin(MPU_GPIO_PORT_IIC, MPU_IIC_SDA_PIN, GPIO_PIN_RESET)		/* SDA = 0 */

#define MPU_IIC_SDA_READ()  HAL_GPIO_ReadPin(MPU_GPIO_PORT_IIC, MPU_IIC_SDA_PIN)	/* 读SDA口线状态 */
#else	/* 这个分支选择直接寄存器操作实现IO读写 */
/*　注意：如下写法，在IAR最高级别优化时，会被编译器错误优化 */
#define MPU_IIC_SCL_1()  MPU_GPIO_PORT_IIC->BSRR = MPU_IIC_SCL_PIN				/* SCL = 1 */
#define MPU_IIC_SCL_0()  MPU_GPIO_PORT_IIC->BRR = MPU_IIC_SCL_PIN				/* SCL = 0 */

#define MPU_IIC_SDA_1()  MPU_GPIO_PORT_IIC->BSRR = MPU_IIC_SDA_PIN				/* SDA = 1 */
#define MPU_IIC_SDA_0()  MPU_GPIO_PORT_IIC->BRR = MPU_IIC_SDA_PIN				/* SDA = 0 */

#define MPU_IIC_SDA_READ()  ((MPU_GPIO_PORT_IIC->IDR & MPU_IIC_SDA_PIN) != 0)	/* 读SDA口线状态 */
#endif

void MPU_IIC_GPIO_Init(void);

/*
*********************************************************************************************************
*	函 数 名: IIC_Delay
*	功能说明: IIC总线位延迟，最快400KHz
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void MPU_IIC_Delay(void)
{
    uint8_t i;

    /*　
     	下面的时间是通过安富莱AX-Pro逻辑分析仪测试得到的。
    	CPU主频72MHz时，在内部Flash运行, MDK工程不优化
    	循环次数为10时，SCL频率 = 205KHz
    	循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us
     	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us

    IAR工程编译效率高，不能设置为7
    */
    for (i = 0; i < 10; i++);
}

/*
*********************************************************************************************************
*	函 数 名: IIC_Start
*	功能说明: CPU发起IIC总线启动信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void MPU_IIC_Start(void)
{
    /* 当SCL高电平时，SDA出现一个下跳沿表示IIC总线启动信号 */
    MPU_IIC_SDA_1();
    MPU_IIC_SCL_1();
    MPU_IIC_Delay();
    MPU_IIC_SDA_0();
    MPU_IIC_Delay();
    MPU_IIC_SCL_0();
    MPU_IIC_Delay();
}

/*
*********************************************************************************************************
*	函 数 名: IIC_Start
*	功能说明: CPU发起IIC总线停止信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void MPU_IIC_Stop(void)
{
    /* 当SCL高电平时，SDA出现一个上跳沿表示IIC总线停止信号 */
    MPU_IIC_SDA_0();
    MPU_IIC_SCL_1();
    MPU_IIC_Delay();
    MPU_IIC_SDA_1();
}

/*
*********************************************************************************************************
*	函 数 名: IIC_SendByte
*	功能说明: CPU向IIC总线设备发送8bit数据
*	形    参：_ucByte ： 等待发送的字节
*	返 回 值: 无
*********************************************************************************************************
*/
void MPU_IIC_Send_Byte(uint8_t _ucByte)
{
    uint8_t i;

    /* 先发送字节的高位bit7 */
    for (i = 0; i < 8; i++)
    {
        if (_ucByte & 0x80)
        {
            MPU_IIC_SDA_1();
        }
        else
        {
            MPU_IIC_SDA_0();
        }
        MPU_IIC_Delay();
        MPU_IIC_SCL_1();
        MPU_IIC_Delay();
        MPU_IIC_SCL_0();
        if (i == 7)
        {
            MPU_IIC_SDA_1(); // 释放总线
        }
        _ucByte <<= 1;	/* 左移一个bit */
        MPU_IIC_Delay();
    }
}

/*
*********************************************************************************************************
*	函 数 名: IIC_Ack
*	功能说明: CPU产生一个ACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void MPU_IIC_Ack(void)
{
    MPU_IIC_SDA_0();	/* CPU驱动SDA = 0 */
    MPU_IIC_Delay();
    MPU_IIC_SCL_1();	/* CPU产生1个时钟 */
    MPU_IIC_Delay();
    MPU_IIC_SCL_0();
    MPU_IIC_Delay();
    MPU_IIC_SDA_1();	/* CPU释放SDA总线 */
}

/*
*********************************************************************************************************
*	函 数 名: IIC_NAck
*	功能说明: CPU产生1个NACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/

void MPU_IIC_NAck(void)
{
    MPU_IIC_SDA_1();	/* CPU驱动SDA = 1 */
    MPU_IIC_Delay();
    MPU_IIC_SCL_1();	/* CPU产生1个时钟 */
    MPU_IIC_Delay();
    MPU_IIC_SCL_0();
    MPU_IIC_Delay();
}

/*
*********************************************************************************************************
*	函 数 名: IIC_ReadByte
*	功能说明: CPU从IIC总线设备读取8bit数据
*	形    参：无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
uint8_t MPU_IIC_Read_Byte(uint8_t ack)
{
    uint8_t i;
    uint8_t value;

    /* 读到第1个bit为数据的bit7 */
    value = 0;
    for (i = 0; i < 8; i++)
    {
        value <<= 1;
        MPU_IIC_SCL_1();
        MPU_IIC_Delay();
        if (MPU_IIC_SDA_READ())
        {
            value++;
        }
        MPU_IIC_SCL_0();
        MPU_IIC_Delay();
    }
    if(ack==0)
        MPU_IIC_NAck();
    else
        MPU_IIC_Ack();
    return value;
}

/*
*********************************************************************************************************
*	函 数 名: IIC_WaitAck
*	功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
*	形    参：无
*	返 回 值: 返回0表示正确应答，1表示无器件响应
*********************************************************************************************************
*/
uint8_t MPU_IIC_Wait_Ack(void)
{
    uint8_t re;

    MPU_IIC_SDA_1();	/* CPU释放SDA总线 */
    MPU_IIC_Delay();
    MPU_IIC_SCL_1();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
    MPU_IIC_Delay();
    if (MPU_IIC_SDA_READ())	/* CPU读取SDA口线状态 */
    {
        re = 1;
    }
    else
    {
        re = 0;
    }
    MPU_IIC_SCL_0();
    MPU_IIC_Delay();
    return re;
}


/*
*********************************************************************************************************
*	函 数 名: IIC_GPIO_Config
*	功能说明: 配置IIC总线的GPIO，采用模拟IO的方式实现
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void MPU_IIC_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    MPU_RCC_IIC_ENABLE;	/* 打开GPIO时钟 */

    GPIO_InitStructure.Pin = MPU_IIC_SCL_PIN | MPU_IIC_SDA_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;  	/* 开漏输出 */
    HAL_GPIO_Init(MPU_GPIO_PORT_IIC, &GPIO_InitStructure);

    /* 给一个停止信号, 复位IIC总线上的所有设备到待机模式 */
    MPU_IIC_Stop();
}

/*
*********************************************************************************************************
*	函 数 名: IIC_CheckDevice
*	功能说明: 检测IIC总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
*	形    参：_Address：设备的IIC总线地址
*	返 回 值: 返回值 0 表示正确， 返回1表示未探测到
*********************************************************************************************************
*/
uint8_t MPU_IIC_CheckDevice(uint8_t _Address)
{
    uint8_t ucAck;

    MPU_IIC_GPIO_Init();		/* 配置GPIO */

    MPU_IIC_Start();		/* 发送启动信号 */

    /* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
    MPU_IIC_Send_Byte(_Address|IIC_WR);
    ucAck = MPU_IIC_Wait_Ack();	/* 检测设备的ACK应答 */

    MPU_IIC_Stop();			/* 发送停止信号 */

    return ucAck;
}


//初始化MPU6050
//返回值:0,成功
//    其他,错误代码
uint8_t MPU_Init(void)
{
    uint8_t res;

    MPU_IIC_Init();//初始化IIC总线
    MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X80);	//复位MPU6050
    delay_ms(100);
    MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X00);	//唤醒MPU6050
    MPU_Set_Gyro_Fsr(3);					//陀螺仪传感器,±2000dps
    MPU_Set_Accel_Fsr(0);					//加速度传感器,±2g
    MPU_Set_Rate(50);						//设置采样率50Hz
    MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//关闭所有中断
    MPU_Write_Byte(MPU_USER_CTRL_REG,0X00);	//I2C主模式关闭
    MPU_Write_Byte(MPU_FIFO_EN_REG,0X00);	//关闭FIFO
    MPU_Write_Byte(MPU_INTBP_CFG_REG,0X80);	//INT引脚低电平有效
    res=MPU_Read_Byte(MPU_DEVICE_ID_REG);
    if(res==MPU_ADDR)//器件ID正确
    {
        MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X01);	//设置CLKSEL,PLL X轴为参考
        MPU_Write_Byte(MPU_PWR_MGMT2_REG,0X00);	//加速度与陀螺仪都工作
        MPU_Set_Rate(50);						//设置采样率为50Hz
    } else return 1;
    return 0;
}
//设置MPU6050陀螺仪传感器满量程范围
//fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
//返回值:0,设置成功
//    其他,设置失败
uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
{
    return MPU_Write_Byte(MPU_GYRO_CFG_REG,fsr<<3);//设置陀螺仪满量程范围
}
//设置MPU6050加速度传感器满量程范围
//fsr:0,±2g;1,±4g;2,±8g;3,±16g
//返回值:0,设置成功
//    其他,设置失败
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
    return MPU_Write_Byte(MPU_ACCEL_CFG_REG,fsr<<3);//设置加速度传感器满量程范围
}
//设置MPU6050的数字低通滤波器
//lpf:数字低通滤波频率(Hz)
//返回值:0,设置成功
//    其他,设置失败
uint8_t MPU_Set_LPF(uint16_t lpf)
{
    uint8_t data=0;
    if(lpf>=188)data=1;
    else if(lpf>=98)data=2;
    else if(lpf>=42)data=3;
    else if(lpf>=20)data=4;
    else if(lpf>=10)data=5;
    else data=6;
    return MPU_Write_Byte(MPU_CFG_REG,data);//设置数字低通滤波器
}
//设置MPU6050的采样率(假定Fs=1KHz)
//rate:4~1000(Hz)
//返回值:0,设置成功
//    其他,设置失败
uint8_t MPU_Set_Rate(uint16_t rate)
{
    uint8_t data;
    if(rate>1000)rate=1000;
    if(rate<4)rate=4;
    data=1000/rate-1;
    data=MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);	//设置数字低通滤波器
    return MPU_Set_LPF(rate/2);	//自动设置LPF为采样率的一半
}

//得到温度值
//返回值:温度值(扩大了100倍)
short MPU_Get_Temperature(void)
{
    uint8_t buf[2];
    short raw;
    float temp;
    MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf);
    raw=((uint16_t)buf[0]<<8)|buf[1];
    temp=36.53+((double)raw)/340;
    return temp*100;;
}
//得到陀螺仪值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
uint8_t MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
    uint8_t buf[6],res;
    res=MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
    if(res==0)
    {
        *gx=((uint16_t)buf[0]<<8)|buf[1];
        *gy=((uint16_t)buf[2]<<8)|buf[3];
        *gz=((uint16_t)buf[4]<<8)|buf[5];
    }
    return res;;
}
//得到加速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
uint8_t MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
    uint8_t buf[6],res;
    res=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
    if(res==0)
    {
        *ax=((uint16_t)buf[0]<<8)|buf[1];
        *ay=((uint16_t)buf[2]<<8)|buf[3];
        *az=((uint16_t)buf[4]<<8)|buf[5];
    }
    return res;;
}
//IIC连续写
//addr:器件地址
//reg:寄存器地址
//len:写入长度
//buf:数据区
//返回值:0,正常
//    其他,错误代码
uint8_t MPU_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
    uint8_t i;
    MPU_IIC_Start();
    MPU_IIC_Send_Byte((addr<<1)|0);//发送器件地址+写命令
    if(MPU_IIC_Wait_Ack())	//等待应答
    {
        MPU_IIC_Stop();
        return 1;
    }
    MPU_IIC_Send_Byte(reg);	//写寄存器地址
    MPU_IIC_Wait_Ack();		//等待应答
    for(i=0; i<len; i++)
    {
        MPU_IIC_Send_Byte(buf[i]);	//发送数据
        if(MPU_IIC_Wait_Ack())		//等待ACK
        {
            MPU_IIC_Stop();
            return 1;
        }
    }
    MPU_IIC_Stop();
    return 0;
}
//IIC连续读
//addr:器件地址
//reg:要读取的寄存器地址
//len:要读取的长度
//buf:读取到的数据存储区
//返回值:0,正常
//    其他,错误代码
uint8_t MPU_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
    MPU_IIC_Start();
    MPU_IIC_Send_Byte((addr<<1)|0);//发送器件地址+写命令
    if(MPU_IIC_Wait_Ack())	//等待应答
    {
        MPU_IIC_Stop();
        return 1;
    }
    MPU_IIC_Send_Byte(reg);	//写寄存器地址
    MPU_IIC_Wait_Ack();		//等待应答
    MPU_IIC_Start();
    MPU_IIC_Send_Byte((addr<<1)|1);//发送器件地址+读命令
    MPU_IIC_Wait_Ack();		//等待应答
    while(len)
    {
        if(len==1)*buf=MPU_IIC_Read_Byte(0);//读数据,发送nACK
        else *buf=MPU_IIC_Read_Byte(1);		//读数据,发送ACK
        len--;
        buf++;
    }
    MPU_IIC_Stop();	//产生一个停止条件
    return 0;
}
//IIC写一个字节
//reg:寄存器地址
//data:数据
//返回值:0,正常
//    其他,错误代码
uint8_t MPU_Write_Byte(uint8_t reg,uint8_t data)
{
    MPU_IIC_Start();
    MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);//发送器件地址+写命令
    if(MPU_IIC_Wait_Ack())	//等待应答
    {
        MPU_IIC_Stop();
        return 1;
    }
    MPU_IIC_Send_Byte(reg);	//写寄存器地址
    MPU_IIC_Wait_Ack();		//等待应答
    MPU_IIC_Send_Byte(data);//发送数据
    if(MPU_IIC_Wait_Ack())	//等待ACK
    {
        MPU_IIC_Stop();
        return 1;
    }
    MPU_IIC_Stop();
    return 0;
}
//IIC读一个字节
//reg:寄存器地址
//返回值:读到的数据
uint8_t MPU_Read_Byte(uint8_t reg)
{
    uint8_t res;
    MPU_IIC_Start();
    MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);//发送器件地址+写命令
    MPU_IIC_Wait_Ack();		//等待应答
    MPU_IIC_Send_Byte(reg);	//写寄存器地址
    MPU_IIC_Wait_Ack();		//等待应答
    MPU_IIC_Start();
    MPU_IIC_Send_Byte((MPU_ADDR<<1)|1);//发送器件地址+读命令
    MPU_IIC_Wait_Ack();		//等待应答
    res=MPU_IIC_Read_Byte(0);//读取数据,发送nACK
    MPU_IIC_Stop();			//产生一个停止条件
    return res;
}
