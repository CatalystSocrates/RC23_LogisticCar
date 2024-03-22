#include "show.h"
#include "main.h"
#include "tim.h"
extern float pitch,roll,yaw; 		    
extern short aacx,aacy,aacz;			
extern short gyrox,gyroy,gyroz;		
extern float temp;
extern float Aacx,Aacy,Aacz,Gyrox,Gyroy;
extern int pwma, pwmb,pwmc, pwmd;
extern COLOR_HSL hsl;

void screenshow(void){  
		ST7735_DrawFloat(5, 10, KF_X(Aacy,Aacz, -Gyrox)*100 , ST7735_WHITE, ST7735_BLACK, &Font_7x10);
    ST7735_DrawFloat(5, 25, KF_Y(Aacx,Aacz, Gyroy)*100 , ST7735_WHITE, ST7735_BLACK, &Font_7x10);
		ST7735_DrawData(5, 40, "PWMA:" , ST7735_WHITE, ST7735_BLACK, &Font_7x10);
		ST7735_DrawInt(45, 40, PWMA , ST7735_WHITE, ST7735_BLACK, &Font_7x10);
		ST7735_DrawData(5, 55, "PWMB:" , ST7735_WHITE, ST7735_BLACK, &Font_7x10);
		ST7735_DrawInt(45, 55, PWMB , ST7735_WHITE, ST7735_BLACK, &Font_7x10);
		ST7735_DrawData(5, 70, "PWMC:" , ST7735_WHITE, ST7735_BLACK, &Font_7x10);
		ST7735_DrawInt(45, 70, PWMC , ST7735_WHITE, ST7735_BLACK, &Font_7x10);
		ST7735_DrawData(5, 85, "PWMD:" , ST7735_WHITE, ST7735_BLACK, &Font_7x10);
		ST7735_DrawInt(45, 85, PWMD , ST7735_WHITE, ST7735_BLACK, &Font_7x10);
		ST7735_DrawString(5,100,mapRGBCtoColor((&hsl)->h, (&hsl)->s, (&hsl)->l),ST7735_WHITE, ST7735_BLACK, &Font_7x10);
}

