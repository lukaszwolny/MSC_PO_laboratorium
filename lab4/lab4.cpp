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

#include "Komponent.h"
#include "SerialObject.h"
#include "ParallerObject.h"

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


void test_ModelARX_brakPobudzenia()
{
	//Sygnatura testu:
	std::cerr << "ModelARX (-0.4 | 0.6 | 1 | 0 ) -> test zerowego pobudzenia: ";
	try
	{
		// Przygotowanie danych:
		ModelARX instancjaTestowa({ -0.4 }, { 0.6 }, 1, 0);
		constexpr size_t LICZ_ITER = 30;
		std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu (tu same 0)
		std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy (tu same 0)
		std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy
		std::vector<double> faktSygWy2(LICZ_ITER);  // faktyczna sekwencja wy
		std::vector<double> faktSygWy3(LICZ_ITER);  // faktyczna sekwencja wy

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

		//Teraz test z szeregowe/równoległe:
		// Symulacja modelu:
		Komponent* tree = new SerialObject;// SerialObject;ParallerObject
		Komponent* leaf_1 = new ModelARX({ -0.4 }, { 0.6 }, 1, 0);
		tree->Add(leaf_1);
		for (int i = 0; i < LICZ_ITER; i++) {
			faktSygWy2[i] = tree->symuluj(sygWe[i]);
			//std::cout << "tree " << i + 1 << " y= " << faktSygWy2[i] << std::endl;
		}
		// Walidacja poprawności i raport:
		if (porownanieSekwencji(spodzSygWy, faktSygWy2))
			std::cerr << "OK2 - SerialObject!\n";
		else
		{
			std::cerr << "FAIL!\n";
			raportBleduSekwencji(spodzSygWy, faktSygWy2);
		}
		Komponent* tree_b = new ParallerObject;// SerialObject;ParallerObject
		Komponent* leaf_2 = new ModelARX({ -0.4 }, { 0.6 }, 1, 0);
		tree_b->Add(leaf_2);
		for (int i = 0; i < LICZ_ITER; i++) {
			faktSygWy3[i] = tree_b->symuluj(sygWe[i]);
			//std::cout << "tree " << i + 1 << " y= " << faktSygWy2[i] << std::endl;
		}
		// Walidacja poprawności i raport:
		if (porownanieSekwencji(spodzSygWy, faktSygWy3))
			std::cerr << "OK3 - ParallerObject!\n";
		else
		{
			std::cerr << "FAIL!\n";
			raportBleduSekwencji(spodzSygWy, faktSygWy3);
		}


	}
	catch (...)
	{
		std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
	}
}

