#include "MPU6050_6Axis_MotionApps20.h"

uint8_t MPU6050::dmpInitialize() {
    // reset device
    reset();
    delay(30); // wait after reset

    // disable sleep mode
    setSleepEnabled(false);

    // get MPU hardware revision
    setMemoryBank(0x10, true, true);
    setMemoryStartAddress(0x06);
    setMemoryBank(0, false, false);

    // get X/Y/Z gyro offsets
    int8_t xgOffsetTC = getXGyroOffsetTC();
    int8_t ygOffsetTC = getYGyroOffsetTC();
    int8_t zgOffsetTC = getZGyroOffsetTC();

    // setup weird slave stuff (?)
    setSlaveAddress(0, 0x7F);
    setI2CMasterModeEnabled(false);
    setSlaveAddress(0, 0x68);
    resetI2CMaster();
    delay(20);

    // load DMP code into memory banks
    if (writeProgMemoryBlock(dmpMemory, MPU6050_DMP_CODE_SIZE)) {
      
        // write DMP configuration

        if (writeProgDMPConfigurationSet(dmpConfig, MPU6050_DMP_CONFIG_SIZE)) {
            setClockSource(MPU6050_CLOCK_PLL_ZGYRO);

            //Setting DMP and FIFO_OFLOW interrupts enabled...
            setIntEnabled(0x12);

            //Setting sample rate to 200Hz
            setRate(4); // 1khz / (1 + 4) = 200 Hz

            setExternalFrameSync(MPU6050_EXT_SYNC_TEMP_OUT_L);

            //Setting DLPF bandwidth to 42Hz
            setDLPFMode(MPU6050_DLPF_BW_42);

            //Setting gyro sensitivity to +/- 2000 deg/sec...
            setFullScaleGyroRange(MPU6050_GYRO_FS_2000);

            //Setting DMP configuration bytes (function unknown)...
            setDMPConfig1(0x03);
            setDMPConfig2(0x00);

            //Clearing OTP Bank flag...
            setOTPBankValid(false);

            //Setting X/Y/Z gyro offset TCs to previous values...
            setXGyroOffsetTC(xgOffsetTC);
            setYGyroOffsetTC(ygOffsetTC);
            setZGyroOffsetTC(zgOffsetTC);

            //Writing final memory update 1/7 (function unknown)...
            uint8_t dmpUpdate[16], j;
            uint16_t pos = 0;
            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);

            //Writing final memory update 2/7 (function unknown)...
            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);

            resetFIFO();

            uint16_t fifoCount = getFIFOCount();
            uint8_t fifoBuffer[128];

            getFIFOBytes(fifoBuffer, fifoCount);

            setMotionDetectionThreshold(2);
            setZeroMotionDetectionThreshold(156);
            setMotionDetectionDuration(80);
            setZeroMotionDetectionDuration(0);

            resetFIFO();

            setFIFOEnabled(true);
            
            setDMPEnabled(true);

            resetDMP();

            //Writing final memory update 3/7 (function unknown)...
            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);

            //Writing final memory update 4/7 (function unknown)...
            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);

            //Writing final memory update 5/7 (function unknown)...
            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);

            //Waiting for FIFO count > 2...
            while ((fifoCount = getFIFOCount()) < 3);

            getFIFOBytes(fifoBuffer, fifoCount);

            //Reading final memory update 6/7 (function unknown)...
            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            readMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);

            while ((fifoCount = getFIFOCount()) < 3);

            getFIFOBytes(fifoBuffer, fifoCount);

            //Writing final memory update 7/7 (function unknown)...
            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);

            //DMP is good to go! Finally.

            //Disabling DMP (you turn it on later)...
            
            setDMPEnabled(false);

            //Setting up internal 42-byte (default) DMP packet buffer...
            dmpPacketSize = 42;

            //Resetting FIFO and clearing INT status one last time...
            resetFIFO();
            getIntStatus();
            
        } else {
            //ERROR! DMP configuration verification failed.
            return 2; // configuration block loading failed
        }
    } else {
        //ERROR! DMP code verification failed.
        return 1; // main binary block loading failed
    }
    return 0; // success
}

bool MPU6050::dmpPacketAvailable() {
    return getFIFOCount() >= dmpGetFIFOPacketSize();
}

