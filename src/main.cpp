#include <Arduino.h>

TaskHandle_t task1handle = NULL;
TaskHandle_t task2handle = NULL;

SemaphoreHandle_t serialsempahore;

void vtask1(void *param)
{
  TickType_t xLastWakeTime = xTaskGetTickCount();
  for (;;)
  {
    if (task2handle != NULL) // Avoid assertion error
      xTaskNotifyGive(task2handle);

    if (xSemaphoreTake(serialsempahore, portMAX_DELAY) == pdTRUE)
    {
      Serial.println("hello from task 1");
      xSemaphoreGive(serialsempahore);
    }
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(3000));
  }
}

void vtask2(void *param)
{
  for (;;)
  {
    {
      ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
      if (xSemaphoreTake(serialsempahore, portMAX_DELAY) == pdTRUE)
      {
        Serial.println("hello from task 2");
        xSemaphoreGive(serialsempahore);
      }
    }
  }
}

void setup()
{
  serialsempahore = xSemaphoreCreateMutex();
  Serial.begin(115200);

  xTaskCreate(vtask1, "task1", 1024 * 2, NULL, 1, &task1handle);
  xTaskCreate(vtask2, "task2", 1024 * 2, NULL, 1, &task2handle);
}

void loop()
{
  // put your main code here, to run repeatedly:
}
