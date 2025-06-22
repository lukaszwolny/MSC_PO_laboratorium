#pragma once
#include "SygnalDekorator.h"
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>
#include <fstream>
//Serializacja/Deserializacja
#include "json.hpp"
using json = nlohmann::json;

/**
* @class SinusGenerator
* @brief Klasa generatora sygnalu sinusoidalnego.
*
* Dziedziczy po klasie SygnalDekorator i dodaje do sygnalu bazowego sygnal sin o okreslonej amplitudzie i czestotliwosci.
*/
class SinusGenerator : public SygnalDekorator {
private:
    double amplituda;/// Amplituda generowanego sygnalu sinusoidalnego.
    double czestotliwosc;/// Czestotliwosc sygnalu sinusoidalnego.
    double x1;/// Poprzednia probka x1.
    double x2;/// Poprzednia probka x2.
    double omega;/// Wartosc omega, obliczana jako 2*PI*f.

public:
    /**
    * @brief Konstruktor klasy SinusGenerator.
    * @param sygnalgenerator Wskaznik na dekorowany generator sygnalu.
    * @param A Amplituda generowanego sygnalu sinusoidalnego.
    * @param f Czestotliwosc sygnalu sinusoidalnego.
    */
    SinusGenerator(SygnalGenerator* sygnalgenerator, double A, double f);

    /**
    * @brief Generuje wartosc sygnalu z komponentem sinusoidalnym.
    * @return Zwraca wygenerowana probke sygnalu.
    */
    double generuj() override;

    /**
    * @brief Zapisuje obiekt generatora sygnalu sinusoidalnego do formatu JSON.
    * @return Obiekt json zawierajacy dane generatora.
    */
    json zapiszDoJson() override;

    /**
    * @brief Zapisuje obiekt generatora sygnalu sinusoidalnego, w formacie JSON, do pliku.
    * @param nazwaPliku Sciezka do pliku, do ktorego zostanie zapisany generator.
    */
    void zapiszDoPliku(const std::string& nazwaPliku) override;
};