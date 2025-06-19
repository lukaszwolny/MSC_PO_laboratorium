#include "RegulatorPID.h"
#include <iostream>

//Konstruktory
RegulatorPID::RegulatorPID(double K_) : pamiecCalkujaca(0.0), pamiecRozniczkujaca(0.0)
{
    setK(K_);
    setTI(0.0); // TI = 0
    setTD(0.0); // TD = 0
}
RegulatorPID::RegulatorPID(double K_, double TI_) : pamiecCalkujaca(0.0), pamiecRozniczkujaca(0.0)
{
    setK(K_);
    setTI(TI_); 
    setTD(0.0);
}
RegulatorPID::RegulatorPID(double K_, double TI_, double TD_) : pamiecCalkujaca(0.0), pamiecRozniczkujaca(0.0)
{
    setK(K_);
    setTI(TI_); 
    setTD(TD_);
}

void RegulatorPID::setNieujemne(double& nastawa, double wartosc) {
  if (wartosc < 0.0) {
      std::cout<<std::endl<<"Blad!, Nastawa nie moze byc ujemna. (zmiana na 0.0)"<<std::endl;
      nastawa = 0.0;//Ustaw na zero
  }else{
      nastawa = wartosc;
  }
}

void RegulatorPID::setK(double nowaWartosc){
  setNieujemne(K, nowaWartosc);
}
void RegulatorPID::setTI(double nowaWartosc) {
  setNieujemne(TI, nowaWartosc);
}

void RegulatorPID::setTD(double nowaWartosc) {
  setNieujemne(TD, nowaWartosc);
}

double RegulatorPID::symuluj(double e_i) {
  //P
  double P = K * e_i;

  //I
  pamiecCalkujaca += e_i;

  double I = 0.0;
  if(TI <= 0){
    I = 0.0;
  }else{
    I = (1 / TI) * pamiecCalkujaca;
  }
  
  //D
  double D = 0.0;
  D = TD * (e_i - pamiecRozniczkujaca);

  pamiecRozniczkujaca = e_i;

  return P + I + D;
}