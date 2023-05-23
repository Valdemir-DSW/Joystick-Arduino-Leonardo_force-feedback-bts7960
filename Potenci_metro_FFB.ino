
#include "BTS7960.h"

#include "Joystick.h"

int valorPotenciometro = analogRead(A4);

//X-axis & Y-axis REQUIRED
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, 
  JOYSTICK_TYPE_MULTI_AXIS, 14, 0,
  true, false, false, //X,Y,Z
  true, true, true,//Rx,Ry,Rz
  false, false, false, false, false);
int valorMapeado = map(valorPotenciometro, 0, 1023, 14, 100);


  int gain = valorMapeado;





  //Aqui você regula os efeitos dp FFB
  int damper = 40;
  int inertia = 50;
  int friction = 44;
  int spring = 14;
  int constant = 65;
//Aqui você calibra os potenciômetros dos pedais Entre valor mínimo e máximo você pode usar entre 0 até 1023 
  int min_acel = 10;
  int max_acel = 1000;

  int min_frei = 10;
  int max_frei = 1000;

  int min_enbre = 10;
  int max_enbre = 1000;

  int min_hb = 10;
  int max_hb = 1000;
  



  

  int pwm_f = 30;

  int antonio = pwm_f * 1000;



const uint8_t EN = 8;
const uint8_t L_PWM = 9;
const uint8_t R_PWM = 10;

BTS7960 motorffb(EN, L_PWM, R_PWM);

Gains mygains[2];
EffectParams myeffectparams[2];
int32_t forces[2] = {0};
int address = 0; 



void setup(){
 TCCR1B &= ~(_BV(CS12) | _BV(CS11) | _BV(CS10)); // Limpar os bits de prescaler existentes
 TCCR1B |= _BV(CS10); // Configurar o prescaler para 1


  pinMode(13, OUTPUT);

 

  Joystick.setRzAxisRange(min_acel, max_acel);
  Joystick.setRxAxisRange (min_frei, max_frei);
  Joystick.setXAxisRange (0, 1023);
  Joystick.setRyAxisRange(min_enbre, max_enbre);
  
 
    Serial.begin(115200);
    
   
    
    //Steering wheel
    //Joystick.setXAxisRange(-512, 512);
    //set X Axis gains
    mygains[0].totalGain = gain;//0-100
    mygains[0].springGain = spring;//0-100
    mygains[0].constantGain = constant;//0-100
    mygains[0].frictionGain = friction;//0-100
    mygains[0].inertiaGain = inertia;//0-100
    mygains[0].damperGain = damper;//0-100
    
    //enable gains REQUIRED
    Joystick.setGains(mygains);
    Joystick.begin();

    // Inicialização do encoder

}
void loop() {
  // Your code here
  // ...

  motorffb.Enable();

  Joystick.setRyAxis(analogRead(A3));
  Joystick.setRxAxis(analogRead(A2));
  Joystick.setRzAxis(analogRead(A1));
  Joystick.setXAxis(analogRead(A0));

  //int value = (Position,pulsosVolta*(-1.4),pulsosVolta*1.4,0,pulsosVolta*2.8);
  //set X Axis Spring Effect Param
  //myeffectparams[0].springMaxPosition = 512;
  //myeffectparams[0].springPosition = value;//0-1023

  //Steering wheel
  //myeffectparams[0].springMaxPosition = 512;
  //myeffectparams[0].springPosition = value - 512; //-512-512

  //Send HID data to PC

  //Recv HID-PID data from PC and calculate forces
  //Steering wheel

  Joystick.setEffectParams(myeffectparams);
  Joystick.getForce(forces);
  if (forces[0] > 0) {
    motorffb.TurnRight(abs(forces[0]));
  } else {
    motorffb.TurnLeft(abs(forces[0]));
  }
}
