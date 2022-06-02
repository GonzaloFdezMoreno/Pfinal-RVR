#include "BlackJack.h"

int main(int argc, char **argv)
{
    BlackJackServer es(argv[1], argv[2]);

    es.do_messages();

    return 0;
}
