#include "stm32f7xx_hal.h"


#define WATER_LEVEL_SENSOR_PIN GPIO_PIN_0
#define WATER_CONTROL_VALVE_PIN GPIO_PIN_1
#define TEMP_SENSOR_PIN GPIO_PIN_2
#define REFRIGERATION_CONTROL_PIN GPIO_PIN_3
#define ICE_TRAY_FULL_SENSOR_PIN GPIO_PIN_4
#define USER_CONTROL_PIN GPIO_PIN_5


#define LED1_PIN GPIO_PIN_0 
#define LED2_PIN GPIO_PIN_7  
#define LED_GPIO_PORT GPIOB 


typedef enum {
    STATE_IDLE,
    STATE_MAKING_ICE,
    STATE_DISPENSING_ICE,
    STATE_ERROR
} IceMakerState;


IceMakerState currentState = STATE_IDLE;
uint8_t waterLevel = 0;
uint8_t temperature = 0;
uint8_t iceTrayFull = 0;
uint8_t userControlOn = 0;
const uint8_t WATER_LEVEL_THRESHOLD = 50;  


void initializeHardware(void);
void readSensors(void);
void updateState(void);
void controlActuators(void);
void updateUserIndications(void);

int main(void) {
    HAL_Init();
    initializeHardware();
    
    while (1) {
        readSensors();
        updateState();
        controlActuators();
        updateUserIndications();
        
        HAL_Delay(100); 
    }
}

void initializeHardware(void) {
  
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = LED1_PIN | LED2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);
}

void readSensors(void) {
    // Read water level sensor
    // Read temperature sensor
    // Read ice tray full sensor
    // Read user control input
}

void updateState(void) {
    switch (currentState) {
        case STATE_IDLE:
            if (userControlOn && !iceTrayFull && waterLevel > 0) {
                currentState = STATE_MAKING_ICE;
            }
            break;
        case STATE_MAKING_ICE:
            if (iceTrayFull || !userControlOn || waterLevel == 0) {
                currentState = STATE_IDLE;
            }
            break;
        case STATE_DISPENSING_ICE:
            // Logic for dispensing ice 
            break;
        case STATE_ERROR:
            // Error handling logic
            break;
    }
}

void controlActuators(void) {
    switch (currentState) {
        case STATE_IDLE:
            HAL_GPIO_WritePin(GPIOA, REFRIGERATION_CONTROL_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOA, WATER_CONTROL_VALVE_PIN, GPIO_PIN_RESET);
            break;
        case STATE_MAKING_ICE:
            HAL_GPIO_WritePin(GPIOA, REFRIGERATION_CONTROL_PIN, GPIO_PIN_SET);
            if (waterLevel < WATER_LEVEL_THRESHOLD) {
                HAL_GPIO_WritePin(GPIOA, WATER_CONTROL_VALVE_PIN, GPIO_PIN_SET);
            } else {
                HAL_GPIO_WritePin(GPIOA, WATER_CONTROL_VALVE_PIN, GPIO_PIN_RESET);
            }
            break;
        case STATE_DISPENSING_ICE:
            // Implement ice dispensing logic here
            break;
        case STATE_ERROR:
            // Implement error handling here
            HAL_GPIO_WritePin(GPIOA, REFRIGERATION_CONTROL_PIN, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOA, WATER_CONTROL_VALVE_PIN, GPIO_PIN_RESET);
            break;
    }
}

void updateUserIndications(void) {
  
    HAL_GPIO_WritePin(LED_GPIO_PORT, LED1_PIN, iceTrayFull ? GPIO_PIN_SET : GPIO_PIN_RESET);
    
   
    HAL_GPIO_WritePin(LED_GPIO_PORT, LED2_PIN, (waterLevel < WATER_LEVEL_THRESHOLD) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}






