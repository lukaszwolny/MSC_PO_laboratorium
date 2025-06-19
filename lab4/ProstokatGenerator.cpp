#include "ProstokatGenerator.h"

ProstokatGenerator::ProstokatGenerator(SygnalGenerator * sygnalgenerator, double A, double f, double wyp) : SygnalDekorator(sygnalgenerator), amplituda(A), czestotliwosc(f), wypelnienie(wyp) {
    probkiNaOkres = std::round(1 / czestotliwosc);
    probkiWysokie = std::round(probkiNaOkres * wypelnienie);
    licznik = 0;
}

double ProstokatGenerator::generuj() {
    double result = 0.0;

    result = (licznik < probkiWysokie) ? amplituda : -amplituda;//0
    licznik = (licznik + 1) % probkiNaOkres;

    return result + SygnalDekorator::generuj();
}

//Serializacja
json ProstokatGenerator::zapiszDoJson() {
    json j;
    j["Typ"] = "ProstokatGenerator";
    j["A"] = amplituda;
    j["F"] = czestotliwosc;
    j["Wyp"] = wypelnienie;
    j["Baza"] = sygnalgenerator_->zapiszDoJson();
    return j;
};

void ProstokatGenerator::zapiszDoPliku(const std::string& nazwaPliku) {
    std::ofstream plik(nazwaPliku);
    if (plik.is_open()) {
        plik << zapiszDoJson().dump(4); // 4 spacje dla czytelnoœci
        plik.close();
        std::cout << "Zapisano do pliku: " << nazwaPliku << std::endl;
    }
    else {
        std::cout << "Blad!" << std::endl;
    }
};