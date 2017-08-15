/* Based on  the AMIS-30543 stepper motor
driver.
*/


#include <SPI.h>
#include <AMIS30543.h>
#include <LiquidCrystal.h> 

const uint8_t amisDirPin = 2;
const uint8_t amisStepPin = 3;
const uint8_t amisSlaveSelect = 4;

//LCD definitions
//const int rs = 7, en = 6, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd (7, 6, 9, 10, 11, 12);

AMIS30543 stepper;

//Optical counter definitions
byte initial_State;
byte actual_State;
int turn_Count;

void setup()
{ 

//Pedal Off detection
pinMode(8, INPUT_PULLUP); //Connect pedal switch to D8
//digitalWrite(8,LOW);
digitalWrite(amisStepPin, LOW);

//Rotation counter
pinMode(5, INPUT_PULLUP);

//pinMode(13, OUTPUT); //Used fo debug purposes
//digitalWrite(13,LOW);
  
  SPI.begin();
  stepper.init(amisSlaveSelect);

  // Drive the NXT/STEP and DIR pins low initially.
  digitalWrite(amisStepPin, LOW);
  pinMode(amisStepPin, OUTPUT);
  digitalWrite(amisDirPin, LOW);
  pinMode(amisDirPin, OUTPUT);

  // Give the driver some time to power up.
  delay(1);

  // Reset the driver to its default settings.
  stepper.resetSettings();

  // Set the current limit.  You should change the number here to
  // an appropriate value for your particular system.
  stepper.setCurrentMilliamps(2000);

  // Set the number of microsteps that correspond to one full step.
  stepper.setStepMode(32);
   setDirection(0);
  // Enable the motor outputs.
  //stepper.enableDriver();

  lcd.begin(16, 2); // Initialize LCD
  delay(200);    
  lcd.clear();    //Clear LCD
  delay(200);  //wait
 
  lcd.setCursor(0,0);
  lcd.print("Turns");
  lcd.setCursor(0,1);
  lcd.print("0");
   lcd.display();

  turn_Count = 0;

 pinMode(A1, INPUT_PULLUP); //Reset number of Turns pin 

  //Serial.begin(9600); 

}

void loop()
{
  // Step in the default direction 1000 times.  
 if (digitalRead(8) == LOW)
  {   
    //digitalWrite(13,HIGH);
     stepper.enableDriver();
    int val = map (analogRead(A0), 0, 1023,2000,0);
    step(val);
  }
  else
  {
     stepper.disableDriver();
     //digitalWrite(13,LOW);
    }
    
   actual_State = digitalRead(5);
  if ((actual_State == LOW) && (initial_State == HIGH))
  {
    turn_Count++;  //increment counter
    
//    lcd.setCursor(0,1); //--\
//    lcd.print(" ");     //-- \
//    lcd.setCursor(0,3); //--  \   //Cler the second line before writing any new turn count
//    lcd.print(" ");     //--  /
//    lcd.setCursor(0,4); //-- /
//    lcd.print(" ");     //--/
    
    lcd.setCursor(0,1);
    lcd.print(turn_Count);
  }
   initial_State = digitalRead(5); //read initial state of rotation counter pin

   if (digitalRead (A1) == LOW)
   {
     turn_Count = 0;

    lcd.setCursor(0,1); //Reset the counter on the second line only
    lcd.print(" ");    
    lcd.print(" ");
    lcd.print(" "); 
      
    lcd.setCursor(0,1); // diplay counter as 0
    lcd.print(turn_Count);
   }
  //Serial.println(turn_Count); 
}

// Sends a pulse on the NXT/STEP pin to tell the driver to take
// one step, and also delays to control the speed of the motor.
void step(int val)
{
  // The NXT/STEP minimum high pulse width is 2 microseconds.
  digitalWrite(amisStepPin, HIGH);
  delayMicroseconds(3);
  digitalWrite(amisStepPin, LOW);
  delayMicroseconds(3);

  // The delay here controls the stepper motor's speed.  You can
  // increase the delay to make the stepper motor go slower.  If
  // you decrease the delay, the stepper motor will go fast, but
  // there is a limit to how fast it can go before it starts
  // missing steps.
  delayMicroseconds(val);
}

// Writes a high or low value to the direction pin to specify
// what direction to turn the motor.
void setDirection(bool dir)
{
  // The NXT/STEP pin must not change for at least 0.5
  // microseconds before and after changing the DIR pin.
  delayMicroseconds(1);
  digitalWrite(amisDirPin, dir);
  delayMicroseconds(1);
}
