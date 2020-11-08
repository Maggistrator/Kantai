#ifndef SESSIONS
#define SESSIONS
using namespace std;

class Player
{
    public:
    char* name;                 // Имя игрока, вводится единожды и не изменяется

    list<int> scores;           // Очки за все проведенные за сессию игры
    Player(char* name)
    {
        this->name = name;
    }
};

static struct Session
{
    list<Player*> players;
    Player* current_player;
} current_session;

#endif // SESSIONS