uint8_t MPU6050::dmpGetAccel(int32_t *data, const uint8_t* packet) {
    if (packet == 0) packet = dmpPacketBuffer;
    data[0] = (((uint32_t)packet[28] << 24) | ((uint32_t)packet[29] << 16) | ((uint32_t)packet[30] << 8) | packet[31]);
    data[1] = (((uint32_t)packet[32] << 24) | ((uint32_t)packet[33] << 16) | ((uint32_t)packet[34] << 8) | packet[35]);
    data[2] = (((uint32_t)packet[36] << 24) | ((uint32_t)packet[37] << 16) | ((uint32_t)packet[38] << 8) | packet[39]);
    return 0;
}

uint8_t MPU6050::dmpGetAccel(int16_t *data, const uint8_t* packet) {
    if (packet == 0) packet = dmpPacketBuffer;
    data[0] = (packet[28] << 8) | packet[29];
    data[1] = (packet[32] << 8) | packet[33];
    data[2] = (packet[36] << 8) | packet[37];
    return 0;
}

uint8_t MPU6050::dmpGetAccel(VectorInt16 *v, const uint8_t* packet) {
    if (packet == 0) packet = dmpPacketBuffer;
    v -> x = (packet[28] << 8) | packet[29];
    v -> y = (packet[32] << 8) | packet[33];
    v -> z = (packet[36] << 8) | packet[37];
    return 0;
}

uint8_t MPU6050::dmpGetQuaternion(int32_t *data, const uint8_t* packet) {
    if (packet == 0) packet = dmpPacketBuffer;
    data[0] = (((uint32_t)packet[0] << 24) | ((uint32_t)packet[1] << 16) | ((uint32_t)packet[2] << 8) | packet[3]);
    data[1] = (((uint32_t)packet[4] << 24) | ((uint32_t)packet[5] << 16) | ((uint32_t)packet[6] << 8) | packet[7]);
    data[2] = (((uint32_t)packet[8] << 24) | ((uint32_t)packet[9] << 16) | ((uint32_t)packet[10] << 8) | packet[11]);
    data[3] = (((uint32_t)packet[12] << 24) | ((uint32_t)packet[13] << 16) | ((uint32_t)packet[14] << 8) | packet[15]);
    return 0;
}

uint8_t MPU6050::dmpGetQuaternion(int16_t *data, const uint8_t* packet) {
    if (packet == 0) packet = dmpPacketBuffer;
    data[0] = ((packet[0] << 8) | packet[1]);
    data[1] = ((packet[4] << 8) | packet[5]);
    data[2] = ((packet[8] << 8) | packet[9]);
    data[3] = ((packet[12] << 8) | packet[13]);
    return 0;
}

uint8_t MPU6050::dmpGetQuaternion(Quaternion *q, const uint8_t* packet) {
    int16_t qI[4];
    uint8_t status = dmpGetQuaternion(qI, packet);
    if (status == 0) {
        q -> w = (float)qI[0] / 16384.0f;
        q -> x = (float)qI[1] / 16384.0f;
        q -> y = (float)qI[2] / 16384.0f;
        q -> z = (float)qI[3] / 16384.0f;
        return 0;
    }
    return status; // int16 return value, indicates error if this line is reached
}

uint8_t MPU6050::dmpGetGyro(int32_t *data, const uint8_t* packet) {
    if (packet == 0) packet = dmpPacketBuffer;
    data[0] = (((uint32_t)packet[16] << 24) | ((uint32_t)packet[17] << 16) | ((uint32_t)packet[18] << 8) | packet[19]);
    data[1] = (((uint32_t)packet[20] << 24) | ((uint32_t)packet[21] << 16) | ((uint32_t)packet[22] << 8) | packet[23]);
    data[2] = (((uint32_t)packet[24] << 24) | ((uint32_t)packet[25] << 16) | ((uint32_t)packet[26] << 8) | packet[27]);
    return 0;
}

uint8_t MPU6050::dmpGetGyro(int16_t *data, const uint8_t* packet) {
    if (packet == 0) packet = dmpPacketBuffer;
    data[0] = (packet[16] << 8) | packet[17];
    data[1] = (packet[20] << 8) | packet[21];
    data[2] = (packet[24] << 8) | packet[25];
    return 0;
}

