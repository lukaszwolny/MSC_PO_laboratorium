#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

#define DEBUG //MAIN DEBUG

#include "RegulatorPID.h"
#include "ModelARX.h"
#include "PetlaJedenKrok.h"

#include "WartoscStalaGenerator.h"
#include "SinusGenerator.h"
#include "ProstokatGenerator.h"
#include "TrojkatGenerator.h"
#include "SzumBialyGenerator.h"
#include "OgranicznikAmplitudy.h"


#ifdef DEBUG

//Funkcje pomocnicze dla testów:

void raportBleduSekwencji(std::vector<double>& spodz, std::vector<double>& fakt)
{
	constexpr size_t PREC = 3;
	std::cerr << std::fixed << std::setprecision(PREC);
	std::cerr << "  Spodziewany:\t";
	for (auto& el : spodz)
		std::cerr << el << ", ";
	std::cerr << "\n  Faktyczny:\t";
	for (auto& el : fakt)
		std::cerr << el << ", ";
	std::cerr << std::endl << std::endl;
}

bool porownanieSekwencji(std::vector<double>& spodz, std::vector<double>& fakt)
{
	constexpr double TOL = 1e-1;	// tolerancja dla porównań zmiennoprzecinkowych
	bool result = fakt.size() == spodz.size();
	for (int i = 0; result && i < fakt.size(); i++)
		result = fabs(fakt[i] - spodz[i]) < TOL;
	return result;
}

void test_RegulatorP_brakPobudzenia()
{
	//Sygnatura testu:
	std::cerr << "RegP (k = 0.5) -> test zerowego pobudzenia: ";
	try
	{
		// Przygotowanie danych:
		RegulatorPID instancjaTestowa(0.5);
		constexpr size_t LICZ_ITER = 30;
		std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu (tu same 0)
		std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy (tu same 0)
		std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

		// Symulacja modelu:

		for (int i = 0; i < LICZ_ITER; i++)
			faktSygWy[i] = instancjaTestowa.symuluj(sygWe[i]);

		// Walidacja poprawności i raport:
		if (porownanieSekwencji(spodzSygWy, faktSygWy))
			std::cerr << "OK!\n";
		else
		{
			std::cerr << "FAIL!\n";
			raportBleduSekwencji(spodzSygWy, faktSygWy);
		}
	}
	catch (...)
	{
		std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
	}
}

void test_RegulatorP_skokJednostkowy()
{
	//Sygnatura testu:
	std::cerr << "RegP (k = 0.5) -> test skoku jednostkowego: ";

	try
	{
		// Przygotowanie danych:
		RegulatorPID instancjaTestowa(0.5);
		constexpr size_t LICZ_ITER = 30;
		std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu 
		std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy 
		std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

		// Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
		for (int i = 0; i < LICZ_ITER; i++)
			sygWe[i] = !!i;
		spodzSygWy = { 0.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5 };

		// Symulacja modelu:
		for (int i = 0; i < LICZ_ITER; i++)
			faktSygWy[i] = instancjaTestowa.symuluj(sygWe[i]);

		// Walidacja poprawności i raport:
		if (porownanieSekwencji(spodzSygWy, faktSygWy))
			std::cerr << "OK!\n";
		else
		{
			std::cerr << "FAIL!\n";
			raportBleduSekwencji(spodzSygWy, faktSygWy);
		}
	}
	catch (...)
	{
		std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
	}
}

void test_RegulatorPI_skokJednostkowy_1()
{
	//Sygnatura testu:
	std::cerr << "RegPI (k = 0.5, TI = 1.0) -> test skoku jednostkowego nr 1: ";

	try
	{
		// Przygotowanie danych:
		RegulatorPID instancjaTestowa(0.5,1.0);
		constexpr size_t LICZ_ITER = 30;
		std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu 
		std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy 
		std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

		// Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
		for (int i = 0; i < LICZ_ITER; i++)
			sygWe[i] = !!i;
		spodzSygWy = { 0, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5, 11.5, 12.5, 13.5, 14.5, 15.5, 16.5, 17.5, 18.5, 19.5, 20.5, 21.5, 22.5, 23.5, 24.5, 25.5, 26.5, 27.5, 28.5, 29.5 };

		// Symulacja modelu:
		for (int i = 0; i < LICZ_ITER; i++)
			faktSygWy[i] = instancjaTestowa.symuluj(sygWe[i]);

		// Walidacja poprawności i raport:
		if (porownanieSekwencji(spodzSygWy, faktSygWy))
			std::cerr << "OK!\n";
		else
		{
			std::cerr << "FAIL!\n";
			raportBleduSekwencji(spodzSygWy, faktSygWy);
		}
	}
	catch (...)
	{
		std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
	}
}

