#include <vector>
#include <random>
#include <ctime>
#include <string.h>

const int N_CARTASPORPALO = 13; //no hay joker
const int N_PALOS = 4; //n cartas por palo

//enum Palo{Corazones,Picas,Diamantes,Treboles};


class Mazo{
private:

std::vector<int> mazo;

public:

 Mazo() {
        std::srand(std::time(nullptr));
        for (int j = 0; j < N_PALOS; j++){
            for (int i = 1; i = N_CARTASPORPALO; i++){
                mazo.push_back(i);
            }
        }
        Barajar();
    }

//Cambia carta a carta el lugar del mazo intercambiandolas 2 a 2
 void Barajar() { 
        int aux;
        for (int i = 0; i < mazo.size(); i++) {
            int pos = std::rand() % mazo.size();
            aux = mazo[pos];
            mazo[pos] = mazo[i];
            mazo[i] = aux;
        }
    }

//devuelve la ultima carta (la primera del mazo)
 int sacaCarta(){
       int pCarta=mazo.back();
       mazo.pop_back(); //Eliminamos la carta del mazo
       return pCarta;
    } 


//Si hay un as cambia el valor de la carta a eleccion, 13 o 1
    int casoAsValeTrece(int carta){
       carta=13;
       return carta;
    }


};