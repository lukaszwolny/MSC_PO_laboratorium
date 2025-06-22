#include "PetlaJedenKrok.h"

PetlaJedenKrok::PetlaJedenKrok() : wyjsciePoprzednie(0.0) {}

double PetlaJedenKrok::operator()(RegulatorPID& regulator, Komponent& model, double wartoscZadana) {
    //double zadana = 1.0;

    //krok 1
    double uchyb = wartoscZadana - wyjsciePoprzednie;

    //krok 2 regulator
    double sterowanie = regulator.symuluj(uchyb);

    //krok 3 modelARX
    double wyjscie = model.symuluj(sterowanie);

    wyjsciePoprzednie = wyjscie;

    //wartosc regulowana
    return wyjscie;
}