void test_RegulatorPI_skokJednostkowy_2()
{
	//Sygnatura testu:
	std::cerr << "RegPI (k = 0.5, TI = 10.0) -> test skoku jednostkowego nr 2: ";

	try
	{
		// Przygotowanie danych:
		RegulatorPID instancjaTestowa(0.5, 10.0);
		constexpr size_t LICZ_ITER = 30;
		std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu 
		std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy 
		std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

		// Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
		for (int i = 0; i < LICZ_ITER; i++)
			sygWe[i] = !!i;
		spodzSygWy = { 0, 0.6, 0.7, 0.8, 0.9, 1, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3, 3.1, 3.2, 3.3, 3.4 };

		// Symulacja modelu:
		for (int i = 0; i < LICZ_ITER; i++)
			faktSygWy[i] = instancjaTestowa.symuluj(sygWe[i]);

		// Walidacja poprawności i raport:
		if (porownanieSekwencji(spodzSygWy, faktSygWy))
			std::cerr << "OK!\n";
		else
		{
			std::cerr << "FAIL!\n";
			raportBleduSekwencji(spodzSygWy, faktSygWy);
		}
	}
	catch (...)
	{
		std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
	}
}

void test_RegulatorPID_skokJednostkowy()
{
	//Sygnatura testu:
	std::cerr << "RegPID (k = 0.5, TI = 10.0, TD = 0.2) -> test skoku jednostkowego: ";

	try
	{
		// Przygotowanie danych:
		RegulatorPID instancjaTestowa(0.5, 10.0, 0.2);
		constexpr size_t LICZ_ITER = 30;
		std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu 
		std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy 
		std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy

		// Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
		for (int i = 0; i < LICZ_ITER; i++)
			sygWe[i] = !!i;
		spodzSygWy = { 0, 0.8, 0.7, 0.8, 0.9, 1, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3, 3.1, 3.2, 3.3, 3.4 };

		// Symulacja modelu:
		for (int i = 0; i < LICZ_ITER; i++)
			faktSygWy[i] = instancjaTestowa.symuluj(sygWe[i]);

		// Walidacja poprawności i raport:
		if (porownanieSekwencji(spodzSygWy, faktSygWy))
			std::cerr << "OK!\n";
		else
		{
			std::cerr << "FAIL!\n";
			raportBleduSekwencji(spodzSygWy, faktSygWy);
		}
	}
	catch (...)
	{
		std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
	}
}

void symulacja_petli()
{
	std::cerr << "symulacja_petli:\n";

	constexpr size_t LICZ_ITER = 30;
	std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu  - wartosc zadana
	std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy - wyjscie - wartosc regulowana

	//RegulatorPID i ModelARX
	//RegulatorPID regulator(0.5);
	RegulatorPID regulator(0.5, 2.0);
	//RegulatorPID regulator(0.4, 2.0, 0.2);
	ModelARX obiekt({ -0.4 }, { 0.6 }, 1, 0.0);

	//wartosc zadana = 1.
	for (int i = 0; i < LICZ_ITER; i++)
		sygWe[i] = 1;

	PetlaJedenKrok petla;
	// Symulacja petli:
	for (int i = 0; i < LICZ_ITER; i++)
		faktSygWy[i] = petla(regulator, obiekt, sygWe[i]);

	//wynik
	for (int i = 0; i < LICZ_ITER; i++)
		std::cout<<"Krok "<<i+1<<" y= "<<faktSygWy[i]<<std::endl;

}

