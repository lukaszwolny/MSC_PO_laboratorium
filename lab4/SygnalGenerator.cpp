#include "SygnalGenerator.h"

#include "WartoscStalaGenerator.h"
#include "SinusGenerator.h"
#include "TrojkatGenerator.h"
#include "ProstokatGenerator.h"
#include "SzumBialyGenerator.h"
#include "OgranicznikAmplitudy.h"


SygnalGenerator* SygnalGenerator::odczytZPliku(const std::string& nazwaPliku) {
    std::ifstream plik(nazwaPliku);
    json j;
    if (plik.is_open()) {
        plik >> j;
        plik.close();
    }
    return odczytZJson(j);
};

SygnalGenerator* SygnalGenerator::odczytZJson(const json& j) {
    std::string typ = j.at("Typ");

    //sprawdzanie typu generatora i go utworzenie. Podstawowy nie bedzie mial w sobie innych
    if (typ == "WartoscStalaGenerator") {
        double poziom = j.at("Poziom");
        return new WartoscStalaGenerator(poziom);
    }
    else if (typ == "SinusGenerator") {
        double A = j.at("A");
        double F = j.at("F");
        SygnalGenerator* baza = odczytZJson(j.at("Baza"));//jesli sygnal ma jakas baze, to na zasadzie rekurencji tworzenie.
        return new SinusGenerator(baza, A, F);
    }
    else if (typ == "TrojkatGenerator") {
        double A = j.at("A");
        double F = j.at("F");
        SygnalGenerator* baza = odczytZJson(j.at("Baza"));
        return new TrojkatGenerator(baza, A, F);
    }
    else if (typ == "ProstokatGenerator") {
        double A = j.at("A");
        double F = j.at("F");
        double Wyp = j.at("Wyp");
        SygnalGenerator* baza = odczytZJson(j.at("Baza"));
        return new ProstokatGenerator(baza, A, F, Wyp);
    }
    else if (typ == "SzumBialyGenerator") {
        double A = j.at("A");
        SygnalGenerator* baza = odczytZJson(j.at("Baza"));
        return new SzumBialyGenerator(baza, A);
    }
    else if (typ == "OgranicznikAmplitudy") {
        double A = j.at("A");
        SygnalGenerator* baza = odczytZJson(j.at("Baza"));
        return new OgranicznikAmplitudy(baza, A);
    }
    SygnalGenerator* nic = nullptr;
    return nic;
};