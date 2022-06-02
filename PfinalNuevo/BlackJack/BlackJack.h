#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <memory>
#include <ctime>

#include "Serializable.h"
#include "Socket.h"


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


class Message: public Serializable {
public:
    static const size_t MESSAGE_SIZE = 9 * sizeof(char) + 5 * sizeof(uint8_t);

    enum MessageType{
        LOGIN=0, //se conecta un jugador, tiene nickname
        LOGOUT=1, //se desconecta un jugador
        HIT=2, //pide una carta
        STAND=3, //se planta con sus cartas
        CARDS=4, //las cartas que tiene el jugador
        CARDS_OPONENT=5, //las cartas que tiene el oponente que se podran ver todas menos la primera
        LOGIN_INFO=6, //el nickname de cada jugador que se da a los clientes
        WINNER=7, //quien ha ganado la partida aunque pueden perder ambos tambien
        END_ROUND=8, //se acaba la ronda y se reinicia la partida
        START
    };

    Message() : nick(""), type(0), sum1(0), sum2(0),resta1(0),resta2(0) {};

    Message(const std::string& n, const std::string& m,uint8_t n1, uint8_t n2,uint8_t rest1,uint8_t rest2) : nick(n), messageX(m),sum1(n1),sum2(n2),resta1(rest1),resta2(rest2) {};

    void to_bin();
    int from_bin(char * obj);


    std::string nick;
    uint8_t type;
    std::string messageX;
    //std::vector<int>  message1;
    //std::vector<int>  message2;
    uint8_t sum1;
    uint8_t sum2;
    uint8_t resta1;
    uint8_t resta2;
    

};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
 *  Clase para el servidor de BlackJack
 */
class BlackJackServer
{
public:
    BlackJackServer(const char * s, const char * p): socket(s, p)
    {
        socket.bind();
    };

    /**
     *  Thread principal del servidor recive mensajes en el socket y
     *  lo distribuye a los clientes. Mantiene actualizada la lista de clientes
     */
    void do_messages();

private:
    /**
     *  Lista de clientes conectados al servidor de BlackJack, representados por
     *  su socket
     */
    std::vector<std::unique_ptr<Socket>> clients;


    /**
     * Socket del servidor
     */
    Socket socket;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
 *  Clase para el cliente de BlackJack
 */
class BlackJackClient
{
public:
    /**
     * @param s dirección del servidor
     * @param p puerto del servidor
     * @param n nick del usuario
     */
    BlackJackClient(const char * s, const char * p, const char * n):socket(s, p),
        nick(n){};

    /**
     *  Envía el mensaje de login al servidor
     */
    void login();

    /**
     *  Envía el mensaje de logout al servidor
     */
    void logout();

    /**
     *  Rutina principal para el Thread de E/S. Lee datos de STDIN (std::getline)
     *  y los envía por red vía el Socket.
     */
    void input_thread();

    /**
     *  Rutina del thread de Red. Recibe datos de la red y los "renderiza"
     *  en STDOUT
     */
    void net_thread();

    void creaMazo();
    void baraja();
    int sacaCarta();
    int checkCards(const std::vector<int>& mano);

private:

    /**
     * Socket para comunicar con el servidor
     */
    Socket socket;

    /**
     * Nick del usuario
     */
    std::string nick;


    std::vector<int> mazo;

    //std::vector<int> myCards; //vector donde estan mis cartas
    int myCards; //int que almacena la suma de los numeros de mis cartas
    //std::vector<int> opponentCards; //vector donde estan las cartas del oponente (puesto por si acaso)
    int opponentCards; //int que almacena la suma de los numeros de las cartas del oponente
    int fCardOfOp;
    int fMyCards;

    //const int N_CARTASPORPALO = 13; //no hay joker
    //const int N_PALOS = 4; //n cartas por palo

    bool myStand = false;
    bool opStand = false;
    bool round = false;

};

