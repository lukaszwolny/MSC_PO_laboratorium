#include "SzumBialyGenerator.h"

SzumBialyGenerator::SzumBialyGenerator(SygnalGenerator* sygnalgenerator, double amp)
    : SygnalDekorator(sygnalgenerator), amplituda(amp), distribution(-amp, amp) {
    std::random_device rd;
    generator.seed(rd());// Inicjalizacja generatora liczb losowych
}

double SzumBialyGenerator::generuj() {
    // szum losowy z zakresu [-A, +A]
    double szum = distribution(generator);

    return szum + SygnalDekorator::generuj();
}

//Serializacja
json SzumBialyGenerator::zapiszDoJson() {
    json j;
    j["Typ"] = "SzumBialyGenerator";
    j["A"] = amplituda;
    j["Baza"] = sygnalgenerator_->zapiszDoJson();
    return j;
};
void SzumBialyGenerator::zapiszDoPliku(const std::string& nazwaPliku) {
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