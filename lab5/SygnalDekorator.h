#pragma once
#include "SygnalGenerator.h"

/**
* @class SygnalDekorator
* @brief Klasa reprezentujaca dekorator dla generatorw sygnalow.
* 
* Dziedziczy po klasie SygnalGenerator i przekazuje wywolania do opakowanego obiektu SygnalGenerator.
*/
class SygnalDekorator : public SygnalGenerator {
  protected:
    SygnalGenerator* sygnalgenerator_;/// Wskaznik na opakowany generator sygnalu.
  public:
    /**
    * @brief Konstruktor klasy SygnalDekorator.
    * @param sygnalgenerator Wskaznik na generator sygnalu, ktory ma zostac udekorowany.
    */
    SygnalDekorator(SygnalGenerator* sygnalgenerator);

    /**
    * @brief Generuje probke sygnalu, delegujac wywolanie do opakowanego generatora.
    * @return Zwraca wygenerowana probke sygnalu, przez opakowany generator.
    */
    double generuj() override;
};