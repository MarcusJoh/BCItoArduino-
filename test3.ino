
#include <Servo.h> 

const int buttonPin = 8;     // the number of the pushbutton pin
const int buttonPin2 = 9;
const int buttonPin3 = 11;
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to


boolean buttonState = 0;// variable for reading the pushbutton status
boolean buttonState2 = 0;
boolean buttonState3 = 0;

//int vib = 5;           // the pin that the vib is attached to

int speakerPin = 10; //piezo

#define GREENLED1  6 //ledlamps
#define GREENLED2  A1
#define GREENLED3  2
#define YELLOWLED1 4
#define YELLOWLED2 7
#define YELLOWLED3 12
#define YELLOWLED4 13

#define REDLED 5

Servo myservo;  // create servo object to control a servo 

int val;  // value read from the serial aka the mindwave
int sensorValue = 0; // value read from the pot
boolean active=true; 

void setup() 
{ 
  Serial.begin(9600);
  myservo.attach(3);  // attaches the servo on pin 3 to the servo object
  myservo.write(30); 

  pinMode(buttonPin, INPUT);  
  digitalWrite(buttonPin, HIGH);
  pinMode(buttonPin2, INPUT);  
  digitalWrite(buttonPin2, HIGH);
  pinMode(buttonPin3, INPUT); 
  digitalWrite(buttonPin3, HIGH);

  pinMode(speakerPin, OUTPUT);

  pinMode(GREENLED1, OUTPUT);
  pinMode(GREENLED2, OUTPUT);
  pinMode(GREENLED3, OUTPUT);
  pinMode(YELLOWLED1, OUTPUT);
  pinMode(YELLOWLED2, OUTPUT);
  pinMode(YELLOWLED3, OUTPUT);
  pinMode(YELLOWLED4, OUTPUT);
  pinMode(REDLED, OUTPUT);

} 

