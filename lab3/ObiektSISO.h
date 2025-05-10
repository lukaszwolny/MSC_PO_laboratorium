#pragma once

/**
* @class ObiektSISO
* @brief Abstrakcyjna klasa bazowa dla obiektów typu SISO (Single Input, Single Output).
*/
class ObiektSISO {
    public:
        /**
        * @brief Czysto wirtualna metoda symulacji.
        * @param u Wartość wejściowa.
        * @return Wartość wyjściowa po przetworzeniu wejścia przez obiekt.
        */
        virtual double symuluj(double u) = 0; // Czysto wirtualna metoda

        /**
        * @brief Wirtualny destruktor klasy bazowej.
        */
        virtual ~ObiektSISO() {}
    };