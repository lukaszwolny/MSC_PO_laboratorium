#include "WartoscStalaGenerator.h"

WartoscStalaGenerator::WartoscStalaGenerator(double wartosc) : wartosc(wartosc) {}

double WartoscStalaGenerator::generuj() {
  return wartosc;
}

//Serializacja
json WartoscStalaGenerator::zapiszDoJson() {
    json j;
    j["Typ"] = "WartoscStalaGenerator";
    j["Poziom"] = wartosc;
    return j;
};

void WartoscStalaGenerator::zapiszDoPliku(const std::string& nazwaPliku) {
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