#include "TCS34725.h"
#include "system.h"

#define us_num 50

#define TCS34725_SCL_OUT_H HAL_GPIO_WritePin(TCS34725_SCL_GPIO_Port, TCS34725_SCL_Pin, GPIO_PIN_SET)
#define TCS34725_SCL_OUT_L HAL_GPIO_WritePin(TCS34725_SCL_GPIO_Port, TCS34725_SCL_Pin, GPIO_PIN_RESET)
#define TCS34725_SDA_OUT_H HAL_GPIO_WritePin(TCS34725_SDA_GPIO_Port, TCS34725_SDA_Pin, GPIO_PIN_SET)
#define TCS34725_SDA_OUT_L HAL_GPIO_WritePin(TCS34725_SDA_GPIO_Port, TCS34725_SDA_Pin, GPIO_PIN_RESET)
#define TCS34725_SDA_IN HAL_GPIO_ReadPin(TCS34725_SDA_GPIO_Port, TCS34725_SDA_Pin)


void TCS34725_I2C_Init(void)
{
	TCS34725_SDA_OUT_H;
	TCS34725_SCL_OUT_H;
	//PY_Delay_us_t(1000000) ;
}

void TCS34725_I2C_Start(void)
{
	Delay_us_t(us_num) ;
	TCS34725_SDA_OUT_H;
	TCS34725_SCL_OUT_H;
	Delay_us_t(us_num/2) ;
	TCS34725_SDA_OUT_L;
	Delay_us_t(us_num/2) ;
	TCS34725_SCL_OUT_L;
}

void TCS34725_I2C_Stop(void)
{
	TCS34725_SCL_OUT_L;
	Delay_us_t(us_num) ;
	TCS34725_SDA_OUT_L;
	Delay_us_t(us_num) ;
	TCS34725_SCL_OUT_H;
	Delay_us_t(us_num) ;
	TCS34725_SDA_OUT_H;
	Delay_us_t(us_num) ;
}

void TCS34725_I2C_Write_Ack(void)
{

  Delay_us_t(us_num/2) ;
	TCS34725_SDA_OUT_L;
	Delay_us_t(us_num/2) ;
	TCS34725_SCL_OUT_H;
	Delay_us_t(us_num) ;
	TCS34725_SCL_OUT_L;
	TCS34725_SDA_OUT_H;

}

uint8_t TCS34725_I2C_Read_Ack(void)
{
	uint8_t status=0;

	TCS34725_SCL_OUT_L;
	Delay_us_t(us_num/2) ;
	TCS34725_SDA_OUT_H;
	Delay_us_t(us_num/2) ;
	status = TCS34725_SDA_IN;
	TCS34725_SCL_OUT_H;
	Delay_us_t(us_num) ;
	TCS34725_SCL_OUT_L;
	TCS34725_SDA_OUT_L;

	return status;

}


void TCS34725_I2C_Send_Byte(uint8_t txd)
{


    for(uint8_t i=0;i<8;i++)
    {
    	Delay_us_t(us_num/2) ;
        if((txd&0x80)>>7) TCS34725_SDA_OUT_H;
        else TCS34725_SDA_OUT_L;
        txd<<=1;
        Delay_us_t(us_num/2) ;
        TCS34725_SCL_OUT_H;
        Delay_us_t(us_num) ;
		TCS34725_SCL_OUT_L;
    }

    TCS34725_SDA_OUT_L;
}

uint8_t TCS34725_I2C_Read_Byte(unsigned char rdack)
{
	uint8_t rxd=0;


    for(uint8_t i=0;i<8;i++ )
	{
    	TCS34725_SCL_OUT_L;
    	Delay_us_t(us_num/2) ;
    	TCS34725_SDA_OUT_H;
    	Delay_us_t(us_num/2) ;
    	TCS34725_SCL_OUT_H;
        rxd<<=1;
        if(TCS34725_SDA_IN) rxd++;
        Delay_us_t(us_num) ;
    }

    TCS34725_SCL_OUT_L;
    TCS34725_SDA_OUT_H;

    if (rdack) TCS34725_I2C_Write_Ack();

    return rxd;
}

