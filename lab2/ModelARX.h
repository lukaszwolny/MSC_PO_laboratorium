#pragma once
#include "ObiektSISO.h"
#include <deque>
#include <random>
#include <iostream>
#include <fstream>
#include <vector>
//Serializacja/Deserializacja
#include "json.hpp"
using json = nlohmann::json;

/**
 * @class ModelARX
 * @brief Klasa reprezentujaca model ARX.
 */
class ModelARX : public ObiektSISO {
private:
    std::vector<double> A;/// Wspolczynniki wielomiany A (mianownik)
    std::vector<double> B;/// Wspolczynniki wielomianu B (licznik)
    unsigned int opoznienieTransportowe;/// opoznienie transportowe (>=1)
    double odchylenieStandardowe;/// odchylenie standardowe (moc zaklocenia)

    std::deque<double> pamiecWejscia;/// Kolejka dla sygnalow wejsciowych - u
    std::deque<double> pamiecWyjscia;/// Kolejka dla sygnalow wyjsciowych - y
    std::deque<double> pamiecOpoznienia;/// Kolejka dla opoznienia transportowego k

    std::default_random_engine gen;/// Generator liczb losowych do zaklocen

    /**
    * @brief Symuluje losowe zaklocenie na podstawie odchylenia standardowego.
    * @return Wartosc zaklocenia.
    */
    double symulujZaklocenie();

public:
    /**
    * @brief Zapisuje model do obiektu JSON.
    * @return Obiekt json zawierajacy dane modelu.
    */
    json zapiszDoJson() const;

    /**
    * @brief Zapisuje model do pliku w formacie JSON.
    * @param nazwaPliku Nazwa pliku do zapisu.
    */
    void zapiszDoPliku(const std::string& nazwaPliku) const;

    /**
    * @brief Konstruktor ladujacy model z pliku JSON.
    * @param nazwaPliku Nazwa pliku JSON.
    */
    ModelARX(const std::string& nazwaPliku);

    /**
    * @brief Konstruktor tworzacy model na podstawie wspolczynnikow.
    * @param a Wspolczynniki A (mianownik).
    * @param b Wspolczynniki B (licznik).
    * @param opoznienie Opoznienie transportowe (k).
    * @param odchylenie Odchylenie standardowe zaklocen.
    */
    ModelARX(const std::vector<double>& a, const std::vector<double>& b, unsigned int opoznienie, double odchylenie);

    /**
    * @brief Ustawia wspolczynniki wielomianu A.
    * @param a Wektor wspolczynnikow A.
    */
    void setA(const std::vector<double>& a);

    /**
    * @brief Ustawia wspolczynniki wielomianu B.
    * @param b Wektor wspolczynnikow B.
    */
    void setB(const std::vector<double>& b);

    /**
    * @brief Ustawia opoznienie transportowe.
    * @param opoznienie Nowa wartosc opoznienia.
    */
    void setOpoznienieTransportowe(unsigned int opoznienie);

    /**
    * @brief Ustawia odchylenie standardowe zaklocen.
    * @param odchylenie Nowa wartosc odchylenia.
    */   
    void setOdchylenieStandardowe(double odchylenie);

    /**
    * @brief Symuluje odpowiedz modelu na sygnal wejsciowy.
    * @param u Aktualny sygnal wejsciowy.
    * @return Odpowiedz modelu (wyjscie).
    */
    double symuluj(double u) override;//przeciazona metoda symuluj
};