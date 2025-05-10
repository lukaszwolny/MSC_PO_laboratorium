#pragma once
#include "ObiektSISO.h"

/**
* @class RegulatorPID
* @brief Klasa reprezentujaca regulator PID dziedziczaca po ObiektSISO.
*/
class RegulatorPID : public ObiektSISO {
  private:
  
    double K;/// Nastawa czesci proporcjonalnej - wzmocnienie (k)
    double TI;/// Nastawa czesci calkujacej - czas calkowania (Ti)
    double TD;/// Nastawa czesci rozniczkujacej - czas rozniczkowania (Td)
    
    double pamiecCalkujaca;/// Pamiec czesci calkujacej (suma uchybow)
    double pamiecRozniczkujaca;/// Pamiec czesci rozniczkujacej (poprzedni uchyb)

    /**
    * @brief Pomocnicza metoda sprawdzajaca, czy wartosc jest nieujemna.
    * @param nastawa Referencja do parametru, ktory modyfikujemy.
    * @param wartosc Nowa wartosc do przypisania.
    */
    void setNieujemne(double& nastawa, double wartosc);

  public:
    /**
    * @brief Konstruktor regulatora P.
    * @param k_ Wzmocnienie proporcjonalne.
    */
   RegulatorPID(double k_);

    /**
    * @brief Konstruktor regulatora PI.
    * @param k_ Wzmocnienie proporcjonalne.
    * @param TI_ Czas calkowania.
    */
   RegulatorPID(double k_, double TI_);

    /**
    * @brief Konstruktor regulatora PID.
    * @param k_ Wzmocnienie proporcjonalne.
    * @param TI_ Czas calkowania.
    * @param TD_ Czas rozniczkowania.
    */
   RegulatorPID(double k_, double TI_, double TD_);

    /**
    * @brief Ustawia nowe wzmocnienie proporcjonalne.
    * @param nowaWartosc Nowa wartosc K.
    */
   void setK(double nowaWartosc);

    /**
    * @brief Ustawia nowy czas calkowania.
    * @param nowaWartosc Nowa wartosc TI.
    */
   void setTI(double nowaWartosc);

    /**
    * @brief Ustawia nowy czas rozniczkowania.
    * @param nowaWartosc Nowa wartosc TD.
    */
   void setTD(double nowaWartosc);

    /**
    * @brief Symuluje dzialanie regulatora dla zadanego uchybu.
    * @param e_i Aktualny uchyb (wartosc zadana).
    * @return Wyjscie regulatora (sygnal sterujacy).
    */
   double symuluj(double e_i) override;
};