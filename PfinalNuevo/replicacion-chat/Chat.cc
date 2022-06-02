#include "Chat.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void Message::to_bin()
{
    /*alloc_data(MESSAGE_SIZE);

    memset(_data, 0, MESSAGE_SIZE);

    //Serializar los campos type, nick y message en el buffer _data
    char * tmp = _data;
    memcpy(tmp, &type, sizeof(uint8_t));
    tmp += sizeof(uint8_t);
    memcpy(tmp, nick.c_str(), 8 * sizeof(char));
    tmp += 8 * sizeof(char);
    memcpy(tmp, message.c_str(), 80 * sizeof(char));*/
     alloc_data(MESSAGE_SIZE);

        memset(_data, 0, MESSAGE_SIZE);

        char* tmp = _data;

        memcpy(tmp, nick.c_str(), 9);
        tmp += 8;

        memcpy(tmp, "\0", sizeof(char));
        tmp++;

        memcpy(tmp, &type, sizeof(uint8_t));
        tmp += sizeof(uint8_t);
            
        memcpy(tmp, &sum1, sizeof(uint8_t));
        tmp += sizeof(uint8_t);

         memcpy(tmp, &sum2, sizeof(uint8_t));
        tmp += sizeof(uint8_t);

        memcpy(tmp, &resta1, sizeof(uint8_t));
        tmp += sizeof(uint8_t);

        memcpy(tmp, &resta2, sizeof(uint8_t));




}

