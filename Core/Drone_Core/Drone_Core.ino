#include "Esc.h"
#include "Gyro.h"

// PINS BRANCHEMENT :
// Moteur gauche : pin 9
// Moteur droite : pin 10

// Gyro SCL      : pin A5
// Gyro SDA      : pin A4
// Gyro INT      : pin 2

/* ***************** */
// VARIABLES GENERALES

uint8_t state;

/* ***************** */
// VARIABLES COMMANDE
float throttleAmp = 0, throttleConst = 0, consigne = 0;
float Kp=1,Ki=0,Kd=0.2;
float eps1, eps0, epsc, eps_int;

/* ***************** */
// VARIABLES GYRO

Gyro gyro;

void dmpDataReady(){
  gyro.setInterrupt();
}

int initStatus;
float roll;

/* ***************** */
// VARIABLES MOTEURS

Esc leftEsc(TIMER_1_A); // pin 9
Esc rightEsc(TIMER_1_B);// pin 10

float throttleLeft, throttleRight;
float const CONST_THROTTLE = 0, AMP_THROTTLE = 0;

/* ***************** */
// VARIABLES SERIAL

// processing packet (write)

const int processingPacketLength = 13;
//                                             { start,state,actual_time,thLeft,thRight,roll,end }
uint8_t processingPacket[processingPacketLength]={  '$' ,  0  ,  0,0,0,0  , 0,0  ,  0,0  ,0,0 ,'&' };
bool packetToWrite = false;

// arduino packet (read)

const int arduinoPacketLength = 13;

//      { start,state,thConst,thAmp,end }
uint8_t arduinoPacket[arduinoPacketLength];
int serialCount = 0;                 // current packet byte position
int synced = 0;
boolean packetToTreat = false;

// VARIABLES TIME

// unit : µs
unsigned long actual_time=0;
unsigned long last_time=0;
float delta_t;

/* ***************** */
/* ***************** */

void setup() {
  // initialisation du port Série (communication Processing)
  Serial.begin(115200);
  Serial.write(processingPacket,processingPacketLength);
  // initialisation du gyroscope et retour sur son état
  gyro.initialize(dmpDataReady);
  initStatus = gyro.getInitStatus();

  // initialisation des deux ESC (pwm)
  leftEsc.setup();
  rightEsc.setup();
  
  leftEsc.setThrottle(0);
  rightEsc.setThrottle(0);

  if(initStatus == 0) {
  // si le gyro a bien été initialisé:
  // etat non calibré
  state = 3;
  } else {
  // arrêt d'urgence : empêchement du lancement du programme
    state = 0;
  }

  // ENVOI SERIE FIN SETUP
  packetToWrite = true;
  
}

// DIFFERENTS ETATS DU SYSTEME :
// 0: ARRET D'URGENCE (touche E clavier ou interface graphique)
// 1: ARRET (systeme calibre)
// 2: MARCHE
// 3: ARRET (systeme non calibre)

void loop() {
  if(state!=0) {
    // BOUCLE INTEGRALE HORS ARRÊT D'URGENCE
    // TODO
    // ***

    // RECEPTION DONNEES VERS ARDUINO
    while (Serial.available() > 0) {
      uint8_t ch = Serial.read();

      if (synced == 0 && ch != '$') {
        // initial synchronization - also used to resync/realign if needed
      } else {
        synced = 1;
        
        if ((serialCount == arduinoPacketLength-1 && ch != '&'))  {
          serialCount = 0;
          synced = 0;
        }

        if (serialCount > 0 || ch == '$') {
          arduinoPacket[serialCount] = ch;
          serialCount++;
          if (serialCount == arduinoPacketLength) {
            serialCount = 0; // restart packet byte position
            packetToTreat = true;
          }
        }
      }
    }

    // TRAITEMENT DES DONNEES ARDUINO
    if (packetToTreat) {
      state = arduinoPacket[1];
      bytesToFloat(arduinoPacket[2],arduinoPacket[3],throttleConst);
      bytesToFloat(arduinoPacket[4],arduinoPacket[5],throttleAmp);
      bytesToFloat(arduinoPacket[6],arduinoPacket[7],Kp);
      bytesToFloat(arduinoPacket[8],arduinoPacket[9],Ki);
      bytesToFloat(arduinoPacket[10],arduinoPacket[11],Kd);
      packetToTreat = false;
    }

    

    // LECTURE DONNEES GYROSCOPE
    gyro.operateGyro();
    if(gyro.hasNewData()){
      roll = gyro.getRoll();
      packetToWrite = true;
      gyro.setNoDataReady();
    }

    // GESTION DU TEMPS
    if(packetToWrite){
      last_time = actual_time;
      actual_time = micros();
      delta_t = (actual_time-last_time)*0.000001;
    }
    
    // ETAT OFF
    if(state == 1) {
      throttleLeft = 0;
      throttleRight = 0;
      leftEsc.setThrottle(throttleLeft);
      rightEsc.setThrottle(throttleRight);  
    }

    // ETAT ON
    if(state == 2 && packetToWrite) {
      
      // détermination de la consigne
      eps0 = eps1;
      eps1 = (roll - consigne)/90*throttleAmp;
      epsc = eps1*Kp;
      epsc += (eps1-eps0)/delta_t*Kd;
      eps_int += (eps1+eps0)/2*delta_t;
      epsc += eps_int*Ki;
      if(abs(epsc)>throttleAmp) epsc = (epsc/abs(epsc))*throttleAmp;
      
      throttleLeft = throttleConst + epsc;
      throttleRight = throttleConst - epsc;
      
      // pilotage des moteurs
      leftEsc.setThrottle(throttleLeft);
      rightEsc.setThrottle(throttleRight);
    }

    // ETAT NON CALIBRE
    if(state == 3) {
      throttleLeft, throttleRight = 0;
      leftEsc.setThrottle(throttleLeft);
      rightEsc.setThrottle(throttleRight);
    }

    // ENVOI DES DONNEES VERS PROCESSING
    
        if(packetToWrite){
    // prepare the packet
    processingPacket[1] = state;
    intToBytes(actual_time,processingPacket[2],processingPacket[3],processingPacket[4],processingPacket[5]);
    floatToBytes(throttleLeft,processingPacket[6],processingPacket[7]);
    floatToBytes(throttleRight,processingPacket[8],processingPacket[9]);
    floatToBytes(roll+180,processingPacket[10],processingPacket[11]);
    //
    
    // send the packet
    Serial.write(processingPacket,processingPacketLength);
    packetToWrite = false;
  }
  

  // ETAT ARRET URGENCE  
  } else {
    leftEsc.setThrottle(0);
    rightEsc.setThrottle(0);
  }
}

/* ***************** */
/* ***************** */

// FONCTIONS SERIAL

// convert a float to the upper and lower bytes
void floatToBytes(float const& fval, uint8_t & byte1, uint8_t & byte2){
  int ival = floor(fval*100);
  byte1 = ival>>8;
  byte2 = ival;
}

// convert the upper byte and the lower byte to the float
void bytesToFloat(uint8_t const& byte1, uint8_t const& byte2, float & fval){
  fval = ((float)(byte1 << 8 | byte2))/100;
}

// convert an 4-byte-int to the 4 bytes
void intToBytes(unsigned long const& ival, uint8_t & byte1, uint8_t & byte2, uint8_t & byte3, uint8_t & byte4){
  byte1 = ival>>24;
  byte2 = ival>>16;
  byte3 = ival>>8;
  byte4 = ival;
}
