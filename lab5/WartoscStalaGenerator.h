#pragma once
#include "SygnalGenerator.h"

#include <iostream>
#include <fstream>
//Serializacja/Deserializacja
#include "json.hpp"
using json = nlohmann::json;

/**
* @class WartoscStalaGenerator
* @brief Klasa generatora sygnalu o wartosci stalej.
* 
* Dziedziczy po klasie SygnalGenerator i zwraca te sama wartosc, ktora zostala zadana.
*/
class WartoscStalaGenerator : public SygnalGenerator {
  private:
      double wartosc;/// Wartosc stala generowana przez generator.

  public:
    /**
    * @brief Konstruktor klasy WartoscStalaGenerator.
    * @param wartosc Wartosc stala, która bêdzie generowana.
    */
    WartoscStalaGenerator(double wartosc);

    /**
    * @brief Generuje wartosc sygnalu.
    * @return Zwraca zadana, stala wartosc.
    */
    double generuj() override;

    /**
    * @brief Zapisuje obiekt generatora sygnalu wartosci stalej do formatu JSON.
    * @return Obiekt json zawierajacy dane generatora.
    */
    json zapiszDoJson() override;

    /**
    * @brief Zapisuje obiekt generatora sygnalu wartosci stalej, w formacie JSON, do pliku.
    * @param nazwaPliku Sciezka do pliku, do ktorego zostanie zapisany generator.
    */
    void zapiszDoPliku(const std::string& nazwaPliku) override;
  };