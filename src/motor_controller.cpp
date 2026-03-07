#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "config.h"

float* angle_pt_local;
float* target_pt_local;
int current_step = 0;
int target_step = 0;

void set_motor_phase(int phase) {
    digitalWrite(MOTOR_PIN_1, phase == 0);
    digitalWrite(MOTOR_PIN_2, phase == 1);
    digitalWrite(MOTOR_PIN_3, phase == 2);
    digitalWrite(MOTOR_PIN_4, phase == 3);
}

void set_interval(void *pvParameters) {
    set_motor_phase(current_step % 4);
    while (true) {
        // Calculate target_step
        target_step = (*target_pt_local) * total_step / 180;

        // Compare target_step and current_step to increase or decrease current_step
        if (target_step > current_step) {
            current_step++;
            set_motor_phase(current_step % 4);

        } else if (target_step < current_step) {
            current_step--;
            set_motor_phase(current_step % 4);

        } else {
            // (Optional) Turn off all LEDs on the motor driver
            set_motor_phase(5);
        }

        // Update angle so that it can be synced to Notion preview
        *angle_pt_local = current_step * 180.0F / total_step;

        // Delay step_duration_ms. Do not use delay(step_duration_ms)
        vTaskDelay(pdMS_TO_TICKS(step_duration_ms)); // Non-blocking delay
    }
}

void init_motor_controller(float * angle_pt, float * target_pt) {
    angle_pt_local = angle_pt;
    target_pt_local = target_pt;

    current_step = (*angle_pt_local) * total_step / 180;
    target_step = (*target_pt_local) * total_step / 180;
    /* Set Pin Mode */
    pinMode(MOTOR_PIN_1, OUTPUT);
    pinMode(MOTOR_PIN_2, OUTPUT);
    pinMode(MOTOR_PIN_3, OUTPUT);
    pinMode(MOTOR_PIN_4, OUTPUT);
    xTaskCreate(set_interval, "motor", 2048, NULL, 1, NULL);
}

void calibration(float calibration_angle) {
    current_step = calibration_angle * total_step / 180;
}