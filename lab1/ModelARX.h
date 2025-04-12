#include <vector>
#include <deque>
#include <random>
#include <stdexcept>
#include <iostream>

#include "ObiektSISO.h"

#include <fstream>
#include <vector>

//Serializacja/Deserializacja
#include "json.hpp"
using json = nlohmann::json;

class ModelARX : public ObiektSISO {
private:
    std::vector<double> A; // Wspolczynniki wielomiany A (mianownik)
    std::vector<double> B; // Wspolczynniki wielomianu B (licznik)
    unsigned int opoznienieTransportowe; // opoznienie transportowe (>=1)
    double odchylenieStandardowe; // odchylenie standardowe (moc zaklocenia)

    std::deque<double> pamiecWejscia; // Kolejka dla sygnalow wejsciowych - u
    std::deque<double> pamiecWyjscia; // Kolejka dla sygnalow wyjsciowych - y
    std::deque<double> pamiecOpoznienia; // Kolejka dla opóźnienia transportowego k

    std::default_random_engine gen;

    // Metoda do symulacji zaklocen
    double symulujZaklocenie();

public:

//-----------------------------------------------------------------------
    json zapiszDoJson() const;
    void zapiszDoPliku(const std::string& nazwaPliku) const;
    //konstruktor z pliku
    ModelARX(const std::string& nazwaPliku);
//-----------------------------------------------------------------------

    // Konstruktor argumentowy
    ModelARX(const std::vector<double>& a, const std::vector<double>& b, unsigned int opoznienie, double odchylenie);

    // ustawienie wspolczynnikow A i B
    void setA(const std::vector<double>& a);
    void setB(const std::vector<double>& b);

    //k i odchylenie
    void setOpoznienieTransportowe(unsigned int opoznienie);
    void setOdchylenieStandardowe(double odchylenie);

    double symuluj(double u) override;//przeciazona metoda symuluj
};