uint8_t MPU6050::dmpGetGyro(VectorInt16 *v, const uint8_t* packet) {
    if (packet == 0) packet = dmpPacketBuffer;
    v -> x = (packet[16] << 8) | packet[17];
    v -> y = (packet[20] << 8) | packet[21];
    v -> z = (packet[24] << 8) | packet[25];
    return 0;
}

uint8_t MPU6050::dmpGetLinearAccel(VectorInt16 *v, VectorInt16 *vRaw, VectorFloat *gravity) {
    // get rid of the gravity component (+1g = +8192 in standard DMP FIFO packet, sensitivity is 2g)
    v -> x = vRaw -> x - gravity -> x*8192;
    v -> y = vRaw -> y - gravity -> y*8192;
    v -> z = vRaw -> z - gravity -> z*8192;
    return 0;
}

uint8_t MPU6050::dmpGetLinearAccelInWorld(VectorInt16 *v, VectorInt16 *vReal, Quaternion *q) {
    // rotate measured 3D acceleration vector into original state
    // frame of reference based on orientation quaternion
    memcpy(v, vReal, sizeof(VectorInt16));
    v -> rotate(q);
    return 0;
}

uint8_t MPU6050::dmpGetGravity(VectorFloat *v, Quaternion *q) {
    v -> x = 2 * (q -> x*q -> z - q -> w*q -> y);
    v -> y = 2 * (q -> w*q -> x + q -> y*q -> z);
    v -> z = q -> w*q -> w - q -> x*q -> x - q -> y*q -> y + q -> z*q -> z;
    return 0;
}

uint8_t MPU6050::dmpGetEuler(float *data, Quaternion *q) {
    data[0] = atan2(2*q -> x*q -> y - 2*q -> w*q -> z, 2*q -> w*q -> w + 2*q -> x*q -> x - 1);   // psi
    data[1] = -asin(2*q -> x*q -> z + 2*q -> w*q -> y);                              // theta
    data[2] = atan2(2*q -> y*q -> z - 2*q -> w*q -> x, 2*q -> w*q -> w + 2*q -> z*q -> z - 1);   // phi
    return 0;
}

uint8_t MPU6050::dmpGetYawPitchRoll(float *data, Quaternion *q, VectorFloat *gravity) {
    
    /* JEFF ROSENBERG WORK
    // yaw: (about Z axis)
    data[0] = atan2(2*q -> x*q -> y - 2*q -> w*q -> z, 2*q -> w*q -> w + 2*q -> x*q -> x - 1);
    // pitch: (nose up/down, about Y axis)
    data[1] = atan(gravity -> x / sqrt(gravity -> y*gravity -> y + gravity -> z*gravity -> z));
    // roll: (tilt left/right, about X axis)
    data[2] = atan(gravity -> y / sqrt(gravity -> x*gravity -> x + gravity -> z*gravity -> z));
    */

    /* yonoodle correction
    https://github.com/jrowberg/i2cdevlib/issues/222
     */

    // yaw: (about Z axis)
    data[0] = atan2(2*q -> x*q -> y - 2*q -> w*q -> z, 2*q -> w*q -> w + 2*q -> x*q -> x - 1);
    //pitch:
    data[1] = atan2(gravity -> x , sqrt(gravity -> y*gravity -> y + gravity -> z*gravity -> z));
    //roll:
    data[2] = atan2(gravity -> y , gravity -> z);
    
    if(gravity->z<0) {
      if(data[1]>0) {
        data[1] = 3.1415926 - data[1] ;
      } else {
        data[1] = -3.1415926 - data[1] ;
      }
    }
     
    return 0;
}

uint8_t MPU6050::dmpProcessFIFOPacket(const unsigned char *dmpData) {
    return 0;
}

uint8_t MPU6050::dmpReadAndProcessFIFOPacket(uint8_t numPackets, uint8_t *processed) {
    uint8_t status;
    uint8_t buf[dmpPacketSize];
    for (uint8_t i = 0; i < numPackets; i++) {
        // read packet from FIFO
        getFIFOBytes(buf, dmpPacketSize);

        // process packet
        if ((status = dmpProcessFIFOPacket(buf)) > 0) return status;
        
        // increment external process count variable, if supplied
        if (processed != 0) (*processed)++;
    }
    return 0;
}

uint16_t MPU6050::dmpGetFIFOPacketSize() {
    return dmpPacketSize;
}
