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
        if(items != MAX_RECORDS) {
            if(highscores[items] != nullptr) delete highscores[items];
            highscores[items] = new Record(name, m_score);
            items++;
        } else {
            items = 0;
            addRecord(name, m_score);
        }
	}

    void sort(Record ** arr, int size)
    {
        for(int i = 0; i < size; i++) {
            for(int j = 0; j < size - 1; j++) {
                Record* a = arr[j];
                Record* b = arr[j + 1];
                if(a->score > b->score) {
                    Record* temp = a;
                    arr[j] = arr[j+1];
                    arr[j+1] = temp;
                }
            }
        }
    }


    void updateOverallHighscores () {
        Record* merge_array[MAX_RECORDS * 2];
        for(int i = 0; i < (MAX_RECORDS * 2); i++) {
            merge_array[i] = highscores[i];
            merge_array[MAX_RECORDS + i] = overall_highscores[i];
            cout << "merged highscore #" << i << " is " << merge_array[i] << endl;
        }
        cout << "---------------------"<< endl;
        sort(merge_array, MAX_RECORDS * 2);
        for(int i = 0; i < MAX_RECORDS; i++) {
            overall_highscores[i] = highscores[i];
            cout << "highscore #" << i << " is " << highscores[i] << endl;
            cout << "overall highscore #" << i << " is " << overall_highscores[i] << endl;
            if(overall_highscores[i] != nullptr) overall_items++;
        }
    }

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
            cout << "opening file is " << (ofs1 ? "succsessful" : "unsuccsessful") << endl;
            try
            {
                int list_size = overall_items;
                cout << "list size is " << list_size << endl;
                ofs1.write((char *)&list_size, sizeof(int));
                for(Record* rec: highscores){
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

        ~Session()
        {
            for(Record* rec: highscores) delete rec;
            for(Record* rec: overall_highscores) delete rec;
        }
} current_session;

#endif // SESSIONS
