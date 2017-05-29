#include "Gyro.h"

Gyro::Gyro(){
  //ctor
}

void Gyro::initialize(void(*dmpDataReady)()){
  
  /* ** I2C CONNEXION ** */

  // join I2C bus (I2Cdev library doesn't do this automatically)
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
      Wire.begin();
      Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
  #endif

  mpu.initialize();
  pinMode(INTERRUPT_PIN, INPUT);

  // verify connection
  coReady = mpu.testConnection();

  /* ** DMP CONFIGURATION ** */
  
  // intinializing DMP
  devStatus = mpu.dmpInitialize();

  // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

  // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
    // turn on the DMP, now that it's ready
    mpu.setDMPEnabled(true);

    // enable Arduino interrupt detection
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();

    // set DMP Ready flag
    dmpReady = true;

    // get expected DMP packet size for later comparison
    packetSize = mpu.dmpGetFIFOPacketSize();
  }

  /* ** INIT STATUS VALUE ** */

  coReady ? initStatus = devStatus : initStatus = 3;

}

int Gyro::getInitStatus() const {
  return(initStatus);
}

float Gyro::getRoll() const {
  return ypr[2]*180/PI;
}

void Gyro::setInterrupt(){
  mpuInterrupt = true;
}

void Gyro::operateGyro(){
  if (dmpReady && (mpuInterrupt || fifoCount >= packetSize)){
        // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
      // wait for correct available data length, should be a VERY short wait
      while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
      
      // read a packet from FIFO
      mpu.getFIFOBytes(fifoBuffer, packetSize);
      
      // track FIFO count here in case there is > 1 packet available
      // (this lets us immediately read more without waiting for an interrupt)
      fifoCount -= packetSize;

      // Update quaternion, ypr and teapot
      mpu.dmpGetQuaternion(&q, fifoBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
      updateTime();
      newDataReady = true;     
    }

  }
}

bool Gyro::hasNewData() const {
  return newDataReady;
}

void Gyro::setNoDataReady() {
  newDataReady = false;
}

void Gyro::setInitialTime(){
  initialTime = millis();
}

void Gyro::updateTime(){
  currentTime = millis() - initialTime;
}

void Gyro::getMeasureTime(){
  return currentTime;
}