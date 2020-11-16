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
        highscores[items] = new Record(name, m_score);
        cout << "-- > in metod addRecord(), record: " << highscores[items] << " added. It's " << items << "'th" << endl;
        addHighRecord(name, m_score);
        items++;
	}

	void addHighRecord( char* name, int m_score)
	{
        if(overall_items < MAX_RECORDS){
            overall_highscores[overall_items] = new Record(name, m_score);
            cout << "-- > in metod addHighRecord(), record: " << overall_highscores[overall_items] << " added. It's " << overall_items << "'th" << endl;
            overall_items++;
            sort(overall_highscores, overall_items);
            writeOverallHighscores();
        printOverallHighscores();
        } else {
            cout << "---too many items, choosing who to kill now---" << endl;
            int i = overall_items-1; // iterator for existing scores
            int j = -1;   // new score possible position
            while(overall_highscores[i]->score < m_score && i >= 0) {
                cout << "item #" << i << "(" << overall_highscores[i]->score << ")" << " bigger then " << " new item (" << m_score << ")" << endl;
                j = i;
                i--;
            }
            if(j != -1) {
                delete overall_highscores[j]->name;
                delete overall_highscores[j];
                overall_highscores[j] = new Record(name, m_score);
                writeOverallHighscores();
        printOverallHighscores();
            }
        }
	}

    void sort(Record ** arr, int size)
    {
        cout << "--------sort--------"<< endl;
        for(int i = 0; i < size; i++) {
            for(int j = 0, k = 0; j < size - 1; j++) {
                Record* a = arr[j];
                cout << "size is: " << size << endl;
                Record* b = arr[j + 1];
                if( a == 0 || b == 0) {
                    cout << "returning" << endl;
                    cout << "---------------------"<< endl;
                    return;
                }
                if(a->score > b->score) {
                    Record* temp = a;
                    arr[j] = arr[j+1];
                    arr[j+1] = temp;
                }
            }
        }
        cout << "---------------------"<< endl;
    }

    void shift(Record ** arr, int head, int tail, int size)
    {
        Record* temp = arr[head+1];
        for(int i = head; i < size; i++) {
            temp = arr[i+1];
            arr[i+1] = arr[i];
        }
    }

	 bool loadOverallHighscores()
        {
            clearOverallHighscores();
            ifstream infile;
            try
            {
                cout << "----------reading file--------- " << endl;
                infile.open("res/save.dat", ios::binary | ios::in);

                //reading list lenght
                int list_size = 0;
                infile.read((char *)&list_size, sizeof(int));
                cout << "list size is " << list_size << endl;

                for(int i = 0; i < list_size; i++ )
                {
                    //reading name lenght
                    int player_name_lenght = 0;
                    infile.read((char *)&player_name_lenght, sizeof(int));
                    cout << "player_name_lenght " << player_name_lenght << endl;

                    //reading name
                    char *m_temp_name = new char[player_name_lenght + 1];
                    for (int i = player_name_lenght; i < player_name_lenght + 1; i++ ){
                        m_temp_name[i] = ' ';
                    }

                    infile.read(m_temp_name, player_name_lenght);
                    cout << "m_temp_name " << m_temp_name << endl;
                    m_temp_name[player_name_lenght + 1] = '\0';

                    // reading scores
                    int m_scores = 0;
                    infile.read((char *)&m_scores, sizeof(int));
                    cout << "m_scores " << m_scores << endl;

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
                cout << "-------------writing file-----------" << endl;
                ofstream ofs1("res/save.dat", ios::binary | ios::out);
                cout << "opening file is " << (ofs1.good() ? "succsessful" : "unsuccsessful") << endl;
                cout << "list size is " << overall_items << endl;
                if(ofs1.good()) {
                    ofs1.write((char *)&overall_items, sizeof(int));
                    cout << "writing 'overall_items', they are: " << overall_items << endl;
                    for(Record* rec: overall_highscores){
                        if(rec) {
                            cout << "rec: " << rec << endl;
                            int player_name_lenght = strlen(rec->name);
                            ofs1.write((char *)&player_name_lenght, sizeof(int));
                            cout << "writing 'player_name_lenght', they are: " << player_name_lenght << endl;
                            ofs1.write(rec->name, sizeof(char)*(player_name_lenght));
                            cout << "writing 'rec->name', they are: " << rec->name << endl;
                            ofs1.write((char *)&(rec->score), sizeof(int));
                            cout << "writing 'rec->score', they are: " << rec->score << endl;
                        }
                    }
                }
            ofs1.close();
        }


        void printOverallHighscores( )
        {
                cout << "------------ printing file --------- " << endl;
                ofstream file;
                file.open ("res/save.txt");
                cout << "opening file is " << (file.good() ? "succsessful" : "unsuccsessful") << endl;
                cout << "list size is " << overall_items << endl;
                if(file.good()) {
                    for(int i = 0; i < overall_items; i++) {
                        Record* rec = overall_highscores[i];
                        file << "name: " << rec->name << " score: " << rec->score << '\n';
                        cout << "writing -- > " << "name: " << rec->name << " score: " << rec->score << '\n';
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
            for(Record* rec: highscores) delete rec;
            for(Record* rec: overall_highscores) delete rec;
        }
} current_session;

#endif // SESSIONS