uint8_t TCS3472_Enable_Status = 0;
void TCS3472_WRITE( uint8_t WrAddr, uint8_t data)
{
	  uint8_t daddr = 0x52; //TCS3472 device address (0x29<<1)

	  TCS34725_I2C_Start();
	  TCS34725_I2C_Send_Byte(daddr);
	  TCS34725_I2C_Read_Ack();
  	TCS34725_I2C_Send_Byte(WrAddr|0x80);
  	TCS34725_I2C_Read_Ack();
  	TCS34725_I2C_Send_Byte(data);
  	TCS34725_I2C_Read_Ack();
  	TCS34725_I2C_Stop();
}

uint8_t TCS3472_READ_1Byte( uint8_t RdAddr)
{

	  uint8_t RegValue = 0;
	  uint8_t daddr = 0x52; //TCS3472 device address (0x29<<1)

	  TCS34725_I2C_Start();
	  TCS34725_I2C_Send_Byte(daddr);
	  TCS34725_I2C_Read_Ack();
  	TCS34725_I2C_Send_Byte(RdAddr|0x80); //Repeated byte protocol transaction
  	TCS34725_I2C_Read_Ack();

  	TCS34725_I2C_Start();
	  TCS34725_I2C_Send_Byte(daddr+1);
	  TCS34725_I2C_Read_Ack();
	  RegValue=TCS34725_I2C_Read_Byte(0);
  	TCS34725_I2C_Stop();

	  return RegValue;
}

uint16_t TCS3472_READ_2Byte( uint8_t RdAddr)
{

	  uint8_t RegValueH = 0, RegValueL = 0;
	  uint8_t daddr = 0x52; //TCS3472 device address (0x29<<1)

	  TCS34725_I2C_Start();
	  TCS34725_I2C_Send_Byte(daddr);
	  TCS34725_I2C_Read_Ack();
  	TCS34725_I2C_Send_Byte(RdAddr|0xa0); //Auto-increment protocol transaction
  	TCS34725_I2C_Read_Ack();

  	TCS34725_I2C_Start();
	  TCS34725_I2C_Send_Byte(daddr+1);
	  TCS34725_I2C_Read_Ack();
	  RegValueL=TCS34725_I2C_Read_Byte(1);
	  RegValueH=TCS34725_I2C_Read_Byte(0);
  	TCS34725_I2C_Stop();

	  return (((uint16_t)RegValueH)<<8)|RegValueL;
}
void TCS3472_PON_Enable(void)
{
	TCS3472_Enable_Status |= TCS3472_ENABLE_PON;

	TCS3472_WRITE(TCS3472_ENABLE, TCS3472_Enable_Status);
}
void TCS3472_PON_Disable(void)
{
	TCS3472_Enable_Status &= (~TCS3472_ENABLE_PON);

	TCS3472_WRITE(TCS3472_ENABLE, TCS3472_Enable_Status);
}


void TCS3472_AEN_Enable(void)
{
	TCS3472_Enable_Status |=  TCS3472_ENABLE_AEN;

	TCS3472_WRITE(TCS3472_ENABLE, TCS3472_Enable_Status);
}

void TCS3472_AEN_Disable(void)
{
	TCS3472_Enable_Status &= (~TCS3472_ENABLE_AEN);

	TCS3472_WRITE(TCS3472_ENABLE, TCS3472_Enable_Status);
}

void TCS3472_WEN_Enable(void)
{
	TCS3472_Enable_Status |= TCS3472_ENABLE_WEN;

	TCS3472_WRITE(TCS3472_ENABLE, TCS3472_Enable_Status);
}
void TCS3472_WEN_Disable(void)
{
	TCS3472_Enable_Status &= (~TCS3472_ENABLE_WEN);

	TCS3472_WRITE(TCS3472_ENABLE, TCS3472_Enable_Status);
}

void TCS3472_AIEN_Enable(void)
{
	TCS3472_Enable_Status |= TCS3472_ENABLE_AIEN;

	TCS3472_WRITE(TCS3472_ENABLE, TCS3472_Enable_Status);
}
void TCS3472_AIEN_Disable(void)
{
	TCS3472_Enable_Status &= (~TCS3472_ENABLE_AIEN);

	TCS3472_WRITE(TCS3472_ENABLE, TCS3472_Enable_Status);
}

