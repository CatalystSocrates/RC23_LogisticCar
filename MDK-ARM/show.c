#include "show.h"
//#include "main.h"
#include "system.h"
//#include "encoder.h"
//#include "DataScop_DP.h"
//#include "usart.h"

void screenshow(void){  
		ST7735_DrawFloat(5, 10, KF_X(Aacy,Aacz, -Gyrox)*100 , ST7735_WHITE, ST7735_BLACK, &Font_7x10);
    ST7735_DrawFloat(5, 25, KF_Y(Aacx,Aacz, Gyroy)*100 , ST7735_WHITE, ST7735_BLACK, &Font_7x10);
		ST7735_DrawData(5, 40, "PWMA:" , ST7735_WHITE, ST7735_BLACK, &Font_7x10);
		ST7735_DrawInt(45, 40, EncoderA , ST7735_WHITE, ST7735_BLACK, &Font_7x10);
		ST7735_DrawData(5, 55, "PWMB:" , ST7735_WHITE, ST7735_BLACK, &Font_7x10);
		ST7735_DrawInt(45, 55, EncoderB , ST7735_WHITE, ST7735_BLACK, &Font_7x10);
		ST7735_DrawData(5, 70, "PWMC:" , ST7735_WHITE, ST7735_BLACK, &Font_7x10);
		ST7735_DrawInt(45, 70, EncoderC , ST7735_WHITE, ST7735_BLACK, &Font_7x10);
		ST7735_DrawData(5, 85, "PWMD:" , ST7735_WHITE, ST7735_BLACK, &Font_7x10);
		ST7735_DrawInt(45, 85, PWMD , ST7735_WHITE, ST7735_BLACK, &Font_7x10);
		ST7735_DrawString(5,100,mapRGBCtoColor((&hsl)->h, (&hsl)->s, (&hsl)->l),ST7735_WHITE, ST7735_BLACK, &Font_7x10);
}

void DataScope(void)
{   
    int Send_Count,i;//计数需要的变量
    
    DataScope_Get_Channel_Data( EncoderA, 1 );      
    DataScope_Get_Channel_Data( Target_Velocity, 2 );      
    //	DataScope_Get_Channel_Data( 0, 3 );              
    //	DataScope_Get_Channel_Data( 0 , 4 );   
    //	DataScope_Get_Channel_Data(0, 5 ); //用您要显示的数据替换0就行了
    //	DataScope_Get_Channel_Data(0 , 6 );//用您要显示的数据替换0就行了
    //	DataScope_Get_Channel_Data(0, 7 );
    //	DataScope_Get_Channel_Data( 0, 8 ); 
    //	DataScope_Get_Channel_Data(0, 9 );  
    //	DataScope_Get_Channel_Data( 0 , 10);//一共可以打印10个数据查看波形
    Send_Count = DataScope_Data_Generate(2);//打印几个数据就在这里改为几
    for( i = 0 ; i < Send_Count; i++) 
    {
        HAL_UART_Transmit(&huart1, &DataScope_OutPut_Buffer[i], 1, HAL_MAX_DELAY); // 发送数据到串口
    }
}

