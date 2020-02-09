# сканер шины i2c
Поиск устройств на шине i2c в диапазоне адресов 0x00 - 0x7F. Каждый вызов функции i2c_scan возвращает адрес найденого устройства на шине или 0xFF если ничего не нашли.

Используется библиотека HAL. 
Перед использованием сканера сканируемая шина i2c должна быть настроена в 8 битном режиме. Конфигурация шины i2c должна быть в файле i2c.h (при генерации проекта в CubeMX установить галку на поле "Generate peripheral initialization as a pair of '.c/.h' file per peripheral" в Project Manager->Code Generator->Generated files ).

Пример:
```
#include "i2c_scaner.h"

int main(void)
{

  /* MCU Configuration */

  printf("start search\r\n");
  for(uint8_t addr = 0; addr != 0xFF;)
  {
    addr = i2c_scan(&hi2c1);
	
    if(addr == 0xFF)
    {
      printf("end search\r\n");
    }
    else
    {
      printf("device addr 0x%x\r\n", addr);
    }
  }

  while (1)
  {

  }
}
```