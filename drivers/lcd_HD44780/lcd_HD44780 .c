/*
 * lcd_HD44780.c
 *
 *  Created on: 3 янв. 2020 г.
 *      Author: Alexey
 */

/* Includes ------------------------------------------------------------------*/
#include "lcd_HD44780.h"
#include "delay_us.h"

/* Private macro -------------------------------------------------------------*/
//управлене дисплеем
#define LCD_RS_LOW()      (LCD_RS_GPIO_PORT->BSRR = (uint32_t)LCD_RS_PIN << 16U)
#define LCD_RS_HIGH()     (LCD_RS_GPIO_PORT->BSRR = LCD_RS_PIN)
#define LCD_E_LOW()       (LCD_E_GPIO_PORT->BSRR = (uint32_t)LCD_E_PIN << 16U)
#define LCD_E_HIGH()      (LCD_E_GPIO_PORT->BSRR = LCD_E_PIN)

//маска пинов
#if defined (LCD_DATA_BUS_8_BIT)
#define LCD_PIN_MASK    ( LCD_DB0_PIN | LCD_DB1_PIN | LCD_DB2_PIN | LCD_DB3_PIN | LCD_DB4_PIN | LCD_DB5_PIN | LCD_DB6_PIN | LCD_DB7_PIN )
#elif defined (LCD_DATA_BUS_4_BIT)
#define LCD_PIN_MASK    ( LCD_DB4_PIN | LCD_DB5_PIN | LCD_DB6_PIN | LCD_DB7_PIN )
#endif

/* Private variables ---------------------------------------------------------*/
const uint8_t row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };

/* Private function prototypes -----------------------------------------------*/
#if defined (LCD_DATA_BUS_8_BIT)
  static void send_byte(uint8_t data);
#elif defined (LCD_DATA_BUS_4_BIT)
  static void send_halfbyte(uint8_t data);
#endif
static void LCD_GPIO_Init(void);

/* Private functions ---------------------------------------------------------*/
#if defined (LCD_DATA_BUS_8_BIT)
/**
  * @brief  Отправка байта
  */
static void send_byte(uint8_t data)
{
  uint16_t data_set = 0;

  LCD_E_HIGH();
  if(data & 0x80) data_set |= LCD_DB7_PIN;
  if(data & 0x40) data_set |= LCD_DB6_PIN;
  if(data & 0x20) data_set |= LCD_DB5_PIN;
  if(data & 0x10) data_set |= LCD_DB4_PIN;
  if(data & 0x08) data_set |= LCD_DB3_PIN;
  if(data & 0x04) data_set |= LCD_DB2_PIN;
  if(data & 0x02) data_set |= LCD_DB1_PIN;
  if(data & 0x01) data_set |= LCD_DB0_PIN;
  LCD_DATA_GPIO_PORT->BSRR = ((uint32_t)(~data_set & LCD_PIN_MASK) << 16U) | data_set;
  Delay_us(1);
  LCD_E_LOW();
}
#endif

#if defined (LCD_DATA_BUS_4_BIT)
/**
  * @brief  Отправка старшей половины байта
  * для отправки младшей половины пердать функции (data << 4)
  */
static void send_halfbyte(uint8_t data)
{
  uint16_t data_set = 0;

  LCD_E_HIGH();
  if(data & 0x80) data_set |= LCD_DB7_PIN;
  if(data & 0x40) data_set |= LCD_DB6_PIN;
  if(data & 0x20) data_set |= LCD_DB5_PIN;
  if(data & 0x10) data_set |= LCD_DB4_PIN;
  // одной командой сбрасываем и выставляем на порту нужные биты
  LCD_DATA_GPIO_PORT->BSRR = ((uint32_t)(~data_set & LCD_PIN_MASK) << 16U) | data_set;
  Delay_us(1);
  LCD_E_LOW();
  Delay_us(1);
}
#endif

/**
  * @brief  Настройка выводов микроконтроллера
  */
