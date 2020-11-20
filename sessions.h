#ifndef SESSIONS
#define SESSIONS

#include <fstream>

#define MAX_RECORDS 10

/* single game result */
struct Record
{
    Record( char* m_name, int m_score ) : name(m_name), score(m_score) {}

	char* name;
	int score;

};

static struct Session
{
private:
    int items = 0, overall_items = 0;
public:
    char* current_player;
	Record* highscores[MAX_RECORDS];
	Record* overall_highscores[MAX_RECORDS];

	void addRecord( char* name, int m_score )
	{
	    char * current_name = new char[32];
        strcpy(current_name, name);
        strcat(current_name, "\0");
	    highscores[items % (MAX_RECORDS-1)] = new Record(current_name, m_score);
        addHighRecord(name, m_score);
        items++;
	}

	void addHighRecord( char* name, int m_score)
	{
	    char * current_name = new char[32];
        strcpy(current_name, name);
        strcat(current_name, "\0");
        if(overall_items < MAX_RECORDS-1){
            overall_highscores[overall_items] = new Record(current_name, m_score);
            overall_items++;
            sort(overall_highscores, overall_items);
            writeOverallHighscores();
        } else {
            int i = MAX_RECORDS-1; // iterator for existing scores
            int j = -1;   // new score possible position
            while(overall_highscores[i]->score < m_score && i >= 0) {
                j = i;
                i--;
            }
            if(j != -1) {
                delete overall_highscores[j];
                overall_highscores[j] = new Record(name, m_score);
                writeOverallHighscores();
            }
        }
	}

    void sort(Record ** arr, int size)
    {
        for(int i = 0; i < size; i++) {
            for(int j = 0, k = 0; j < size - 1; j++) {
                Record* a = arr[j];
                Record* b = arr[j + 1];
                if( a == 0 || b == 0) {
                    return;
                }
                if(a->score < b->score) {
                    Record* temp = a;
                    arr[j] = arr[j+1];
                    arr[j+1] = temp;
                }
            }
        }
    }

	 bool loadOverallHighscores()
        {
            clearOverallHighscores();
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
                    char *m_temp_name = new char[player_name_lenght + 1];
                    infile.read(m_temp_name, player_name_lenght);
                    for (int i = player_name_lenght; i < player_name_lenght + 1; i++ ) m_temp_name[i] = ' ';
                    m_temp_name[player_name_lenght + 1] = '\0';

                    // reading scores
                    int m_scores = 0;
                    infile.read((char *)&m_scores, sizeof(int));

                    addHighRecord(m_temp_name, m_scores);
                }
            }
            catch (const std::exception& e)
            {
                std::cout << e.what();
                return false;
            }
        }

        void writeOverallHighscores( )
        {
                ofstream ofs1("res/save.dat", ios::binary | ios::out);
                if(ofs1.good()) {
                    ofs1.write((char *)&overall_items, sizeof(int));
                    for(Record* rec: overall_highscores){
                        if(rec) {
                            int player_name_lenght = strlen(rec->name);
                            ofs1.write((char *)&player_name_lenght, sizeof(int));
                            ofs1.write(rec->name, sizeof(char)*(player_name_lenght));
                            ofs1.write((char *)&(rec->score), sizeof(int));
                        }
                    }
                }
            ofs1.close();
        }


        void printOverallHighscores( )
        {
                ofstream file;
                file.open ("res/save.txt");
                if(file.good()) {
                    for(int i = 0; i < overall_items; i++) {
                        Record* rec = overall_highscores[i];
                        file << "name: " << rec->name << " score: " << rec->score << '\n';
                    }
                }
                file.close();
        }

        void clearOverallHighscores()
        {
            for (int i = 0; i < MAX_RECORDS; i++) {
                if(overall_highscores[i] != nullptr) {
                    delete overall_highscores[i]->name;
                    delete overall_highscores[i];
                    overall_highscores[i] = nullptr;
                }
            } overall_items = 0;
        }

        ~Session()
        {
            for(Record* rec: highscores) {
                delete rec->name;
                delete rec;
            }
            clearOverallHighscores();
        }
} current_session;

#endif // SESSIONS
