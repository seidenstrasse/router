#include <Arduino.h>
#include "ardusss7.h"
#include "ss7content.h"
#include <AccelStepper.h>

#define ENDSTOP1 6
#define S1_ENABLE 2
#define MICROSTEP A5

#define MAXPOSITIONS 6
#define DISTANCE 1000
uint16_t position[MAXPOSITIONS]{30,1030,2030,3085,4085,5085};

#define ID 2
AccelStepper stepper(1,3,4);

void init_stepper(){
    pinMode(ENDSTOP1, INPUT_PULLUP);
    
    pinMode(S1_ENABLE, OUTPUT); //ENABLE STEPPER
    digitalWrite(S1_ENABLE, LOW);
    
    pinMode(MICROSTEP, OUTPUT);
    digitalWrite(MICROSTEP, LOW);
    
    stepper.setAcceleration(1500);
    stepper.setMaxSpeed(3000);
    home();
}

void home(){
    if(!digitalRead(ENDSTOP1)){
        stepper.runToNewPosition(100);
    }
    uint16_t stepout = 0;
    while(digitalRead(ENDSTOP1)){
        //if(stepout >= (DISTANCE*MAXPOSITIONS)) break;
        stepper.moveTo(-1);
        stepout += 1;
        stepper.run();
        stepper.setCurrentPosition(0);
        // stepper.runToNewPosition(0);
        //stepper.run();
        digitalWrite(11,LOW); //Kein plan
    }
   select(0);
}

void select(uint8_t tube){
    if(tube < MAXPOSITIONS){
        stepper.runToNewPosition(position[tube]);
    }
    else{
        home();
    }
}


void setup() {
  Serial.begin(9600);
  init_stepper();
  Serial.print("Initializing SSS7...");
  SSS7.init();
  Serial.println("Done !");
  //select(3);
};


void loop() {
  uint8_t msg[SSS7_PAYLOAD_SIZE];
    memset(msg, 0, SSS7_PAYLOAD_SIZE);
    msg[0] = 'H';
    msg[1] = 'e';
    msg[2] = 'l';
    msg[3] = 'l';
    msg[4] = 'o';
    msg[5] = ' ';
    msg[6] = 'W';
    msg[7] = 'o';
    msg[8] = 'r';
    msg[9] = 'l';
    msg[10] = 'd';


    while(1) {
    
    if(SSS7.hasReceived()) {
      uint8_t msg1[SSS7_PAYLOAD_SIZE];
      SSS7.getReceived(msg1);
      Serial.print("Got data:");
      Serial.println((char*)msg1);
              Serial.println(msg1[0]);
              Serial.println(msg1[1]);
              Serial.println(msg1[2]);
              Serial.println(msg1[3]);
              
            if((msg1[0] == MSG_CONNECT) && (msg1[2] == ID)){
              Serial.println(msg1[0]);
              Serial.println(msg1[1]);
              Serial.println(msg1[2]);
              Serial.println(msg1[3]);
              select(msg1[3]);
              uint8_t msg[SSS7_PAYLOAD_SIZE];
              memset(msg, 0, SSS7_PAYLOAD_SIZE);
              msg[0] = MSG_CONNECTED;
              msg[1] = ID;
              msg[2] = msg1[2];
              msg[3] = msg1[3];
              Serial.println("Waiting to send");
              while(!SSS7.canSend());
              Serial.println("Sending ...");
              SSS7.send(msg);
              while(!SSS7.canSend());
              Serial.println("Finished Sending");
              if(SSS7.sendFailed()) {
                  Serial.println("Send failed");
              }
              msg[0] = MSG_DETECT;
              msg[1] = ID;
              msg[2] = 0;
              msg[3] = 0;
              delay(5000);
              Serial.println("Waiting to send1");
              while(!SSS7.canSend());
              Serial.println("Sending ...1");
              SSS7.send(msg);
              while(!SSS7.canSend());
              Serial.println("Finished Sending1");
              if(SSS7.sendFailed()) {
                  Serial.println("Send failed1");
              }
              msg[0] = MSG_REQUEST_DIRECT;
              msg[1] = 6;
              msg[2] = 0;
              msg[3] = 3;
              delay(5000);
              Serial.println("Waiting to send1");
              while(!SSS7.canSend());
              Serial.println("Sending ...1");
              SSS7.send(msg);
              while(!SSS7.canSend());
              Serial.println("Finished Sending1");
              if(SSS7.sendFailed()) {
                  Serial.println("Send failed1");
              }
    }
    }
    }

};

