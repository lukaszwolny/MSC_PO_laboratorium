#pragma once
#include <iostream>
#include "SygnalDekorator.h"

#include <iostream>
#include <fstream>
//Serializacja/Deserializacja
#include "json.hpp"
using json = nlohmann::json;

/**
* @class OgranicznikAmplitudy
* @brief Klasa ograniczajaca wartosc amplitudy sygnalu.
*
* Dziedziczy po klasie SygnalDekorator i ogranicza wartosc amplitudy sygnalu bazowego do okreslonej wartosci.
*/
class OgranicznikAmplitudy : public SygnalDekorator {
private:
    double maxAmplituda;/// Maksymalna amplituda generowanego sygnalu.

public:
    /**
    * @brief Konstruktor klasy OgranicznikAmplitudy.
    * @param sygnalgenerator Wskaznik na dekorowany generator sygnalu.
    * @param maxAmp Maksymalna amplituda generowanego sygnalu.
    */
    OgranicznikAmplitudy(SygnalGenerator* sygnalgenerator, double maxAmp);

    /**
    * @brief Generuje wartosc sygnalu z ograniczona wartoscia amplitudy.
    * @return Zwraca wygenerowana probke sygnalu z ograniczona amplituda.
    */
    double generuj() override;

    /**
    * @brief Zapisuje obiekt generatora ograniczajacego wartosc amplitudy do formatu JSON.
    * @return Obiekt json zawierajacy dane generatora.
    */
    json zapiszDoJson() override;

    /**
    * @brief Zapisuje obiekt generatora ograniczajacego wartosc amplitudy, w formacie JSON, do pliku.
    * @param nazwaPliku Sciezka do pliku, do ktorego zostanie zapisany generator.
    */
    void zapiszDoPliku(const std::string& nazwaPliku) override;
};