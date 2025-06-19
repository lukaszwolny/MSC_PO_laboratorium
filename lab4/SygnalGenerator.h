#pragma once
#include <iostream>
#include <fstream>
#include "json.hpp"
using json = nlohmann::json;

/**
* @class SygnalGenerator
* @brief Abstrakcyjna klasa bazowa dla generatorow sygnalow.
*/
class SygnalGenerator {
public:
    /**
    * @brief Wirtualny destruktor klasy bazowej.
    */
    virtual ~SygnalGenerator() {};

    /**
    * @brief Czysto wirtualna metoda generujaca kolejna probke sygnalu.
    * @return Wygenerowana probka sygnalu.
    */
    virtual double generuj() = 0;

    /**
    * @brief Czysto wirtualna metoda. Zapisuje obiekt generatora sygnalu do formatu JSON.
    * @return Obiekt json zawierajacy dane generatora.
    */
    virtual json zapiszDoJson() = 0;

    /**
    * @brief Czysto wirtualna metoda. Zapisuje obiekt generatora sygnalu, w formacie JSON, do pliku.
    * @param nazwaPliku Sciezka do pliku, do ktorego zostanie zapisany generator.
    */
    virtual void zapiszDoPliku(const std::string& nazwaPliku) = 0;

    /**
    * @brief Tworzy obiekt SygnalGenerator na podstawie zawartosci pliku JSON.
    * @param nazwaPliku Sciezka do pliku zawierajacego strukture JSON z opisem generatora.
    * @return Wskaznik do utworzonego obiektu SygnalGenerator.
    */
    static SygnalGenerator* odczytZPliku(const std::string& nazwaPliku);

    /**
    * @brief Tworzy obiekt SygnalGenerator na podstawie struktury JSON.
    * @param j Obiekt JSON zawierajacy dane do odtworzenia generatora.
    * @return Wskaznik do utworzonego obiektu SygnalGenerator.
    */
    static SygnalGenerator* odczytZJson(const json& j);
};