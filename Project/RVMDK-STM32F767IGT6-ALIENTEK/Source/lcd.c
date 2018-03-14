#include "lcd.h"
#include "stdlib.h"
#include "font.h" 
#include "ltdc.h" 

SRAM_HandleTypeDef SRAM_Handler;
u32 POINT_COLOR=0xFF000000;
u32 BACK_COLOR =0xFFFFFFFF;

_lcd_dev lcddev;

u16 LCD_BGR2RGB(u16 c)
{
    u16  r,g,b,rgb;   
    b=(c>>0)&0x1f;
    g=(c>>5)&0x3f;
    r=(c>>11)&0x1f;     
    rgb=(b<<11)+(g<<5)+(r<<0);         
    return(rgb);
} 

void opt_delay(u8 i)
{
    while(i--);
}

u32 LCD_ReadPoint(u16 x,u16 y)
{
     u16 r=0,g=0,b=0;
    if(x>=lcddev.width||y>=lcddev.height)return 0;
    return LTDC_Read_Point(x,y);
}

void LCD_DisplayOn(void)
{                       
    LTDC_Switch(1);
}

void LCD_DisplayOff(void)
{       
    LTDC_Switch(0);
}

void LCD_DrawPoint(u16 x,u16 y)
{
    if(lcdltdc.pwidth!=0)
    {
        LTDC_Draw_Point(x,y,POINT_COLOR);
    }else
    {
        LCD_SetCursor(x,y);
        LCD_WriteRAM_Prepare();
        LCD->LCD_RAM=POINT_COLOR; 
    }
}

void LCD_Fast_DrawPoint(u16 x,u16 y,u32 color)
{       
    if(lcdltdc.pwidth!=0)
    {
        LTDC_Draw_Point(x,y,color);
        return;
    }
}

void LCD_Display_Dir(u8 dir)
{
    lcddev.dir=dir;
    LTDC_Display_Dir(dir);
    lcddev.width=lcdltdc.width;
    lcddev.height=lcdltdc.height;
}

void LCD_MPU_Config(void)
{    
    MPU_Region_InitTypeDef MPU_Initure;

    HAL_MPU_Disable();
    MPU_Initure.Enable=MPU_REGION_ENABLE;
    MPU_Initure.Number=LCD_REGION_NUMBER;
    MPU_Initure.BaseAddress=LCD_ADDRESS_START;
    MPU_Initure.Size=LCD_REGION_SIZE;
    MPU_Initure.SubRegionDisable=0X00;
    MPU_Initure.TypeExtField=MPU_TEX_LEVEL0;
    MPU_Initure.AccessPermission=MPU_REGION_FULL_ACCESS;
    MPU_Initure.DisableExec=MPU_INSTRUCTION_ACCESS_ENABLE;
    MPU_Initure.IsShareable=MPU_ACCESS_NOT_SHAREABLE;
    MPU_Initure.IsCacheable=MPU_ACCESS_NOT_CACHEABLE;
    MPU_Initure.IsBufferable=MPU_ACCESS_BUFFERABLE;
    HAL_MPU_ConfigRegion(&MPU_Initure);
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_FMC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7|GPIO_PIN_8|\
                     GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;
    GPIO_Initure.Pull=GPIO_PULLUP;
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;
    GPIO_Initure.Alternate=GPIO_AF12_FMC;
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);
    
    GPIO_Initure.Pin=GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|\
                     GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
}

void LCD_Init(void)
{       
    GPIO_InitTypeDef GPIO_Initure;
    FMC_NORSRAM_TimingTypeDef FSMC_ReadWriteTim;
    FMC_NORSRAM_TimingTypeDef FSMC_WriteTim;
    
    lcddev.id=LTDC_PanelID_Read();
    if(lcddev.id!=0)
    {
        LTDC_Init();
    }

    LCD_Display_Dir(0);
    LCD_LED(1);
    LCD_Clear(WHITE);
}

void LCD_Clear(u32 color)
{
    u32 index=0;      
    u32 totalpoint=lcddev.width;
    if(lcdltdc.pwidth!=0)
    {
        LTDC_Clear(color);
    }else
    {
        totalpoint*=lcddev.height;
        LCD_SetCursor(0x00,0x0000);
        LCD_WriteRAM_Prepare();
        for(index=0;index<totalpoint;index++)
        {
            LCD->LCD_RAM=color;    
        }
    }
}






























