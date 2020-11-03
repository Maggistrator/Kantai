#ifndef SESSIONS
#define SESSIONS
using namespace std;

class Player
{
    char* name;                 // Имя игрока, вводится единожды и не изменяется

    public:
    list<int> scores;           // Очки за все проведенные за сессию игры
    Player(const char* name)
    {
        this->name = name;
    }
};

static struct Session
{
    list<Player> players;
} current_session;

#endif // SESSIONS
