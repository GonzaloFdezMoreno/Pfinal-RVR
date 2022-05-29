#include <string>
#include <string.h>
#include <unistd.h>
#include <memory>
#include <thread>
#include <algorithm>

#include "Mazo.cc"
#include "Message.cc"
#include "Socket.h"

class GameServer {
public:
    GameServer(const char * s, const char * p): socket(s, p, true) {};
    ~GameServer() {};

    void do_messages() {
        Message msg;
        Socket* s;
        while (true) {
            std::cout << "Number of clients: " << clients.size() << std::endl;
            s = socket.accept();
            GameServer* server = this;

            socket.recv(msg, *s);
            checkLogin(msg, s);

            //si no hay suficientes jugadores sigue esperando conexiones
            if (clients.size() < NUM_PLAYERS) continue;

            //envia la informacion al otro jugador
            for (int i = 0; i << NUM_PLAYERS; i++) {
                Message msg(clients[i].second, Message::LOGIN_INFO);
                sendPlayers(msg);
            }

            //comienza la partida y se sigue jugando hasta que uno se desconecte
            while (clients.size() == NUM_PLAYERS){
                game();
            }
            std::cout << "The game has ended\n";
        }
        delete s;
    }

private:
    enum Hands {High_Card, Pair, Two_Pair, Three_of_a_kind, Straight, Flush, Full_House, Four_of_a_kind, Straight_Flush, Royal_Flush};

    Socket socket;
    Deck* deck;
    int hands[NUM_PLAYERS][2];
    std::vector<int> cardsTable; //cartas en la mesa (puesto por si aca, borrarlo si no tiene uso)
    std::vector<std::pair<std::unique_ptr<Socket>, std::string>> clients; //lista de clientes

    std::vector<int> myCards; //vector donde estan mis cartas
    int sumOfCards; //int que almacena la suma de los numeros de mis cartas
    std::vector<int> opponentCards; //vector donde estan las cartas del oponente (puesto por si acaso)
    int sumOfOpponent; //int que almacena la suma de los numeros de las cartas del oponente

    void game() {
        deck = new Deck();
        Message m;

        std::cout << "2 cards are given to each player\n";
        /*for(int i = 0; i < clients.size(); i++){
            m = Message(clients[i].second, Message::CARDS, deck->draw(), deck->draw());
            socket.send(m, *clients[i].first.get());
            hands[i][0] = m.message1, hands[i][1] = m.message2;
        }*/
        //de momento solo roba mis cartas, falta enviar el mensaje con las cartas y que al recibirlas se añadan al vector 
        //del oponente
        for(int i = 0; i < 2; i++){
            myCards.push_back(deck->draw());
        }
        //ahora tocaria recibir el mensaje de las cartas del oponente y añadirlas a su vector
        //Falta el resto de metodos 

    }

    void closeGame() {
        if(deck != nullptr) delete deck;
        //cardsTable.clear();
        myCards.clear();
        opponentCards.clear();
    }

    //manda mensaje a todos los jugadores
    void sendPlayers(Message& m){
        for (int j = 0; j < clients.size(); j++){
            socket.send(m, *clients[j].first.get());
        }
    }

    //comprueba si hay mensaje LOGIN y si lo hay añade el cliente nuevo a la lista
    bool checkLogin(Message& m, Socket* s) {
        if (m.type != Message::LOGIN) return false;
        clients.push_back(std::pair<std::unique_ptr<Socket>, std::string>(std::move(std::make_unique<Socket>(*s)), m.nickname));
        std::cout << m.nickname << " has connected" << std::endl;
        return true;
    }

    //comprueba si hay mensaje LOGOUT y si lo hay se desconecta al jugador y avisa al resto
     bool checkLogout(Message& m) {
        if (m.type != Message::LOGOUT) return false;
        auto it = clients.begin();
        while (it != clients.end() && !((it->second) == m.nickname)) it++;
        if(it != clients.end()) {
            std::cout << m.nick << " has disconnected" << std::endl;
            clients.erase(it);
            Message msg("Server", Message::LOGOUT);
            sendPlayers(msg);
        }
        else {std::cout << "Can't find " << m.nick << std::endl; return false;}
        return true;
    }

    //continuar a partir de linea 192

    void checkCards(){
        int total = 0;
        //primero sumo las cartas de mi mano
        for(int i = 0; i < myCards.size(); i++){
            total += myCards[i];
        }
        sumOfCards = total;
        total = 0;
        //sumo las cartas del oponente excepto la primera
        for(int i = 1; i < opponentCards.size(); i++){
            total += opponentCards[i];
        }
        sumOfOpponent = total;
    }
    
};

int main(int arg, char *argv[]) {
    if (arg != 3) return -1;
    GameServer es(argv[1], argv[2]);
    es.do_messages();
    return 0;
}