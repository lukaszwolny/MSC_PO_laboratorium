#pragma once
#include "SygnalDekorator.h"
#include <random>

#include <iostream>
#include <fstream>
//Serializacja/Deserializacja
#include "json.hpp"
using json = nlohmann::json;

/**
* @class SzumBialyGenerator
* @brief Klasa generatora szumu bialego.
*
* Dziedziczy po klasie SygnalDekorator i dodaje do sygnalu bazowego sygnal szumu bialego o okreslonej amplitudzie.
*/
class SzumBialyGenerator : public SygnalDekorator {
private:
    double amplituda;/// Amplituda generowanego szumu bialego.
    std::default_random_engine generator;/// Generator liczb pseudolosowych.
    std::uniform_real_distribution<double> distribution;/// Rozklad rownomierny wykorzystywany do generowania szumu bialego.

public:
    /**
    * @brief Konstruktor klasy SzumBialyGenerator.
    * @param sygnalgenerator Wskaznik na dekorowany generator sygnalu.
    * @param A Amplituda generowanego sygnalu szumu bialego.
    */
    SzumBialyGenerator(SygnalGenerator* sygnalgenerator, double amp);

    /**
    * @brief Generuje wartosc sygnalu z komponentem szumu bialego.
    * @return Zwraca wygenerowana probke sygnalu.
    */
    double generuj() override;

    /**
    * @brief Zapisuje obiekt generatora sygnalu szumu bialego do formatu JSON.
    * @return Obiekt json zawierajacy dane generatora.
    */
    json zapiszDoJson() override;

    /**
    * @brief Zapisuje obiekt generatora sygnalu szumu bialego, w formacie JSON, do pliku.
    * @param nazwaPliku Sciezka do pliku, do ktorego zostanie zapisany generator.
    */
    void zapiszDoPliku(const std::string& nazwaPliku) override;
};