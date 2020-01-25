/*
 * Created by Jin
 * Data: 01/22/2020
 */

#include "ServoTimerISR.h"

ServoTimerISR::ServoTimerISR(int timer_number){
    global_servo_counter.servo_channel = -1;
    _timer_number = timer_number;
}

void ServoTimerISR::InitializeServo() {
    switch(_timer_number) {
        case 1:
            TCCR1A = 0;                 // set entire TCCR1A register to 0
            TCCR1B = 0;                 // same for TCCR1B
            TCCR1C = 0;

            TCCR1B |= (1 << CS11);      // 8 prescaler
            TCNT1  = 0;                 // initialize counter value to 0
            TIFR1 |= (1 << OCF1A);      // clear any pending interrupts
            TIMSK1 |= (1 << OCIE1A);    // enable the output compare interrupt
            break;
        case 3:
            TCCR3A = 0;                 // set entire TCCR3A register to 0
            TCCR3B = 0;                 // same for TCCR3B
            TCCR3C = 0;

            TCCR3B |= (1 << CS31);      // 8 prescaler
            TCNT3  = 0;                 // initialize counter value to 0
            TIFR3 |= (1 << OCF3A);      // clear any pending interrupts
            TIMSK3 |= (1 << OCIE1A);    // enable the output compare interrupt
            break;
        case 4:
            TCCR4A = 0;                 // set entire TCCR4A register to 0
            TCCR4B = 0;                 // same for TCCR4B
            TCCR4C = 0;

            TCCR4B |= (1 << CS41);      // 8 prescaler
            TCNT4  = 0;                 // initialize counter value to 0
            TIFR4 |= (1 << OCF4A);      // clear any pending interrupts
            TIMSK4 |= (1 << OCIE4A);    // enable the output compare interrupt
            break;
        case 5:
            TCCR5A = 0;                 // set entire TCCR5A register to 0
            TCCR5B = 0;                 // same for TCCR5B
            TCCR5C = 0;

            TCCR5B |= (1 << CS51);      // 8 prescaler
            TCNT5  = 0;                 // initialize counter value to 0
            TIFR5 |= (1 << OCF5A);      // clear any pending interrupts
            TIMSK5 |= (1 << OCIE5A);    // enable the output compare interrupt
            break;
        default:
            TCCR5A = 0;                 // set entire TCCR5A register to 0
            TCCR5B = 0;                 // same for TCCR5B
            TCCR5C = 0;

            TCCR5B |= (1 << CS51);      // 8 prescaler
            TCNT5  = 0;                 // initialize counter value to 0
            TIFR5 |= (1 << OCF5A);      // clear any pending interrupts
            TIMSK5 |= (1 << OCIE5A);    // enable the output compare interrupt
            break;
    }

    for (unsigned int i = 0; i < NUMBER_SERVOS; i++) {
        pinMode(global_servos[i].pin, OUTPUT);
    }
}

void ServoTimerISR::Run(uint8_t servo_id, uint16_t pwm_value) {
    static unsigned int selected_servo = 0;
    error_code = 0;
    if (servo_id < 0 || servo_id >= NUMBER_SERVOS) {
        error_code = ERROR_OUTOFRANGE;
    }
    selected_servo = servo_id;

    if (pwm_value < MIN_PWM || pwm_value > MAX_PWM) {
        error_code = ERROR_OUTOFRANGE;
    }
    if (error_code == 0) {
        uint16_t _pwm_value = pwm_value;
        SetServoUSec(selected_servo, _pwm_value);
    }
//    else {
//        Serial.print("ERROR_"); Serial.println(error_code);
//        Serial.print("pwm : "); Serial.println(pwm_value);
//    }

}

void ServoTimerISR::SetServoUSec(unsigned int servo_id, unsigned int pwm_value) {
    uint8_t oldSREG;
    if (servo_id < NUMBER_SERVOS) {
        if (pwm_value < MIN_PWM) {
            pwm_value = MIN_PWM;
        }
        if (pwm_value > MAX_PWM) {
            pwm_value = MAX_PWM;
        }
        pwm_value -= TRIM_DURATION;
        pwm_value = uSecToTicks(pwm_value);
        oldSREG = SREG;
        cli();
        global_servos[servo_id].ticks = pwm_value;
        SREG = oldSREG;
    }
}