static void LCD_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LCD_RS_GPIO_CLK_ENABLE();
  LCD_E_GPIO_CLK_ENABLE();
  LCD_DATA_GPIO_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_RS_GPIO_PORT, LCD_RS_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_E_GPIO_PORT, LCD_E_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LCD_DATA_GPIO_PORT, LCD_PIN_MASK, GPIO_PIN_RESET);

  /*Configure GPIO pin : RS_pin */
  GPIO_InitStruct.Pin = LCD_RS_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LCD_RS_GPIO_PORT, &GPIO_InitStruct);

  /*Configure GPIO pin : E_pin */
  GPIO_InitStruct.Pin = LCD_E_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LCD_E_GPIO_PORT, &GPIO_InitStruct);

  /*Configure GPIO pin : Data_pins */
  GPIO_InitStruct.Pin = LCD_PIN_MASK;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LCD_DATA_GPIO_PORT, &GPIO_InitStruct);
}

/* Exported functions --------------------------------------------------------*/
/**
  * @brief  первоначальная настройка дисплея
  */
void LCD_Init(void)
{
  LCD_GPIO_Init();
  HAL_Delay(40);
#if defined (LCD_DATA_BUS_8_BIT)
  LCD_Command(LCD_BUS_8_BIT);
  HAL_Delay(4);
  LCD_Command(LCD_BUS_8_BIT);
  Delay_us(100);
  LCD_Command(LCD_BUS_8_BIT);
  LCD_Command(LCD_BUS_8_BIT | LCD_2_LINE_5x8);
#elif defined (LCD_DATA_BUS_4_BIT)
  LCD_RS_LOW();
  send_halfbyte(LCD_BUS_8_BIT);
  HAL_Delay(4);
  send_halfbyte(LCD_BUS_8_BIT);
  Delay_us(100);
  send_halfbyte(LCD_BUS_8_BIT);
  Delay_us(50);
  send_halfbyte(LCD_BUS_4_BIT);
  Delay_us(50);
  LCD_Command(LCD_BUS_4_BIT | LCD_2_LINE_5x8);
#endif

  LCD_Command(LCD_DISPLAY_OFF | LCD_CURSOR_OFF | LCD_CURSOR_BLINK_OFF);
  LCD_Command(LCD_CLEAR);
  HAL_Delay(2);
  LCD_Command(LCD_MOVE_RIGHT | LCD_SHIFT_CURSOR);

  LCD_Command(LCD_DISPLAY_ON);
}

/**
  * @brief  Отправка команды на дисплей
  * @note   команды LCD_CLEAR и LCD_RETURN_HOME выполняются ~2мс
  *         не отправляйте новых данных на дисплей ренее чем через ~2мс после этих команд
  */
void LCD_Command(uint8_t cmd)
{
  LCD_RS_LOW();
#if defined (LCD_DATA_BUS_8_BIT)
  send_byte(cmd);
#elif defined (LCD_DATA_BUS_4_BIT)
  send_halfbyte(cmd);
  send_halfbyte(cmd << 4);
#endif
  Delay_us(50); //команды выполняются примерно 38 мкс
}

/**
  * @brief  Запись данных в DDRAM или CGRAM
  */
void LCD_Write(uint8_t data)
{
  LCD_RS_HIGH();
#if defined (LCD_DATA_BUS_8_BIT)
  send_byte(data);
#elif defined (LCD_DATA_BUS_4_BIT)
  send_halfbyte(data);
  send_halfbyte(data << 4);
#endif
  Delay_us(50); //команды выполняются примерно 38 мкс
}

/**
  * @brief  печать строки в текущюю позицию курсора
  * @param  c - указатель на строку
  */
void LCD_String(const char *c)
{
  while(*c != 0)
  {
    LCD_Write(*c++);
  }
}

/**
  * @brief  перенос курсора в заданную позицию
  * @param  col - столбец 0...20
  * @param  row - строка 0...3
  */
void LCD_SetCursor(uint8_t col, uint8_t row)
{
  if (row > 3)
  {
    row = 0;
  }
  LCD_Command(LCD_SET_DDRAM | (col + row_offsets[row]));
}

/*******************************  END OF FILE  ********************************/
