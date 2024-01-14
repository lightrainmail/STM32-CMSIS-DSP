#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "main.h"
#include "spi.h"
#include "lcd.h"

#define USE_HORIZONTAL 0  //显示方向控制


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 240
#define LCD_H 280

#else
#define LCD_W 280
#define LCD_H 240
#endif

#define SPIHandle   hspi1


//-----------------LCD�˿ڶ���----------------

#define RES_GPIO_Port   GPIOA
#define RES_Pin         GPIO_PIN_6

#define DC_GPIO_Port    GPIOB
#define DC_Pin          GPIO_PIN_6

#define CS_GPIO_Port    GPIOA
#define CS_Pin          GPIO_PIN_9

#define BLK_GPIO_Port   GPIOC
#define BLK_Pin         GPIO_PIN_7


#define LCD_RES_Clr()  lcd_interface_res_clr()
#define LCD_RES_Set()  lcd_interface_res_set()

#define LCD_DC_Clr()   lcd_interface_dc_lcr()//DC
#define LCD_DC_Set()   lcd_interface_dc_set()
 		     
#define LCD_CS_Clr()   lcd_interface_cs_clr()//CS
#define LCD_CS_Set()   lcd_interface_cs_set()

#define LCD_BLK_Clr()  lcd_interface_blk_clr()//BLK
#define LCD_BLK_Set()  lcd_interface_blk_set()

void lcd_interface_gpio_init(void); //��ʼ��GPIO��
void lcd_interface_write_8bit_data(uint8_t data);   //����Ļд��8Bit����
void lcd_interface_write_16bit_data(uint16_t data); //����Ļд��16Bit����
void lcd_interface_delayms(uint32_t delay);     //ms�ӳٽӿں���
void lcd_interface_write_any_data(uint8_t *pdata,uint16_t len);

void lcd_interface_res_clr(void);
void lcd_interface_res_set(void);
void lcd_interface_dc_lcr(void);
void lcd_interface_dc_set(void);
void lcd_interface_cs_clr(void);
void lcd_interface_cs_set(void);
void lcd_interface_blk_clr(void);
void lcd_interface_blk_set(void);

void lcd_interface_fast_fill_color(uint16_t color);

#endif




