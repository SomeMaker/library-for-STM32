/*
 *  i2c_scaner.c
 *
 *  Created on: 9 февр. 2020 г.
 *      Author: alexey.sidorov12@gmail.com
 */

/* Includes ------------------------------------------------------------------*/
#include "i2c_scaner.h"

/**
  * @brief  Сканер шины i2c
  *         ищет устройства на шине i2c по адресам от 0x00 до 0x7F
  *         повторный запуск продолжает поиск устройств с последнего найденого
  * @param  i2c Pointer to a I2C_HandleTypeDef structure that contains
  *         the configuration information for the specified I2C.
  * @retval адрес найденного устройства, или 0xFF если ничего не нашли
  */
uint8_t i2c_scan(I2C_HandleTypeDef* i2c)
{
  static uint8_t addr = 0;
  uint8_t data = 0;

  for (; addr <= 0x7F; addr++)
  {
    if (HAL_I2C_Master_Transmit(i2c, addr << 1, &data, 0, 50) == HAL_OK)
    {
      addr++;
      return (addr-1);
    }
  }
  addr = 0;
  return 0xFF;
}

/********************************END OF FILE***********************************/
