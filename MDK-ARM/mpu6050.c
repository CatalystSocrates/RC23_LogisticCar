#include "stm32f1xx_hal.h"
#include "mpu6050.h"
#include <inttypes.h>
#include "system.h"
#include "stm32f1xx_hal.h"

/* ����IIC�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */
#define MPU_GPIO_PORT_IIC     MPU6050_SDA_GPIO_Port                       /* GPIO�˿� */
#define MPU_RCC_IIC_ENABLE    __HAL_RCC_GPIOB_CLK_ENABLE()       /* GPIO�˿�ʱ�� */
#define MPU_IIC_SCL_PIN       MPU6050_SCL_Pin                  /* ���ӵ�SCLʱ���ߵ�GPIO */
#define MPU_IIC_SDA_PIN       MPU6050_SDA_Pin                  /* ���ӵ�SDA�����ߵ�GPIO */

/* �����дSCL��SDA�ĺ꣬�����Ӵ���Ŀ���ֲ�ԺͿ��Ķ��� */
#if 1	/* �������룺 1 ѡ��GPIO�Ŀ⺯��ʵ��IO��д */
#define MPU_IIC_SCL_1()  HAL_GPIO_WritePin(MPU_GPIO_PORT_IIC, MPU_IIC_SCL_PIN, GPIO_PIN_SET)		/* SCL = 1 */
#define MPU_IIC_SCL_0()  HAL_GPIO_WritePin(MPU_GPIO_PORT_IIC, MPU_IIC_SCL_PIN, GPIO_PIN_RESET)		/* SCL = 0 */

#define MPU_IIC_SDA_1()  HAL_GPIO_WritePin(MPU_GPIO_PORT_IIC, MPU_IIC_SDA_PIN, GPIO_PIN_SET)		/* SDA = 1 */
#define MPU_IIC_SDA_0()  HAL_GPIO_WritePin(MPU_GPIO_PORT_IIC, MPU_IIC_SDA_PIN, GPIO_PIN_RESET)		/* SDA = 0 */

#define MPU_IIC_SDA_READ()  HAL_GPIO_ReadPin(MPU_GPIO_PORT_IIC, MPU_IIC_SDA_PIN)	/* ��SDA����״̬ */
#else	/* �����֧ѡ��ֱ�ӼĴ�������ʵ��IO��д */
/*��ע�⣺����д������IAR��߼����Ż�ʱ���ᱻ�����������Ż� */
#define MPU_IIC_SCL_1()  MPU_GPIO_PORT_IIC->BSRR = MPU_IIC_SCL_PIN				/* SCL = 1 */
#define MPU_IIC_SCL_0()  MPU_GPIO_PORT_IIC->BRR = MPU_IIC_SCL_PIN				/* SCL = 0 */

#define MPU_IIC_SDA_1()  MPU_GPIO_PORT_IIC->BSRR = MPU_IIC_SDA_PIN				/* SDA = 1 */
#define MPU_IIC_SDA_0()  MPU_GPIO_PORT_IIC->BRR = MPU_IIC_SDA_PIN				/* SDA = 0 */

#define MPU_IIC_SDA_READ()  ((MPU_GPIO_PORT_IIC->IDR & MPU_IIC_SDA_PIN) != 0)	/* ��SDA����״̬ */
#endif

void MPU_IIC_GPIO_Init(void);

/*
*********************************************************************************************************
*	�� �� ��: IIC_Delay
*	����˵��: IIC����λ�ӳ٣����400KHz
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void MPU_IIC_Delay(void)
{
    uint8_t i;

    /*��
     	�����ʱ����ͨ��������AX-Pro�߼������ǲ��Եõ��ġ�
    	CPU��Ƶ72MHzʱ�����ڲ�Flash����, MDK���̲��Ż�
    	ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz
    	ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us
     	ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us

    IAR���̱���Ч�ʸߣ���������Ϊ7
    */
    for (i = 0; i < 10; i++);
}

