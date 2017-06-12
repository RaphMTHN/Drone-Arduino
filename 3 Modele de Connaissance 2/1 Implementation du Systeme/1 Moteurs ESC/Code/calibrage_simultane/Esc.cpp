#include "Esc.h"

Esc::Esc(int aTimer)
{
    timer = aTimer;
}
void Esc::setup()
{
    // CAS TIMER_1

    if(timer == TIMER_1_A || timer == TIMER_1_B)
    {
        if(timer == TIMER_1_A)
        {
            pinMode(9,OUTPUT);
        }

        if(timer == TIMER_1_B)
        {
            pinMode(10,OUTPUT);
        }
        // TCCR1A : TIMER_1_CONTROL_REGISTER_A
        // bits 7:6 : Compare Output Mode for Channel A (Set output to low level).
        // bits 5:4 : Compare Output Mode for Channel B (Set output to low level).
        // bits 3:2 : r�serv�s
        // bits 1:0 :  Waveform Generation Mode : PWM, Phase Correct avec max ICR1 (coupl� avec bits  WGM13:2 de TCCR1B)
        TCCR1A = 0b10100010;

        // TCCR1B : TIMER_1_CONTROL_REGISTER_B
        // bits 7:6 : Noise canceler : not activated
        // bits 5   : Reserved
        // bits 4:3 : WGM13:2 (voir ci dessus)
        // bits 2:0 : Clock - Prescaler select (cf datasheet ATMEL 328P , p156)
        TCCR1B = 0b00010001;

        // ICR1 : INPUT_CAPTURE_REGISTER
        // 16 bits value : on choisit 2^14 pour avoir une fr�quence de 488Hz
        ICR1 = 0b0100000000000000;

        // Amplitude OCR (bits)
        //OCR_Max = 16383;
        OCR_Max = 16000;
        OCR_Min = 8192;
    }

    // CAS TIMER_2

    if(timer == TIMER_2_A || timer == TIMER_2_B)
    {
        if(timer == TIMER_2_A)
        {
            pinMode(11,OUTPUT);
        }

        if(timer == TIMER_2_B)
        {
            pinMode(3,OUTPUT);
        }
        // TCCR2A : TIMER_2_CONTROL_REGISTER_A
        // bits 7:6 : Compare Output Mode for Channel A (Set output to low level).
        // bits 5:4 : Compare Output Mode for Channel B (Normal port operation, OC2B disconnected.)
        // bits 3:2 : r�serv�s
        // bits 1:0 :  Waveform Generation Mode : PWM, Phase Correct avec max ICR1 (coupl� avec bits  WGM13:2 de TCCR1B)
        TCCR2A = 0b10100001;

        // TCCR2B : TIMER_2_CONTROL_REGISTER_B
        // bits 7:6 : Force Output Compare
        // bits 5:4   : Reserved
        // bits 3 : WGM22 (voir ci dessus)
        // bits 2:0 : Clock - Prescaler select (cf datasheet ATMEL 328P , p156)
        TCCR2B = 0b00000100;

        // Amplitude OCR (bits)
        OCR_Max = 255;
        OCR_Min = 128;
    }
}

/*
    g�n�re le signal PWM appropri� � la commande de pouss�e
    entr�e : pourcentage de pouss�e � envoyer - v�rification effectu�e
*/
void Esc::setThrottle(double throttle)
{
    // VERIFICATION DU POURCENTAGE DE POUSSEE
    if(throttle <0.0){
    throttle = 0;
    }
    if(throttle >100.0){
    throttle = 100;
    }

    // MODIFICATION DU REGISTRE APPROPRIE POUR GENERER SIGNAL
    lastRegVal = throttle/100*(OCR_Max - OCR_Min) + OCR_Min;

    if(timer == TIMER_1_A)
    {
        OCR1A = lastRegVal;
    }
    if(timer == TIMER_1_B)
    {
        OCR1B = lastRegVal;
    }
    if(timer == TIMER_2_A)
    {
        OCR2A = lastRegVal;
    }
    if(timer == TIMER_2_B)
    {
        OCR2B = lastRegVal;
    }
}

int Esc::getLastRegVal() const
{
    return lastRegVal;
}

void Esc::startCalib()
{
    timeStartCalib = millis();
    setThrottle(100);
}

void Esc::calib()
{
    int actualTime = millis();
    if( (actualTime - timeStartCalib) > ESC_CALIB_DURATION )
    {
        calibState = true;
    }
    else
    {
        float throttle = (1-(millis()-timeStartCalib)/ESC_CALIB_DURATION) * 100;
        setThrottle(throttle);
    }
}

bool Esc::isCalib() const
{
    return calibState;
}
