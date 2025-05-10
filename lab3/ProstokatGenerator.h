#pragma once
#include "SygnalDekorator.h"
#include <cmath>

#include <iostream>
#include <fstream>
//Serializacja/Deserializacja
#include "json.hpp"
using json = nlohmann::json;

/**
* @class ProstokatGenerator
* @brief Klasa generatora sygnalu prostokatnego.
*
* Dziedziczy po klasie SygnalDekorator i dodaje do sygnalu bazowego sygnal prostokatny o okreslonej amplitudzie, czestotliwosci i wypelnieniu.
*/
class ProstokatGenerator : public SygnalDekorator {
private:
    double amplituda;/// Amplituda generowanego sygnalu prostokatnego.
    double czestotliwosc;/// Czestotliwosc sygnalu prostokatnego.
    double wypelnienie;/// Wspolczynnik wypelnienia sygnalu prostokatnego.
    int probkiNaOkres;/// Liczba probek przypadajace na jeden okres sygnalu.
    int probkiWysokie;/// Liczba probek, w ktorych sygnal jest na poziomie wysokim.
    int licznik;/// Licznik aktualnej probki w okresie.
public:
    /**
    * @brief Konstruktor klasy ProstokatGenerator.
    * @param sygnalgenerator Wskaznik na dekorowany generator sygnalu.
    * @param A Amplituda generowanego sygnalu prostokatnego.
    * @param f Czestotliwosc sygnalu prostokatnego.
    * @param wyp Wspolczynnik wypelnienia sygnalu prostokatnego.
    */
    ProstokatGenerator(SygnalGenerator* sygnalgenerator, double A, double f, double wyp);

    /**
    * @brief Generuje wartosc sygnalu z komponentem prostokatnym.
    * @return Zwraca wygenerowana probke sygnalu.
    */
    double generuj() override;

    /**
    * @brief Zapisuje obiekt generatora sygnalu prostokatnego do formatu JSON.
    * @return Obiekt json zawierajacy dane generatora.
    */
    json zapiszDoJson() override;

    /**
    * @brief Zapisuje obiekt generatora sygnalu prostokatnego, w formacie JSON, do pliku.
    * @param nazwaPliku Sciezka do pliku, do ktorego zostanie zapisany generator.
    */
    void zapiszDoPliku(const std::string& nazwaPliku) override;
};