#include "utils.h"

double weights[243], pos_weights[15][15];

char getCommand(){
    string command;
    while (true)
    {
        getline(cin, command);
        if (command == "N" || command == "L" || command == "Q")
        {
            return command[0];
        }
        else
        {
            cout << "Invalid command. Please try again." << endl;
        }
    }
}

void getCoordinate(int& x, int& y, const string prompt){
    string line;
    char trash;
    cout << prompt;
    while (true){
        if (!getline(cin, line)){
            throw domain_error("Invalid input.");
        }
        istringstream iss(line);
        if (iss >> x && iss >> y && !(iss >> trash)){
            return;
        }
        else {
            cout << "Invalid input. Please try again." << endl;
        }
    }
}

fstream openFile(const string& prompt){
    fstream file;
    string file_name;
    while(true) {
        cout << prompt;
        getline(cin, file_name);
        if (file_name == "q") exit(0);
        file.open(file_name.c_str());
        if (file.fail()){
            cout << "Invalid file name. Please try again." << endl;
        }
        else {
            cout << "Success!" << endl;
            return file;
        }
    }
}