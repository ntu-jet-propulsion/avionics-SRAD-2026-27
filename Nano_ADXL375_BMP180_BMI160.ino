// Arduino Nano I2C:
// SDA → A4
// SCL → A5

// BMI160:
// Vin → 3.3V
// GND → GND
// SDA → A4
// SCL → A5

// BMP180:
// Vin → 3.3V
// GND → GND
// SDA → A4
// SCL → A5

// ADXL375:
// Vin → 3.3V
// GND → GND
// SDA → A4
// SCL → A5

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include <BMI160Gen.h>


Adafruit_BMP085 bmp;

// kalman filter structure
struct Kalman {
  float x;      // state
  float p;      // uncertainty
  float q;      // process noise
  float r;      // measurement noise
};

Kalman kalAlt = {0, 1, 0.01, 1};
Kalman kalRoll = {0, 1, 0.01, 0.5};
Kalman kalPitch = {0, 1, 0.01, 0.5};

float kalmanUpdate(Kalman &k, float measurement) {
  k.p += k.q;
  float kGain = k.p / (k.p + k.r);
  k.x += kGain * (measurement - k.x);
  k.p *= (1 - kGain);
  return k.x;
}

// variable definitions
int accXi, accYi, accZi;
int gyroXi, gyroYi, gyroZi;
float rollAcc, pitchAcc;
float altitude;
unsigned long lastTime;
float accX, accY, accZ;
float gyroX, gyroY, gyroZ;

// boring setup ew
const float LOOP_DT = 0.01; // 100 Hz fixed timestep
unsigned long lastLoopMicros = 0;

// ADXL375 gyro bias
float gyroBiasX = 0;
float gyroBiasY = 0;
float gyroBiasZ = 0;

void calibrateGyro() {
  const int samples = 50;
  float sx = 0, sy = 0, sz = 0;

  for (int i = 0; i < samples; i++) {
    BMI160.readGyro(gyroXi, gyroYi, gyroZi);
    sx += gyroXi;
    sy += gyroYi;
    sz += gyroZi;
    delay(5);
  }

  gyroBiasX = sx / samples;
  gyroBiasY = sy / samples;
  gyroBiasZ = sz / samples;
}

void setup() {
  Serial.begin(1000);
  Wire.begin();   // A4 = SDA, A5 = SCL

//detect BMI160 connected?
  if (!BMI160.begin(BMI160GenClass::I2C_MODE)) {
    Serial.println("BMI160 not detected");
    while (1);
  }

  BMI160.setGyroRange(250);
  BMI160.setAccelerometerRange(2);

  calibrateGyro();

//detect BMP180 connected?
  if (!bmp.begin()) {
    Serial.println("BMP180 not detected");
    while (1);
  }

  lastTime = micros();
  Serial.println("Sensors initialized");
}

// for entire flight
void loop() {
  byte error, address;
  int nDevices = 0;

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at 0x");
      Serial.println(address, HEX);
      nDevices++;
    }
  }

  if (nDevices == 0)
    Serial.println("No I2C devices found");

  delay(2000);

  unsigned long now = micros();
  if (now - lastTime < 10000) return;
  lastTime += 10000;

  float dt = LOOP_DT;

  //read BMI160 
  BMI160.readAccelerometer(accXi, accYi, accZi);
  BMI160.readGyro(gyroXi, gyroYi, gyroZi);

  gyroX -= gyroBiasX;
  gyroY -= gyroBiasY;
  gyroZ -= gyroBiasZ;

  // convert to g and deg/s
  accX = accXi / 16384.0;   // g
  accY = accYi / 16384.0;
  accZ = accZi / 16384.0;

  gyroX = gyroXi / 131.0;   // deg/s
  gyroY = gyroYi / 131.0;
  gyroZ = gyroZi / 131.0;

// accelerometer angles
  rollAcc = atan2(accY, accZ) * 180 / PI;
  pitchAcc = atan2(-accX, sqrt(accY * accY + accZ * accZ)) * 180 / PI;

// gyro 
  kalRoll.x += gyroX * dt;
  kalPitch.x += gyroY * dt;

// kalman type shi
  float roll = kalmanUpdate(kalRoll, rollAcc);
  float pitch = kalmanUpdate(kalPitch, pitchAcc);

// to read BMP180
  float pressure = bmp.readPressure() / 100.0; // hPa
  altitude = bmp.readAltitude(1013.25);
  altitude = kalmanUpdate(kalAlt, altitude);

// Allan Variance
  Serial.print(gyroX);
  Serial.print(",");
  Serial.print(gyroY);
  Serial.print(",");
  Serial.println(gyroZ);

// for the rotating part
  static float velX = 0, velY = 0, velZ = 0;
  static float posX = 0, posY = 0, posZ = 0;

  float rollRad = roll * PI / 180.0;
  float pitchRad = pitch * PI / 180.0;

// gravity-compensated acceleration - compute roll and pitch
  float accWorldX = accX * cos(pitchRad) + accZ * sin(pitchRad);
  float accWorldY = accY * cos(rollRad) - accZ * sin(rollRad);
  float accWorldZ = accZ * cos(rollRad) * cos(pitchRad) - 1.0; // remove 1g

  velX += accWorldX * 9.81 * dt;
  velY += accWorldY * 9.81 * dt;
  velZ += accWorldZ * 9.81 * dt;

  posX += velX * dt;
  posY += velY * dt;
  posZ += velZ * dt;

  float altError = altitude - posZ;
  posZ += 0.05 * altError;
  velZ += 0.01 * altError;

// give me da output :D
  Serial.print("Alt(m): ");
  Serial.print(altitude, 2);
  Serial.print(" | Roll: ");
  Serial.print(roll, 2);
  Serial.print(" | Pitch: ");
  Serial.println(pitch, 2);
}