/*
*********************************************************************************************************
*	�� �� ��: IIC_Start
*	����˵��: CPU����IIC���������ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void MPU_IIC_Start(void)
{
    /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾIIC���������ź� */
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
*	�� �� ��: IIC_Start
*	����˵��: CPU����IIC����ֹͣ�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void MPU_IIC_Stop(void)
{
    /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾIIC����ֹͣ�ź� */
    MPU_IIC_SDA_0();
    MPU_IIC_SCL_1();
    MPU_IIC_Delay();
    MPU_IIC_SDA_1();
}

/*
*********************************************************************************************************
*	�� �� ��: IIC_SendByte
*	����˵��: CPU��IIC�����豸����8bit����
*	��    �Σ�_ucByte �� �ȴ����͵��ֽ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void MPU_IIC_Send_Byte(uint8_t _ucByte)
{
    uint8_t i;

    /* �ȷ����ֽڵĸ�λbit7 */
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
            MPU_IIC_SDA_1(); // �ͷ�����
        }
        _ucByte <<= 1;	/* ����һ��bit */
        MPU_IIC_Delay();
    }
}

/*
*********************************************************************************************************
*	�� �� ��: IIC_Ack
*	����˵��: CPU����һ��ACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void MPU_IIC_Ack(void)
{
    MPU_IIC_SDA_0();	/* CPU����SDA = 0 */
    MPU_IIC_Delay();
    MPU_IIC_SCL_1();	/* CPU����1��ʱ�� */
    MPU_IIC_Delay();
    MPU_IIC_SCL_0();
    MPU_IIC_Delay();
    MPU_IIC_SDA_1();	/* CPU�ͷ�SDA���� */
}

/*
*********************************************************************************************************
*	�� �� ��: IIC_NAck
*	����˵��: CPU����1��NACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/

void MPU_IIC_NAck(void)
{
    MPU_IIC_SDA_1();	/* CPU����SDA = 1 */
    MPU_IIC_Delay();
    MPU_IIC_SCL_1();	/* CPU����1��ʱ�� */
    MPU_IIC_Delay();
    MPU_IIC_SCL_0();
    MPU_IIC_Delay();
}

