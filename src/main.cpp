#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Adafruit_PWMServoDriver.h"

#define SDA_PIN 17
#define SCL_PIN 16

#define SERVO_MIN_PULSE_WIDTH 103
#define SERVO_MAX_PULSE_WIDTH 512

#define CAPTURE_BUTTON_PIN 13
#define LOOSE_BUTTON_PIN 12
#define SW_BTN_PIN 14
#define VRX_PIN 32
#define VRY_PIN 33

#define BOTTOM_SERVO_IDX 0
#define FRONT_BACK_CTRL_SERVO_IDX 1
#define HEIGHT_CTRL_SERVO_IDX 2
#define CLAWS_SERVO_IDX 3

#define ADC_INPUT_THRESHOLD_MIN 100
#define ADC_INPUT_THRESHOLD_MAX 4000

static Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
static uint8_t servo_angles[4] = {0};

// uint16_t calc_pulse_tick(uint16_t pulse_width) {
//     float tick_length = 1000000.0 / 50.0 / 4096;
//     Serial.printf("tick length: %f\n", tick_length);
//     uint16_t tick_cnt = (double)pulse_width / tick_length;
//     Serial.printf("tick cnt: %d\n", tick_cnt);
//     return tick_cnt;
// }

void write_servo_angle(uint8_t channel, uint8_t angle) {
    uint16_t tick = map(angle, 0, 180, SERVO_MIN_PULSE_WIDTH, SERVO_MAX_PULSE_WIDTH);
    pwm.setPWM(channel, 0, tick);
}

void init_buttons() {
    pinMode(CAPTURE_BUTTON_PIN, INPUT);
    pinMode(LOOSE_BUTTON_PIN, INPUT);
    pinMode(SW_BTN_PIN, INPUT);
}

void init_servos() {
    for (int i = 0; i < 4; i++) {
        write_servo_angle(i, 90);
        servo_angles[i] = 90;
    }
}

void back_task(void*) {
    while (1) {
        int capture_data = digitalRead(CAPTURE_BUTTON_PIN);
        int loose_data = digitalRead(LOOSE_BUTTON_PIN);
        int sw_data = digitalRead(SW_BTN_PIN);
        int x_value = analogRead(VRX_PIN);
        int y_value = analogRead(VRY_PIN);

        Serial.printf("capture: %d, loose: %d, sw: %d, x: %d, y: %d\n", capture_data, loose_data, sw_data, x_value, y_value);

        if (capture_data == 1) {
            write_servo_angle(CLAWS_SERVO_IDX, 10);
            servo_angles[CLAWS_SERVO_IDX] = 10;
        } else if (loose_data == 1) {
            write_servo_angle(CLAWS_SERVO_IDX, 90);
            servo_angles[CLAWS_SERVO_IDX] = 90;
        }

        if (sw_data == 0 && y_value < ADC_INPUT_THRESHOLD_MIN) {
            if (servo_angles[BOTTOM_SERVO_IDX] > 0) {
                write_servo_angle(BOTTOM_SERVO_IDX, servo_angles[BOTTOM_SERVO_IDX]-1);
                --servo_angles[BOTTOM_SERVO_IDX];
            }
        } else if (sw_data == 0 && y_value > ADC_INPUT_THRESHOLD_MAX) {
            if (servo_angles[BOTTOM_SERVO_IDX] < 180) {
                write_servo_angle(BOTTOM_SERVO_IDX, servo_angles[BOTTOM_SERVO_IDX]+1);
                ++servo_angles[BOTTOM_SERVO_IDX];
            }
        }

        if (sw_data == 0 && x_value < ADC_INPUT_THRESHOLD_MIN) {
            if (servo_angles[FRONT_BACK_CTRL_SERVO_IDX] < 180) {
                write_servo_angle(FRONT_BACK_CTRL_SERVO_IDX, servo_angles[FRONT_BACK_CTRL_SERVO_IDX]+1);
                ++servo_angles[FRONT_BACK_CTRL_SERVO_IDX];
            }
        } else if (sw_data == 0 && x_value > ADC_INPUT_THRESHOLD_MAX) {
            if (servo_angles[FRONT_BACK_CTRL_SERVO_IDX] > 0) {
                write_servo_angle(FRONT_BACK_CTRL_SERVO_IDX, servo_angles[FRONT_BACK_CTRL_SERVO_IDX]-1);
                --servo_angles[FRONT_BACK_CTRL_SERVO_IDX];
            }
        }

        if (sw_data == 1 && x_value < ADC_INPUT_THRESHOLD_MIN) {
            if (servo_angles[HEIGHT_CTRL_SERVO_IDX] > 0) {
                write_servo_angle(HEIGHT_CTRL_SERVO_IDX, servo_angles[HEIGHT_CTRL_SERVO_IDX]-1);
                --servo_angles[HEIGHT_CTRL_SERVO_IDX];
            }
        } else if (sw_data == 1 && x_value > ADC_INPUT_THRESHOLD_MAX) {
            if (servo_angles[HEIGHT_CTRL_SERVO_IDX] < 180) {
                write_servo_angle(HEIGHT_CTRL_SERVO_IDX, servo_angles[HEIGHT_CTRL_SERVO_IDX]+1);
                ++servo_angles[HEIGHT_CTRL_SERVO_IDX];
            }
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("start==>");

    pwm.begin(SDA_PIN, SCL_PIN, 100000);
    pwm.setPWMFreq(55);

    init_buttons();
    init_servos();

    xTaskCreate(back_task, "joystic_task", 2048, NULL, 10, NULL);
}

void loop() {
    // write_angle(0);
    // delay(1000);
    // write_angle(90);
    // delay(1000);
    // write_angle(180);
    // delay(1000);
    // for (int pulselen = SERVO_MIN_PULSE_WIDTH; pulselen < SERVO_MAX_PULSE_WIDTH; pulselen+=10) {
    //     // uint16_t pulse_tick_cnt = calc_pulse_tick(pulselen);
    //     pwm.setPWM(0, 0, pulselen);
    //     pwm.setPWM(1, 0, pulselen);
    //     pwm.setPWM(2, 0, pulselen);
    //     pwm.setPWM(3, 0, pulselen);
    // }
    // delay(2000);
    // for (int pulselen = SERVO_MAX_PULSE_WIDTH; pulselen > SERVO_MIN_PULSE_WIDTH; pulselen-=10) {
    //     // uint16_t pulse_tick_cnt = calc_pulse_tick(pulselen);
    //     pwm.setPWM(0, 0, pulselen);
    //     pwm.setPWM(1, 0, pulselen);
    //     pwm.setPWM(2, 0, pulselen);
    //     pwm.setPWM(3, 0, pulselen);
    // }
    // delay(2000);
}
