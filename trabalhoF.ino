#include "I2Cdev.h"
#include "MPU6050.h"
#include "math.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

MPU6050 accelgyro;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high

int16_t ax, ay, az;
int16_t gx, gy, gz;

#define OUTPUT_READABLE_ACCELGYRO


#define LED_PIN 13
bool blinkState = false;

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    Serial.begin(38400);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();


    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");


    // configure Arduino LED pin for output
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    float ax1,ay1,az1;
    float mediax,mediay,mediaz;
    float AccXangle;
    AccXangle = 0;
    mediax = 0;
    mediay = 0;
    mediaz = 0;
    ax1 = ax;
    ay1 = ay;
    az1 = az;

    /*for(int n=0;n<100;n++){
      accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
      mediax = mediax + ax;
      mediay = mediay + ay;
      mediaz = mediaz + az;
    }
      mediax = mediax/100;
      mediay = mediay/100;
      mediaz = mediaz/100;*/

     AccXangle = (atan2(ax, sqrt(pow(ay,2) + pow(az,2)))*180) / 3.14;
    // these methods (and a few others) are also available
    //accelgyro.getAcceleration(&ax, &ay, &az);
    //accelgyro.getRotation(&gx, &gy, &gz);

    #ifdef OUTPUT_READABLE_ACCELGYRO
        // display tab-separated accel/gyro x/y/z values
        Serial.print("a/g:\t");
        Serial.print((ax1/16384)*9.8); Serial.print("\t");
        Serial.print((ay1/16384)*9.8); Serial.print("\t");
        Serial.print((az1/16384)*9.8); Serial.print("\t");
        Serial.print((mediax/16384)*9.8); Serial.print("\t");
        Serial.print((mediay/16384)*9.8); Serial.print("\t");
        Serial.print((mediaz/16384)*9.8); Serial.print("\t");
        Serial.println(AccXangle); Serial.print("\t");
       // Serial.print(gx); Serial.print("\t");
       // Serial.print(gy); Serial.print("\t");
       // Serial.println(gz);
    #endif

    #ifdef OUTPUT_BINARY_ACCELGYRO
        Serial.write((uint8_t)(ax >> 8)); Serial.write((uint8_t)(ax & 0xFF));
        Serial.write((uint8_t)(ay >> 8)); Serial.write((uint8_t)(ay & 0xFF));
        Serial.write((uint8_t)(az >> 8)); Serial.write((uint8_t)(az & 0xFF));
      //  Serial.write((uint8_t)(gx >> 8)); Serial.write((uint8_t)(gx & 0xFF));
      //  Serial.write((uint8_t)(gy >> 8)); Serial.write((uint8_t)(gy & 0xFF));
      //  Serial.write((uint8_t)(gz >> 8)); Serial.write((uint8_t)(gz & 0xFF));
    #endif
}
