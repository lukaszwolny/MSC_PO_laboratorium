#pragma once
#include "SygnalDekorator.h"
#include <cmath>

#include <iostream>
#include <fstream>
//Serializacja/Deserializacja
#include "json.hpp"
using json = nlohmann::json;

/**
* @class TrojkatGenerator
* @brief Klasa generatora sygnalu trojkatnego.
*
* Dziedziczy po klasie SygnalDekorator i dodaje do sygnalu bazowego sygnal trojkatny o okreslonej amplitudzie i czestotliwosci.
*/
class TrojkatGenerator : public SygnalDekorator {
private:
    double amplituda;/// Amplituda generowanego sygnalu trojkatnego.
    double czestotliwosc;/// Czestotliwosc sygnalu trojkatnego.
    int probkiNaOkres;/// Liczba probek przypadajace na jeden okres sygnalu.
    int licznik;/// Licznik aktualnej probki w okresie.
public:
    /**
    * @brief Konstruktor klasy TrojkatGenerator.
    * @param sygnalgenerator Wskaznik na dekorowany generator sygnalu.
    * @param A Amplituda generowanego sygnalu trojkatnego.
    * @param f Czestotliwosc sygnalu trojkatnego.
    */
    TrojkatGenerator(SygnalGenerator* sygnalgenerator, double A, double f);

    /**
    * @brief Generuje wartosc sygnalu z komponentem trojkatnym.
    * @return Zwraca wygenerowana probke sygnalu.
    */
    double generuj() override;

    /**
    * @brief Zapisuje obiekt generatora sygnalu trojkatnego do formatu JSON.
    * @return Obiekt json zawierajacy dane generatora.
    */
    json zapiszDoJson() override;

    /**
    * @brief Zapisuje obiekt generatora sygnalu trojkatnego, w formacie JSON, do pliku.
    * @param nazwaPliku Sciezka do pliku, do ktorego zostanie zapisany generator.
    */
    void zapiszDoPliku(const std::string& nazwaPliku) override;
};