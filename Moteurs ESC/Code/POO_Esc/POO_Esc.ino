#include "Esc.h"

Esc esc_1(TIMER_1_A); // moteur gauche
//Esc esc_2(TIMER_1_B); // moteur droit

float throttle;

void setup() {
  Serial.begin(115200);

    Serial.println("Entrez un caractere pour demarrer la PWM");
    Serial.println("(/!\\ 100% !!)");
    Serial.println("Verifier que les moteurs sont branches et que les -bips- sont espaces de 2 secs");
  while (!Serial.available()) {
  ;
  }
  Serial.read();
  Serial.println("Calibrage ESC ...");
  esc_1.setup();
  esc_1.setThrottle(100);

  //esc_2.setup();
  //esc_2.setThrottle(100);

  Serial.println("Entrez un caractere pour lancer la calibration");
  while (!Serial.available()) {
  ;
  }
  Serial.read();

  Serial.println("Calibrage ESC ...");
  
  esc_1.startCalib();
  while(!esc_1.isCalib())
  {
    esc_1.calib();
  }

  /*
  esc_2.startCalib();
  while(!esc_2.isCalib())
  {
    esc_2.calib();
  }
   */
  
  Serial.println("Calibrage effectue !");
  Serial.println("...");

  Serial.println("Entrez le pourcentage du ratio PWM");
  while (!Serial) {
  ;
  }
}

void loop() {
  if(Serial.available()){
    throttle = Serial.parseFloat(); // Lecture données Séries
    esc_1.setThrottle(throttle);
    //esc_2.setThrottle(throttle);
    Serial.print("PWM ratio set to ");
    Serial.println(esc_1.getLastRegVal());
    //Serial.println(esc_2.getLastRegVal());
  }
}
