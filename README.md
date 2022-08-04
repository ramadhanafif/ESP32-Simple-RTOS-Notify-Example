# ESP32-Simple-RTOS-Notify-Example
The simplest notification to synchronize between tasks.

Project created using PlatformIO. The code are located in src/main.cpp

Remember that `xTaskNotifyGive(xTaskHandle)` has assertion built-in to check passed handle.
If task2 was not created before task1, `xTaskNotifyGive` will crash the program due to assertion error.
