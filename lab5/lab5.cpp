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

#include <cstdio>


#include <iostream>
#include <fstream>
#include "json.hpp"
using json = nlohmann::json;

#include <iostream>
#include <filesystem>

#include <cstddef> 

#ifdef DEBUG


//===================================================================================== lab5 ==========================================================================================


void wypiszZJson(const json& j, int poziom = 0) {
	// Wcięcie dla estetyki drzewa
	std::string wciecie(poziom * 2, ' ');

	std::string typ = j.at("Typ");
	std::cout << wciecie << typ;

	// Wypisz parametry
	if (typ == "WartoscStalaGenerator") {
		std::cout << " (Poziom = " << j.at("Poziom") << ")";
	}
	else if (typ == "SinusGenerator") {
		std::cout << " (A = " << j.at("A") << ", F = " << j.at("F") << ")";
	}
	else if (typ == "TrojkatGenerator") {
		std::cout << " (A = " << j.at("A") << ", F = " << j.at("F") << ")";
	}
	else if (typ == "ProstokatGenerator") {
		std::cout << " (A = " << j.at("A") << ", F = " << j.at("F") << ", Wyp = " << j.at("Wyp") << ")";
	}
	else if (typ == "SzumBialyGenerator") {
		std::cout << " (A = " << j.at("A") << ")";
	}
	else if (typ == "OgranicznikAmplitudy") {
		std::cout << " (A = " << j.at("A") << ")";
	}

	std::cout << "\n";

	// Jeżeli ma bazę –rekurencyjnie
	if (j.contains("Baza")) {
		wypiszZJson(j.at("Baza"), poziom + 1);
	}
}



using namespace std;

namespace fs = std::filesystem;

