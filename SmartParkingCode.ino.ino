#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

Servo gateServo;

// IR Sensors
#define ENTRY_IR 2
#define EXIT_IR 3

// Servo Motor Pin
#define SERVO_PIN 9

// Parking Slots
int totalSlots = 4;
int availableSlots = 4;

void setup()
{
  pinMode(ENTRY_IR, INPUT);
  pinMode(EXIT_IR, INPUT);

  // Attach Servo
  gateServo.attach(SERVO_PIN);

  // Gate Closed Initially
  gateServo.write(0);

  // LCD Start
  lcd.init();
  lcd.backlight();

  // Serial Monitor
  Serial.begin(9600);

  lcd.setCursor(0,0);
  lcd.print("SMART PARKING");

  Serial.println("SMART PARKING SYSTEM");

  delay(2000);
  lcd.clear();
}

void loop()
{
  int entry = digitalRead(ENTRY_IR);
  int exitSensor = digitalRead(EXIT_IR);

  // ---------------- ENTRY ----------------
  if(entry == LOW)
  {
    Serial.println("Car Detected At Entry");

    if(availableSlots > 0)
    {
      // Open Gate
      gateServo.write(90);

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Gate Open");

      Serial.println("Gate Opened");

      delay(2000);

      // Decrease Slot Count
      availableSlots--;

      lcd.clear();

      // Show Available Slots
      lcd.setCursor(0,0);
      lcd.print("Slots Left:");
      lcd.print(availableSlots);

      lcd.setCursor(0,1);

      // Parking Priority
      // P2 -> P3 -> P4 -> P1

      if(availableSlots == 3)
      {
        lcd.print("Go to P2");
        Serial.println("Nearest Parking: P2");
      }
      else if(availableSlots == 2)
      {
        lcd.print("Go to P3");
        Serial.println("Nearest Parking: P3");
      }
      else if(availableSlots == 1)
      {
        lcd.print("Go to P4");
        Serial.println("Nearest Parking: P4");
      }
      else if(availableSlots == 0)
      {
        lcd.print("Go to P1");
        Serial.println("Nearest Parking: P1");
      }

      // Serial Monitor
      Serial.print("Available Slots: ");
      Serial.println(availableSlots);

      delay(3000);

      // Close Gate
      gateServo.write(0);

      Serial.println("Gate Closed");
      Serial.println("--------------------");

      // Wait Until Car Passes
      while(digitalRead(ENTRY_IR) == LOW);
    }
    else
    {
      lcd.clear();

      lcd.setCursor(0,0);
      lcd.print("Slots Left:0");

      lcd.setCursor(0,1);
      lcd.print("Parking Full");

      Serial.println("Parking Full");

      delay(2000);
    }
  }

  // ---------------- EXIT ----------------
  if(exitSensor == LOW)
  {
    Serial.println("Car Detected At Exit");

    if(availableSlots < totalSlots)
    {
      // Open Gate
      gateServo.write(90);

      lcd.clear();

      lcd.setCursor(0,0);
      lcd.print("Car Exiting");

      Serial.println("Exit Gate Opened");

      delay(2000);

      // Increase Slot Count
      availableSlots++;

      lcd.clear();

      lcd.setCursor(0,0);
      lcd.print("Slots Left:");
      lcd.print(availableSlots);

      lcd.setCursor(0,1);

      // Show Next Parking
      if(availableSlots == 4)
      {
        lcd.print("Next: P2");
      }
      else if(availableSlots == 3)
      {
        lcd.print("Next: P3");
      }
      else if(availableSlots == 2)
      {
        lcd.print("Next: P4");
      }
      else
      {
        lcd.print("Next: P1");
      }

      // Serial Monitor
      Serial.print("Available Slots: ");
      Serial.println(availableSlots);

      delay(3000);

      // Close Gate
      gateServo.write(0);

      Serial.println("Exit Gate Closed");
      Serial.println("--------------------");

      // Wait Until Car Leaves
      while(digitalRead(EXIT_IR) == LOW);
    }
  }

  // ---------------- DEFAULT LCD ----------------
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Slots Left:");
  lcd.print(availableSlots);

  lcd.setCursor(0,1);

  if(availableSlots == 0)
  {
    lcd.print("Parking Full");
  }
  else if(availableSlots == 4)
  {
    lcd.print("Next: P2");
  }
  else if(availableSlots == 3)
  {
    lcd.print("Next: P3");
  }
  else if(availableSlots == 2)
  {
    lcd.print("Next: P4");
  }
  else
  {
    lcd.print("Next: P1");
  }

  delay(500);
}