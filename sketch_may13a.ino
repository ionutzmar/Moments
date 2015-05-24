#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int switchPin = 6;
const int greenLed = 7;
const int yellowLed = 8;
const int lampPin = 9;
const int servoPin = 10;
const int lightPin = A3;
const int tempPin = A4;
const int piezo = A2;
const int quiteKnock = 6;
const int loudKnock = 1000;
const int lightThreshold = 250;
int numberOfKnocks = 0;
int knockInterval = 600;
int switchState = 0;
int prevSwitchState = 0;
unsigned long previousTime = 0;
unsigned long currentTime = 0;
unsigned long buttonTime = 0;
unsigned long presentKnockTime = 0;
unsigned long lastKnockTime = 0;
unsigned long tempTime = 0;
int angle = 0;
int lightValue;
int scrollingSpeed = 300;
int gameSpeed = 200;
int gameStage = 1;   // 1 = welcome; 2 = gameover; 0 = play;
int score = 0;
int manPosition = 1;
int obstacleOnePosition = 10;
int obstacleTwoPosition = 16;
int turnYellowLedOff = 0;
String line1 = "Welcome to TimiX";
String line2 = "Press the button you have!!!";
String line3 = "To play again press the button!";
int stringStart, stringStop = 0;
int scrollCursor = 16;
boolean doSomething = false;
boolean isHigh = false;
boolean lampOn = false;
float temperature;
float tempArray[200];
float tempSum;
float temp;
int tempNumber = 200;
int counter = -1;
int tempUnits;
//int tempDecimal;
int action;

byte manRunning1[8] = {
  B01110,
  B11111,
  B01110,
  B00100,
  B00100,
  B01010,
  B10001,
};

