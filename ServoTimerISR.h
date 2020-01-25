/*
 * Created by Jin
 * Data: 01/22/2020
 */

#include <avr/pgmspace.h>
#include <Arduino.h>
//hardware
#define uSecToTicks(_uSec)  (( clockCyclesPerMicrosecond() * _uSec) / 8)
#define MIN_PWM                 750     // minimum value of PWM
#define MAX_PWM                 2250    // maximum value of PWM
#define DEFAULT_PWM             1500    // default value of PWM
#define REFRESH_PERIOD          20000   // minimum time period to refresh servos in microseconds, 20mSec
#define TRIM_DURATION           2       // compensation ticks to trimming for digitalWrite delays
#define MAX_SERVOS_PER_TIMER    12      // maximum number of servos controlled by one timer
#define NUMBER_SERVOS           8       // the number of connected servos to main board
//error
#define ERROR_UNKNOWN           1
#define ERROR_OUTOFRANGE        2
//serial communication (maybe deleted later)
#define SERDEBUG_CODE
#define MAX_SERIAL_BUFFER       12

struct servo_counter {
    //counter for the servo being pulsed for each timer (or -1 if refresh period)
    int8_t servo_channel;
};
static volatile struct servo_counter global_servo_counter;



class ServoTimerISR {
public:
    ServoTimerISR(int timer_number);
    void Run(uint8_t servo_id, uint16_t pwm_value);
    void SetServoUSec(unsigned int servo_id, unsigned int pwm_value);
    void InitializeServo();

public:
    struct servo_configuration {
        volatile unsigned int ticks;
        unsigned int pin;
    };
    //pin number of Arduino mega connected with Servos: 2, 3, 4, 5, 6, 7, 8, 9
    struct servo_configuration global_servos[NUMBER_SERVOS] = {
            {uSecToTicks(DEFAULT_PWM), 2},
            {uSecToTicks(DEFAULT_PWM), 3},
            {uSecToTicks(DEFAULT_PWM), 4},
            {uSecToTicks(DEFAULT_PWM), 5},
            {uSecToTicks(DEFAULT_PWM), 6},
            {uSecToTicks(DEFAULT_PWM), 7},
            {uSecToTicks(DEFAULT_PWM), 8},
            {uSecToTicks(DEFAULT_PWM), 9}
    };
    uint8_t error_code = 0;
    int _timer_number = 0;
};