/*
*********************************************************************************************************
*	�� �� ��: IIC_ReadByte
*	����˵��: CPU��IIC�����豸��ȡ8bit����
*	��    �Σ���
*	�� �� ֵ: ����������
*********************************************************************************************************
*/
uint8_t MPU_IIC_Read_Byte(uint8_t ack)
{
    uint8_t i;
    uint8_t value;

    /* ������1��bitΪ���ݵ�bit7 */
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
*	�� �� ��: IIC_WaitAck
*	����˵��: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
*	��    �Σ���
*	�� �� ֵ: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
*********************************************************************************************************
*/
uint8_t MPU_IIC_Wait_Ack(void)
{
    uint8_t re;

    MPU_IIC_SDA_1();	/* CPU�ͷ�SDA���� */
    MPU_IIC_Delay();
    MPU_IIC_SCL_1();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
    MPU_IIC_Delay();
    if (MPU_IIC_SDA_READ())	/* CPU��ȡSDA����״̬ */
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
*	�� �� ��: IIC_GPIO_Config
*	����˵��: ����IIC���ߵ�GPIO������ģ��IO�ķ�ʽʵ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void MPU_IIC_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    MPU_RCC_IIC_ENABLE;	/* ��GPIOʱ�� */

    GPIO_InitStructure.Pin = MPU_IIC_SCL_PIN | MPU_IIC_SDA_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;  	/* ��©��� */
    HAL_GPIO_Init(MPU_GPIO_PORT_IIC, &GPIO_InitStructure);

    /* ��һ��ֹͣ�ź�, ��λIIC�����ϵ������豸������ģʽ */
    MPU_IIC_Stop();
}

/*
*********************************************************************************************************
*	�� �� ��: IIC_CheckDevice
*	����˵��: ���IIC�����豸��CPU�����豸��ַ��Ȼ���ȡ�豸Ӧ�����жϸ��豸�Ƿ����
*	��    �Σ�_Address���豸��IIC���ߵ�ַ
*	�� �� ֵ: ����ֵ 0 ��ʾ��ȷ�� ����1��ʾδ̽�⵽
*********************************************************************************************************
*/
uint8_t MPU_IIC_CheckDevice(uint8_t _Address)
{
    uint8_t ucAck;

    MPU_IIC_GPIO_Init();		/* ����GPIO */

    MPU_IIC_Start();		/* ���������ź� */

    /* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
    MPU_IIC_Send_Byte(_Address|IIC_WR);
    ucAck = MPU_IIC_Wait_Ack();	/* ����豸��ACKӦ�� */

    MPU_IIC_Stop();			/* ����ֹͣ�ź� */

    return ucAck;
}


//��ʼ��MPU6050
//����ֵ:0,�ɹ�
//    ����,�������
uint8_t MPU_Init(void)
{
    uint8_t res;

    MPU_IIC_Init();//��ʼ��IIC����
    MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X80);	//��λMPU6050
    delay_ms(100);
    MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X00);	//����MPU6050
    MPU_Set_Gyro_Fsr(3);					//�����Ǵ�����,��2000dps
    MPU_Set_Accel_Fsr(0);					//���ٶȴ�����,��2g
    MPU_Set_Rate(50);						//���ò�����50Hz
    MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//�ر������ж�
    MPU_Write_Byte(MPU_USER_CTRL_REG,0X00);	//I2C��ģʽ�ر�
    MPU_Write_Byte(MPU_FIFO_EN_REG,0X00);	//�ر�FIFO
    MPU_Write_Byte(MPU_INTBP_CFG_REG,0X80);	//INT���ŵ͵�ƽ��Ч
    res=MPU_Read_Byte(MPU_DEVICE_ID_REG);
    if(res==MPU_ADDR)//����ID��ȷ
    {
        MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X01);	//����CLKSEL,PLL X��Ϊ�ο�
        MPU_Write_Byte(MPU_PWR_MGMT2_REG,0X00);	//���ٶ��������Ƕ�����
        MPU_Set_Rate(50);						//���ò�����Ϊ50Hz
    } else return 1;
    return 0;
}
//����MPU6050�����Ǵ����������̷�Χ
//fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
//����ֵ:0,���óɹ�
//    ����,����ʧ��
uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
{
    return MPU_Write_Byte(MPU_GYRO_CFG_REG,fsr<<3);//���������������̷�Χ
}
//����MPU6050���ٶȴ����������̷�Χ
//fsr:0,��2g;1,��4g;2,��8g;3,��16g
//����ֵ:0,���óɹ�
//    ����,����ʧ��
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
    return MPU_Write_Byte(MPU_ACCEL_CFG_REG,fsr<<3);//���ü��ٶȴ����������̷�Χ
}
//����MPU6050�����ֵ�ͨ�˲���
//lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ��
uint8_t MPU_Set_LPF(uint16_t lpf)
{
    uint8_t data=0;
    if(lpf>=188)data=1;
    else if(lpf>=98)data=2;
    else if(lpf>=42)data=3;
    else if(lpf>=20)data=4;
    else if(lpf>=10)data=5;
    else data=6;
    return MPU_Write_Byte(MPU_CFG_REG,data);//�������ֵ�ͨ�˲���
}
//����MPU6050�Ĳ�����(�ٶ�Fs=1KHz)
//rate:4~1000(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ��
uint8_t MPU_Set_Rate(uint16_t rate)
{
    uint8_t data;
    if(rate>1000)rate=1000;
    if(rate<4)rate=4;
    data=1000/rate-1;
    data=MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);	//�������ֵ�ͨ�˲���
    return MPU_Set_LPF(rate/2);	//�Զ�����LPFΪ�����ʵ�һ��
}

