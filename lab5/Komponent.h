#pragma once
#include <cstddef>

#include "ObiektSISO.h"

/**
* @class Komponent
* @brief Abstrakcyjna klasa bazowa reprezentujaca komponent systemu.
*
* Dziedziczy po klasie ObiektSISO i udostepnia interfejs umozliwiajacy dodawanie, usuwanie oraz dostep do komponentow podrzednych.
*/
class Komponent : public ObiektSISO {
protected:
  Komponent *component;/// Wskaznik do komponentu podrzednego.

public:
    /**
    * @brief Wirtualna metoda dodajaca komponent podrzedny.
    * @param component Wskaznik do komponentu, ktory ma zostac dodany.
    */
    virtual void Add(Komponent *component) {};

    /**
    * @brief Wirtualna metoda usuwajaca komponent podrzedny.
    * @param component Wskaznik do komponentu, ktory ma zostac usuniety.
    */
    virtual void Remove(Komponent *component) {};

    /**
    * @brief Zwraca wskazany komponent podrzedny.
    * @param n Indeks komponentu.
    * @return Wskaznik do komponentu podrzednego.
    */
    virtual Komponent* getChild(int n);

    /**
    * @brief Zwraca liczbe dzieci danego komponentu.
    * 
    * Metoda sprawdza, czy komponent posiada podlaczony wewnetrzny komponent.
    * 
    * @return Liczba dzieci (0 lub 1).
    */
    virtual size_t getChildrenCount() const;

    /**
    * @brief Wirtualny destruktor klasy bazowej.
    */
    virtual ~Komponent() = default;
};

/*
  //virtual double symuluj(double u) = 0;// sim
*/