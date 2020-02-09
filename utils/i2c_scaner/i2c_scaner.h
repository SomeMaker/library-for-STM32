/*
 *  i2c_scaner.h
 *
 *  возвращает адрес найденого устройства (от 0x00 до 0x7F),
 *  если ничего не нашел возвращает 0xFF
 *  каждый последующий запуск продолжает поиск устройств на шине
 *  пока не найдется очередное устройство или не закончится диапазон адресов
 *
 *  Created on: 9 февр. 2020 г.
 *      Author: alexey.sidorov12@gmail.com
 */

#ifndef I2C_SCANER_H_
#define I2C_SCANER_H_
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "i2c.h"

uint8_t i2c_scan(I2C_HandleTypeDef* i2c);

#ifdef __cplusplus
}
#endif
#endif /* I2C_SCANER_H_ */

/********************************END OF FILE***********************************/