//�õ��¶�ֵ
//����ֵ:�¶�ֵ(������100��)
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
//�õ�������ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
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
//�õ����ٶ�ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
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
//IIC����д
//addr:������ַ
//reg:�Ĵ�����ַ
//len:д�볤��
//buf:������
//����ֵ:0,����
//    ����,�������
uint8_t MPU_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
    uint8_t i;
    MPU_IIC_Start();
    MPU_IIC_Send_Byte((addr<<1)|0);//����������ַ+д����
    if(MPU_IIC_Wait_Ack())	//�ȴ�Ӧ��
    {
        MPU_IIC_Stop();
        return 1;
    }
    MPU_IIC_Send_Byte(reg);	//д�Ĵ�����ַ
    MPU_IIC_Wait_Ack();		//�ȴ�Ӧ��
    for(i=0; i<len; i++)
    {
        MPU_IIC_Send_Byte(buf[i]);	//��������
        if(MPU_IIC_Wait_Ack())		//�ȴ�ACK
        {
            MPU_IIC_Stop();
            return 1;
        }
    }
    MPU_IIC_Stop();
    return 0;
}
//IIC������
//addr:������ַ
//reg:Ҫ��ȡ�ļĴ�����ַ
//len:Ҫ��ȡ�ĳ���
//buf:��ȡ�������ݴ洢��
//����ֵ:0,����
//    ����,�������
uint8_t MPU_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
    MPU_IIC_Start();
    MPU_IIC_Send_Byte((addr<<1)|0);//����������ַ+д����
    if(MPU_IIC_Wait_Ack())	//�ȴ�Ӧ��
    {
        MPU_IIC_Stop();
        return 1;
    }
    MPU_IIC_Send_Byte(reg);	//д�Ĵ�����ַ
    MPU_IIC_Wait_Ack();		//�ȴ�Ӧ��
    MPU_IIC_Start();
    MPU_IIC_Send_Byte((addr<<1)|1);//����������ַ+������
    MPU_IIC_Wait_Ack();		//�ȴ�Ӧ��
    while(len)
    {
        if(len==1)*buf=MPU_IIC_Read_Byte(0);//������,����nACK
        else *buf=MPU_IIC_Read_Byte(1);		//������,����ACK
        len--;
        buf++;
    }
    MPU_IIC_Stop();	//����һ��ֹͣ����
    return 0;
}
//IICдһ���ֽ�
//reg:�Ĵ�����ַ
//data:����
//����ֵ:0,����
//    ����,�������
uint8_t MPU_Write_Byte(uint8_t reg,uint8_t data)
{
    MPU_IIC_Start();
    MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);//����������ַ+д����
    if(MPU_IIC_Wait_Ack())	//�ȴ�Ӧ��
    {
        MPU_IIC_Stop();
        return 1;
    }
    MPU_IIC_Send_Byte(reg);	//д�Ĵ�����ַ
    MPU_IIC_Wait_Ack();		//�ȴ�Ӧ��
    MPU_IIC_Send_Byte(data);//��������
    if(MPU_IIC_Wait_Ack())	//�ȴ�ACK
    {
        MPU_IIC_Stop();
        return 1;
    }
    MPU_IIC_Stop();
    return 0;
}
//IIC��һ���ֽ�
//reg:�Ĵ�����ַ
//����ֵ:����������
uint8_t MPU_Read_Byte(uint8_t reg)
{
    uint8_t res;
    MPU_IIC_Start();
    MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);//����������ַ+д����
    MPU_IIC_Wait_Ack();		//�ȴ�Ӧ��
    MPU_IIC_Send_Byte(reg);	//д�Ĵ�����ַ
    MPU_IIC_Wait_Ack();		//�ȴ�Ӧ��
    MPU_IIC_Start();
    MPU_IIC_Send_Byte((MPU_ADDR<<1)|1);//����������ַ+������
    MPU_IIC_Wait_Ack();		//�ȴ�Ӧ��
    res=MPU_IIC_Read_Byte(0);//��ȡ����,����nACK
    MPU_IIC_Stop();			//����һ��ֹͣ����
    return res;
}
