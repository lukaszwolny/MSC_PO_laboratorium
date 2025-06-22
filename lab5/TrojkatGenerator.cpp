#include "TrojkatGenerator.h"

TrojkatGenerator::TrojkatGenerator(SygnalGenerator * sygnalgenerator, double A, double f) : SygnalDekorator(sygnalgenerator), amplituda(A), czestotliwosc(f) {
    probkiNaOkres = std::round(1 / czestotliwosc);
    licznik = 0;
}

double TrojkatGenerator::generuj() {
    double result = 0.0;

    int polowaOkresu = probkiNaOkres / 2;
    double zbocze = (2.0 * amplituda) / polowaOkresu;

    if (licznik < polowaOkresu) {
        // czesc narastajaca
        result = -amplituda + zbocze * licznik;//bez tej amplitudy to od zera
    }
    else {
        // czesc opadajaca
        result = amplituda - zbocze * (licznik - polowaOkresu);//bez tej amplitudy to do zera
        //od polowy licznik trzba dostosowac
    }
    licznik = (licznik + 1) % probkiNaOkres;

    return result + SygnalDekorator::generuj();
}

//Serializacja
json TrojkatGenerator::zapiszDoJson() {
    json j;
    j["Typ"] = "TrojkatGenerator";
    j["A"] = amplituda;
    j["F"] = czestotliwosc;
    j["Baza"] = sygnalgenerator_->zapiszDoJson();
    return j;
};

void TrojkatGenerator::zapiszDoPliku(const std::string& nazwaPliku) {
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