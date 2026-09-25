#include <iostream>
#include <string>

#include "CampusCompass.h"

using namespace std;

int main() {
    CampusCompass compass;
    compass.ParseCSV("data/edges.csv", "data/classes.csv");
    int numberOfCommands;
    string command;
    cin >> numberOfCommands;
    cin.ignore();
    for (int i = 0; i < numberOfCommands; i++) {
        getline(cin, command);
        compass.ParseCommand(command);
    }
    return 0;
}