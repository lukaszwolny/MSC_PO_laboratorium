#include "SinusGenerator.h"

SinusGenerator::SinusGenerator(SygnalGenerator * sygnalgenerator, double A, double f) : SygnalDekorator(sygnalgenerator), amplituda(A), czestotliwosc(f) {
    omega = 2.0 * M_PI * czestotliwosc;
    x1 = 0.0;
    x2 = amplituda * sin(omega);//amplituda
}

double SinusGenerator::generuj() {
    double result = 0.0;

    double xNew = 2.0 * cos(omega) * x2 - x1;
    result = x1;//x0
    x1 = x2;
    x2 = xNew;

    return result + SygnalDekorator::generuj();
}

//Serializacja
json SinusGenerator::zapiszDoJson() {
    json j;
    j["Typ"] = "SinusGenerator";
    j["A"] = amplituda;
    j["F"] = czestotliwosc;
    j["Baza"] = sygnalgenerator_->zapiszDoJson();
    return j;
};

void SinusGenerator::zapiszDoPliku(const std::string& nazwaPliku) {
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