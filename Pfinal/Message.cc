#include <string>
#include <string.h>
#include <uinstd.h>
#include <vector>
#include <memory>
#include "Serializable.h"

class Message: public Serializable {
public:
    static const size_t MESSAGE_SIZE = 9 * sizeof(char) + 3 * sizeof(uint8_t);

    enum MessageType{
        LOGIN, //se conecta un jugador, tiene nickname
        LOGOUT, //se desconecta un jugador
        HIT, //pide una carta
        STAND, //se planta con sus cartas
        CARDS, //las cartas que tiene el jugador
        CARDS_OPONENT, //las cartas que tiene el oponente que se podran ver todas menos la primera
        LOGIN_INFO, //el nickname de cada jugador que se da a los clientes
        WINNER, //quien ha ganado la partida aunque pueden perder ambos tambien
        END_ROUND //se acaba la ronda y se reinicia la partida
    }

    Message() : nickname(""), type(0), message1(0), message2(0) {};

    Message(const std::string& n, const uint8_t& t, const uint8_t& m1 = 0, const uint8_t& m2 = 0) : nickname(n), type(t), message1(m1), message2(m2) {};

    void to_bin() {
        alloc_data(MESSAGE_SIZE);

        memset(_data, 0, MESSAGE_SIZE);

        char* tmp = _data;

        memcpy(tmp, nickname.c_str(), 9);
        tmp += 8;

        memcpy(tmp, "\0", sizeof(char));
        tmp++;

        memcpy(tmp, &type, sizeof(uint8_t));
        tmp += sizeof(uint8_t);
            
        memcpy(tmp, &message1, sizeof(uint8_t));
        tmp += sizeof(uint8_t);

        memcpy(tmp, &message2, sizeof(uint8_t));
    }

    int from_bin(char * obj){
        char* tmp = obj;

        nickname = tmp;
        tmp += 9;

        memcpy(&type, tmp, sizeof(uint8_t));
        tmp += sizeof(uint8_t);
            
        memcpy(&message1, tmp, sizeof(uint8_t));
        tmp += sizeof(uint8_t);

        memcpy(&message2, tmp, sizeof(uint8_t));

        return 0;
    }

    std::string nickname;
    uint8_t type;
    uint8_t message1;
    uint8_t message2;
};