void test_symulacja_sygnalow()
{
	constexpr size_t LICZ_ITER = 30;
	std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy 
	std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy
	// Przygotowanie danych:
	SygnalGenerator* sygnal_wartoscstala_test = new WartoscStalaGenerator(3.0);//3
	SygnalGenerator* sygnal_sin_test = new SinusGenerator(new WartoscStalaGenerator(0.0), 4.0, 0.05);//4
	SygnalGenerator* sygnal_prostokat_test = new ProstokatGenerator(new WartoscStalaGenerator(0.0), 3.0, 0.05, 0.5);//3 0.5
	SygnalGenerator* sygnal_trojkat_test = new TrojkatGenerator(new WartoscStalaGenerator(0.0), 3.0, 0.05);//3 0.5

	// Symulacja sygnal_wartoscstala_test
	std::cerr << "sygnal_wartoscstala_test: ";
	spodzSygWy = { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 ,3, 3, 3, 3, 3};
	for (int i = 0; i < LICZ_ITER; ++i) {
		faktSygWy[i] = sygnal_wartoscstala_test->generuj();
		//std::cout << faktSygWy[i] << std::endl;
	}
	if (porownanieSekwencji(spodzSygWy, faktSygWy))
		std::cerr << "OK!\n";
	else
	{
		std::cerr << "FAIL!\n";
		raportBleduSekwencji(spodzSygWy, faktSygWy);
	}
	// Symulacja sygnal_sin_test
	std::cerr << "sygnal_sin_test: ";
	spodzSygWy = { 0.000,	1.236,	2.351,	3.236,	3.804,	4.000,	3.804,	3.236,	2.351,	1.236,	0.000, - 1.236, - 2.351, - 3.236 ,- 3.804, - 4.000 ,- 3.804 ,- 3.236, - 2.351, - 1.236,	0.000	,1.236	,2.351,	3.236,	3.804	,4.000	,3.804	,3.236	,2.351,	1.236};
	for (int i = 0; i < LICZ_ITER; ++i) {
		faktSygWy[i] = sygnal_sin_test->generuj();
		//std::cout << faktSygWy[i] << std::endl;
	}
	if (porownanieSekwencji(spodzSygWy, faktSygWy))
		std::cerr << "OK!\n";
	else
	{
		std::cerr << "FAIL!\n";
		raportBleduSekwencji(spodzSygWy, faktSygWy);
	}
	// Symulacja sygnal_prostokat_test
	std::cerr << "sygnal_prostokat_test: ";
	spodzSygWy = { 3,	3,	3,	3,	3,	3,	3,	3,	3,	3, - 3, - 3, - 3, - 3, - 3, - 3, - 3, - 3, - 3, - 3,	3,	3,	3,	3,	3,	3,	3,	3,	3,	3};
	for (int i = 0; i < LICZ_ITER; ++i) {
		faktSygWy[i] = sygnal_prostokat_test->generuj();
		//std::cout << faktSygWy[i] << std::endl;
	}
	if (porownanieSekwencji(spodzSygWy, faktSygWy))
		std::cerr << "OK!\n";
	else
	{
		std::cerr << "FAIL!\n";
		raportBleduSekwencji(spodzSygWy, faktSygWy);
	}
	// Symulacja sygnal_trojkat_test
	std::cerr << "sygnal_trojkat_test: ";
	spodzSygWy = { -3, - 2.4, - 1.8, - 1.2, - 0.6,	0	,0.6,	1.2,	1.8,	2.4,	3,	2.4,	1.8,	1.2,	0.6,	0, - 0.6, - 1.2, - 1.8, - 2.4 ,- 3, - 2.4, - 1.8, - 1.2, - 0.6,	0,	0.6,	1.2,	1.8,	2.4};
	for (int i = 0; i < LICZ_ITER; ++i) {
		faktSygWy[i] = sygnal_trojkat_test->generuj();
		//std::cout << faktSygWy[i] << std::endl;
	}
	if (porownanieSekwencji(spodzSygWy, faktSygWy))
		std::cerr << "OK!\n";
	else
	{
		std::cerr << "FAIL!\n";
		raportBleduSekwencji(spodzSygWy, faktSygWy);
	}


	//Polaczenie
	SygnalGenerator* sygnal = new OgranicznikAmplitudy(
		new SzumBialyGenerator(
			new TrojkatGenerator(
				new ProstokatGenerator(
					new SinusGenerator(
						new WartoscStalaGenerator(1.0),//stala
						4.0, 0.05),//Sinus
					3.0, 0.05, 0.5),//Prostokat
				3.0, 0.05),// Trojkat
			0.0),// Szum
	10.0);// Wartosc maksymalna amplitudy

  for (int i = 0; i < 30; ++i) {
	  double sample = sygnal->generuj();
	  //std::cout << sample << std::endl;
  }


  //Testy
  SygnalGenerator* sygnal_wartoscstala = new WartoscStalaGenerator(0.0);
  SygnalGenerator* szum = new SzumBialyGenerator(sygnal_wartoscstala, 1.0);
	SygnalGenerator* sygnal_test = new OgranicznikAmplitudy(
	  new WartoscStalaGenerator(7.0),
	5.0);


	std::cout << "========================TEST================================" << std::endl;
	SygnalGenerator* sygnal_test2 = new TrojkatGenerator(
		new SinusGenerator(
			new TrojkatGenerator(
				new WartoscStalaGenerator(4.0),
				2.0, 0.05),
			4.0, 0.05),
		3.0, 0.05);
	for (int i = 0; i < 30; ++i) {
		double sample3333 = sygnal_test2->generuj();
		std::cout << sample3333 << std::endl;
	}
	std::cout << "========================KONIEC TEST================================" << std::endl;
  for (int i = 0; i < 30; ++i) {
	  double sample = sygnal_test->generuj();
	  //std::cout << sample << std::endl;
  }

}

