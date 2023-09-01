#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>
#include "util.hpp"
// #include "display.hpp"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using I2C
#define OLED_RESET -1 // Reset pin
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// OLED GPIO
// SDA 8
// SCK/SCL 9

// Ultrasound GPIO
#define TRIG_PIN_A 18
#define ECHO_PIN_A 16

// LED GPIO
#define LED_PIN_A 4
#define LED_PIN_B 5

struct Calibration
{
    float aStart = 0;
} calibration;

float readPinDistance(int trigPin, int echoPin)
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH);
    return (duration * 0.0343) / 2;
}

void drawProgressbar(int x, int y, int width, int height, int progress)
{
    progress = progress > 100 ? 100 : progress; // set the progress value to 100
    progress = progress < 0 ? 0 : progress;     // start the counting to 0-100
    float bar = ((float)(width - 1) / 100) * progress;
    display.drawRect(x, y, width, height, WHITE);
    display.fillRect(x + 2, y + 2, bar, height - 4, WHITE); // initailize the graphics fillRect(int x, int y, int width, int height)
}

void setup()
{
    Serial.begin(115200);

    // Initialize the OLED object
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ; // Don't proceed, loop forever
    }

    // Initialise LED PINS
    pinMode(LED_PIN_A, OUTPUT); // sets the A digital pin of the Arduino in output mode
    pinMode(LED_PIN_B, OUTPUT); // sets the B digital pin of the Arduino in output mode

    // Clear the buffer.
    display.clearDisplay();

    // Ultrasonic Sensor
    pinMode(TRIG_PIN_A, OUTPUT);
    pinMode(ECHO_PIN_A, INPUT);
    calibration.aStart = readPinDistance(TRIG_PIN_A, ECHO_PIN_A);

    // Debug
    Serial.print("A Sensor Distance (cm): ");
    Serial.println(calibration.aStart);

    // Display Depth
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 22);
    display.print(calibration.aStart);
    display.print("cm");
    display.display();
    delay(1000);
    display.clearDisplay();
}

void loop()
{
    // Ultrasonic Measurement
    float distanceA = readPinDistance(TRIG_PIN_A, ECHO_PIN_A);
    int binLevel = (int)roundf(max(0.f, min((1 - (distanceA / calibration.aStart)), 1.f)) * 100.f);

    // Do Shit
    if (binLevel > 80)
    {
        // Light Red
        digitalWrite(LED_PIN_A, HIGH); // red LED lights up
        digitalWrite(LED_PIN_B, LOW);  // green LED goes out
    }
    else
    {
        // Light Green
        digitalWrite(LED_PIN_A, LOW);  // red LED lights up
        digitalWrite(LED_PIN_B, HIGH); // green LED goes out
    }

    // Debug
    serialPrintf("Raw Depth (cm): %f\n", distanceA);
    serialPrintf("Calculated Depth (cm): %f\n", abs(distanceA - calibration.aStart));

    // Progress Bar
    display.clearDisplay();
    display.setTextSize(2); // set font size to 2 you can set it up to 3
    display.setCursor(0, 1);
    display.setTextColor(WHITE);
    display.print(binLevel);
    display.print("% FULL");                   // set the word % FULL to be display on the oled
    drawProgressbar(0, 20, 120, 30, binLevel); // declare the graphics fillrect(int x, int y, int width, int height)
    display.display();
    delay(100);
}