int Message::from_bin(char * obj)
{
    /*alloc_data(MESSAGE_SIZE);

    memcpy(static_cast<void *>(_data), bobj, MESSAGE_SIZE);

    //Reconstruir la clase usando el buffer _data
    char * tmp = _data;
    memcpy(&type, tmp, sizeof(uint8_t));
    tmp += sizeof(uint8_t);
    nick = tmp;
    tmp += 8 * sizeof(char);
    message = tmp;

    return 0;*/
    char* tmp = obj;

    nick = tmp;
    tmp += 9;
    memcpy(&type, tmp, sizeof(uint8_t));
    tmp += sizeof(uint8_t);
            
    memcpy(&sum1, tmp, sizeof(uint8_t));
    tmp += sizeof(uint8_t);

    memcpy(&sum2, tmp, sizeof(uint8_t));
    tmp += sizeof(uint8_t);

    memcpy(&resta1, tmp, sizeof(uint8_t));
    tmp += sizeof(uint8_t);

    memcpy(&resta2, tmp, sizeof(uint8_t));
    tmp += sizeof(uint8_t);

    messageX = tmp;

    return 0;



}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChatServer::do_messages()
{
    while (true)
    {
        /*
         * NOTA: los clientes están definidos con "smart pointers", es necesario
         * crear un unique_ptr con el objeto socket recibido y usar std::move
         * para añadirlo al vector
         */

        //Recibir Mensajes en y en función del tipo de mensaje
        Message message;
        Socket *messageSocket = new Socket(socket);
        socket.recv(message, messageSocket);

        // - LOGIN: Añadir al vector clients
        if(message.type == message.LOGIN){
            std::cout << "LOGIN " << *messageSocket << "\n";
            std::unique_ptr<Socket> uptr = std::make_unique<Socket>(*messageSocket);
            messageSocket = nullptr;
            clients.push_back(std::move(uptr));
           
        }

        // - LOGOUT: Eliminar del vector clients
        else if(message.type == message.LOGOUT){
            std::cout << "LOGOUT " << *messageSocket << "\n";
            auto it = clients.begin();
            while(it != clients.end() && !(**it == *messageSocket)) ++it;
            if(it == clients.end()) std::cout << "Client not found\n";
            else clients.erase(it);
        }

        // - MESSAGE: Reenviar el mensaje a todos los clientes (menos el emisor)
        else if(message.type == message.HIT){
            std::cout << "HIT " << *messageSocket << "\n";
            for(auto it = clients.begin(); it != clients.end(); ++it){
                if(!(**it == *messageSocket)) socket.send(message, **it);
            }
            
        }
         else if(message.type == message.START){
            std::cout << "START " << *messageSocket << "\n";
            for(auto it = clients.begin(); it != clients.end(); ++it){
                if(!(**it == *messageSocket)) socket.send(message, **it);
            }
            
        }

        else if(message.type == message.STAND){
            std::cout << "STAND " << *messageSocket << "\n";
            for(auto it = clients.begin(); it != clients.end(); ++it){
                if(!(**it == *messageSocket)) socket.send(message, **it);
            }
            
        }

         else if(message.type == message.END_ROUND){
            std::cout << "ROUND_OVER " << *messageSocket << "\n";
            for(auto it = clients.begin(); it != clients.end(); ++it){
                if(!(**it == *messageSocket)) socket.send(message, **it);
            }
            
        }
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChatClient::login()
{
    std::string msg;
    myCards=0;
    opponentCards=0;
    fCardOfOp=0;
    fMyCards=0;
    Message em(nick, msg,myCards,opponentCards,fMyCards,fCardOfOp);
    em.type = Message::LOGIN;
    creaMazo();

    socket.send(em, socket);
}

void ChatClient::logout()
{
    myCards=0;
    opponentCards=0;
    fCardOfOp=0;
    fMyCards=0;
    
    std::string msg;
    

    Message em(nick, msg,myCards,opponentCards,fMyCards,fCardOfOp);
    em.type = Message::LOGOUT;

    socket.send(em, socket);
}

void ChatClient::input_thread()
{
    bool chat = true;
    while (chat)
    {
        if(myStand && opStand){
            std::cout << "Ambos estan en stand\n";
        }

        // Leer stdin con std::getline
        std::string msg;

        std::cout<<"Escriba: start para empezar o exit para salir\n";
        std::cout << "Desea pedir o plantarse" << "\n"<<"Hit-> 1 \nStand->2\n";

        std::cout << "Valor de tus cartas: " << myCards << "\n";
        std::cout << "Valor cartas del oponente: ?+"<<opponentCards-fCardOfOp<<"\n";
        std::getline(std::cin, msg);

        if(msg == "exit"){
            chat = false;
        }
        else if(msg=="start"){
            myStand = false;
            opStand = false;

            myCards=0;
            opponentCards=0;
            fCardOfOp=0;
            fMyCards=0;

            myCards+=sacaCarta();
            fMyCards=myCards;
            myCards+=sacaCarta();

            opponentCards+=sacaCarta();
            fCardOfOp=opponentCards;
            opponentCards+=sacaCarta();

            Message em(nick, msg,myCards,opponentCards,fMyCards,fCardOfOp);
            em.type=Message::START;

            /*em.sum1=myCards;
            em.resta1=fMyCards;
            em.sum2=opponentCards;
            em.resta2=fCardOfOp;*/

            socket.send(em, socket);
        }


        else if (msg=="1"){
            myCards+=sacaCarta();
           
            // Enviar al servidor usando socket
            Message em(nick, msg,myCards,opponentCards,fMyCards,fCardOfOp);
            em.type = Message::HIT;

            socket.send(em, socket);
            

        }
        
        else if (msg=="2"){
            // Enviar al servidor usando socket
            myStand = true;
           
            Message em(nick, msg,myCards,opponentCards,fMyCards,fCardOfOp);

            em.type = Message::STAND;

            socket.send(em, socket);
        }

        if(myCards>21||opponentCards>21){

            Message em(nick,"8",myCards,opponentCards,fMyCards,fCardOfOp);

            em.type = Message::END_ROUND;

            if(myCards>21&&opponentCards<=21){

                std::cout<<myCards<<"-------"<<opponentCards;

                std::cout << "Gana el oponente" << "\n";

                

            }

            else if(myCards>21&&opponentCards>21){

                std::cout<<myCards<<"-------"<<opponentCards;

                std::cout << "Ambos jugadores pierden" << "\n";

                

            }

            else{

                std::cout<<myCards<<"-------"<<opponentCards;

                std::cout << "Tu ganas!!! "<< "\n";

                

            }

            socket.send(em, socket);

            chat=false;

        }


    }
    logout();
}

void ChatClient::net_thread()
{
    Message em;
    Socket *mSocket = new Socket(socket);
    while(true)
    {
        //Recibir Mensajes de red
        socket.recv(em, mSocket);

        //Mostrar en pantalla el mensaje de la forma "nick: mensaje"
        std::cout << "Valor de tus cartas: " << em.sum2 << "\n";
        std::cout << "Valor cartas de "<< em.nick<<": ?+"<<em.sum1-em.resta1<<"\n";
        std::cout << "Desea pedir o plantarse" << "\n"<<"Hit-> 1 \nStand->2\n";
        
        myCards=em.sum2;
        opponentCards=em.sum1;
        
        fCardOfOp=em.resta2;
        fMyCards=em.resta1;

        if(em.type == Message::STAND){
            opStand = true;
        }

        if(myStand && opStand){
            std::cout << "ambos estan en stand\n";
        }

        if(em.type==8){

            if(myCards>21&&opponentCards<=21){

                std::cout<<myCards<<"-------"<<opponentCards;

                std::cout << "Gana el oponente" << "\n";


            }

            else if(myCards>21&&opponentCards>21){

                std::cout<<myCards<<"-------"<<opponentCards;

                std::cout << "Ambos jugadores pierden" << "\n";

            }

            else{

                std::cout<<myCards<<"-------"<<opponentCards;

                std::cout << "Tu ganas!!! "<< "\n";

            }

        }

    }
}

 int ChatClient::checkCards(const std::vector<int>& mano){
        int total = 0;
        //primero sumo las cartas de mi mano
        for(int i = 0; i < mano.size(); i++){
            total += mano[i];
        }
        return total;
       
    }

void ChatClient::creaMazo() {
    if(mazo.size()!=0){
       mazo.clear();
    }
    std::srand(std::time(nullptr));
    for (int j = 0; j < 4; j++){
        for (int i = 1; i <= 13; i++){
             mazo.push_back(i);
            }
        }
    baraja();
}

void ChatClient::baraja(){
     int aux;
        for (int i = 0; i < mazo.size(); i++) {
            int pos = std::rand() % mazo.size();
            aux = mazo[pos];
            mazo[pos] = mazo[i];
            mazo[i] = aux;
        }
}

int ChatClient::sacaCarta(){
    int pCarta=mazo.back();
       mazo.pop_back(); //Eliminamos la carta del mazo
       return pCarta;
}