byte obstacle[8] = {
  B01110,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

byte sadFace[8] = {
  B00000,
  B00000,
  B01010,
  B00000,
  B00000,
  B01110,
  B10001,
};

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(switchPin, INPUT);
  pinMode(greenLed,  OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(lampPin, OUTPUT);
  lcd.createChar(0, manRunning1);
  lcd.createChar(1, obstacle);
  lcd.createChar(2, sadFace);
  for (int i = 0; i < tempNumber; i++)
  {
    tempArray[i] = ((analogRead(tempPin)/1024.0)*5.0 - 0.5)*100;
    tempSum += tempArray[i];
  }
  temp = tempSum / tempNumber;
//  pinMode(8, OUTPUT);
//  
//  for (int i = 50; i <= 4000; i++)
//  {
//    tone(8, i, 50);
//    delay(50);
//  }
}

void loop()
{
  
  
  ///The Game:
  switchState = digitalRead(switchPin);
  currentTime = millis();
  
  if ((prevSwitchState == switchState) && (gameStage == 1) && (currentTime - previousTime > scrollingSpeed))  /////Welcome
  {
    previousTime = currentTime;
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(scrollCursor, 1);
    lcd.print(line2.substring(stringStart,stringStop));
 
    if(stringStart == 0 && scrollCursor > 0)
    {
        scrollCursor--;
        stringStop++;
    }
    else if (stringStart == stringStop)
    {
      stringStart = stringStop = 0;
      scrollCursor = 16;
    } 
    else if (stringStop == line2.length() && scrollCursor == 0) 
    {
      stringStart++;
    } 
    else 
    {
      stringStart++;
      stringStop++;
    }

  }
  
  if ((prevSwitchState != switchState) && (switchState == HIGH) && (gameStage == 1))   /////////// Button Pressed on Welcome Screen
  {
    gameStage = 0;
    lcd.clear();
  }
  
  if (gameStage == 0 && (currentTime - previousTime > gameSpeed))  ////////////////// The Game!!!
  {
    previousTime = currentTime;
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(score);
    
    if ((switchState == HIGH) && (currentTime - buttonTime > (5 * gameSpeed)))
    {
      buttonTime = currentTime;
      manPosition = 0;
      lcd.setCursor(3, 0);
      lcd.write(byte(0));
      
    }
    else
    {
      if (currentTime - buttonTime < (4 * gameSpeed))
      {
        manPosition = 0;
        lcd.setCursor(3, 0);
        lcd.write(byte(0));
      }
      else
      {
        manPosition = 1;
        lcd.setCursor(3, 1);
        lcd.write(byte(0));
      }
    }

    
    if ((obstacleOnePosition > 0) && (obstacleTwoPosition > 0))
    {
      obstacleOnePosition--;
      obstacleTwoPosition--;
    }
    else if (obstacleOnePosition > 0)
    {
      obstacleTwoPosition = 16;
      obstacleOnePosition--;
      score++;
    }
    else //if (obstacleTwoPosition > 0)
    {
      obstacleOnePosition = 16;
      obstacleTwoPosition--;
      score++;
    } 
        
    lcd.setCursor(obstacleOnePosition, 1);
    lcd.write(byte(1));
    lcd.setCursor(obstacleTwoPosition, 1);
    lcd.write(byte(1));
    
    if (((obstacleOnePosition == 4) || (obstacleTwoPosition == 4) || (obstacleOnePosition == 3) || (obstacleTwoPosition == 3)) && manPosition == 1)
    {
      gameStage = 2;
      stringStop, stringStart = 0;
      
    }
  }
  
  if (gameStage == 2)   ////////////////// Game Over
  {
    if (currentTime - previousTime > scrollingSpeed)
    {
      previousTime = currentTime;
      score = 0;
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("Game over ");
      lcd.write(byte(2));
      lcd.setCursor(scrollCursor, 1);
      lcd.print(line3.substring(stringStart,stringStop));
   
      if(stringStart == 0 && scrollCursor > 0)
      {
          scrollCursor--;
          stringStop++;
      }
      else if (stringStart == stringStop)
      {
        stringStart = stringStop = 0;
        scrollCursor = 16;
      } 
      else if (stringStop == line3.length() && scrollCursor == 0) 
      {
        stringStart++;
      } 
      else 
      {
        stringStart++;
        stringStop++;
      }
    }
    if (switchState == HIGH)
      gameStage = 0;
  }
  prevSwitchState = switchState;
  
lightValue = analogRead(lightPin);
   if (lampOn && lightValue > lightThreshold)
   {
     digitalWrite(lampPin, LOW);
     lampOn = false;
   }
   else if (!lampOn && lightValue < lightThreshold - 100)
   {
     digitalWrite(lampPin, HIGH);
     lampOn = true;
   }
  //Serial.println(lightValue);
  
  ////Temperature
 // float temperature = ((analogRead(tempPin) / 1024.0) * 5.0 - .5) * 100;
  temperature = ((analogRead(tempPin)/1024.0)*5.0 - 0.5)*100;
  if (counter > tempNumber - 1)
    counter = 0;
  else
    counter++;
   tempSum -= tempArray[counter];
   tempArray[counter] = temperature;
   tempSum += tempArray[counter];
   temp = tempSum / tempNumber;
   tempUnits = (int) temp;
   //tempDecimal = ((int) (temp * 100)) % 100;
   //tempDecimal = (int) (temp * 100) - (((int) (temp * 100)) / 100) * 100;
//   Serial.println(temp);
//   Serial.println(tempUnits);
//   Serial.println(tempDecimal);
  
  
  
  
  // Knocks
knocks(analogRead(piezo));

if (currentTime - presentKnockTime > knockInterval && numberOfKnocks == 2 && doSomething && gameStage != 0)
{
  //Serial.println("play");
  doSomething = false;
  digitalWrite(greenLed, HIGH);
  isHigh = true;
  action = 1;
  Serial.write(tempUnits);
  //Serial.write(tempDecimal);
  Serial.write(action);
  tempTime = currentTime;
}
else if (currentTime - presentKnockTime > knockInterval && numberOfKnocks == 3 && doSomething && gameStage != 0)
{
  //Serial.println("forward");
  doSomething = false;
  digitalWrite(greenLed, HIGH);
  isHigh = true;
  action = 2;
  Serial.write(tempUnits);
  //Serial.write(tempDecimal);
  Serial.write(action);
  tempTime = currentTime;
}
else if (currentTime - presentKnockTime > knockInterval && numberOfKnocks == 4 && doSomething && gameStage != 0)
{
  //Serial.println("backward");
  doSomething = false;
  digitalWrite(greenLed, HIGH);
  isHigh = true;
  action = 3;
  Serial.write(tempUnits);
  //Serial.write(tempDecimal);
  Serial.write(action);
  tempTime = currentTime;
}
else if (gameStage != 0 && currentTime - tempTime > 500)
{
  action = 0;
  Serial.write(tempUnits);
  //Serial.write(tempDecimal);
  Serial.write(action);
  tempTime = currentTime;
}


if (isHigh && currentTime - presentKnockTime > knockInterval + 500)
{
  digitalWrite(greenLed, LOW);
  isHigh = false; 
}
  
  if (currentTime - presentKnockTime > 30 && turnYellowLedOff == 1)
  {
      digitalWrite(yellowLed, LOW);
      turnYellowLedOff = 0;
  }
  
}

void knocks(int value) {
  if (value > quiteKnock && value < loudKnock && gameStage != 0){
    digitalWrite(yellowLed, HIGH);
    lastKnockTime = presentKnockTime;
    presentKnockTime = currentTime;
    turnYellowLedOff = 1;
    if (presentKnockTime - lastKnockTime < knockInterval)
      {
        numberOfKnocks++;
        doSomething = true;
        //Serial.println(numberOfKnocks);
      }
    else
      numberOfKnocks = 1;
     delay(50);
  }
}
  
