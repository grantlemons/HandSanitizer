#include <Adafruit_MotorShield.h>
#include <Arduino.h>

//* Pins
const int triggerPin = 10;
const int echoPin = 11;

// Stepper motor creation and instance variables
const int delayBeforeNext = 3500;
const int degreesToDispense = 30;
const int stepsPerRevolute = 200;
const double motorSpeed = 1;
int stepCount;
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_StepperMotor *myMotor = AFMS.getStepper(stepsPerRevolute, 2);

void setup() {
    AFMS.begin();
}

bool sensor() {
    // Clears the triggerPin
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    // Sets the triggerPin on HIGH state for 10 micro seconds
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    double duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    double distanceCM= duration*0.034/2;

    if ( 12 > distanceCM > 4 ) return true;
    else return false;
}

void dispense() {
    //TODO: Protothread activate so it can run simultaneously
    light.color( 255, 0, 0 );
    light.setBrightness( 255 );
    if ( motorSpeed > 0 ) {
        myMotor->setSpeed(100);

        //* Rotate cam to dispense
        myMotor->step( ( stepsPerRevolute / 360 ) * degreesToDispense, FORWARD, SINGLE );
        stepCount += ( stepsPerRevolute / 360 ) * degreesToDispense;

        delay( 300 );
        
        //* Return to original position
        myMotor->step( ( stepsPerRevolute / 360 ) * degreesToDispense, BACKWARD, SINGLE );
        stepCount -= ( stepsPerRevolute / 360 ) * degreesToDispense;
    }
    light.color( 0, 255, 0 );
    light.setBrightness( 255 );
}

void loop() {
    dispense();
    if ( sensor() ) {
        dispense();
        delay( delayBeforeNext );
    }
}