int main()
{
	//========================= lab5 ==============================
	//Domyślne wartości
	SygnalGenerator* sygnal = nullptr;
	RegulatorPID regulator(1.0, 0.0, 0.0);
	Komponent* baza = new SerialObject;//Główny model ARX w którym będą łączone inne.

	PetlaJedenKrok petla;

	int wybor_glowny = 0;
	int wybor_syg = 0;
	int wybor_syg_do_generacji = 0;
	int wybor_PID = 0;
	int wybor_ARX = 0;
	int wybor_ARX_tworzenie = 0;

	std::string nazwa_pliku = "";

	//Dla sygnalow:
	double stala_new = 0.0;
	double sin_A_new = 0.0;
	double sin_F_new = 0.0;
	double trojkat_A_new = 0.0;
	double trojkat_F_new = 0.0;
	double prostokat_A_new = 0.0;
	double prostokat_F_new = 0.0;
	double prostokat_wyp_new = 0.0;
	double szum_A_new = 0.0;
	double ogranicznik_A_new = 0.0;

	//Dla PID:
	double regulator_P = 1.0;
	double regulator_I = 0.0;
	double regulator_D = 0.0;

	//Dla petli
	// Domyślne ustawienia
	int liczbaKrokow = 30;
	std::string nazwaPliku = "wyniki_symulacji.txt";

	std::ofstream plikStart("wyniki_symulacji.txt", std::ios::trunc); // TRUNC = zeruj plik
	plikStart.close();

	//Dla ARX:
	SerialObject* glownyModel = nullptr; // Bedzie uzywany w symulacji
	
	std::vector<ParallerObject*> wszystkiePolaczenia;
	std::vector<SerialObject*> wszystkieSzeregowe;
	 
	//std::vector<ModelARX*> modele;
	std::vector<std::pair<std::string, ModelARX*>> modele;
	//Jesli juz sa jakies modele w plikach to zapisac je
	for (const auto& entry : fs::directory_iterator(".")) {
		const std::string nazwaPliku = entry.path().filename().string();
		if (nazwaPliku.find("Obiekt") == 0 && nazwaPliku.find(".txt") != std::string::npos) {
			ModelARX* nowyModel = new ModelARX(nazwaPliku);
			modele.emplace_back(nazwaPliku, nowyModel);
		}
	}


	while (true) {
		std::cout << "=========Menu Glowne========\n1. Generuj Sygnal\n2. Paramtetry Regulatora PID\n3. Parametry Modelu ARX\n4. Uruchom Symulacje\n5. Zapisz do pliku\n6. Zakoncz\n";
		std::cout << "Wybor: "; std::cin >> wybor_glowny;
		wybor_PID = 0;
		wybor_ARX = 0;
		switch (wybor_glowny) {
		case 1:
			//Generuj sygnał
			//===================================================  Generuj sygnał  =========================================================
			do {
				std::cout << "=========Generuj Sygnal========\n1. Pokaz\n2. Stworz\n3. Wczytaj z pliku\n4. Powrot\n";
				std::cout << "Wybor: "; std::cin >> wybor_syg;

				wybor_syg_do_generacji = 0;
				//GenSygnWej();
				
				switch (wybor_syg) {
				case 1: {
					//===================================================  Pokaż z pliku  =========================================================
					//Pokaż z pliku
					std::ifstream plik("Sygnaly_lab5.txt");
					if (!plik.is_open()) {
						std::cerr << "Nie mozna otworzyc pliku. Najpierw wygeneruj SYGNAL\n";
						break;
					}

					json j;
					plik >> j;
					plik.close();

					std::cout << "Struktura sygnalu:\n";
					wypiszZJson(j);
					break; }
					  //===================================================  END Pokaż z pliku  =========================================================
				case 2:
					//Zapisz pokolei - nowy sygnał + do pliku
					//===================================================  nowy sygnał  =========================================================
					
					//Wartosc stala
					std::cout << "Wartosc Stala "; 
					std::cout << "Wybor: "; std::cin >> stala_new;
					sygnal = new WartoscStalaGenerator(stala_new);

					while (wybor_syg_do_generacji != 5) {
						std::cout << "=========Generuj Sygnal========\n1. Sinus\n2. Trojkat\n3. Prostokat\n4. Szum\n5. Ograniczenie Amplitudy(Zakonczenie)\n";
						std::cout << "Wybor: "; std::cin >> wybor_syg_do_generacji;

						switch (wybor_syg_do_generacji) {
						case 1:{
							//Wartosc Sinus
							std::cout << "Sinus \n";
							std::cout << "A (np. 3.0) \n";
							std::cout << "Wybor: "; std::cin >> sin_A_new;
							std::cout << "f w Hz (wczesniej 0.05) \n";
							std::cout << "Wybor: "; std::cin >> sin_F_new;
							double okres_sin = 1.0 / sin_F_new;
							sygnal = new SinusGenerator(sygnal, sin_A_new, okres_sin);

							break; }
						case 2:{
							//Wartosc trojkat
							std::cout << "trojkat \n";
							std::cout << "A (np. 3.0) \n";
							std::cout << "Wybor: "; std::cin >> trojkat_A_new;
							std::cout << "f w Hz (wczesniej 0.05) \n";
							std::cout << "Wybor: "; std::cin >> trojkat_F_new;
							double okres_troj = 1.0 / trojkat_F_new;
							sygnal = new TrojkatGenerator(sygnal, trojkat_A_new, okres_troj);

							break; }
						case 3: {
							//Wartosc Prostokat
							std::cout << "Prostokat \n";
							std::cout << "A (np. 3.0) \n";
							std::cout << "Wybor: "; std::cin >> prostokat_A_new;
							std::cout << "f w Hz (wczesniej 0.05) \n";
							std::cout << "Wybor: "; std::cin >> prostokat_F_new;
							std::cout << "wyp (np. 0.5) \n";
							std::cout << "Wybor: "; std::cin >> prostokat_wyp_new;
							double okres_pros = 1.0 / prostokat_F_new;
							sygnal = new ProstokatGenerator(sygnal, prostokat_A_new, okres_pros, prostokat_wyp_new);

							break; }
						case 4:
							//Wartosc Szum 
							std::cout << "Szum \n";
							std::cout << "A (np. 3.0) \n";
							std::cout << "Wybor: "; std::cin >> szum_A_new;

							sygnal = new SzumBialyGenerator(sygnal, szum_A_new);

							break;
						case 5:
							//Ograniczenie A
							std::cout << "Ograniczenie A \n";
							std::cout << "A (np. 3.0) \n";
							std::cout << "Wybor: "; std::cin >> ogranicznik_A_new;

							sygnal = new OgranicznikAmplitudy(sygnal, ogranicznik_A_new);
							sygnal->zapiszDoPliku("Sygnaly_lab5.txt");

							
							break;
						default:
							std::cout << "Nieznana opcja, sprobuj ponownie.\n";
						}
					}

					break;
					//===================================================  END nowy sygnał  =========================================================
				case 3:
					//Wczytaj sygnał z pliku
					//===================================================  Wczytaj sygnał z pliku  =========================================================
					std::cout << "=========Wczytaj Sygnal z pliku========\n";
					std::cout << "Podaj nazwe pliku ( np. plik.txt): "; std::cin >> nazwa_pliku;
					sygnal = SygnalGenerator::odczytZPliku(nazwa_pliku);
					sygnal->odczytZPliku(nazwa_pliku);//odczyt

					/*for (int i = 0; i < 30; ++i) {
						double sample = sygnal->generuj();
						std::cout << sample << std::endl;
					}*/

					sygnal->zapiszDoPliku("Sygnaly_lab5.txt");//zapis do roboczego pliku

					break;
					//===================================================  END Wczytaj sygnał z pliku  =========================================================
				case 4:
					std::cout << "Powrot do menu glownego...\n";
					break;
				default:
					std::cout << "Nieznana opcja, sprobuj ponownie.\n";
				}
			} while (wybor_syg != 4);
			
			
			
			break;
			//===================================================  END Generuj sygnał  =========================================================
		case 2:
			//Regulator PID
			//===================================================  Regulator PID  =========================================================
			while (wybor_PID != 4) {
				std::cout << "=========Wybierz PID========\n1. Pokaz\n2. Ustaw\n3. Reset (pamieci)\n4. Powrot\n";
				std::cout << "Wybor: "; std::cin >> wybor_PID;

				switch (wybor_PID) {
				case 1:
					//Pokaz PID
					std::cout << "Regulator PID\n";
					std::cout << "P " << regulator_P;
					std::cout << "\nI " << regulator_I;
					std::cout << "\nD " << regulator_D << std::endl;

					break;
				case 2:
					//Ustaw PID
					std::cout << "Regulator PID\n";
					std::cout << "P " << regulator_P;
					std::cout << "\nWybor: "; std::cin >> regulator_P;
					std::cout << "\nI " << regulator_I;
					std::cout << "\nWybor: "; std::cin >> regulator_I;
					std::cout << "\nD " << regulator_D;
					std::cout << "Wybor: "; std::cin >> regulator_D;

					regulator.setK(regulator_P);
					regulator.setTI(regulator_I);
					regulator.setTD(regulator_D);
					//regulator = RegulatorPID(regulator_P, regulator_I, regulator_D);

					break;
				case 3:
					//Reset
					regulator_P = 1.0;
					regulator_I = 0.0;
					regulator_D = 0.0;
					regulator = RegulatorPID(1.0, 0.0, 0.0);
					std::cout << "Reset PID\n";
					break;
				case 4:
					//Powrót
					std::cout << "Powrot do menu glownego...\n";
					break;
				default:
					std::cout << "Nieznana opcja, sprobuj ponownie.\n";
				}
			}
			break;
			//===================================================  END Regulator PID  =========================================================
		case 3:
			//Model ARX
			//===================================================  Model ARX  =========================================================
			//Do wektora przechowywanie obiektów / szeregowych modeli / rownoleglych modeli
			while (wybor_ARX != 8) {
				std::cout << "=========Wybierz ARX========\n1. Pokaz\n2. Edytuj model ARX\n3. Dodaj model ARX\n4. Dodaj szeregowo\n5. Dodaj rownolegle\n6. Stworz finalny MODEL ARX\n7. Modyfikacje\n8. Powrot\n";
				std::cout << "Wybor: "; std::cin >> wybor_ARX;

				switch (wybor_ARX) {
				case 1:
					//Pokaz ARX
					//=================================================== Pokaz Modele ARX  =========================================================
					if (modele.empty()) {
						std::cout << "Brak modeli ARX.\n";
					}
					else {
						for (size_t i = 0; i < modele.size(); ++i) {
							std::cout << "Model #" << i + 1 << " (z pliku: " << modele[i].first << "):\n";
							std::ifstream plik(modele[i].first);
							json j;
							plik >> j;
							//A
							std::cout << "A[";
							for (const auto& a : j["A"]) {
								std::cout << a.get<double>() << " ";
							}
							std::cout << "] ";

							//B
							std::cout << "B[";
							for (const auto& b : j["B"]) {
								std::cout << b.get<double>() << " ";
							}
							std::cout << "] ";

							//opóźnienie i odchylenie
							std::cout << "k=" << j["opoznienie"].get<int>() << " ";
							std::cout << "odch=" << j["odchylenie"].get<double>();

							std::cout << "\n";
						}
					}

					//Wyswietl polaczenia rownolegle
					if (wszystkiePolaczenia.empty()) {
						std::cout << "\nBrak polaczen rownoleglych.\n";
					}
					else {
						std::cout << "\nPolaczenia rownolegle:\n";
						for (size_t i = 0; i < wszystkiePolaczenia.size(); ++i) {
							std::cout << "Parallel" << i + 1 << " ("
								<< wszystkiePolaczenia[i]->getChildrenCount() << " modele):\n";

							for (size_t j = 0; j < wszystkiePolaczenia[i]->getChildrenCount(); ++j) {
								Komponent* dziecko = wszystkiePolaczenia[i]->getChild(j);
								//nazwa pliku
								for (const auto& m : modele) {
									if (m.second == dziecko) {
										std::cout << "  - " << m.first << "\n";
										break;
									}
								}
							}
						}
					}

					//Wyswietl polaczenia szeregowe
					if (wszystkieSzeregowe.empty()) {
						std::cout << "\nBrak polaczen szeregowych.\n";
					}
					else {
						std::cout << "\nPolaczenia szeregowe:\n";
						for (size_t i = 0; i < wszystkieSzeregowe.size(); ++i) {
							std::cout << "Serial" << i + 1 << " ("
								<< wszystkieSzeregowe[i]->getChildrenCount() << " modele):\n";

							for (size_t j = 0; j < wszystkieSzeregowe[i]->getChildrenCount(); ++j) {
								Komponent* dziecko = wszystkieSzeregowe[i]->getChild(j);
								// nazwa pliku
								for (const auto& m : modele) {
									if (m.second == dziecko) {
										std::cout << "  - " << m.first << "\n";
										break;
									}
								}
							}
						}
					}

					// 4. Wyswietl koncowy model (jesli istnieje)
					if (glownyModel != nullptr) {
						std::cout << "\nKoncowy model szeregowy:\n";
						for (size_t i = 0; i < glownyModel->getChildrenCount(); ++i) {
							Komponent* dziecko = glownyModel->getChild(i);

							// Sprawdz typ komponentu
							if (auto model = dynamic_cast<ModelARX*>(dziecko)) {
								for (const auto& m : modele) {
									if (m.second == model) {
										std::cout << " - " << m.first << " (pojedynczy)\n";
										break;
									}
								}
							}
							else if (auto paraller = dynamic_cast<ParallerObject*>(dziecko)) {
								// Znajdz indeks tego polaczenia
								size_t idx = 0;
								for (; idx < wszystkiePolaczenia.size(); ++idx) {
									if (wszystkiePolaczenia[idx] == paraller) break;
								}
								std::cout << " - Paraller" << idx + 1 << " (rownolegly)\n";
							}
							else if (auto serial = dynamic_cast<SerialObject*>(dziecko)) {
								// Znajdz indeks tego polaczenia
								size_t idx = 0;
								for (; idx < wszystkieSzeregowe.size(); ++idx) {
									if (wszystkieSzeregowe[idx] == serial) break;
								}
								std::cout << " - Serial" << idx + 1 << " (szeregowy)\n";
							}
						}
					}
					else {
						std::cout << "\nBrak koncowego modelu.\n";
					}
					break;
					//=================================================== Pokaz Modele ARX  =========================================================
				case 2: {
					//===================================================  Edytuj Model ARX  =========================================================
					if (modele.empty()) {
						std::cout << "Brak modeli ARX do modyfikacji!\n";
						break;
					}

					// Wyświetl dostępne modele
					std::cout << "\nDostepne modele ARX:\n";
					for (size_t i = 0; i < modele.size(); ++i) {
						std::cout << i + 1 << ". " << modele[i].first;

						// Sprawdź czy model jest używany w jakimś połączeniu
						bool uzywany = false;

						// Sprawdz w rownoleglych
						for (auto pol : wszystkiePolaczenia) {
							for (size_t j = 0; j < pol->getChildrenCount(); ++j) {
								if (pol->getChild(j) == modele[i].second) {
									uzywany = true;
									break;
								}
							}
							if (uzywany) break;
						}

						// Sprawdz w szeregowych
						if (!uzywany) {
							for (auto pol : wszystkieSzeregowe) {
								for (size_t j = 0; j < pol->getChildrenCount(); ++j) {
									if (pol->getChild(j) == modele[i].second) {
										uzywany = true;
										break;
									}
								}
								if (uzywany) break;
							}
						}

						// Sprawdz w modelu końcowym
						if (!uzywany && glownyModel != nullptr) {
							for (size_t j = 0; j < glownyModel->getChildrenCount(); ++j) {
								if (glownyModel->getChild(j) == modele[i].second) {
									uzywany = true;
									break;
								}
							}
						}

						if (uzywany) {
							std::cout << " (uzywany)";
						}
						std::cout << "\n";
					}

					// Wybór modelu
					std::cout << "Wybierz model do modyfikacji (0 aby anulowac): ";
					size_t wybor;
					std::cin >> wybor;

					if (wybor == 0) break;
					if (wybor < 1 || wybor > modele.size()) {
						std::cout << "Nieprawidlowy wybor!\n";
						break;
					}

					ModelARX* model = modele[wybor - 1].second;
					std::string nazwaPliku = modele[wybor - 1].first;

					// Odczytaj parametry z pliku
					std::ifstream plik(nazwaPliku);
					if (!plik.is_open()) {
						std::cout << "Nie mozna otworzyc pliku do odczytu!\n";
						break;
					}

					// Wyświetl aktualne parametry z pliku
					std::cout << "\nAktualne parametry modelu " << nazwaPliku << ":\n";
					std::string linia;
					while (std::getline(plik, linia)) {
						std::cout << linia << "\n";
					}
					plik.close();
					bool zmieniono = false;
					while (true) {
						std::cout << "\nOpcje modyfikacji:\n"
							<< "1. Zmien wspolczynniki A\n"
							<< "2. Zmien wspolczynniki B\n"
							<< "3. Zmien opoznienie\n"
							<< "4. Zmien odchylenie\n"
							<< "5. Zapisz zmiany i wyjdz\n"
							<< "Wybierz opcje: ";

						int opcja;
						std::cin >> opcja;

						if (opcja == 5) { //|| opcja == 6
							if (opcja == 5 && zmieniono) {
								// Zapisz zmiany do pliku
								model->zapiszDoPliku(nazwaPliku);
								std::cout << "Zapisano zmiany do pliku " << nazwaPliku << "\n";
							}
							break;
						}

						switch (opcja) {
						case 1: { // Zmiana A
							std::cout << "Podaj nowe wspolczynniki A (po spacji, zakoncz dowolnym znakiem (a)):\n";
							std::vector<double> noweA;
							double wartosc;
							while (std::cin >> wartosc) {
								noweA.push_back(wartosc);
							}
							std::cin.clear();
							std::cin.ignore(1000, '\n');

							if (!noweA.empty()) {
								
								model->setA(noweA);
								zmieniono = true;
								std::cout << "Zaktualizowano A.\n";
							}
							else {
								std::cout << "Brak danych - pozostawiono stare wartosci.\n";
							}
							break;
						}
						case 2: { // Zmiana B
							std::cout << "Podaj nowe wspolczynniki B (po spacji, zakoncz dowolnym znakiem):\n";
							std::vector<double> noweB;
							double wartosc;
							while (std::cin >> wartosc) {
								noweB.push_back(wartosc);
							}
							std::cin.clear();
							std::cin.ignore(1000, '\n');

							if (!noweB.empty()) {
								
								model->setB(noweB);
								zmieniono = true;
								std::cout << "Zaktualizowano B.\n";
							}
							else {
								std::cout << "Brak danych - pozostawiono stare wartosci.\n";
							}
							break;
						}
						case 3: { // Zmiana opóźnienia
							std::cout << "Podaj nowe opoznienie (>=1): ";

							unsigned int noweOpoznienie;
							if (std::cin >> noweOpoznienie) {
								if (noweOpoznienie >= 1) {
									
									model->setOpoznienieTransportowe(noweOpoznienie);
									zmieniono = true;
									std::cout << "Zaktualizowano opoznienie.\n";
								}
								else {
									std::cout << "Nieprawidlowa wartosc! Pozostawiono stare opoznienie.\n";
								}
							}
							else {
								std::cin.clear();
								std::cin.ignore(1000, '\n');
								std::cout << "Blad wejscia! Pozostawiono stare opoznienie.\n";
							}
							break;
						}
						case 4: { // Zmiana odchylenia
							std::cout << "Podaj nowe odchylenie: ";

							double noweOdchylenie;
							if (std::cin >> noweOdchylenie) {
								
								model->setOdchylenieStandardowe(noweOdchylenie);
								zmieniono = true;
								std::cout << "Zaktualizowano odchylenie.\n";
							}
							else {
								std::cin.clear();
								std::cin.ignore(1000, '\n');
								std::cout << "Blad wejscia! Pozostawiono stare odchylenie.\n";
							}
							break;
						}
						default:
							std::cout << "Nieprawidlowa opcja!\n";
						}
					}


					//===================================================  END Edytuj Model ARX  =========================================================
					break; }
				case 3:
					//Stworz ARX
					//===================================================  Stworz Model ARX  =========================================================
					while (wybor_ARX_tworzenie != 3) {
						std::cout << "=== Tworzenie modelu ARX ===\n";
						std::cout << "1. Stworz nowy model ARX\n2. Wczytaj model ARX z pliku\n3. Powrot\n";
						std::cout << "Wybor: ";	std::cin >> wybor_ARX_tworzenie;

						std::vector<double> wspA;
						std::vector<double> wspB;

						switch (wybor_ARX_tworzenie) {
						case 1: {
							//nowy model
							size_t rzadA, rzadB, opoznienie;
							double odchylenie;

							std::cout << "Podaj rzad A (np 2): ";
							std::cin >> rzadA;
							std::cout << "Podaj rzad B (np 2): ";
							std::cin >> rzadB;
							std::cout << "Podaj opoznienie (k) (np 2): ";
							std::cin >> opoznienie;
							std::cout << "Podaj odchylenie (np 0.0): ";
							std::cin >> odchylenie;


							std::cout << "Podaj wspolczynniki A:\n";
							for (size_t i = 0; i < rzadA; ++i) {
								double vala;
								std::cin >> vala;
								wspA.push_back(vala);
							}

							std::cout << "Podaj wspolczynniki B:\n";
							for (size_t i = 0; i < rzadB; ++i) {
								double valb;
								std::cin >> valb;
								wspB.push_back(valb);
							}

							ModelARX* nowyModel = new ModelARX(wspA, wspB, opoznienie, odchylenie);
							std::cout << "Dodano model ARX nr " << modele.size() + 1 << ".\n";
							// Serializacja do pliku
							nowyModel->zapiszDoPliku("Obiekt" + std::to_string(modele.size() + 1) + ".txt");
							std::cout << "Model ARX zostal zapisany do pliku: " << "Obiekt" + std::to_string(modele.size() + 1) + ".txt" << "\n";
							modele.emplace_back("Obiekt" + std::to_string(modele.size() + 1) + ".txt", nowyModel);
							//modele.push_back(nowyModel);

							break; }
					//===================================================  END Stworz Model ARX  =========================================================
						case 2:{
							//z pliku wczytaj
							//===================================================  z pliku wczytaj ARX  =========================================================
							std::cout << "=========Wczytaj Model ARX z pliku========\n";
							std::cout << "Podaj nazwe pliku ( np. plik.txt): "; std::cin >> nazwa_pliku;

							ModelARX* nowyModel_zpliku = new ModelARX(nazwa_pliku);
							std::cout << "Dodano model ARX nr " << modele.size() + 1 << ".\n";
							// Serializacja do pliku
							nowyModel_zpliku->zapiszDoPliku("Obiekt" + std::to_string(modele.size() + 1) + ".txt");
							std::cout << "Model ARX zostal zapisany do pliku: " << "Obiekt" + std::to_string(modele.size() + 1) + ".txt" << "\n";
							modele.emplace_back("Obiekt" + std::to_string(modele.size() + 1) + ".txt", nowyModel_zpliku);

							break; }
							//===================================================  END z pliku wczytaj ARX  =========================================================
						case 3:
							//Powrót
							std::cout << "Powrot...\n";

							break;
						default:
							std::cout << "Nieznana opcja, sprobuj ponownie.\n";
						}
					}
					break;
					//===================================================  END Stworz Model ARX  =========================================================
				case 4:{//Szeregowe
					// Sprawdz czy sa dostepne modele
					if (modele.empty()) {
						std::cout << "Brak modeli do polaczenia!\n";
						break;
					}

					// Znajdz modele nie bedace czescia istniejacych polaczen
					std::vector<ModelARX*> dostepneModele;
					std::vector<bool> czyUzywany(modele.size(), false);

					// Sprawdz ktore modele sa juz w polaczeniach
					for (size_t i = 0; i < modele.size(); ++i) {
						bool jestWPolaczeniu = false;

						// Sprawdz w rownoleglych
						for (auto polaczenie : wszystkiePolaczenia) {
							for (size_t j = 0; j < polaczenie->getChildrenCount(); ++j) {
								if (polaczenie->getChild(j) == modele[i].second) {
									jestWPolaczeniu = true;
									break;
								}
							}
							if (jestWPolaczeniu) break;
						}

						// Sprawdz w szeregowych (nowy kontener)
						if (!jestWPolaczeniu) {
							for (auto polaczenie : wszystkieSzeregowe) {
								for (size_t j = 0; j < polaczenie->getChildrenCount(); ++j) {
									if (polaczenie->getChild(j) == modele[i].second) {
										jestWPolaczeniu = true;
										break;
									}
								}
								if (jestWPolaczeniu) break;
							}
						}

						if (!jestWPolaczeniu) {
							dostepneModele.push_back(modele[i].second);
							czyUzywany[i] = true;
						}
					}

					// Wyswietl dostepne modele
					std::cout << "\nDostepne modele ARX:\n";
					int licznik = 1;
					for (size_t i = 0; i < modele.size(); ++i) {
						if (czyUzywany[i]) {
							std::cout << licznik++ << ". " << modele[i].first << "\n";
						}
					}

					if (dostepneModele.empty()) {
						std::cout << "Wszystkie modele sa juz w polaczeniach!\n";
						break;
					}

					// Utworz nowe polaczenie SZEREGOWE
					SerialObject* nowePolaczenie = new SerialObject();
					std::cout << "\nWybierz numery modeli W KOLEJNOSCI (oddziel spacjami, zakoncz 0): ";

					int wybor;
					while (std::cin >> wybor && wybor != 0) {
						if (wybor > 0 && wybor <= static_cast<int>(dostepneModele.size())) {
							nowePolaczenie->Add(dostepneModele[wybor - 1]);
							std::cout << "Dodano: " << modele[wybor - 1].first << "\n";
						}
						else {
							std::cout << "Nieprawidlowy numer!\n";
						}
					}

					if (nowePolaczenie->getChildrenCount() > 0) {
						// Nadaj nazwe "SerialX" gdzie X to numer kolejny
						std::string nazwa = "Serial" + std::to_string(wszystkieSzeregowe.size() + 1);
						wszystkieSzeregowe.push_back(nowePolaczenie);

						std::cout << "\nUtworzono nowe polaczenie SZEREGOWE: " << nazwa << " zawierajace "
							<< nowePolaczenie->getChildrenCount() << " modele\n";
					}
					else {
						delete nowePolaczenie;
						std::cout << "Anulowano - puste polaczenie.\n";
					}
					break;}
				case 5:{
					//Rownolegle
					// Sprawdz czy sa dostepne modele
					if (modele.empty()) {
						std::cout << "Brak modeli do polaczenia!\n";
						break;
					}

					// Znajdz modele nie bedace czescia zadnych polaczen
					std::vector<ModelARX*> dostepneModele;
					std::vector<bool> czyUzywany(modele.size(), false);

					for (size_t i = 0; i < modele.size(); ++i) {
						bool jestWPolaczeniu = false;

						// Sprawdz w rownoleglych
						for (auto polaczenie : wszystkiePolaczenia) {
							for (size_t j = 0; j < polaczenie->getChildrenCount(); ++j) {
								if (polaczenie->getChild(j) == modele[i].second) {
									jestWPolaczeniu = true;
									break;
								}
							}
							if (jestWPolaczeniu) break;
						}

						// Sprawdz w szeregowych
						if (!jestWPolaczeniu) {
							for (auto polaczenie : wszystkieSzeregowe) {
								for (size_t j = 0; j < polaczenie->getChildrenCount(); ++j) {
									if (polaczenie->getChild(j) == modele[i].second) {
										jestWPolaczeniu = true;
										break;
									}
								}
								if (jestWPolaczeniu) break;
							}
						}

						if (!jestWPolaczeniu) {
							dostepneModele.push_back(modele[i].second);
							czyUzywany[i] = true;
						}
					}


					// Wyświetl dostępne modele
					std::cout << "\nDostepne modele ARX:\n";
					int licznik = 1;
					for (size_t i = 0; i < modele.size(); ++i) {
						if (czyUzywany[i]) {
							std::cout << licznik++ << ". " << modele[i].first << "\n";
						}
					}

					if (dostepneModele.empty()) {
						std::cout << "Wszystkie modele sa juz w polaczeniach!\n";
						break;
					}

					// Utwórz nowe połączenie
					ParallerObject* nowePolaczenie = new ParallerObject();
					std::cout << "\nWybierz numery modeli (oddziel spacjami, zakoncz 0): ";

					int wybor;
					while (std::cin >> wybor && wybor != 0) {
						if (wybor > 0 && wybor <= static_cast<int>(dostepneModele.size())) {
							nowePolaczenie->Add(dostepneModele[wybor - 1]);
							std::cout << "Dodano: " << modele[wybor - 1].first << "\n";
						}
						else {
							std::cout << "Nieprawidlowy numer!\n";
						}
					}

					if (nowePolaczenie->getChildrenCount() > 0) {
						// Nadaj nazwę "ParallelX" gdzie X to numer kolejny
						std::string nazwa = "Parallel" + std::to_string(wszystkiePolaczenia.size() + 1);
						wszystkiePolaczenia.push_back(nowePolaczenie);

						std::cout << "\nUtworzono nowe polaczenie: " << nazwa << " zawierajace "
							<< nowePolaczenie->getChildrenCount() << " modele\n";
					}
					else {
						delete nowePolaczenie;
						std::cout << "Anulowano - puste polaczenie.\n";
					}
					break; }
				case 6: { // Tworzenie koncowego modelu ARX (szeregowy)
					std::vector<Komponent*> wszystkieKomponenty;

					// 1. Dodaj zwykle modele ARX
					std::cout << "\nDostepne pojedyncze modele ARX:\n";
					for (size_t i = 0; i < modele.size(); ++i) {
						bool jestUzyty = false;

						// Sprawdz czy model jest juz w jakims polaczeniu
						for (auto pol : wszystkiePolaczenia) {
							for (size_t j = 0; j < pol->getChildrenCount(); ++j) {
								if (pol->getChild(j) == modele[i].second) {
									jestUzyty = true;
									break;
								}
							}
							if (jestUzyty) break;
						}

						if (!jestUzyty) {
							for (auto pol : wszystkieSzeregowe) {
								for (size_t j = 0; j < pol->getChildrenCount(); ++j) {
									if (pol->getChild(j) == modele[i].second) {
										jestUzyty = true;
										break;
									}
								}
								if (jestUzyty) break;
							}
						}

						if (!jestUzyty) {
							std::cout << wszystkieKomponenty.size() + 1 << ". " << modele[i].first << " (pojedynczy)\n";
							wszystkieKomponenty.push_back(modele[i].second);
						}
					}

					// 2. Dodaj istniejace polaczenia rownolegle
					std::cout << "\nDostepne polaczenia rownolegle:\n";
					for (size_t i = 0; i < wszystkiePolaczenia.size(); ++i) {
						std::cout << wszystkieKomponenty.size() + 1 << ". Parallel" << i + 1 << "\n";
						wszystkieKomponenty.push_back(wszystkiePolaczenia[i]);
					}

					// 3. Dodaj istniejace polaczenia szeregowe
					std::cout << "\nDostepne polaczenia szeregowe:\n";
					for (size_t i = 0; i < wszystkieSzeregowe.size(); ++i) {
						std::cout << wszystkieKomponenty.size() + 1 << ". Serial" << i + 1 << "\n";
						wszystkieKomponenty.push_back(wszystkieSzeregowe[i]);
					}

					if (wszystkieKomponenty.empty()) {
						std::cout << "Brak komponentow do polaczenia!\n";
						break;
					}

					// Utworz koncowy model szeregowy
					SerialObject* koncowyModel = new SerialObject();
					std::cout << "\nWybierz komponenty W KOLEJNOSCI (numery, spacje, 0 konczy): ";

					int wybor;
					while (std::cin >> wybor && wybor != 0) {
						if (wybor > 0 && wybor <= static_cast<int>(wszystkieKomponenty.size())) {
							koncowyModel->Add(wszystkieKomponenty[wybor - 1]);
							std::cout << "Dodano: ";

							// Wyswietl co dodano
							if (dynamic_cast<ParallerObject*>(wszystkieKomponenty[wybor - 1])) {
								std::cout << "Parallel" << wybor << "\n";
							}
							else if (dynamic_cast<SerialObject*>(wszystkieKomponenty[wybor - 1])) {
								std::cout << "Serial" << wybor << "\n";
							}
							else {
								for (const auto& m : modele) {
									if (m.second == wszystkieKomponenty[wybor - 1]) {
										std::cout << m.first << "\n";
										break;
									}
								}
							}
						}
						else {
							std::cout << "Nieprawidlowy numer!\n";
						}
					}

					if (koncowyModel->getChildrenCount() > 0) {
						// Tutaj zapisz koncowyModel jako glowny model do symulacji
						std::cout << "\nUtworzono koncowy model szeregowy z "
							<< koncowyModel->getChildrenCount() << " komponentami\n";

						// Przypisz do zmiennej globalnej/klasy do uzycia w symulacji
						glownyModel = koncowyModel;
					}
					else {
						delete koncowyModel;
						std::cout << "Anulowano - pusty model.\n";
					}
					break;}
				case 7:
					//Modyfikacje
					 // Sprawdz czy sa jakies polaczenia
					if (wszystkiePolaczenia.empty() && wszystkieSzeregowe.empty()) {
						std::cout << "Brak polaczen do modyfikacji!\n";
						break;
					}

					// Wybor typu polaczenia
					std::cout << "\nWybierz typ polaczenia:\n";
					std::cout << "1. Rownolegle\n";
					std::cout << "2. Szeregowe\n";
					std::cout << "Wybierz: ";

					int typ;
					std::cin >> typ;

					if (typ == 1) { // Dla rownoleglych
						if (wszystkiePolaczenia.empty()) {
							std::cout << "Brak polaczen rownoleglych!\n";
							break;
						}

						// Wyswietl dostepne polaczenia rownolegle
						std::cout << "\nDostepne polaczenia rownolegle:\n";
						for (size_t i = 0; i < wszystkiePolaczenia.size(); ++i) {
							std::cout << i + 1 << ". Parallel" << i + 1 << " ("
								<< wszystkiePolaczenia[i]->getChildrenCount() << " dzieci)\n";
						}

						// Wybor polaczenia
						std::cout << "Wybierz polaczenie: ";
						size_t wyborPolaczenia;
						std::cin >> wyborPolaczenia;

						if (wyborPolaczenia < 1 || wyborPolaczenia > wszystkiePolaczenia.size()) {
							std::cout << "Nieprawidlowy wybor!\n";
							break;
						}

						auto polaczenie = wszystkiePolaczenia[wyborPolaczenia - 1];

						// Wyswietl dzieci wybranego polaczenia
						std::cout << "\nDzieci w polaczeniu Parallel" << wyborPolaczenia << ":\n";
						for (size_t i = 0; i < polaczenie->getChildrenCount(); ++i) {
							auto dziecko = polaczenie->getChild(i);

							if (auto model = dynamic_cast<ModelARX*>(dziecko)) {
								// Znajdz nazwe pliku dla modelu ARX
								for (const auto& m : modele) {
									if (m.second == model) {
										std::cout << i + 1 << ". " << m.first << " (pojedynczy)\n";
										break;
									}
								}
							}
							else if (dynamic_cast<ParallerObject*>(dziecko)) {
								std::cout << i + 1 << ". Parallel" << dziecko << "\n";
							}
							else if (dynamic_cast<SerialObject*>(dziecko)) {
								std::cout << i + 1 << ". Serial" << dziecko << "\n";
							}
						}

						// Wybor dziecka do usuniecia
						std::cout << "Wybierz numer dziecka do usuniecia (0 aby anulowac): ";
						size_t wyborDziecka;
						std::cin >> wyborDziecka;

						if (wyborDziecka == 0) break;
						if (wyborDziecka > polaczenie->getChildrenCount()) {
							std::cout << "Nieprawidlowy numer dziecka!\n";
							break;
						}

						// Usun dziecko
						auto dziecko = polaczenie->getChild(wyborDziecka - 1);


						polaczenie->Remove(dziecko);
						std::cout << "Usunieto komponent.\n";

						if (polaczenie->getChildrenCount() == 0) {
							// Usun z listy globalnej
							for (auto it = wszystkiePolaczenia.begin(); it != wszystkiePolaczenia.end(); ++it) {
								if (*it == polaczenie) {
									delete* it; // Zwolnienie pamięci
									wszystkiePolaczenia.erase(it);
									std::cout << "Usunieto puste polaczenie rownolegle.\n";
									break;
								}
							}
						}

					}
					else if (typ == 2) { // Dla szeregowych 
						if (wszystkieSzeregowe.empty()) {
							std::cout << "Brak polaczen szeregowych!\n";
							break;
						}

						std::cout << "\nDostepne polaczenia szeregowe:\n";
						for (size_t i = 0; i < wszystkieSzeregowe.size(); ++i) {
							std::cout << i + 1 << ". Serial" << i + 1 << " ("
								<< wszystkieSzeregowe[i]->getChildrenCount() << " dzieci)\n";
						}

						// Wybor polaczenia
						std::cout << "Wybierz polaczenie: ";
						size_t wyborPolaczenia;
						std::cin >> wyborPolaczenia;

						if (wyborPolaczenia < 1 || wyborPolaczenia > wszystkieSzeregowe.size()) {
							std::cout << "Nieprawidlowy wybor!\n";
							break;
						}

						auto polaczenie = wszystkieSzeregowe[wyborPolaczenia - 1];

						// Wyswietl dzieci wybranego polaczenia
						std::cout << "\nDzieci w polaczeniu Parallel" << wyborPolaczenia << ":\n";
						for (size_t i = 0; i < polaczenie->getChildrenCount(); ++i) {
							auto dziecko = polaczenie->getChild(i);

							if (auto model = dynamic_cast<ModelARX*>(dziecko)) {
								// Znajdz nazwe pliku dla modelu ARX
								for (const auto& m : modele) {
									if (m.second == model) {
										std::cout << i + 1 << ". " << m.first << " (pojedynczy)\n";
										break;
									}
								}
							}
							else if (dynamic_cast<ParallerObject*>(dziecko)) {
								std::cout << i + 1 << ". Parallel" << dziecko << "\n";
							}
							else if (dynamic_cast<SerialObject*>(dziecko)) {
								std::cout << i + 1 << ". Serial" << dziecko << "\n";
							}
						}

						// Wybor dziecka do usuniecia
						std::cout << "Wybierz numer dziecka do usuniecia (0 aby anulowac): ";
						size_t wyborDziecka;
						std::cin >> wyborDziecka;

						if (wyborDziecka == 0) break;
						if (wyborDziecka > polaczenie->getChildrenCount()) {
							std::cout << "Nieprawidlowy numer dziecka!\n";
							break;
						}

						// Usun dziecko
						auto dziecko = polaczenie->getChild(wyborDziecka - 1);
						polaczenie->Remove(dziecko);
						std::cout << "Usunieto komponent.\n";

						if (polaczenie->getChildrenCount() == 0) {
							for (auto it = wszystkieSzeregowe.begin(); it != wszystkieSzeregowe.end(); ++it) {
								if (*it == polaczenie) {
									delete* it;
									wszystkieSzeregowe.erase(it);
									std::cout << "Usunieto puste polaczenie szeregowe.\n";
									break;
								}
							}
						}


					}

					break;
				case 8:
					//Powrót
					std::cout << "Powrot do menu glownego...\n";
					break;
				default:
					std::cout << "Nieznana opcja, sprobuj ponownie.\n";
				}
			}

			break;
			//===================================================  END Model ARX  =========================================================
		case 4: {
			//Symulacja
			if (glownyModel == nullptr) {
				std::cout << "Brak modelu do symulacji! Najpierw utworz koncowy model.\n";
				break;
			}
			if (sygnal == nullptr) {
				std::cout << "Brak generatora sygnalu!\n";
				break;
			}


			
			int wybor_petla = 0;
			while (wybor_petla != 4) {
				std::cout << "\n==== MENU SYMULACJI ====\n"
					<< "1. Uruchom symulacje (" << liczbaKrokow << " krokow)\n"
					<< "2. Ustaw liczbe krokow (aktualnie: " << liczbaKrokow << ")\n"
					<< "3. Ustaw nazwe pliku wyjsciowego (aktualnie: " << nazwaPliku << ")\n"
					<< "4. Powrot do glownego menu\n"
					<< "Wybierz opcje: ";

				std::cin >> wybor_petla;

				switch (wybor_petla) {
				case 1: { // Uruchom symulacje
					//std::vector<double> sygnalWe(liczbaKrokow, 1.0); // Skok jednostkowy
					//std::vector<double> sygnalWy(liczbaKrokow, 0.0);

					std::ofstream plik(nazwaPliku, std::ios::app);
					plik << "krok;sygnal_we;sygnal_wy\n";

					/*PetlaJedenKrok petla;*/
					for (int i = 0; i < liczbaKrokow; i++) {
						double wejscie = sygnal->generuj();
						double sygnalWy = petla(regulator, *glownyModel, wejscie);

						plik << i << ";" << sygnalWy << "\n";
						std::cout << "Krok " << i + 1 << " y= " << sygnalWy << std::endl;
					}
					// Zapis do pliku
					

					plik.close();

					std::cout << "Symulacja zakonczona. Wyniki zapisano w '" << nazwaPliku << "'\n";
					break;
				}
				case 2: { // Ustaw liczbe krokow
					std::cout << "Podaj nowa liczbe krokow: ";
					std::cin >> liczbaKrokow;
					if (liczbaKrokow < 1) {
						std::cout << "Liczba krokow musi byc wieksza od 0!\n";
						liczbaKrokow = 30;
					}
					break;
				}
				case 3: { // Ustaw nazwe pliku
					std::cout << "Podaj nowa nazwe pliku (z rozszerzeniem .txt): ";
					std::cin >> nazwaPliku;
					break;
				}
				case 4: //Powrót
					std::cout << "Powrot...\n";
					break;
				default:
					std::cout << "Nieprawidlowa opcja!\n";
				}
			}

			break; }
		case 5: {
			//Zapisz do pliku Sygnał i kopiowanie wyniku  (model arx przy tworzeniu robi sie plik)


			int wybor_zapis = 0;
			while (wybor_zapis != 3) {
				std::cout << "\n==== Zapisz do pliku ====\n"
					<< "1. Zapisz wygenerowany sygnal\n"
					<< "2. Zapisz wyniki symulacji\n"
					<< "3. Powrot do glownego menu\n"
					<< "Wybierz opcje: ";

				std::cin >> wybor_zapis;

				switch (wybor_zapis) {
				case 1: {
					std::cout << "=========Zapisz Sygnal do pliku ========\n";
					std::cout << "Podaj nazwe pliku ( np. plik.txt): "; std::cin >> nazwa_pliku;
					if (sygnal != nullptr)
						sygnal->zapiszDoPliku(nazwa_pliku);//Zapis
					else
						std::cout << "Najpierw musisz stworzyc sygnal\n";
					break; }
				case 2: {
					std::cout << "=========Zapisz Wyniki do pliku ========\n";
					std::cout << "Podaj nazwe pliku ( np. plik.txt): "; std::cin >> nazwa_pliku;
					if (fs::exists("wyniki_symulacji.txt") && fs::file_size("wyniki_symulacji.txt") > 0) {
						fs::copy_file("wyniki_symulacji.txt", nazwa_pliku, fs::copy_options::overwrite_existing);
						std::cout << "Plik zostal skopiowany do: " << nazwa_pliku << std::endl;
					}
					else
						std::cout << "Najpierw musisz uruchomic symulacje\n";
					break; }
				case 3:
					break;
				default:
					std::cout << "Nieznana opcja.\n";

				}


			}
			break; }
			
		case 6:
			std::cout << "Koniec programu.\n";

			if (std::remove("Sygnaly_lab5.txt") == 0) {
				std::cout << "Plik Sygnaly_lab5.txt usuniety"  << std::endl;
			}
			else {
				std::cout << "Blad usuwania pliku";
			}

			return 0;
		default:
			std::cout << "Nieznana opcja.\n";
		}
	}
}

