#include "OgranicznikAmplitudy.h"

OgranicznikAmplitudy::OgranicznikAmplitudy(SygnalGenerator* sygnalgenerator, double maxAmp) : SygnalDekorator(sygnalgenerator), maxAmplituda(maxAmp) {}

double OgranicznikAmplitudy::generuj() {
    double result = SygnalDekorator::generuj();// sygnalgenerator_->generuj();SygnalDekorator::generuj()

    if (maxAmplituda <= 0.0) {
        std::cout << std::endl << "Blad!, Maksymalna amplituda musi byc wieksza od zera. (bez efektu, return result)" << std::endl;
        return result;
    }
    else {
        if (result >= maxAmplituda)
            return maxAmplituda;
        else if (result < -maxAmplituda)
            return -maxAmplituda;
    }
    return result;// zwraca to co bylo
}

//Serializacja
json OgranicznikAmplitudy::zapiszDoJson() {
    json j;
    j["Typ"] = "OgranicznikAmplitudy";
    j["A"] = maxAmplituda;
    j["Baza"] = sygnalgenerator_->zapiszDoJson();
    return j;
};

void OgranicznikAmplitudy::zapiszDoPliku(const std::string& nazwaPliku) {
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