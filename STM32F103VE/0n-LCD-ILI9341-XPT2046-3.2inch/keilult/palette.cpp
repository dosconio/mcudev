
#include "led.h"
extern "C" {
#include "palette.h"
#include "bsp_xpt2046_lcd.h"
#include "bsp_ili9341_lcd.h"
/*按钮结构体数组*/
Touch_Button button[BUTTON_NUM];

/*画笔参数*/
Brush_Style brush;

}

 

static void Draw_Color_Button(void *btn);
static void Draw_Clear_Button(void *btn);
static void Draw_Shape_Button(void *btn);

static void Command_Select_Color(void *btn);
static void Command_Select_Brush(void *btn);
static void Command_Clear_Palette(void *btn);

static void LCD_DrawUniLineCircle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint8_t thick );



/**
* @brief  Palette_Init 画板初始化
* @param  无
* @retval 无
*/
void Palette_Init(uint8_t LCD_Mode)
{
  
  uint8_t i;

	ILI9341_GramScan ( LCD_Mode );
	
  /* 整屏清为白色 */
	LCD_SetBackColor(CL_WHITE);
  ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	

  
  /* 初始化按钮 */
	// ...
  
  /* 描绘按钮 */
  for(i=0;i<BUTTON_NUM;i++)
  {
    button[i].draw_btn(&button[i]);
  }
  
	
  /* 初始化画笔 */
  brush.color = CL_BLACK;
  brush.shape = LINE_SINGLE_PIXCEL;
	
	LCD_SetTextColor(brush.color);


}


/**
* @brief  Touch_Button_Down 按键被按下时调用的函数，由触摸屏调用
* @param  x 触摸位置的x坐标
* @param  y 触摸位置的y坐标
* @retval 无
*/
void Touch_Button_Down(uint16_t x,uint16_t y)
{
  uint8_t i;
  for(i=0;i<BUTTON_NUM;i++)
  {
    /* 触摸到了按钮 */
    if(x<=button[i].end_x && y<=button[i].end_y && y>=button[i].start_y && x>=button[i].start_x )
    {
      if(button[i].touch_flag == 0)     /*原本的状态为没有按下，则更新状态*/
      {
      button[i].touch_flag = 1;         /* 记录按下标志 */
      
      button[i].draw_btn(&button[i]);  /*重绘按钮*/
      }        
      
    }
    else if(button[i].touch_flag == 1) /* 触摸移出了按键的范围且之前有按下按钮 */
    {
      button[i].touch_flag = 0;         /* 清除按下标志，判断为误操作*/
      
      button[i].draw_btn(&button[i]);   /*重绘按钮*/
    }

  }
	//LED_BLUE_REF() = false;
}

/**
* @brief  Touch_Button_Up 按键被释放时调用的函数，由触摸屏调用
* @param  x 触摸最后释放时的x坐标
* @param  y 触摸最后释放时的y坐标
* @retval 无
*/
void Touch_Button_Up(uint16_t x,uint16_t y)
{
   uint8_t i; 
   for(i=0;i<BUTTON_NUM;i++)
   {
     /* 触笔在按钮区域释放 */
      if((x<button[i].end_x && x>button[i].start_x && y<button[i].end_y && y>button[i].start_y))
      {        
        button[i].touch_flag = 0;       /*释放触摸标志*/
        
        button[i].draw_btn(&button[i]); /*重绘按钮*/        
      
        button[i].btn_command(&button[i]);  /*执行按键的功能命令*/
        
        break;
      }
    }  
	//LED_BLUE_REF() = true;
}

/**
* @brief  Command_Select_Color 切换画刷颜色，颜色按键的功能执行函数
* @param  btn Touch_Button 类型的按键参数
* @retval 无
*/
static void Command_Select_Color(void *btn)
{
  Touch_Button *ptr = (Touch_Button *)btn;
  
  brush.color = ptr->para;	
	LCD_SetColors(brush.color,CL_WHITE);
  
  if(brush.shape == RUBBER)
  {
    brush.shape = LINE_SINGLE_PIXCEL;
  }

}

/**
* @brief  Command_Select_Brush 切换画刷颜色，画刷按键的功能执行函数
* @param  btn Touch_Button 类型的按键参数
* @retval 无
*/
static void Command_Select_Brush(void *btn)
{
  Touch_Button *ptr = (Touch_Button *)btn;
  brush.shape =(SHAPE) ptr->para;
	LCD_SetColors(brush.color,CL_WHITE);
}

#define ABS(X)  ((X) > 0 ? (X) : -(X))

/**
  * @brief  在两点之间描绘轨迹
  * @param  x1: specifies the point 1 x position.
  * @param  y1: specifies the point 1 y position.
  * @param  x2: specifies the point 2 x position.
  * @param  y2: specifies the point 2 y position.
  * @retval None
  */
static void LCD_DrawUniLineCircle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint8_t thick )
{
  int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
  yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
  curpixel = 0;
  
  deltax = ABS(x2 - x1);        /* The difference between the x's */
  deltay = ABS(y2 - y1);        /* The difference between the y's */
  x = x1;                       /* Start x off at the first pixel */
  y = y1;                       /* Start y off at the first pixel */
  
	
  if (x2 >= x1)                 /* The x-values are increasing */
  {
    xinc1 = 1;
    xinc2 = 1;
  }
  else                          /* The x-values are decreasing */
  {
    xinc1 = -1;
    xinc2 = -1;
  }
  
  if (y2 >= y1)                 /* The y-values are increasing */
  {
    yinc1 = 1;
    yinc2 = 1;
  }
  else                          /* The y-values are decreasing */
  {
    yinc1 = -1;
    yinc2 = -1;
  }
  
  if (deltax >= deltay)         /* There is at least one x-value for every y-value */
  {
    xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
    yinc2 = 0;                  /* Don't change the y for every iteration */
    den = deltax;
    num = deltax / 2;
    numadd = deltay;
    numpixels = deltax;         /* There are more x-values than y-values */
  }
  else                          /* There is at least one y-value for every x-value */
  {
    xinc2 = 0;                  /* Don't change the x for every iteration */
    yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
    den = deltay;
    num = deltay / 2;
    numadd = deltax;
    numpixels = deltay;         /* There are more y-values than x-values */
  }
  
  for (curpixel = 0; curpixel <= numpixels; curpixel++)
  {
		
		//判断边界
		if(x+thick>LCD_X_LENGTH || x-thick<0 || //液晶左右边界
			y+thick>LCD_Y_LENGTH || y-thick<0  ) //液晶上下边界
			continue;

    ILI9341_DrawCircle(x,y,thick,1);             /* Draw the current pixel */
    num += numadd;              /* Increase the numerator by the top of the fraction */
    if (num >= den)             /* Check if numerator >= denominator */
    {
      num -= den;               /* Calculate the new numerator value */
      x += xinc1;               /* Change the x as appropriate */
      y += yinc1;               /* Change the y as appropriate */
    }
    x += xinc2;                 /* Change the x as appropriate */
    y += yinc2;                 /* Change the y as appropriate */
  }
}




/* ------------------------------------------end of file---------------------------------------- */



