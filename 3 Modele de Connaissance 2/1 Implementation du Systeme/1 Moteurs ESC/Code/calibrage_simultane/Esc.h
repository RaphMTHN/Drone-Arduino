#ifndef ESC_H
#define ESC_H
#include <Arduino.h>

/**
 * \file Esc.h
 * \brief ESC controller (air 20 A opto tmotor)
 * \author raphael mathon
 * \version 0.1
 */

const int TIMER_1_A = 0; //pin 9
const int TIMER_1_B = 1; //pin 10
const int TIMER_2_A = 2; //pin 11
const int TIMER_2_B = 3; //pin 3

const float ESC_CALIB_DURATION = 1000;


 /** \class Esc
   * \brief class representing the Esc
   *
   *  Handle the PWM command for the Esc
   */
class Esc
{
public:

    /**
     *  \brief Constructor
     *
     *  \param aTimer : Timer generating the PWM signal
     *  (TIMER_N_X)
     */
    Esc(int aTimer);

    /**
     *  \brief Setup of the ESC
     *
     *  Method setting the proper values in the registers
     *  of the Atmel processor according to the Timer chosen.
     *  Sets the range of throttle
     */
    void setup();

    /**
     *  \brief Setting of the throttle
     *
     *  Modifies the Output Compare Register to ajust the
     *  PWM ratio
     *
     *  \param throttle : throttle in percent to send to the motor
     */
    void setThrottle(double throttle);

    /**
     *  \brief Getter of the last value sent to the OCR register
     *  (not so useful)
     *  \return lastRegVal
     */
    int getLastRegVal() const;

    /**
     *  \brief Start the Calibration process
     */
    void startCalib();

    /**
     *  \brief Management of the calibration
     */
    void calib();

    /**
     *  \brief Check the calibration state
     *  \return true if calibrated
     *  else false
     */
    bool isCalib() const;

private:
    int timer;
    int lastRegVal;
    int OCR_Max;
    int OCR_Min;
    int timeStartCalib;
    bool calibState = false;

};

#endif // ESC_H
