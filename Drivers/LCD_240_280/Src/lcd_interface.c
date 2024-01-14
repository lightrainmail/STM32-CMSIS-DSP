#include "lcd_interface.h"

void lcd_interface_gpio_init(void) {                                                      //���������������Ҫʹ�õ�GPIO�˿�
    /* GPIO Init function*/

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();                         //��������ʱ��
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    HAL_GPIO_WritePin(RES_GPIO_Port,RES_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(DC_GPIO_Port,DC_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(BLK_GPIO_Port,BLK_Pin,GPIO_PIN_SET);


    GPIO_InitStruct.Pin = RES_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(RES_GPIO_Port,&GPIO_InitStruct);

    GPIO_InitStruct.Pin = DC_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DC_GPIO_Port,&GPIO_InitStruct);

    GPIO_InitStruct.Pin = CS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(CS_GPIO_Port,&GPIO_InitStruct);

    GPIO_InitStruct.Pin = BLK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(BLK_GPIO_Port,&GPIO_InitStruct);
}


/******************************************************************************
      ����˵����LCD��������д�뺯��
      ������ݣ�dat  Ҫд��Ĵ�������
      ����ֵ��  ��
******************************************************************************/
void lcd_interface_write_8bit_data(uint8_t data) {
    /* you code here,SPI transmit 8bit data*/
	LCD_CS_Clr();
    HAL_SPI_Transmit(&SPIHandle,&data,1,10);
    LCD_CS_Set();
}

/* @brief ����SPI�������ⳤ�ȵ�����
 * @param *pdata �������ݵ��׵�ַ
 *        len �������ݳ���*/
void lcd_interface_write_any_data(uint8_t *pdata,uint16_t len) {
    LCD_CS_Clr();
    HAL_SPI_Transmit(&SPIHandle,pdata,len,100);
    LCD_CS_Set();
}

/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void lcd_interface_write_16bit_data(uint16_t data) {
    /* SPI transmit 16bit data*/
    uint8_t Data[2];
    Data[0] = data>>8;
    Data[1] = data;

    LCD_CS_Clr();
    HAL_SPI_Transmit(&SPIHandle,Data,2,10);
    LCD_CS_Set();
}

/* LCD��Ҫ�ӳٺ���*/
void lcd_interface_delayms(uint32_t delay) {
    HAL_Delay(delay);
}

/* @brief ʵ�ֽ�RES�������͵�Ч��*/
void lcd_interface_res_clr(void) {
    HAL_GPIO_WritePin(RES_GPIO_Port,RES_Pin,GPIO_PIN_RESET);
}

//ʵ��RES�������ߵ�Ч��
void lcd_interface_res_set(void) {
    HAL_GPIO_WritePin(RES_GPIO_Port,RES_Pin,GPIO_PIN_SET);
}

//ʵ��DC��������Ч��
void lcd_interface_dc_lcr(void) {
    HAL_GPIO_WritePin(DC_GPIO_Port,DC_Pin,GPIO_PIN_RESET);
}

//ʵ��DC��������Ч��
void lcd_interface_dc_set(void) {
    HAL_GPIO_WritePin(DC_GPIO_Port,DC_Pin,GPIO_PIN_SET);
}

//ʵ��CS����Ч��
void lcd_interface_cs_clr(void) {
    HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,GPIO_PIN_RESET);
}

//ʵ��CS��������Ч��
void lcd_interface_cs_set(void) {
    HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,GPIO_PIN_SET);
}

//ʵ��BLK��������Ч��
void lcd_interface_blk_clr(void) {
    HAL_GPIO_WritePin(BLK_GPIO_Port,BLK_Pin,GPIO_PIN_RESET);
}

//ʵ��BLK��������Ч��
void lcd_interface_blk_set(void) {
    HAL_GPIO_WritePin(BLK_GPIO_Port,BLK_Pin,GPIO_PIN_SET);
}

//���ڿ���ˢȫ��
void lcd_interface_fast_fill_color(uint16_t color) {
    uint8_t times = 4;  //��Ҫ�ּ���ˢ��
    uint8_t BuffWide = LCD_W;   //ÿ��ˢ�¾��εĿ������Ļ�Ŀ��
    uint8_t BuffHigh = LCD_H / times;
    static uint8_t Data[33600];   //BuffWide*BuffHigh*2 = 240*280/20*2 = 6720;

    for(int i = 0;i < BuffWide*BuffHigh;i++){
        Data[i*2] = color>>8;
        Data[i*2 + 1] = color;
    }
    for(int i = 0;i < times;i++){
        LCD_Address_Set(0,i*BuffHigh,BuffWide,(i + 1)*BuffHigh - 1);
        LCD_CS_Clr();
        HAL_SPI_Transmit(&SPIHandle,Data,BuffWide*BuffHigh*2,100);
        LCD_CS_Set();
    }
}











