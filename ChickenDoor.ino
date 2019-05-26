#include <EEPROM.h>


// 300 steps per inch  12 " = 3600


#include <AFMotor.h>
AF_Stepper motor(24, 2);

class Door {
    enum State {
      OPEN = 0,
      CLOSED = 1,
      MOVING = 2
    } state = CLOSED;

  public:
    void OpenDoor() {
      if (state == CLOSED) {
        state = MOVING;
        motor.step(3500, BACKWARD, MICROSTEP);
        state = OPEN;
      }
    }


    void CloseDoor() {
      if (state == OPEN) {
        state = MOVING;
        motor.step(3500, FORWARD, MICROSTEP);
        motor.release();  
        state = CLOSED;
      }
    }
    void setup() {
      motor.setSpeed(300);  

    }

    void loop() {

    }

};

Door door;

class DayNight {
    enum State {
      DAY = 0,
      NIGHT = 1
    } state ;
    unsigned int DayValue = 130;   // value to cross into day
    unsigned int NightValue = 20;   // value to cross into night
    unsigned int A0reading;
    
  void checkForDawn(){
    if (state==NIGHT){
      A0reading = analogRead(5);  
      Serial.println(A0reading);
      if (A0reading>DayValue){
     //   digitalWrite(LED_BUILTIN, HIGH); 
        state=DAY;
        Serial.println("   It just turned Day");
      }
      
    }
  }
  void checkForDusk(){
    if (state==DAY){
      A0reading = analogRead(5);
      Serial.println(A0reading);
      if (A0reading<NightValue){
     //     digitalWrite(LED_BUILTIN, LOW); 
        Serial.println("   It just turned Night");
        state=NIGHT;
      }
      
    }
  }

      
  public:
    void setup() {
        state = NIGHT;
    }

    void loop() {
        checkForDawn();
        checkForDusk();
        if (state==DAY){
          Serial.println("It's Day ");
          door.OpenDoor();
        }
        if (state==NIGHT){
          Serial.println("It's Night");
          door.CloseDoor();
        }
    }


    

};

DayNight daynight;


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  door.setup();
  daynight.setup();
  Serial.begin(9600);    
}

void loop() {
  door.loop();
  daynight.loop();
  
  
}
