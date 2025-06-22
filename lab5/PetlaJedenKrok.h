#pragma once
#include "RegulatorPID.h"
#include "ModelARX.h"

/**
* @class PetlaJedenKrok
* @brief Klasa reprezentujaca pojedynczy krok symulacji ukladu regulacji.
*
* Oblicza uchyb, przetwarza go przez regulator, a nastepnie symuluje odpowiedz modelu.
*/
class PetlaJedenKrok {
private:
    double wyjsciePoprzednie;/// Poprzednie wyjscie modelu (do obliczenia uchybu)

public:
    /**
    * @brief Konstruktor domyslny. Inicjalizuje wyjscie poprzednie na 0.0.
    */
    PetlaJedenKrok();

    /**
    * @brief Funktor realizujacy jeden krok petli regulacji.
    * @param regulator Obiekt regulatora PID.
    * @param model Obiekt modelu ARX.
    * @param wartoscZadana Aktualna wartosc zadana.
    * @return Wartość regulowana.
    */
    double operator()(RegulatorPID& regulator, Komponent& model, double wartoscZadana); //ModelARX& model
};