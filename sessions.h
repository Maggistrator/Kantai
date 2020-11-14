#include <vector>
#include <fstream>
#include <bits/stdc++.h>

#ifndef SESSIONS
#define SESSIONS
/* single game result */
struct Record
{
	char* name;
	int score;

	~Record() { delete name; }
};

static struct Session
{
    char* current_player;
	std::vector<Record*> records;
	std::vector<Record*> overall_records;

	void addRecord( char* name, int m_score )
	{
        Record* rec = new Record();
        rec->name = current_player;
        rec->score = m_score;
        records.push_back(rec);
        sort(records.begin(), records.end(), [](Record * f, Record * s) { return f->score < s->score; });
//        for(Record* r: records){
//            cout << r->score << "\t";
//        } cout << endl;
	}

//	void sort(vector<Record*>& col)
//	{
//	    vector<Record*> temp;
//        for(int i = 0; i < col.size(); i++){
//            int i
//            Record* minimal;
//            for(int j = 0; j < col.size(); j++){
//                Record* a = overall_records.at(i);
//                Record* b = overall_records.at(j);
//            }
//        }
//	}

	 bool loadOverallHighscores()
        {
            ifstream infile;
            try
            {
                infile.open("res/save.dat", ios::binary | ios::in);

                //reading list lenght
                int list_size = 0;
                infile.read((char *)&list_size, sizeof(int));

                for(int i = 0; i < list_size; i++ )
                {
                    //reading name lenght
                    int player_name_lenght = 0;
                    infile.read((char *)&player_name_lenght, sizeof(int));

                    //reading name
                    char *m_temp_name = new char[player_name_lenght];
                    infile.read(m_temp_name, sizeof(char) * (player_name_lenght+1));

                    // reading scores
                    int m_scores = 0;
                    infile.read((char *)&m_scores, sizeof(int));

                    addRecord(m_temp_name, m_scores);
                }
            }
            catch (const std::exception& e)
            {
                std::cout << e.what();
                return false;
            }
        }

        bool writeOverallHighscores( )
        {
            ofstream ofs1("res/save.dat", ios::binary | ios::out);
            try
            {
                int list_size = records.size();
                ofs1.write((char *)&list_size, sizeof(int));
                for(Record* rec: records){
                    int player_name_lenght = strlen(rec->name);
                    ofs1.write((char *)&player_name_lenght, sizeof(int));
                    ofs1.write(rec->name, sizeof(char)*(player_name_lenght+1));
                    ofs1.write((char *)rec->score, sizeof(int));
                }
                return true;
            }
            catch (const std::exception& e)
            {
                std::cout << e.what();
                return false;
            }
        }

//        ~Session()
//        {
//            for(Record* rec: records) delete rec;
//        }
} current_session;


//class Player
//{
//    public:
//    char* name;                 // Имя игрока, вводится единожды и не изменяется
//
//    list<int> scores;           // Очки за все проведенные за сессию игры
//    Player(char* name)
//    {
//        this->name = name;
//    }
//};
//
//static struct Session
//{
//    list<Player*> players;
//    Player* current_player;
//} current_session;

#endif // SESSIONS