void loop() 
{  
  buttonState = digitalRead(buttonPin);
  buttonState2 = digitalRead(buttonPin2);
  buttonState3 = digitalRead(buttonPin3);
  sensorValue = analogRead(analogInPin);

  if(Serial.available()>0){
    val = Serial.read();
    if (buttonState3 == LOW) { // checks if the blink switch is on  
      digitalWrite(buttonPin3, HIGH);

      if (val>100){ 
        digitalWrite(REDLED, HIGH);
        active=true;
      }
      else{
        digitalWrite(REDLED, LOW);
        active=false;
      }
    }
    if (val>100){ 
      val= val-100;
    }
    sensorValue = map(sensorValue, 0, 1023, 0, 100);  //maps the sensor value to fit the motor 
    int kalVal= val+sensorValue; // add the value of the potentiometer for the calibration on the focus 
    if(active==true){
      Serial.println(kalVal);
      if (1<0) { // on/off
        digitalWrite(buttonPin3, HIGH);
        boolean hold=false;

        if (kalVal>60){  // checks the focus value to open the hatch if its high enough 
          myservo.write(130); 
        }
        if (kalVal<30){
          myservo.write(30);
        }

      }
      else{
        if (kalVal>60){
          Serial.println("ffff");
          myservo.write(130); 
        }
        else{ 
          myservo.write(30); 
        }

      }



      if (buttonState == LOW) { // checks if the led feedback mode is online 
        digitalWrite(buttonPin, HIGH);


        if (val<30){						// lights up more lamps as the focus value goes up
          digitalWrite(GREENLED1, LOW);
          digitalWrite(GREENLED2, LOW);
          digitalWrite(GREENLED3, LOW);
          digitalWrite(YELLOWLED1, LOW);
          digitalWrite(YELLOWLED2, LOW);
          digitalWrite(YELLOWLED3, LOW);
          digitalWrite(YELLOWLED4, LOW);

        }
        if (val>=30&&val<40){
          digitalWrite(GREENLED1, HIGH);
          digitalWrite(GREENLED2, LOW);
          digitalWrite(GREENLED3, LOW);
          digitalWrite(YELLOWLED1, LOW);
          digitalWrite(YELLOWLED2, LOW);
          digitalWrite(YELLOWLED3, LOW);
          digitalWrite(YELLOWLED4, LOW);

        }
        if (val>=40&&val<50){
          digitalWrite(GREENLED1, HIGH);
          digitalWrite(GREENLED2, HIGH);
          digitalWrite(GREENLED3, LOW);
          digitalWrite(YELLOWLED1, LOW);
          digitalWrite(YELLOWLED2, LOW);
          digitalWrite(YELLOWLED3, LOW);
          digitalWrite(YELLOWLED4, LOW);

        }
        if (val>=50&&val<60){
          digitalWrite(GREENLED1, HIGH);
          digitalWrite(GREENLED2, HIGH);
          digitalWrite(GREENLED3, HIGH);
          digitalWrite(YELLOWLED1, LOW);
          digitalWrite(YELLOWLED2, LOW);
          digitalWrite(YELLOWLED3, LOW);
          digitalWrite(YELLOWLED4, LOW);

        }
        if (val>=60&&val<70){             
          digitalWrite(GREENLED1, HIGH);
          digitalWrite(GREENLED2, HIGH);
          digitalWrite(GREENLED3, HIGH);              
          digitalWrite(YELLOWLED1, HIGH);
          digitalWrite(YELLOWLED2, LOW);
          digitalWrite(YELLOWLED3, LOW);
          digitalWrite(YELLOWLED4, LOW);

        }
        if (val>=70&&val<80){
          digitalWrite(GREENLED1, HIGH);
          digitalWrite(GREENLED2, HIGH);
          digitalWrite(GREENLED3, HIGH);              
          digitalWrite(YELLOWLED1, HIGH);
          digitalWrite(YELLOWLED2, HIGH);
          digitalWrite(YELLOWLED3, LOW);
          digitalWrite(YELLOWLED4, LOW);
        }
        if (val>=80&&val<90){
          digitalWrite(GREENLED1, HIGH);
          digitalWrite(GREENLED2, HIGH);
          digitalWrite(GREENLED3, HIGH);              
          digitalWrite(YELLOWLED1, HIGH);
          digitalWrite(YELLOWLED2, HIGH);
          digitalWrite(YELLOWLED3, HIGH);
          digitalWrite(YELLOWLED4, LOW);

        }
        if (val>=90&&val<100){             
          digitalWrite(GREENLED1, HIGH);
          digitalWrite(GREENLED2, HIGH);
          digitalWrite(GREENLED3, HIGH);              
          digitalWrite(YELLOWLED1, HIGH);
          digitalWrite(YELLOWLED2, HIGH);
          digitalWrite(YELLOWLED3, HIGH);
          digitalWrite(YELLOWLED4, HIGH);
        }
      }    
      else{
        digitalWrite(GREENLED1, LOW);
        digitalWrite(GREENLED2, LOW);
        digitalWrite(GREENLED3, LOW);
        digitalWrite(YELLOWLED1, LOW);
        digitalWrite(YELLOWLED2, LOW);
        digitalWrite(YELLOWLED3, LOW);
        digitalWrite(YELLOWLED4, LOW);

      }

      if (buttonState2 == LOW) { // checks if the vibration feedback mode is online 
        digitalWrite(buttonPin2, HIGH);

        if (val>30){							// vibrates hard if the focus value is high 
          val = map(val, 30, 100, 100, 255); 
          analogWrite(vib, val); 
        }
        else{
          analogWrite(vib, LOW);
        }

      }
      else{
        analogWrite(vib, LOW);
      }

      if (buttonState==HIGH&&buttonState2==HIGH) {  // checks if the sound feedback mode is online 
        Serial.println("gh");
        if (val>30){
          val = map(val, 30, 100, 100, 230);  // the intervals between the clicks lowers the higher the focus value is 
          digitalWrite(speakerPin,HIGH);
          delay (250-val);  
          digitalWrite(speakerPin,LOW);  
          delay (250-val);
          digitalWrite(speakerPin,HIGH);
          delay (250-val);  
          digitalWrite(speakerPin,LOW);  
          delay (250-val); 
          digitalWrite(speakerPin,HIGH);
          delay (250-val);  
          digitalWrite(speakerPin,LOW);  
          delay (250-val);  
        }
        else { 
          digitalWrite(speakerPin,LOW);
        }
      }  
      else { 
        digitalWrite(speakerPin,LOW);
      } 
      delay (100);  
    }
  }

}










