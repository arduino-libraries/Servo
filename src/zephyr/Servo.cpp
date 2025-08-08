#if defined(ARDUINO_ARCH_ZEPHYR)

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>
#include <zephyr/drivers/counter.h>

#include <Arduino.h>
#include <Servo.h>


typedef struct {
    uint8_t pin;
    uint32_t position_tick;
    uint32_t timer_index;
    uint32_t min;
    uint32_t max;
} servoTimer_t;

class ServoTimerHandler{

  inline static uint32_t timer_servo;
  inline static uint32_t servo_timing_period;
  bool timer_is_started;

  inline static servoTimer_t * servos[MAX_ZEPHYR_SERVOS] = {nullptr};
  uint8_t servoCount;

  public:
    ServoTimerHandler(){
      timer_is_started = false;
      timer_servo = 0;
      servoCount = 0;
    }

    int initTimer(){
      if (!timer_is_started){
        const struct device *const counter_dev = DEVICE_DT_GET(TIMER);
        counter_start(counter_dev);
        struct counter_top_cfg top_cfg;
        top_cfg.ticks = counter_us_to_ticks(counter_dev, servo_timer_base_us_tick);
        top_cfg.callback = this->servo_timer_update; 
        top_cfg.user_data = &top_cfg;
        top_cfg.flags = 0;

        int err = counter_set_top_value(counter_dev, &top_cfg);
        if (err){
          return 0;
        }
        else{
          timer_is_started = true;
          return 1;
        }
      }
      return -1;
    }

    static void servo_timer_update(const struct device *counter_dev, void *user_data){
      for (uint8_t i = 0; i < MAX_ZEPHYR_SERVOS; i++){
        if (servos[i]!=nullptr){
          if (timer_servo>servos[i]->position_tick){
            digitalWrite(servos[i]->pin, LOW);
          }
          else{
            digitalWrite(servos[i]->pin, HIGH);
          }
        }
      }
      if (timer_servo>servo_timer_end_tick){
        timer_servo = 0;
      }
      timer_servo++;
    }
    
    int addServo(servoTimer_t * s){
      if (servoCount<MAX_ZEPHYR_SERVOS){
        for (uint8_t i = 0; i<MAX_ZEPHYR_SERVOS; i++){
          if (servos[i]!=nullptr && servos[i]->pin==s->pin){
            return i;
          }
      }
      for (uint8_t i = 0; i<MAX_ZEPHYR_SERVOS; i++){
        if (servos[i]==nullptr){
          servos[i]=s;
          servoCount++;
          return i;
        }
      }
      }
      else{
        return -1;
      }
      return -2;
    }

    int removeServo(int index){
      if (index<MAX_ZEPHYR_SERVOS){
        delete servos[index];
        servos[index]=nullptr;
        servoCount--;
        return index;
      }
      else{
        return -1;
      }
    }

    void writeMicroseconds(int index, int value){
      if (servos[index]!=nullptr){
        servos[index]->position_tick = value/servo_timer_base_us_tick;
      }
    }

    uint32_t getMin(int index){
      if (servos[index]!=nullptr){
        return servos[index]->min;
      }
      return MIN_PULSE_WIDTH;
    }

    uint32_t getMax(int index){
      if (servos[index]!=nullptr){
        return servos[index]->max;
      }
      return MAX_PULSE_WIDTH;
    }

    uint32_t readMicroseconds(int index){
      if (servos[index]!=nullptr){
        return servos[index]->position_tick*servo_timer_base_us_tick;
      }
      return 0;
    }
};

static ServoTimerHandler servo_handle;


Servo::Servo(){
  servo_handle.initTimer();
  servoIndex = 255;
}

uint8_t Servo::attach(int pin){
  return this->attach(pin, MIN_PULSE_WIDTH , MAX_PULSE_WIDTH );
}

uint8_t Servo::attach(int pin, int min, int max){
  pinMode(pin, OUTPUT);
  servoTimer_t * tmp_servo = new servoTimer_t();
  tmp_servo->pin = pin;
  tmp_servo->min = min;
  tmp_servo->max = max;
  servoIndex = servo_handle.addServo(tmp_servo);
  this->min = servo_handle.getMin(servoIndex)/4;
  this->max = servo_handle.getMax(servoIndex)/4;
  return 0;
}


void Servo::detach(){
  servo_handle.removeServo(servoIndex);
  servoIndex = 255;
}

void Servo::write(int value){
  servo_handle.writeMicroseconds(servoIndex, map(value, 0, 180, servo_handle.getMin(servoIndex), servo_handle.getMax(servoIndex)));
}

void Servo::writeMicroseconds(int value){
  servo_handle.writeMicroseconds(servoIndex, value);
}

int Servo::read(){
  return map(servo_handle.readMicroseconds(servoIndex), servo_handle.getMin(servoIndex), servo_handle.getMax(servoIndex), 0, 180);
}

int Servo::readMicroseconds(){
  return servo_handle.readMicroseconds(servoIndex);
}

bool Servo::attached(){
  if (servoIndex==255){
    return false;
  }
  else{
    return true;
  }
}

#endif