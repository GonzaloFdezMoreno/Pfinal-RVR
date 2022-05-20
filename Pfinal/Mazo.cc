#include <vector>
#include <random>
#include <ctime>
#include <string.h>

const int N_JUGADORES = 2;

const int N_CARTAS = 52; //no hay joker
const int N_CPORPALO = 13; //n cartas por palo

enum Palo{Corazones,Picas,Diamantes,Treboles};


class Mazo{
private:

std::vector<int> mazo;

public:

 Mazo() {
        std::srand(std::time(nullptr));
        for (int i = 0; i < N_CARTAS; i++){
            mazo.push_back(i);}
        Barajar();
    }

//Cambia carta a carta el lugar del mazo intercambiandolas 2 a 2
 void Barajar() { 
        int aux;
        for (int i = 0; i < mazo.size(); i++) {
            int pos = std::rand() % N_CARTAS;
            aux = mazo[pos];
            mazo[pos] = mazo[i];
            mazo[i] = aux;
        }
    }
    

};