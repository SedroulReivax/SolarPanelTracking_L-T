#include <Servo.h>

Servo myServo;

// Pin definitions
const int LDR_LEFT = 2;     
const int LDR_RIGHT = 3;    
const int SERVO_PIN = 9;    

// Variables
int leftLDR = 0;
int rightLDR = 0;
int servoPos = 90;     
int moveStep = 3;      
int delayTime = 200;   

// Servo limits
const int MIN_ANGLE = 0;
const int MAX_ANGLE = 180;

unsigned long lastMoveTime = 0;
const unsigned long moveInterval = 300;

// Dead-zone flag
bool inDeadZone = false;

void setup() {
  Serial.begin(9600);
  pinMode(LDR_LEFT, INPUT);
  pinMode(LDR_RIGHT, INPUT);

  myServo.attach(SERVO_PIN);
  myServo.write(servoPos);
  delay(1000);

  Serial.println("Digital Solar Tracker Ready (with Dead-Zone)");
  Serial.println("Torch = Panel moves smoothly toward light");
}

void loop() {
  leftLDR = digitalRead(LDR_LEFT);   
  rightLDR = digitalRead(LDR_RIGHT); 

  String action = "No Move";

  if (millis() - lastMoveTime >= moveInterval) {
    // Case 1: Torch only on LEFT
    if (leftLDR == LOW && rightLDR == HIGH) {
      if (servoPos > MIN_ANGLE) {
        servoPos -= moveStep;
        myServo.write(servoPos);
        action = "← Left";
        inDeadZone = false;
        lastMoveTime = millis();
      }
    }
    // Case 2: Torch only on RIGHT
    else if (leftLDR == HIGH && rightLDR == LOW) {
      if (servoPos < MAX_ANGLE) {
        servoPos += moveStep;
        myServo.write(servoPos);
        action = "→ Right";
        inDeadZone = false;
        lastMoveTime = millis();
      }
    }
    // Case 4: Both DARK
    else {
      action = "No Light";
      inDeadZone = false;
    }
  }

  // Debugging
  Serial.print(leftLDR == LOW ? "LIGHT" : "DARK");
  Serial.print(" | ");
  Serial.print(rightLDR == LOW ? "LIGHT" : "DARK");
  Serial.print(" | Servo: ");
  Serial.print(servoPos);
  Serial.print("° | Action: ");
  Serial.println(action);

  delay(1000);
}