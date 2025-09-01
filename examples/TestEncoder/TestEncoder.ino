/*
  TestEncoder.ino
  Версия: 1.0
  Дата: 01.09.2025
  Автор: RootTool

  Пример использования библиотеки RootEncoder.

  Что делает:
  - Демонстрирует работу ротационного энкодера с подсчётом вращений.
  - Обрабатывает клики кнопки (одиночные/множественные).
  - Обрабатывает удержание кнопки (Hold), сбрасывая счётчик.

  Как работает:
  - Энкодер инициализируется через Encoder.Setup().
  - Каждое вращение вызывает OnRotate с дельтой (±1), которая увеличивает/уменьшает Counter.
  - Каждое нажатие кнопки вызывает OnPressEvent:
    - 0     = Удержание кнопки (локальный счетчик Counter сбрасывается)
    - 1...N = Количество кликов
  - В loop() вызывается Encoder.Update() для отслеживания событий.
  - Serial выводит текущие значения и события для наблюдения.
*/

#include <RootEncoder.h>

RootEncoder Encoder(2, 3, 4); // A(CLK) = 2, B(DT) = 3, Button(SW) = 4

int Counter = 0;

void setup() 
{
  Serial.begin(9600);
  Encoder.Setup(); // Инициализация энкодера
  
  // Лямбда для обработки вращения энкодера
  Encoder.OnRotate = [](int8_t Delta)
  {
    Counter += Delta;
    Serial.print("Delta: ");
    Serial.print(Delta);
    Serial.print(" Counter: ");
    Serial.println(Counter);
  };

  // Лямбда для обработки нажатий и удержаний кнопки
  Encoder.OnPressEvent = [](uint8_t Clicks)
  {
    if(Clicks == 0) 
    {
      Serial.println("Button Held! Resetting counter.");
      Counter = 0;
    }
    else 
    {
      Serial.print("Clicks: ");
      Serial.println(Clicks);
    }
  };
}

void loop() {
  Encoder.Update(); // Опрашиваем энкодер
  delay(5); // Для стабильности и баланса вселенной
}