void symulacja_ukladu_petli() {
	std::cerr << "symulacja_petli:\n";

	constexpr size_t LICZ_ITER = 100;
	std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu  - wartosc zadana
	std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy - wyjscie - wartosc regulowana

	SygnalGenerator* sygnal = new OgranicznikAmplitudy(
		new SzumBialyGenerator(
			new TrojkatGenerator(
				new ProstokatGenerator(
					new SinusGenerator(
						new WartoscStalaGenerator(0.0),//stala
						4.0, 0.05),//Sinus
					3.0, 0.05, 0.5),//Prostokat
				3.0, 0.05),// Trojkat
			0.0),// Szum
		10.0);// Wartosc maksymalna amplitudy


	for (int i = 0; i < LICZ_ITER; ++i) {
		sygWe[i] = sygnal->generuj();
		//std::cout << sygWe[i] << std::endl;
	}


	//RegulatorPID i ModelARX
	//RegulatorPID regulator(0.5);
	RegulatorPID regulator(0.5, 2.0);
	//RegulatorPID regulator(0.4, 2.0, 0.2);
	ModelARX obiekt({ -0.4 }, { 0.6 }, 1, 0.0);

	PetlaJedenKrok petla;
	// Symulacja petli:
	for (int i = 0; i < LICZ_ITER; i++)
		faktSygWy[i] = petla(regulator, obiekt, sygWe[i]);

	//wynik
	for (int i = 0; i < LICZ_ITER; i++)
		std::cout << "Krok " << i + 1 << " y= " << faktSygWy[i] << std::endl;

}

