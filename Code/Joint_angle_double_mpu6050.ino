#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MPU6050.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Pin Assignments
const int buttonUpPin = 2;        // Browse joints
const int buttonSelectPin = 3;    // Calibrate/Confirm
const int buttonCalibratePin = 4; // Inversion toggle & wake-up

String jointNames[] = {"Knee", "Elbow", "Wrist", "Ankle"};
const int totalJoints = 4;
int jointSelection = 0;

enum Mode { SELECTION, CALIBRATION, MEASUREMENT };
Mode currentMode = SELECTION;

MPU6050 mpu1(0x68);  // AD0 = GND
MPU6050 mpu2(0x69);  // AD0 = VCC

float baseAngle = 0.0;
float lastMeasuredAngle = 0.0;
const float angleThreshold = 2.0;
bool invertAngle = false;

unsigned long lastButtonPress = 0;
const unsigned long debounceDelay = 300;
unsigned long lastActivityTime = 0;
const unsigned long inactivityTimeout = 30000;
bool displayOn = true;
bool countdownActive = false;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED not found"));
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.display();

  pinMode(buttonUpPin, INPUT_PULLUP);
  pinMode(buttonSelectPin, INPUT_PULLUP);
  pinMode(buttonCalibratePin, INPUT_PULLUP);

  mpu1.initialize();
  mpu2.initialize();

  if (!mpu1.testConnection()) Serial.println("MPU6050 #1 not connected!");
  if (!mpu2.testConnection()) Serial.println("MPU6050 #2 not connected!");

  Serial.println(F("Both MPU6050s Initialized..."));
}

void loop() {
  unsigned long currentMillis = millis();
  checkInactivity();

  if (currentMode == SELECTION) {
    displaySelectionMenu();

    if ((digitalRead(buttonUpPin) == LOW) && (currentMillis - lastButtonPress > debounceDelay)) {
      jointSelection = (jointSelection + 1) % totalJoints;
      lastButtonPress = currentMillis;
      lastActivityTime = currentMillis;
      countdownActive = false;
    }

    if ((digitalRead(buttonSelectPin) == LOW) && (currentMillis - lastButtonPress > debounceDelay)) {
      currentMode = CALIBRATION;
      lastButtonPress = currentMillis;
      lastActivityTime = currentMillis;
      countdownActive = false;
    }
  }

  else if (currentMode == CALIBRATION) {
    displayCalibrating();
    delay(1000);
    baseAngle = readRelativeAngle();
    lastMeasuredAngle = baseAngle;
    delay(500);
    currentMode = MEASUREMENT;
    lastActivityTime = millis();
  }

  else if (currentMode == MEASUREMENT) {
    int jointSign = getJointSign(jointSelection);
    float angle = (readRelativeAngle() - baseAngle) * jointSign;

    if (invertAngle) angle *= -1;

    displayAngle(angle);
    delay(200);

    if (abs(angle - lastMeasuredAngle) > angleThreshold) {
      lastActivityTime = millis();
      countdownActive = false;
      lastMeasuredAngle = angle;
    }

    if ((digitalRead(buttonUpPin) == LOW) && (currentMillis - lastButtonPress > debounceDelay)) {
      currentMode = SELECTION;
      lastButtonPress = currentMillis;
      lastActivityTime = millis();
      countdownActive = false;
    }

    if ((digitalRead(buttonCalibratePin) == LOW) && (currentMillis - lastButtonPress > debounceDelay)) {
      invertAngle = !invertAngle;
      lastButtonPress = currentMillis;
      lastActivityTime = millis();
    }
  }
}

// Adjust angle polarity based on joint logic
int getJointSign(int joint) {
  switch (joint) {
    case 0: return 1;   // Knee: extension is positive
    case 1: return -1;  // Elbow: flexion is positive
    case 2: return -1;  // Wrist: flexion (inward curl) is positive
    case 3: return 1;   // Ankle: extension (plantarflexion) is positive
    default: return 1;
  }
}

float readRelativeAngle() {
  int16_t ax1, ay1, az1;
  int16_t ax2, ay2, az2;

  mpu1.getAcceleration(&ax1, &ay1, &az1);
  mpu2.getAcceleration(&ax2, &ay2, &az2);

  float angle1 = atan2((float)ax1 / 16384.0, (float)az1 / 16384.0) * 180.0 / PI;
  float angle2 = atan2((float)ax2 / 16384.0, (float)az2 / 16384.0) * 180.0 / PI;

  float relativeAngle = angle2 - angle1;
  if (relativeAngle > 180) relativeAngle -= 360;
  if (relativeAngle < -180) relativeAngle += 360;

  return relativeAngle;
}

void displaySelectionMenu() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("Select:");
  display.setCursor(0, 30);
  display.println(jointNames[jointSelection]);
  display.display();
}

void displayCalibrating() {
  display.clearDisplay();
  display.setCursor(0, 20);
  display.setTextSize(2);
  display.println("Calibrating...");
  display.display();
}

void displayAngle(float angle) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("Angle:");

  display.setCursor(0, 32);
  display.print(angle, 1);
  display.println(" deg");

  // Display INV toggle status (top-right)
  if (invertAngle) {
    display.setTextSize(1);
    display.setCursor(SCREEN_WIDTH - 20, 0);
    display.println("INV");
    display.setTextSize(2);
  }

  display.display();
}

void checkInactivity() {
  unsigned long currentMillis = millis();

  if (displayOn && (currentMillis - lastActivityTime) > inactivityTimeout && !countdownActive) {
    countdownActive = true;
    startCountdown();
  }

  if (displayOn && countdownActive && (currentMillis - lastActivityTime) > (inactivityTimeout + 5000)) {
    display.ssd1306_command(SSD1306_DISPLAYOFF);
    displayOn = false;
  }

  if (!displayOn && (digitalRead(buttonCalibratePin) == LOW)) {
    display.ssd1306_command(SSD1306_DISPLAYON);
    lastActivityTime = millis();
    displayOn = true;
    countdownActive = false;
  }
}

void startCountdown() {
  for (int i = 5; i >= 1; i--) {
    display.clearDisplay();
    display.setCursor(0, 20);
    display.setTextSize(2);
    display.print("Sleep in: ");
    display.print(i);
    display.display();
    delay(1000);
  }
}