uint8_t TCS3472_ID_Verification(void)
{
	uint8_t id;
	id = TCS3472_READ_1Byte(TCS3472_ID);
	if((id==0x44)||(id==0x4d)) return 1;
	else return 0;
}

void TCS34725_Init()
{
	if (!TCS3472_ID_Verification())  //For communication error verification
	{
		while(1)
		{
			printf("error");
			Delay_us(2000000);
		}
	}
	else  //Initial config
	{
		TCS3472_PON_Enable(); //Other register config must be after TCS3472_PON_Enable().

		TCS3472_WRITE(TCS3472_ATIME, TCS3472_INTEGRATIONTIME_24MS);
		TCS3472_WRITE(TCS3472_CONTROL, TCS3472_GAIN_1X);

		TCS3472_AEN_Enable();
	}
}

void TCS34725_GET_RGB(COLOR_RGBC *Rgb)
{
	if(TCS3472_READ_1Byte(TCS3472_STATUS) & TCS3472_STATUS_AVALID)
      {
    	  Rgb ->c = TCS3472_READ_2Byte(TCS3472_CDATAL);
    	  Rgb ->r	= TCS3472_READ_2Byte(TCS3472_RDATAL);
    	  Rgb ->g	= TCS3472_READ_2Byte(TCS3472_GDATAL);
				Rgb ->b = TCS3472_READ_2Byte(TCS3472_BDATAL);
			}
}

//COLOR_RGBC rgb;
//COLOR_HSL  hsl;

//Conversion of RGB to HSL
void RGBtoHSL(COLOR_RGBC *Rgb, COLOR_HSL *Hsl)
{
	uint8_t maxVal,minVal,difVal;
	uint8_t r = Rgb->r*100/Rgb->c;   //[0-100]
	uint8_t g = Rgb->g*100/Rgb->c;
	uint8_t b = Rgb->b*100/Rgb->c;

	maxVal = max3v(r,g,b);
	minVal = min3v(r,g,b);
	difVal = maxVal-minVal;

	//Lightness
	Hsl->l = (maxVal+minVal)/2;   //[0-100]

	if(maxVal == minVal)//if r=g=b, grey
	{
		Hsl->h = 0;
		Hsl->s = 0;
	}
	else
	{
		//Hue
		if(maxVal==r)
		{
			if(g>=b)
				Hsl->h = 60*(g-b)/difVal;
			else
				Hsl->h = 60*(g-b)/difVal+360;
		}
		else
			{
				if(maxVal==g)Hsl->h = 60*(b-r)/difVal+120;
				else
					if(maxVal==b)Hsl->h = 60*(r-g)/difVal+240;
			}

		//Saturation
		if(Hsl->l<=50)Hsl->s=difVal*100/(maxVal+minVal);  //[0-100]
		else
			Hsl->s=difVal*100/(200-(maxVal+minVal));
	}
}

const char* mapRGBCtoColor(float hue, float saturation, float Lightness) {
    // 根据饱和度和亮度判断颜色
	hue = hue - 40;
	float lightness = Lightness /100;
    if (saturation < 0.1) {
        if (lightness < 0.1) return "BLACK";
        else if (lightness > 0.9) return "WHITE";
        else return "GRAY";
    }

    // 根据色相、饱和度和亮度判断颜色
    if (lightness < 0.2) return "BLACK";
    else if (lightness > 0.8) return "WHITE";
    else if (hue < 15.0 || hue >= 345.0) return "RED";
    else if (hue < 45.0) return "ORANGE";
    else if (hue < 75.0) return "YELLOW";
    else if (hue < 105.0) return "LIME";
    else if (hue < 135.0) return "GREEN";
    else if (hue < 165.0) return "TEAL";
    else if (hue < 195.0) return "CYAN";
    else if (hue < 225.0) return "AZURE";
    else if (hue < 255.0) return "BLUE";
    else if (hue < 285.0) return "VIOLET";
    else if (hue < 315.0) return "MAGENTA";
    else if (hue < 345.0) return "PINK";
    else return "UNKNOWN";
}
