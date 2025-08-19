#include <Arduino.h>

#define BUTTON_PIN 21

TaskHandle_t task1handle = NULL;
TaskHandle_t task2handle = NULL;
TaskHandle_t buttonTaskHandle = NULL;

SemaphoreHandle_t serialsempahore;

bool lastButtonState = HIGH; // button is pulled up, so HIGH = not pressed

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
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    if (xSemaphoreTake(serialsempahore, portMAX_DELAY) == pdTRUE)
    {
      Serial.println("hello from task 2");
      xSemaphoreGive(serialsempahore);
    }
  }
}

void buttonPollingTask(void *param)
{
  TickType_t xLastWakeTime = xTaskGetTickCount();
  for (;;)
  {
    bool currentButtonState = digitalRead(BUTTON_PIN);
    
    // Check if button state changed
    if (currentButtonState != lastButtonState)
    {
      if (xSemaphoreTake(serialsempahore, portMAX_DELAY) == pdTRUE)
      {
        if (currentButtonState == LOW)
        {
          Serial.println("Button PRESSED");
        }
        else
        {
          Serial.println("Button RELEASED");
        }
        xSemaphoreGive(serialsempahore);
      }
      lastButtonState = currentButtonState;
    }
    
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(10)); // Poll every 10ms
  }
}

void setup()
{
  serialsempahore = xSemaphoreCreateMutex();
  Serial.begin(115200);
  
  // Initialize button pin with pull-up resistor
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  xTaskCreate(vtask1, "task1", 1024 * 2, NULL, 1, &task1handle);
  xTaskCreate(vtask2, "task2", 1024 * 2, NULL, 1, &task2handle);
  xTaskCreate(buttonPollingTask, "buttonTask", 1024 * 2, NULL, 1, &buttonTaskHandle);
}

void loop()
{
  // put your main code here, to run repeatedly:
}
