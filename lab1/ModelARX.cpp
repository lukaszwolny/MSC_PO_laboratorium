#include "ModelARX.h"

#include <iostream>

#include <fstream>
#include <vector>


//====================================================================================================
// Serializacja do JSON
json ModelARX::zapiszDoJson() const {
    json j;
    j["A"] = A;
    j["B"] = B;
    j["opoznienie"] = opoznienieTransportowe;
    j["odchylenie"] = odchylenieStandardowe;
    return j;
}
// Zapis JSON do pliku
void ModelARX::zapiszDoPliku(const std::string& nazwaPliku) const {
    std::ofstream plik(nazwaPliku);
    if (plik.is_open()) {
        plik << zapiszDoJson().dump(4); // 4 spacje dla czytelności
        plik.close();
        std::cout << "Zapisano do pliku: " << nazwaPliku << std::endl;
    }
    else {
        std::cout << "Blad!" << std::endl;
    }
}
// Konstruktor z pliku
ModelARX::ModelARX(const std::string& nazwaPliku) {
    std::ifstream plik(nazwaPliku);
    if (plik.is_open()) {
        json j;
        plik >> j;
        plik.close();
        //*this = ModelARX(j);
        //A = j["A"].get<std::vector<double>>();
        //B = j["B"].get<std::vector<double>>();
        //setOpoznienieTransportowe = j["opoznienie"].get<int>();
        //setOdchylenieStandardowe = j["odchylenie"].get<double>();
        setA(j["A"].get<std::vector<double>>());
        setB(j["B"].get<std::vector<double>>());
        setOpoznienieTransportowe(j["opoznienie"].get<unsigned int>());
        setOdchylenieStandardowe(j["odchylenie"].get<double>());
    }
}
//====================================================================================================

// Konstruktor argumentowy
ModelARX::ModelARX(const std::vector<double>& a, const std::vector<double>& b, unsigned int opoznienie, double odchylenie) {
    setA(a); // Ustaw A
    setB(b); // Ustaw B
    setOpoznienieTransportowe(opoznienie); // Ustaw k
    setOdchylenieStandardowe(odchylenie); // Ustaw odchylenie standardowe
}
//=========================================================================
double ModelARX::symulujZaklocenie() {
    if (odchylenieStandardowe <= 0.0) {
        //odchylenieStandardowe = 0.1;
        return 0.0;
    }
    else {
        //odchylenie musi byc wieksze od zera zeby cos zwrocilo.
        return std::normal_distribution<double>(0, odchylenieStandardowe)(gen);
    }
    //return std::normal_distribution<double>(0, odchylenieStandardowe)(gen);
    //return 0.0; // Brak zaklocen
}

//=========================================================================
// wielomian A
void ModelARX::setA(const std::vector<double>& a) {
    A = a;
    //Rozmiar bufora
    for (int i = 0; i < A.size(); i++) {
        pamiecWyjscia.push_back(0.0);//rozmiar jako same zera.
    }
}
// wielomian B
void ModelARX::setB(const std::vector<double>& b) {
    B = b;
    //Rozmiar bufora
    for (int i = 0; i < B.size(); i++) {
        pamiecWejscia.push_back(0.0);
    }
}
// opoznienie transportowe (>=1)
void ModelARX::setOpoznienieTransportowe(unsigned int opoznienie) {
    if (opoznienie < 1) {
        opoznienieTransportowe = 1; // (>=1)
    }
    else {
        opoznienieTransportowe = opoznienie;
    }
    //Rozmiar bufora
    for (int i = 0; i < opoznienie; i++) {
        pamiecOpoznienia.push_back(0.0);
    }
}
// odchylenie standardowege
void ModelARX::setOdchylenieStandardowe(double odchylenie) {
    odchylenieStandardowe = odchylenie;
}

// metoda symuluj
double ModelARX::symuluj(double u) {

    double y = 0.0;

    //do opoznienia k dodaj sygnal wej na koniec
    //do pamiecie wejscia na koniec dac to co bylo na poczatku opoznienia k
    //z pamieci wejscia u usunac poczatek.
    pamiecOpoznienia.push_back(u);
    pamiecWejscia.push_back(pamiecOpoznienia.front());
    pamiecWejscia.pop_front();

    //rbegin - od konca
    //Obliczenia:
    y = std::inner_product(B.begin(), B.end(), pamiecWejscia.rbegin(), 0.0) - std::inner_product(A.begin(), A.end(), pamiecWyjscia.rbegin(), 0.0);
    
    // Dodaj zaklocenie
    y += symulujZaklocenie(); // +0

    //usun 1 opoznienie
    //aktualziacja pamieci wyjscia
    pamiecOpoznienia.pop_front();
    pamiecWyjscia.push_back(y);
    pamiecWyjscia.pop_front();

    return y;
}