void test_ser_des() {

	SygnalGenerator* sygnal = new OgranicznikAmplitudy(
		new SzumBialyGenerator(
			new TrojkatGenerator(
				new ProstokatGenerator(
					new SinusGenerator(
						new WartoscStalaGenerator(0.0),//stala
						4.0, 0.05),//Sinus
					3.0, 0.05, 0.5),//Prostokat
				3.0, 0.05),// Trojkat
			1.0),// Szum
		10.0);// Wartosc maksymalna amplitudy

	for (int i = 0; i < 30; ++i) {
		double sample = sygnal->generuj();
		//std::cout << sample << std::endl;
	}

	//Serializacja do pliku (JSON)
	SygnalGenerator* sygnal_testttt = new TrojkatGenerator(new SinusGenerator(new WartoscStalaGenerator(10.0), 3.0, 0.05),4.0, 0.05);
	//sygnal_testttt->zapiszDoPliku("test.txt");
	sygnal_testttt->zapiszDoPliku("test4.txt");
	
	std::cout << "========================TEST DESERIALIZACJI================================" << std::endl;
	//SygnalGenerator* sygnal_test_wartoscstala = new WartoscStalaGenerator(4.0);
	//SygnalGenerator* sygnal_test_wartoscstala = new SinusGenerator(new WartoscStalaGenerator(4.0), 5.0, 0.05);

	SygnalGenerator* sygnal_test_deserializacja = new OgranicznikAmplitudy(
		new SzumBialyGenerator(
			new TrojkatGenerator(
				new ProstokatGenerator(
					new SinusGenerator(
						new WartoscStalaGenerator(0.0),//stala
						4.0, 0.05),//Sinus
					3.0, 0.05, 0.5),//Prostokat
				3.0, 0.05),// Trojkat
			0.0),// Szum
		10.0);// Wartosc maksymalna amplitudy
	for (int i = 0; i < 30; ++i) {
		double sample = sygnal_test_deserializacja->generuj();
		std::cout << sample << std::endl;
	}

	sygnal_test_deserializacja->zapiszDoPliku("testyDlasygnal_test_deserializacja.txt");
	//Deserializacja
	SygnalGenerator* sygnal_testdeserializacja_wynik;
	sygnal_testdeserializacja_wynik = SygnalGenerator::odczytZPliku("testyDlasygnal_test_deserializacja.txt");
	std::cout << "========================TEST DESERIALIZACJI - WYNIK================================" << std::endl;
	for (int i = 0; i < 30; ++i) {
		double sample = sygnal_testdeserializacja_wynik->generuj();
		std::cout << sample << std::endl;
	}
	std::cout << "========================KONIEC TEST DESERIALIZACJI================================" << std::endl;
	//sygnal_testdeserializacja->deserialiacjazpliku("plik.txt");

	//sygnal.zapiszDoPliku("testZpliku.txt");
}

using namespace std;

int main()
{
	//test_RegulatorP_brakPobudzenia();
	//test_RegulatorP_skokJednostkowy();
	//test_RegulatorPI_skokJednostkowy_1();
	//test_RegulatorPI_skokJednostkowy_2();
	//test_RegulatorPID_skokJednostkowy();
	//petla
	//symulacja_petli();

	//==========lab3==================
	//generacja sygnalow
	test_symulacja_sygnalow();
	//SerializacjaDeserializacja
	test_ser_des();
	//Test ukladu z poprzedniego lab
	symulacja_ukladu_petli();
}

#endif

#ifdef MAIN

using namespace std;

int main()
{
  RegulatorPID testy(5);
}
#endif

/*
g++ lab3.cpp ModelARX.cpp RegulatorPID.cpp PetlaJedenKrok.cpp WartoscStalaGenerator.cpp 
TrojkatGenerator.cpp SzumBialyGenerator.cpp SygnalGenerator.cpp SygnalDekorator.cpp SinusGenerator.cpp 
ProstokatGenerator.cpp OgranicznikAmplitudy.cpp 
./a.out 

GeneratorSygnalow - Komponent -> SygnalGenerator
KomponentKonkretny - warosc stala
Dekorator DekoratorSygnalow
2coswTp - to zawwsze takie same to jakas stala.
generowanie sinusa z: 2coswTp x(n-1)x(n-2)
w = omega
Tp = 1. 
na podstawie dwoch poprzednich probek
amplituda to te dwie pierwsze probki n-1 i n-2...
trójkąta o wybranej częstotliwości i wypełnieniu, bez wypelnienia!!!
serializacja tez: nazwa sygnalu, parametry.. (jak ten sygnal jest tworzony zapisac...)


{
	"A": 10.0,
	"Baza": {
		"A": 0.0,
		"Baza": {
			"A": 3.0,
			"Baza": {
				"A": 3.0,
				"Baza": {
					"A": 4.0,
					"Baza": {
						"Poziom": 0.0,
						"Typ": "WartoscStalaGenerator"
					},
					"F": 0.05,
					"Typ": "SinusGenerator"
				},
				"F": 0.05,
				"Typ": "ProstokatGenerator",
				"Wyp": 0.5
			},
			"F": 0.05,
			"Typ": "TrojkatGenerator"
		},
		"Typ": "SzumBialyGenerator"
	},
	"Typ": "OgranicznikAmplitudy"
}
*/