#endif

#ifdef MAIN

using namespace std;

int main()
{
	/*
  //RegulatorPID testy(5);

	//std::cerr << "symulacja dla ARX Paraller:\n";
	//Komponent* tree2 = new ParallerObject;//ParallerObject
	//Komponent* leaf4 = new ModelARX({ -0.4,0.2 }, { 0.6, 0.3 }, 2, 0);
	//Komponent* leaf5 = new ModelARX({ -0.4,0.2 }, { 0.6, 0.3 }, 2, 0);
	//Komponent* leaf6 = new ModelARX({ -0.4,0.2 }, { 0.6, 0.3 }, 2, 0);
	//tree2->Add(leaf4);
	//tree2->Add(leaf5);
	//tree2->Add(leaf6);
	//std::cout << "Pierwszy model: " << tree2->getChild(0) << "\n";
	//std::cout << "Pierwszy model: " << tree2->getChild(1) << "\n";
	//std::cout << "Pierwszy model: " << tree2->getChild(2) << "\n";
	//std::cout << "Pierwszy model: " << tree2->getChild(3) << "\n";

	////std::cout << "Ile: " << dynamic_cast<ParallerObject*>(tree2)->m_child.size() << "\n";
	//std::cout << "Ile: " << tree2->getChildrenCount() << "\n";

	constexpr size_t LICZ_ITER = 100;
	std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu  - wartosc zadana
	std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy - wyjscie - wartosc regulowana

	//RegulatorPID i ModelARX
	//RegulatorPID regulator(0.5);
	RegulatorPID regulator(1.0, 0.5, 0.1); // 0.5, 2.0);
	//RegulatorPID regulator(0.4, 2.0, 0.2);
	ModelARX obiekt({ 0.4 }, { 0.6 }, 2, 0.0);

	std::cerr << "symulacja dla ARX SErial:\n";
	Komponent* tree = new SerialObject;// SerialObject;ParallerObject
	Komponent* leaf1 = new ModelARX({ -0.4,0.2 }, { 0.6, 0.3 }, 2, 0);
	Komponent* leaf2 = new ModelARX({ -0.4,0.2 }, { 0.6, 0.3 }, 2, 0);
	Komponent* leaf3 = new ModelARX({ -0.4,0.2 }, { 0.6, 0.3 }, 2, 0);
	tree->Add(leaf1);
	tree->Add(leaf2);
	//tree->Add(leaf3);
	std::cerr << "symulacja dla ARX Paraller:\n";
	Komponent* tree2 = new ParallerObject;//ParallerObject
	Komponent* leaf4 = new ModelARX({ -0.4,0.2 }, { 0.6, 0.3 }, 2, 0);
	Komponent* leaf5 = new ModelARX({ -0.4,0.2 }, { 0.6, 0.3 }, 2, 0);
	Komponent* leaf6 = new ModelARX({ -0.4,0.2 }, { 0.6, 0.3 }, 2, 0);
	tree2->Add(leaf4);
	tree2->Add(leaf5);
	tree2->Add(leaf6);

	Komponent* leaf7 = new ModelARX({ -0.4,0.2 }, { 0.6, 0.3 }, 2, 0);
	//Koncowe połączenie
	Komponent* model = new SerialObject;
	//model->Add(tree2); //to jest paraller
	//model->Add(tree);//to jest serial
	model->Add(leaf7);


	SygnalGenerator* sygnal = new OgranicznikAmplitudy(
		new SzumBialyGenerator(
			new TrojkatGenerator(
				new ProstokatGenerator(
					new SinusGenerator(
						new WartoscStalaGenerator(1.0),//stala
						0.0, 0.00),//Sinus
					0.0, 0.00, 0.0),//Prostokat
				0.0, 0.00),// Trojkat
			0.0),// Szum
		10.0);// Wartosc maksymalna amplitudy


	////wartosc zadana = 1.
	//for (int i = 0; i < LICZ_ITER; i++)
	//	sygWe[i] = 1;

	PetlaJedenKrok petla;
	// Symulacja petli:
	for (int i = 0; i < LICZ_ITER; i++)
		faktSygWy[i] = petla(regulator, *model, sygnal->generuj());

	//wynik
	for (int i = 0; i < LICZ_ITER; i++)
		std::cout << "Krok " << i + 1 << " y= " << faktSygWy[i] << std::endl;

	//regulator.setK(0.7);
	//regulator.setTI(1.7); //regulator = RegulatorPID(regulator_P, regulator_I, regulator_D);
	////regulator = RegulatorPID(0.7, 1.7);

	//for (int i = 0; i < LICZ_ITER; i++)
	//	faktSygWy[i] = petla(regulator, *model, sygnal->generuj());

	////wynik
	//for (int i = 0; i < LICZ_ITER; i++)
	//	std::cout << "Krok " << i + 1 << " y= " << faktSygWy[i] << std::endl;
	//int xd = 0;
	*/

	
	std::cerr << "symulacja_petli:\n";


	SygnalGenerator* sygnal = new OgranicznikAmplitudy(
		new SzumBialyGenerator(
			new TrojkatGenerator(
				new ProstokatGenerator(
					new SinusGenerator(
						new WartoscStalaGenerator(3.0),//stala
						4.0, 0.05),//Sinus
					3.0, 0.05, 0.5),//Prostokat
				0.0, 0.00),// Trojkat
			0.0),// Szum
		10.0);// Wartosc maksymalna amplitudy


	constexpr size_t LICZ_ITER = 30;
	std::vector<double> sygWe(LICZ_ITER);      // pobudzenie modelu  - wartosc zadana
	std::vector<double> faktSygWy(LICZ_ITER);  // faktyczna sekwencja wy - wyjscie - wartosc regulowana

	//RegulatorPID i ModelARX
	//RegulatorPID regulator(0.5);
	RegulatorPID regulator(0.5, 1.0, 0.0);  //RegulatorPID regulator(1.0, 0.5, 0.1);
	//RegulatorPID regulator(0.4, 2.0, 0.2);
	Komponent* obiekt = new ModelARX({ -0.4, 0.2 }, { 0.6, 0.3 }, 2, 0.0);
	Komponent* obiekt2 = new ModelARX({ -0.4 }, { 0.6 }, 2, 0.0);
	Komponent* obiekt3 = new ModelARX({ -0.4 }, { 0.6 }, 1, 0.0);
	

	Komponent* tree = new SerialObject;// SerialObject;ParallerObject
	Komponent* tree2 = new ParallerObject;//ParallerObject
	tree2->Add(obiekt);
	tree2->Add(obiekt2);

	tree->Add(tree2);
	tree->Add(obiekt3);

	//obiekt.setA({ 1.0, 2.0 });
	//obiekt.setB({ 1.0, 2.0 });

	//wartosc zadana = 1.
	for (int i = 0; i < LICZ_ITER; i++)
		sygWe[i] = !!i;//1;



	PetlaJedenKrok petla;
	// Symulacja petli:
	for (int i = 0; i < LICZ_ITER; i++)
		faktSygWy[i] = petla(regulator, *tree, sygnal->generuj());

	//wynik
	for (int i = 0; i < LICZ_ITER; i++)
		std::cout<<"Krok "<<i+1<<" y= "<<faktSygWy[i]<<std::endl;

	std::cerr << "Zmiana paramterów PID:\n";

	regulator.setK(0.5);
	regulator.setTI(10.0);
	regulator.setTD(0.2);

	for (int i = 0; i < LICZ_ITER; i++)
		faktSygWy[i] = petla(regulator, *tree, sygnal->generuj());//sygWe[i]

	//wynik
	for (int i = 0; i < LICZ_ITER; i++)
		std::cout << "Krok " << i + 1 << " y= " << faktSygWy[i] << std::endl;

	std::cerr << "Zmiana paramterów ARX:\n";

	ModelARX* tmp = dynamic_cast<ModelARX*>(obiekt);
	tmp->setA({0.7, 0.1});
	tmp->setOpoznienieTransportowe(3);

	
	ModelARX* tmp2 = dynamic_cast<ModelARX*>(obiekt3);  //ModelARX* 
	tmp2->setB({ 0.6, 0.3 });
	tmp2->setOpoznienieTransportowe(1);

	for (int i = 0; i < LICZ_ITER; i++)
		faktSygWy[i] = petla(regulator, *tree, sygnal->generuj());

	//wynik
	for (int i = 0; i < LICZ_ITER; i++)
		std::cout << "Krok " << i + 1 << " y= " << faktSygWy[i] << std::endl;

}
#endif

/*
g++ lab3.cpp ModelARX.cpp RegulatorPID.cpp PetlaJedenKrok.cpp WartoscStalaGenerator.cpp 
TrojkatGenerator.cpp SzumBialyGenerator.cpp SygnalGenerator.cpp SygnalDekorator.cpp SinusGenerator.cpp 
ProstokatGenerator.cpp OgranicznikAmplitudy.cpp 
./a.out 

*/