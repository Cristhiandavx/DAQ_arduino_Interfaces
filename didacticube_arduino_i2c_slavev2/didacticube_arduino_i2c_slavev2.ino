#include <Wire.h> //i2c arduino library
#include <FaBo9Axis_MPU9250.h> //IMU libary
#include <MadgwickAHRS.h>
#include <Adafruit_INA219.h>
#include "pitches.h" //file must be in the same project folder
/*
  ######Electromechanical Engineering Graduation project######
  #################The Nihon Gakko University#################
  ##########################Paraguay##########################
  ############################2021############################
  Sketch: didacticube_arduino_i2c_slavev2
  includes lines from other skcetches and libraries such as:
  -Wire
  -FaBo9Axis_MPU9250
  -MadgwickAHRS
  -Adafruit_INA219

  author: Cristhian D. Coronel A. - cdcorone@nihongakko.edu.py
*/
FaBo9Axis fabo_9axis; //IMU library instance
Madgwick madgwick; //Madgwick AHRS algorithm library instance
Adafruit_INA219 ina219; //INA219 library instance
// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};
void playMelody(){
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(3, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(3);
  }
}
void setup() {
  Serial.begin(9600, SERIAL_8N1); //default Serial conf
  if (!fabo_9axis.begin()) {
    Serial.println("FaBo 9Axis I2C device error"); //IMU begin failed
  }
  if (!ina219.begin()) {
    Serial.println("Failed to find INA219 chip"); //INA begin successfull
  }
  //need to calibrate before use?//
  madgwick.begin(24); //begin AHRS lib with sample frequency in Hz. Default value 512.0f. 24 was the optimal to avoid too much drift
  pinMode(2, OUTPUT);
}
//variable declaration
float ax, ay, az; //imu accelerometer variables
float gx, gy, gz; //imu gyroscope variables
float mx, my, mz; //imu magnetometer variables
float temp; //imu temperature variable
float roll, pitch, yaw; //calculated rotation angles
float current_mA = 0;
float power_mW = 0;
const int voltageSensor1 = A6; // sensor 1 pin
float vIn1 = 0.0; // measured voltage on respective sensor (3.3V = max. 16.5V, 5V = max 25V)
float vOut1 = 0.0;
float R1 = 30000.0;
float R2 = 7500.0;
int voltageSensorVal1; // value on respective sensor (0 - 1023)
//loop start
void loop() {
  //reading
  fabo_9axis.readAccelXYZ(&ax, &ay, &az);
  fabo_9axis.readGyroXYZ(&gx, &gy, &gz);
  fabo_9axis.readMagnetXYZ(&mx, &my, &mz);
  fabo_9axis.readTemperature(&temp);
  //data analisys
  //calculate Roll Pitch and Yaw (rotation in X Y Z respectively)
  madgwick.update(gx, gy, gz, ax, ay, az, mx, my, mz);
  //get calculated angles
  roll = madgwick.getRoll();
  pitch = madgwick.getPitch();
  yaw = madgwick.getYaw();
  //current meassuring
  current_mA = ina219.getCurrent_mA(); //current in mA
  //power meassuring
  power_mW = ina219.getPower_mW(); //power in mW
  //voltage meassuring
  voltageSensorVal1 = analogRead(voltageSensor1); //read voltage sensor values 0 - 1023
  vOut1 = (voltageSensorVal1 * 5.0) / 1024.0;
  vIn1 = vOut1 / (R2/(R1+R2)); //voltage in V on sensor 1
  //print all the data
  char buff[8]; //char array with the size of 8bits for variable data, i.e.: 12.123 [6bits] or -180.123 [8bits]
  //Serial.print(""); //accX
  dtostrf(ax, 8, 3, buff); //float number, array width, decimal places after convertion, buffer variable name
  Serial.print(buff);
  Serial.print(","); //accY
  dtostrf(ay, 8, 3, buff);
  Serial.print(buff);
  Serial.print(","); //accZ
  dtostrf(az, 8, 3, buff);
  Serial.print(buff);
  Serial.print(","); //gyrX
  dtostrf(gx, 8, 3, buff);
  Serial.print(buff);
  Serial.print(","); //gyrY
  dtostrf(gy, 8, 3, buff);
  Serial.print(buff);
  Serial.print(","); //gyrZ
  dtostrf(gz, 8, 3, buff);
  Serial.print(buff);
  Serial.print(","); //magX
  dtostrf(mx, 8, 3, buff);
  Serial.print(buff);
  Serial.print(","); //magY
  dtostrf(my, 8, 3, buff);
  Serial.print(buff);
  Serial.print(","); //magZ
  dtostrf(mz, 8, 3, buff);
  Serial.print(buff);
  Serial.print(","); //temp
  dtostrf(temp, 8, 3, buff);
  Serial.print(buff);
  Serial.print(","); //Roll
  dtostrf(roll, 8, 3, buff);
  Serial.print(buff);
  Serial.print(","); //Pitch
  dtostrf(pitch, 8, 3, buff);
  Serial.print(buff);
  Serial.print(","); //Yaw
  dtostrf(yaw, 8, 3, buff);
  Serial.print(buff);
  Serial.print(","); //current_mA
  dtostrf(current_mA, 8, 3, buff);
  Serial.print(buff);
  Serial.print(","); //power_mW
  dtostrf(power_mW, 8, 3, buff);
  Serial.print(buff);
  Serial.print(","); //vIn1
  dtostrf(vIn1, 8, 3, buff);
  Serial.print(buff);
  Serial.println(""); //print a space and add a new line
  delay(500); //delay in millisecconds 250 was the best for raspberry pi A+ v1.1 to avoid java error while on MyOpenLab
  if (Serial.available() > 0) {
    int thisChar = Serial.read();
    if (thisChar == 65) { //65 = "A"
      playMelody();
      Serial.println("A melody was played");
    }
    if (thisChar == 66) { //66 = "B"
      digitalWrite(2, HIGH);
      Serial.println("LED was turned ON");
    }
    
    if (thisChar == 98) { //98 = "b"
      digitalWrite(2, LOW);
      Serial.println("LED was turned OFF");
    }
  }
}
