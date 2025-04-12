// ObiektSISO.h - Klasa abstrakcyjna
class ObiektSISO {
    public:
        virtual double symuluj(double u) = 0; // Czysto wirtualna metoda
        virtual ~ObiektSISO() {} // Wirtualny destruktor
    };



    
/*
g++ lab1.cpp ModelARX.cpp 
./a.out 
*/