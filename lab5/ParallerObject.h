#pragma once
#include "Komponent.h"
#include <list>

/**
* @class ParallerObject
* @brief Klasa reprezentujaca obiekt zlozony z wielu komponentow dzialajacych rownolegle.
*
* Dziedziczy po klasie Komponent i zarzadza lista komponentow potomnych.
*/
class ParallerObject : public Komponent{
private:
  std::list<Komponent*> m_child;/// Kontener przechowujacy wskazniki do komponentow potomnych.
public:

	/**
	* @brief Symuluje dzialanie obiektu na podstawie sygnalu wejsciowego.
	* @param u Wartosc sygnalu wejsciowego.
	* @return Wynik symulacji.
	*/
	double symuluj(double u) override;


	/**
	* @brief Dodaje komponent potomny do listy.
	* @param component Wskaznik do komponentu, ktory ma zostac dodany.
	*/
	void Add(Komponent *component) override;

	/**
	* @brief Usuwa komponent potomny z listy.
	* @param component Wskaznik do komponentu, ktory ma zostac usuniety.
	*/
	void Remove(Komponent *component) override;

	/**
	* @brief Zwraca wskaznik na dziecko o podanym indeksie.
	* @param n Indeks dziecka.
	* @return Wskaznik na dziecko.
	*/
	Komponent* getChild(int n) override;

	/**
	* @brief Zwraca liczbe dzieci komponentu.
	* @return Wskaznik na dziecko.
	*/
	size_t getChildrenCount() const override;

};