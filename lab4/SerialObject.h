#pragma once
#include "Komponent.h"
#include <list>

/**
* @class SerialObject
* @brief Klasa reprezentujaca obiekt zlozony z wielu komponentow dzialajacych szeregowo.
*
* Dziedziczy po klasie Komponent i zarzadza lista komponentow potomnych.
*/
class SerialObject : public Komponent{
private:
  std::list<Komponent*> m_child;/// Kontener przechowujacy wskazniki do komponentow potomnych.
public:
	/**
	* @brief Symuluje dzialanie obiektu na podstawie sygnalu wejsciowego.
	* @param u Wartosc sygnalu wejsciowego.
	* @return Wynik symulacji.
	*/
	double symuluj(double u);

	/**
	* @brief Dodaje komponent potomny do listy.
	* @param component Wskaznik do komponentu, ktory ma zostac dodany.
	*/
	void Add(Komponent *component);

	/**
	* @brief Usuwa komponent potomny z listy.
	* @param component Wskaznik do komponentu, ktory ma zostac usuniety.
	*/
	void Remove(Komponent *component);
};