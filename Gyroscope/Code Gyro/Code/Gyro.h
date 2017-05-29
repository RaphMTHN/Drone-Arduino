#ifndef GYRO_H
#define GYRO_H

// La bibliothèque I2Cdev est utilisé par l'objet MPU5060 pour réaliser toute les
// communications série entre la carte Arduino et le capteur.
#include "I2Cdev.h"

// La bibliothèque MPU6050_6Axis_MotionApps20 inclus le code de l'objet mpu5060.
// MPU5060 contient des méthodes qui effectuent pour la majorité d'entre-elles uniquement
// des lecture de registres contenant les variables d'état du gyroscope, et d'écriture des
// registres de configuration.

// MPU6050_6Axis_MotionApps20 définit les méthodes propres au paramétrage et à l'acquisition de
// données provenant du DMP (données filtrées et exploitables)
#include "MPU6050_6Axis_MotionApps20.h"

// La bibliothèque Arduino Wire.h est requise si l'implémentation I2Cdev 
// I2CDEV_ARDUINO_WIRE est utilisée dans I2Cdev.h (ce qui est le cas normalement)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

// Pin Arduino utilisé pour les interruptions (signalant l'arrivée de données disponibles dans
// le buffer du gyroscope)
#define INTERRUPT_PIN 2

// ----------------------------------------------------------------------------------------------//

class Gyro{

  // ATTRIBUTS ET METHODES HAUT-NIVEAU
public:
  Gyro();                       // constructeur de classe
  
  void initialize(void(*dmpDataReady)()); // initialise le mpu5060 (param: interrupt récupérant le
  // signal électrique du pin INTERRUPT_PIN - similaire à la capture d'un évenement dans une machine
  // d'états)
  int getInitStatus() const;    // retourne le statut d'initialisation

  float getRoll() const;        // retourne l'angle de roulis
  void operateGyro();           // exécute une routine nécessaire à la réception éventuelle de données
  bool hasNewData() const;      // retourne true si de nouvelles données sont disponibles
  void setNoDataReady();        // déclare que l'utilisateur a bien lu les données disponibles

  void setInterrupt();          // set mpuInterrupt to true (?)

  void setInitialTime();        // réinitialise l'instant initial d'acquisition des données
  float getMeasureTime() const; // retourne l'instant d'acquisition des données

// ----------------------------------------------------------------------------------------------//

  // ATTRIBUTS ET METHODES BAS-NIVEAU
  // originalement dans le code démo de mpu5060, encapsulé dans l'objet gyro
private:
  MPU6050 mpu;

  // initStatus : return status after initialisation
  // 0 = init successful
  // 1 = initial memory load failed
  // 2 = DMP configuration updates failed
  // 3 = I2C connection failed
  int initStatus;

  bool newDataReady = false;
  
  // MPU control/status vars
  bool dmpReady = false;  // set true if DMP init was successful
  bool coReady = false;   // set true if I2C connection was successful
  uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
  uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
  uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
  uint16_t fifoCount;     // count of all bytes currently in FIFO
  uint8_t fifoBuffer[64]; // FIFO storage buffer

  // orientation/motion vars
  Quaternion q;           // [w, x, y, z]         quaternion container
  VectorFloat gravity;    // [x, y, z]            gravity vector
  float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

  // packet structure for InvenSense teapot demo
  //uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };

  // time control vars
  float initialTime;            // set the time origin
  float currentTime;            // current time measurements
  void updateTime();            // update the currentTime value

  // Interrupt detection
  volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
};

#endif // GYRO_H