void test_ModelARX_skokJednostkowy_1()
{
	//Sygnatura testu:
	std::cerr << "ModelARX (-0.4 | 0.6 | 1 | 0 ) -> test skoku jednostkowego nr 1: ";

	try
	{
		// Przygotowanie danych:
		ModelARX instancjaTestowa({ -0.4 }, { 0.6 }, 1, 0);
		constexpr size_t LICZ_ITER = 30;
		std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu 
		std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy 
		std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy
		std::vector<double> faktSygWy2(LICZ_ITER);  // faktyczna sekwencja wy
		std::vector<double> faktSygWy3(LICZ_ITER);  // faktyczna sekwencja wy

		// Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
		for (int i = 0; i < LICZ_ITER; i++)
			sygWe[i] = !!i;
		spodzSygWy = { 0, 0, 0.6, 0.84, 0.936, 0.9744, 0.98976, 0.995904, 0.998362, 0.999345, 0.999738, 0.999895, 0.999958, 0.999983, 0.999993, 0.999997, 0.999999, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

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

		//Teraz test z szeregowe/równoległe:
		// Symulacja modelu:
		Komponent* tree_c = new SerialObject;// SerialObject;ParallerObject
		Komponent* leaf_1 = new ModelARX({ -0.4 }, { 0.6 }, 1, 0);
		tree_c->Add(leaf_1);
		for (int i = 0; i < LICZ_ITER; i++) {
			faktSygWy2[i] = tree_c->symuluj(sygWe[i]);
			//std::cout << "tree " << i + 1 << " y= " << faktSygWy2[i] << std::endl;
		}
		// Walidacja poprawności i raport:
		if (porownanieSekwencji(spodzSygWy, faktSygWy2))
			std::cerr << "OK2 - SerialObject!\n";
		else
		{
			std::cerr << "FAIL!\n";
			raportBleduSekwencji(spodzSygWy, faktSygWy2);
		}
		Komponent* tree_b2 = new ParallerObject;// SerialObject;ParallerObject
		Komponent* leaf_2 = new ModelARX({ -0.4 }, { 0.6 }, 1, 0);
		tree_b2->Add(leaf_2);
		for (int i = 0; i < LICZ_ITER; i++) {
			faktSygWy3[i] = tree_b2->symuluj(sygWe[i]);
			//std::cout << "tree " << i + 1 << " y= " << faktSygWy2[i] << std::endl;
		}
		// Walidacja poprawności i raport:
		if (porownanieSekwencji(spodzSygWy, faktSygWy3))
			std::cerr << "OK3 - ParallerObject!\n";
		else
		{
			std::cerr << "FAIL!\n";
			raportBleduSekwencji(spodzSygWy, faktSygWy3);
		}

	}
	catch (...)
	{
		std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
	}
}

void test_ModelARX_skokJednostkowy_2()
{
	//Sygnatura testu:
	std::cerr << "ModelARX (-0.4 | 0.6 | 2 | 0 ) -> test skoku jednostkowego nr 2: ";

	try
	{
		// Przygotowanie danych:
		ModelARX instancjaTestowa({ -0.4 }, { 0.6 }, 2, 0);
		constexpr size_t LICZ_ITER = 30;
		std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu, 
		std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy
		std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy
		std::vector<double> faktSygWy2(LICZ_ITER);  // faktyczna sekwencja wy
		std::vector<double> faktSygWy3(LICZ_ITER);  // faktyczna sekwencja wy

		// Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
		for (int i = 0; i < LICZ_ITER; i++)
			sygWe[i] = !!i;
		spodzSygWy = { 0, 0, 0, 0.6, 0.84, 0.936, 0.9744, 0.98976, 0.995904, 0.998362, 0.999345, 0.999738, 0.999895, 0.999958, 0.999983, 0.999993, 0.999997, 0.999999, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

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

		//Teraz test z szeregowe/równoległe:
		// Symulacja modelu:
		Komponent* tree_d = new SerialObject;// SerialObject;ParallerObject
		Komponent* leaf_1a = new ModelARX({ -0.4 }, { 0.6 }, 2, 0);
		tree_d->Add(leaf_1a);
		for (int i = 0; i < LICZ_ITER; i++) {
			faktSygWy2[i] = tree_d->symuluj(sygWe[i]);
			//std::cout << "tree " << i + 1 << " y= " << faktSygWy2[i] << std::endl;
		}
		// Walidacja poprawności i raport:
		if (porownanieSekwencji(spodzSygWy, faktSygWy2))
			std::cerr << "OK2 - SerialObject!\n";
		else
		{
			std::cerr << "FAIL!\n";
			raportBleduSekwencji(spodzSygWy, faktSygWy2);
		}
		Komponent* tree_b3 = new ParallerObject;// SerialObject;ParallerObject
		Komponent* leaf_3 = new ModelARX({ -0.4 }, { 0.6 }, 2, 0);
		tree_b3->Add(leaf_3);
		for (int i = 0; i < LICZ_ITER; i++) {
			faktSygWy3[i] = tree_b3->symuluj(sygWe[i]);
			//std::cout << "tree " << i + 1 << " y= " << faktSygWy2[i] << std::endl;
		}
		// Walidacja poprawności i raport:
		if (porownanieSekwencji(spodzSygWy, faktSygWy3))
			std::cerr << "OK3 - ParallerObject!\n";
		else
		{
			std::cerr << "FAIL!\n";
			raportBleduSekwencji(spodzSygWy, faktSygWy3);
		}

	}
	catch (...)
	{
		std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
	}
}

void test_ModelARX_skokJednostkowy_3()
{
	//Sygnatura testu:
	std::cerr << "ModelARX (-0.4, 0.2 | 0.6, 0.3 | 2 | 0 ) -> test skoku jednostkowego nr 3: ";
	try
	{
		// Przygotowanie danych:
		ModelARX instancjaTestowa({ -0.4,0.2 }, { 0.6, 0.3 }, 2, 0);
		constexpr size_t LICZ_ITER = 30;
		std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu, 
		std::vector<double> spodzSygWy(LICZ_ITER); // spodziewana sekwencja wy
		std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy
		std::vector<double> faktSygWy2(LICZ_ITER);  // faktyczna sekwencja wy
		std::vector<double> faktSygWy3(LICZ_ITER);  // faktyczna sekwencja wy

		// Symulacja skoku jednostkowego w chwili 1. (!!i - daje 1 dla i != 0);
		for (int i = 0; i < LICZ_ITER; i++)
			sygWe[i] = !!i;
		spodzSygWy = { 0, 0, 0, 0.6, 1.14, 1.236, 1.1664, 1.11936, 1.11446, 1.12191, 1.12587, 1.12597, 1.12521, 1.12489, 1.12491, 1.12499, 1.12501, 1.12501, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125, 1.125 };

		// Symulacja modelu:

		//Teraz test z szeregowe/równoległe:
		// Symulacja modelu:
		Komponent* tree_e = new SerialObject;// SerialObject;ParallerObject
		Komponent* leaf_1b = new ModelARX({ -0.4,0.2 }, { 0.6, 0.3 }, 2, 0);
		tree_e->Add(leaf_1b);
		for (int i = 0; i < LICZ_ITER; i++) {
			faktSygWy2[i] = tree_e->symuluj(sygWe[i]);
			//std::cout << "tree " << i + 1 << " y= " << faktSygWy2[i] << std::endl;
		}
		// Walidacja poprawności i raport:
		if (porownanieSekwencji(spodzSygWy, faktSygWy2))
			std::cerr << "OK2 - SerialObject!\n";
		else
		{
			std::cerr << "FAIL!\n";
			raportBleduSekwencji(spodzSygWy, faktSygWy2);
		}
		Komponent* tree_b4 = new ParallerObject;// SerialObject;ParallerObject
		Komponent* leaf_4 = new ModelARX({ -0.4,0.2 }, { 0.6, 0.3 }, 2, 0);
		tree_b4->Add(leaf_4);
		for (int i = 0; i < LICZ_ITER; i++) {
			faktSygWy3[i] = tree_b4->symuluj(sygWe[i]);
			//std::cout << "tree " << i + 1 << " y= " << faktSygWy2[i] << std::endl;
		}
		// Walidacja poprawności i raport:
		if (porownanieSekwencji(spodzSygWy, faktSygWy3))
			std::cerr << "OK3 - ParallerObject!\n";
		else
		{
			std::cerr << "FAIL!\n";
			raportBleduSekwencji(spodzSygWy, faktSygWy3);
		}
	}
	catch (...)
	{
		std::cerr << "INTERUPTED! (niespodziwany wyjatek)\n";
	}
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
void symulacja_petli_ARX(){
  std::cerr << "symulacja dla ARX:\n";

	constexpr size_t LICZ_ITER = 30;
	std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu  - wartosc zadana
	std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy - wyjscie - wartosc regulowana
	
	std::vector<double> wynik(LICZ_ITER);  // faktyczna sekwencja wy - wyjscie - wartosc regulowana
	std::vector<double> wynikB(LICZ_ITER);  // faktyczna sekwencja wy - wyjscie - wartosc regulowana
	std::vector<double> wynikC(LICZ_ITER);  // faktyczna sekwencja wy - wyjscie - wartosc regulowana
	

	//RegulatorPID i ModelARX
	//RegulatorPID regulator(0.5);
	RegulatorPID regulator(0.5, 2.0);
	//RegulatorPID regulator(0.4, 2.0, 0.2);
	ModelARX obiekt({ -0.4,0.2 }, { 0.6, 0.3 }, 2, 0);
	ModelARX obiekt2({ -0.4,0.2 }, { 0.6, 0.3 }, 2, 0);
	ModelARX obiekt3({ -0.4,0.2 }, { 0.6, 0.3 }, 2, 0);
	ModelARX obiekt4({ -0.4,0.2 }, { 0.6, 0.3 }, 2, 0);
	ModelARX obiekt5({ -0.4,0.2 }, { 0.6, 0.3 }, 2, 0);
	ModelARX obiekt6({ -0.4,0.2 }, { 0.6, 0.3 }, 2, 0);

	//wartosc zadana = 1.
	for (int i = 0; i < LICZ_ITER; i++)
		sygWe[i] = !!i; //sygWe[i] = 1;

	PetlaJedenKrok petla;
	// Symulacja petli:
	//for (int i = 0; i < LICZ_ITER; i++)
	//	faktSygWy[i] = petla(regulator, obiekt, sygWe[i]);

	//wynik
	//for (int i = 0; i < LICZ_ITER; i++)
		//std::cout<<"Krok "<<i+1<<" y= "<<faktSygWy[i]<<std::endl;

  //Test ARX Komponent
	//=================================================================================
	//=================================================================================
	//SErial
	std::cerr << "symulacja dla ARX SErial:\n";
	Komponent* tree = new SerialObject;// SerialObject;ParallerObject
	Komponent* leaf1 = new ModelARX({ -0.4,0.2 }, { 0.6, 0.3 }, 2, 0);
	Komponent* leaf2 = new ModelARX({ -0.4,0.2 }, { 0.6, 0.3 }, 2, 0);
	Komponent* leaf3 = new ModelARX({ -0.4,0.2 }, { 0.6, 0.3 }, 2, 0);
	tree->Add(leaf1);
	tree->Add(leaf2);
	tree->Add(leaf3);
	//Bez tej poniżej sym bo to będzie niżej robione. Tutaj tylko tworze tree jako serial, a nizej symulacja
	//for (int i = 0; i < LICZ_ITER; i++) {
	//	wynik[i] = tree->symuluj(sygWe[i]);
	//	std::cout << "tree SErial" << i + 1 << " y= " << wynik[i] << std::endl;
	//}


	//tree->Remove(leaf3);
	//tree->Remove(leaf3);
	//for (int i = 0; i < LICZ_ITER; i++) {
	//	wynik[i] = tree->symuluj(sygWe[i]);
	//	std::cout << "tree rem " << i + 1 << " y= " << wynik[i] << std::endl;
	//}
	

	//std::cerr << "symulacja dla ARX dodawanych po sobie SErial:\n";
	//std::vector<double> wynik2(LICZ_ITER);
	//std::vector<double> wynik3(LICZ_ITER);
	//std::vector<double> wynik4(LICZ_ITER);
	//for (int i = 0; i < LICZ_ITER; i++) {
	//	wynik2[i] = obiekt.symuluj(sygWe[i]);
	//	wynik3[i] = obiekt2.symuluj(wynik2[i]);
	//	wynik4[i] = obiekt3.symuluj(wynik3[i]);
	//	std::cout << "tree 3 SErial " << i + 1 << " y= " << wynik4[i] << std::endl;
	//}
	//for (int i = 0; i < LICZ_ITER; i++) {
	//	wynik2[i] = obiekt.symuluj(sygWe[i]);
	//	wynik3[i] = obiekt2.symuluj(wynik2[i]);
	//	std::cout << "tree 2" << i + 1 << " y= " << wynik3[i] << std::endl;
	//}

	//Paraller
	std::cerr << "symulacja dla ARX Paraller:\n";
	Komponent* tree2 = new ParallerObject;//ParallerObject
	Komponent* leaf4 = new ModelARX({ -0.4,0.2 }, { 0.6, 0.3 }, 2, 0);
	Komponent* leaf5 = new ModelARX({ -0.4,0.2 }, { 0.6, 0.3 }, 2, 0);
	Komponent* leaf6 = new ModelARX({ -0.4,0.2 }, { 0.6, 0.3 }, 2, 0);
	tree2->Add(leaf4);
	tree2->Add(leaf5);
	tree2->Add(leaf6);
	//for (int i = 0; i < LICZ_ITER; i++) {
	//	wynikB[i] = tree2->symuluj(sygWe[i]);
	//	std::cout << "tree Paraller" << i + 1 << " y= " << wynikB[i] << std::endl;
	//}

	//std::cerr << "symulacja dla ARX dodawanych po sobie ParallerObject:\n";
	//std::vector<double> wynik5(LICZ_ITER);
	//std::vector<double> wynik6(LICZ_ITER);
	//std::vector<double> wynik7(LICZ_ITER);
	//for (int i = 0; i < LICZ_ITER; i++) {
	//	wynik5[i] = obiekt4.symuluj(sygWe[i]);
	//	wynik6[i] = obiekt5.symuluj(sygWe[i]);
	//	wynik7[i] = obiekt6.symuluj(sygWe[i]);
	//	std::cout << "tree 3 ParallerObject " << i + 1 << " y= " << wynik5[i] + wynik6[i] + wynik7[i] << std::endl;
	//}
	//symulacja serial koncowy
	//std::vector<double> wynik8(LICZ_ITER);
	//for (int i = 0; i < LICZ_ITER; i++) {
	//	wynik5[i] = obiekt4.symuluj(sygWe[i]);
	//	wynik6[i] = obiekt5.symuluj(sygWe[i]);
	//	wynik7[i] = obiekt6.symuluj(sygWe[i]);
	//	//std::cout << "tree 3 ParallerObject " << i + 1 << " y= " << wynik5[i] + wynik6[i] + wynik7[i] << std::endl;
	//	wynik2[i] = obiekt.symuluj(wynik5[i] + wynik6[i] + wynik7[i]);
	//	wynik3[i] = obiekt2.symuluj(wynik2[i]);
	//	wynik4[i] = obiekt3.symuluj(wynik3[i]);
	//	std::cout << "tree 3 SErial " << i + 1 << " y= " << wynik4[i] << std::endl;
	//}


	Komponent* leaf7 = new ModelARX({ -0.4,0.2 }, { 0.6, 0.3 }, 2, 0);
	//Koncowe połączenie
	Komponent* model = new SerialObject;
	model->Add(tree);//to jest serial
	model->Add(tree2);//to jest paraller
	model->Add(leaf7);
	for (int i = 0; i < LICZ_ITER; i++) {
		wynikC[i] = model->symuluj(sygWe[i]);
		std::cout << "tree model " << i + 1 << " y= " << wynikC[i] << std::endl;
	}
	model->Remove(tree2);
	model->Remove(leaf7);
	for (int i = 0; i < LICZ_ITER; i++) {
		wynikC[i] = model->symuluj(sygWe[i]);
		std::cout << "tree model 2 " << i + 1 << " y= " << wynikC[i] << std::endl;
	}

	//=================================================================================
	//=================================================================================


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
	//test_symulacja_sygnalow();
	//SerializacjaDeserializacja
	//test_ser_des();
	//Test ukladu z poprzedniego lab
	//symulacja_ukladu_petli();

  //========================= lab4 ==============================
	test_ModelARX_brakPobudzenia();
	test_ModelARX_skokJednostkowy_1();
	test_ModelARX_skokJednostkowy_2();
	test_ModelARX_skokJednostkowy_3();

	symulacja_petli_ARX();

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
g++ lab4.cpp ModelARX.cpp RegulatorPID.cpp PetlaJedenKrok.cpp WartoscStalaGenerator.cpp TrojkatGenerator.cpp 
SzumBialyGenerator.cpp SygnalGenerator.cpp SygnalDekorator.cpp SinusGenerator.cpp ProstokatGenerator.cpp OgranicznikAmplitudy.cpp 
Komponent.cpp ParallerObject.cpp SerialObject.cpp
./a.out 

*/