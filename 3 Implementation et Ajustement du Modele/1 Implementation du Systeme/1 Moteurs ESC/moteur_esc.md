# Programmation de l'objet Esc

## Pourquoi un tel objet

Pour les mêmes raisons qui m'ont poussé à programmer un objet Gyro, encapsulant les parties techniques du code (modification des registres du processeur Atmel pour génerer le signal PWM approrié), j'ai décidé de créer une classe avec une interface d'utilisation claire et lisible facilement.

De plus, la classe Esc permet d'utiliser les deux timers de la carte arduino : les modifications à effectuer pour obtenir le signal adéquat changent en fonction du timer choisi. Ainsi, dans le paradygme de la programmation orienté objet, on a également permis une utilisation polymorphique de la classe : le comportement de l'objet s'adapte en fonction du timer de départ choisi et génère de façon transparente le signal PWM adéquat.


## Ses méthodes :

``` cpp
//constantes de classe
const int TIMER_1_A = 0; //pin 9
const int TIMER_1_B = 1; //pin 10
const int TIMER_2_A = 2; //pin 11
const int TIMER_2_B = 3; //pin 3

const float ESC_CALIB_DURATION = 1000;

//class Esc
//class representant un variateur de vitesse ESC (abstraction du composant)
//Genere la commande pwm approprie pour le pilotage des moteurs
class Esc
{
public:

    //constructor
    //param aTimer : Timer generant le signal PWM (voir constantes ci-dessus)
    //(TIMER_N_X)
    Esc(int aTimer);

    //Setup de l'ESC
    //Method setting the proper values in the registers
    //of the Atmel processor according to the Timer chosen.
    //Sets the range of throttle
    void setup();

    //Setting of the throttle
    //Modifies the Output Compare Register to ajust the
    //PWM ratio
    //param throttle : throttle in percent to send to the motor [0.0;100.0]
    void setThrottle(double throttle);

    //Getter of the last value sent to the OCR register
    //(not so useful)
    int getLastRegVal() const;

    //Start the Calibration process
    void startCalib();

    //brief Management of the calibration
    void calib();

    //Check the calibration state
    //return true if calibrated
    //else false
    bool isCalib() const;
 ```