#include <WiFi.h>
#include <WiFiClient.h>
#define BLYNK_TEMPLATE_ID "TMPL6MrvV20al"
#define BLYNK_TEMPLATE_NAME "Smart Waste Managment"
#define BLYNK_AUTH_TOKEN "l2xVZYlfUJVxZhvzbh8IjTVvJ80lt0eg"
#include <BlynkSimpleEsp32.h>
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define TRIG_PIN 12
#define ECHO_PIN 14
#define CM_TO_INCH 0.393701

// Define LED pins
#define LOW_DISTANCE_LED 17
#define HIGH_DISTANCE_LED 16
long duration_us;
float distanceCm;
char auth[] = "l2xVZYlfUJVxZhvzbh8IjTVvJ80lt0eg";
char ssid[] = "Wokwi-GUEST";  
char pass[] = "";
BlynkTimer timer;
// Initialize LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27 (from DIYables LCD), 16 columns and 2 rows

void setup() {
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(LOW_DISTANCE_LED, OUTPUT);
    pinMode(HIGH_DISTANCE_LED, OUTPUT);
    Serial.begin(9600);
    Blynk.begin(auth, ssid, pass);

  timer.setInterval(1000L, sendSensor);

    // Initialize LCD
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Distance: ");
}

void loop() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration_us = pulseIn(ECHO_PIN, HIGH);
    float distanceCm = 0.017 * duration_us;
    float distanceInch = distanceCm * CM_TO_INCH;

    // Display distance on LCD
    lcd.setCursor(9, 0);
    lcd.print(distanceCm);
    lcd.print(" cm");
    Blynk.run();
    timer.run();
    // Add LED logic
    if (distanceCm <= 100) {
        digitalWrite(LOW_DISTANCE_LED, HIGH); // Turn on low distance LED
        digitalWrite(HIGH_DISTANCE_LED, LOW); // Turn off high distance LED
        lcd.setCursor(0, 1);
        lcd.print("Use the bin");
    } else if (distanceCm > 100) {
        digitalWrite(LOW_DISTANCE_LED, LOW); // Turn off low distance LED
        digitalWrite(HIGH_DISTANCE_LED, HIGH); // Turn on high distance LED
        lcd.setCursor(0, 1);
        lcd.print("Bin Is Full");
    } 
    delay(1000); // Wait for a second
}
void sendSensor()
{
  digitalWrite(TRIG_PIN, LOW);   
  delayMicroseconds(2);       
  digitalWrite(TRIG_PIN, HIGH); 
  delayMicroseconds(10);      
  digitalWrite(TRIG_PIN, LOW);   
  duration_us = pulseIn(ECHO_PIN, HIGH);   
  distanceCm = duration_us * 0.034 / 2;  

  Serial.print("Distance = ");        
  Serial.println(distanceCm);

  if(distanceCm <= 5)
  {

  }
  Blynk.virtualWrite(V0, distanceCm);
  delay(1000);                 //Pause for 1 seconds and start measuring distance again
}
