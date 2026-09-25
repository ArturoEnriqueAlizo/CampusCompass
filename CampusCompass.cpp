#include "CampusCompass.h"
#include <limits>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <set>
#include <limits>
using namespace std;

int CampusCompass::studentZoneCost(Student student) {
    set<int> zoneLocations;
    zoneLocations.insert(student.residence);
    for (size_t i = 0; i < student.classes.size(); i++) {
        string classCode = student.classes[i];
        int classLocation = classLocations[classCode];

        vector<int> path =
            shortestPath(student.residence, classLocation);

        for (size_t j = 0; j < path.size(); j++) {
            zoneLocations.insert(path[j]);
        }
    }
    if (zoneLocations.size() <= 1) {
        return 0;
    }

    map<int, bool> inTree;

    for (int location : zoneLocations) {
        inTree[location] = false;
    }
    int firstLocation = *zoneLocations.begin();
    inTree[firstLocation] = true;
    int totalCost = 0;
    int numberAdded = 1;

    while (numberAdded < static_cast<int>(zoneLocations.size())) {
        int cheapestCost = numeric_limits<int>::max();
        int locationToAdd = -1;
        for (int currentLocation : zoneLocations) {
            if (!inTree[currentLocation]) {
                continue;
            }
            for (size_t i = 0;
                 i < graph[currentLocation].size();
                 i++) {
                Edge edge = graph[currentLocation][i];

                if (edge.closed) {
                    continue;
                }
                if (zoneLocations.find(edge.destination) ==
                    zoneLocations.end()) {
                    continue;
                    }
                if (inTree[edge.destination]) {
                    continue;
                }
                if (edge.time < cheapestCost) {
                    cheapestCost = edge.time;
                    locationToAdd = edge.destination;
                }
                 }
        }
        if (locationToAdd == -1) {
            return -1;
        }
        inTree[locationToAdd] = true;
        totalCost += cheapestCost;
        numberAdded++;
    }
    return totalCost;
}
vector<int> CampusCompass::shortestPath(int start, int end) {
    vector<int> emptyPath;
    if (graph.find(start) == graph.end() ||
        graph.find(end) == graph.end()) {
        return emptyPath;
        }
    map<int, int> distance;
    map<int, bool> visited;
    map<int, int> previous;

    for (auto location : graph) {
        distance[location.first] = numeric_limits<int>::max();
        visited[location.first] = false;
    }
    distance[start] = 0;

    while (true) {
        int currentLocation = -1;
        int smallestDistance = numeric_limits<int>::max();
        for (auto location : distance) {
            int locationID = location.first;
            if (!visited[locationID] &&
                distance[locationID] < smallestDistance) {
                smallestDistance = distance[locationID];
                currentLocation = locationID;
                }
        }
        if (currentLocation == -1) {
            break;
        }
        if (currentLocation == end) {
            break;
        }
        visited[currentLocation] = true;

        for (size_t i = 0; i < graph[currentLocation].size(); i++) {
            Edge edge = graph[currentLocation][i];
            if (edge.closed) {
                continue;
            }
            int nextLocation = edge.destination;
            int newDistance = distance[currentLocation] + edge.time;
            if (newDistance < distance[nextLocation]) {
                distance[nextLocation] = newDistance;
                previous[nextLocation] = currentLocation;
            }
        }
    }
    if (distance[end] == numeric_limits<int>::max()) {
        return emptyPath;
    }
    vector<int> path;
    int current = end;

    path.push_back(current);

    while (current != start) {
        current = previous[current];
        path.push_back(current);
    }
    reverse(path.begin(), path.end());
    return path;
}
int CampusCompass::shortestTime(int start, int end) {
    if (graph.find(start) == graph.end() ||
        graph.find(end) == graph.end()) {
        return -1;
        }
    map<int, int> distance;
    map<int, bool> visited;
    for (auto location : graph) {
        distance[location.first] = numeric_limits<int>::max();
        visited[location.first] = false;
    }
    distance[start] = 0;

    while (true) {
        int currentLocation = -1;
        int smallestDistance = numeric_limits<int>::max();

        for (auto location : distance) {
            int locationID = location.first;
            if (!visited[locationID] &&
                distance[locationID] < smallestDistance) {
                smallestDistance = distance[locationID];
                currentLocation = locationID;
                }
        }
        if (currentLocation == -1) {
            break;
        }
        if (currentLocation == end) {
            return distance[currentLocation];
        }
        visited[currentLocation] = true;
        for (size_t i = 0; i < graph[currentLocation].size(); i++) {
            Edge edge = graph[currentLocation][i];
            if (edge.closed) {
                continue;
            }
            int nextLocation = edge.destination;
            int newDistance = distance[currentLocation] + edge.time;
            if (newDistance < distance[nextLocation]) {
                distance[nextLocation] = newDistance;
            }
        }
    }
    return -1;
}

CampusCompass::CampusCompass() {
}
bool CampusCompass::validName(string name) {
    if (name.empty()) {
        return false;
    }
    for (size_t i = 0; i < name.length(); i++) {
        if (!isalpha(name[i]) && name[i] != ' ') {
            return false;
        }
    }
    return true;
}
bool CampusCompass::validID(string id) {
    if (id.length() != 8) {
        return false;
    }
    for (size_t i = 0; i < id.length(); i++) {
        if (!isdigit(id[i])) {
            return false;
        }
    }
    return true;
}

bool CampusCompass::validClassCode(string code) {
    if (code.length() != 7) {
        return false;
    }
    for (int i = 0; i < 3; i++) {
        if (code[i] < 'A' || code[i] > 'Z') {
            return false;
        }
    }

    for (int i = 3; i < 7; i++) {
        if (!isdigit(code[i])) {
            return false;
        }
    }
    return true;
}

bool CampusCompass::ParseCSV(const string& edges_filepath, const string& classes_filepath) {
    ifstream edgeFile(edges_filepath);
    if (!edgeFile.is_open()) {
        return false;
    }
    string line;
    getline(edgeFile, line);
    while (getline(edgeFile, line)) {
        stringstream ss(line);
        string firstLocationText;
        string secondLocationText;
        string firstName;
        string secondName;
        string timeText;
        getline(ss, firstLocationText, ',');
        getline(ss, secondLocationText, ',');
        getline(ss, firstName, ',');
        getline(ss, secondName, ',');
        getline(ss, timeText, ',');
        try {
            int firstLocation = stoi(firstLocationText);
            int secondLocation = stoi(secondLocationText);
            int travelTime = stoi(timeText);
            Edge firstEdge;
            firstEdge.destination = secondLocation;
            firstEdge.time = travelTime;
            firstEdge.closed = false;
            Edge secondEdge;
            secondEdge.destination = firstLocation;
            secondEdge.time = travelTime;
            secondEdge.closed = false;
            graph[firstLocation].push_back(firstEdge);
            graph[secondLocation].push_back(secondEdge);
        }
        catch (...) {
            return false;
        }
    }
    edgeFile.close();

    ifstream classFile(classes_filepath);
    if (!classFile.is_open()) {
        return false;
    }
    getline(classFile, line);
    while (getline(classFile, line)) {
        stringstream ss(line);
        string classCode;
        string locationText;
        string startTime;
        string endTime;
        getline(ss, classCode, ',');
        getline(ss, locationText, ',');
        getline(ss, startTime, ',');
        getline(ss, endTime, ',');

        try {
            int location = stoi(locationText);
            classLocations[classCode] = location;
        }
        catch (...) {
            return false;
        }
    }
    classFile.close();
    return true;
}
bool CampusCompass::ParseCommand(const string& command) {
    if (command.substr(0, 17) == "printStudentZone ") {
        stringstream ss(command);
        string commandName;
        string studentID;
        string extra;

        if (!(ss >> commandName >> studentID)) {
            cout << "unsuccessful" << endl;
            return false;
        }
        if (ss >> extra) {
            cout << "unsuccessful" << endl;
            return false;
        }
        if (students.find(studentID) == students.end()) {
            cout << "unsuccessful" << endl;
            return false;
        }
        Student student = students[studentID];
        int cost = studentZoneCost(student);
        cout << "Student Zone Cost For "
             << student.name
             << ": "
             << cost
             << endl;
        return true;
    }
    if (command.substr(0, 19) == "printShortestEdges ") {
        stringstream ss(command);
        string commandName;
        string studentID;
        string extra;

        if (!(ss >> commandName >> studentID)) {
            cout << "unsuccessful" << endl;
            return false;
        }
        if (ss >> extra) {
            cout << "unsuccessful" << endl;
            return false;
        }
        if (students.find(studentID) == students.end()) {
            cout << "unsuccessful" << endl;
            return false;
        }
        Student student = students[studentID];
        vector<string> sortedClasses = student.classes;
        sort(sortedClasses.begin(), sortedClasses.end());

        cout << "Time For Shortest Edges: " << student.name << endl;

        for (size_t i = 0; i < sortedClasses.size(); i++) {
            string classCode = sortedClasses[i];
            int classLocation = classLocations[classCode];
            int time = shortestTime(student.residence, classLocation);
            cout << classCode << ": " << time << endl;
        }
        return true;
    }
    if (command.substr(0, 12) == "isConnected ") {
        stringstream ss(command);
        string commandName;
        int firstLocation;
        int secondLocation;
        string extra;

        if (!(ss >> commandName >> firstLocation >> secondLocation)) {
            cout << "unsuccessful" << endl;
            return false;
        }
        if (ss >> extra) {
            cout << "unsuccessful" << endl;
            return false;
        }
        if (connected(firstLocation, secondLocation)) {
            cout << "successful" << endl;
            return true;
        }
        cout << "unsuccessful" << endl;
        return false;
    }
    if (command.substr(0, 16) == "checkEdgeStatus ") {
        stringstream ss(command);
        string commandName;
        int firstLocation;
        int secondLocation;
        string extra;

        if (!(ss >> commandName >> firstLocation >> secondLocation)) {
            cout << "unsuccessful" << endl;
            return false;
        }
        if (ss >> extra) {
            cout << "unsuccessful" << endl;
            return false;
        }
        bool edgeFound = false;

        if (graph.find(firstLocation) != graph.end()) {
            for (size_t i = 0; i < graph[firstLocation].size(); i++) {
                if (graph[firstLocation][i].destination == secondLocation) {
                    edgeFound = true;
                    if (graph[firstLocation][i].closed) {
                        cout << "closed" << endl;
                    }
                    else {
                        cout << "open" << endl;
                    }
                    return true;
                }
            }
        }
        if (!edgeFound) {
            cout << "DNE" << endl;
            return true;
        }
        return true;
    }
    if (command.substr(0, 19) == "toggleEdgesClosure ") {
        stringstream ss(command);
        string commandName;
        int numberOfEdges;

        if (!(ss >> commandName >> numberOfEdges)) {
            cout << "unsuccessful" << endl;
            return false;
        }
        if (numberOfEdges < 1) {
            cout << "unsuccessful" << endl;
            return false;
        }
        for (int i = 0; i < numberOfEdges; i++) {
            int firstLocation;
            int secondLocation;

            if (!(ss >> firstLocation >> secondLocation)) {
                cout << "unsuccessful" << endl;
                return false;
            }
            for (size_t j = 0; j < graph[firstLocation].size(); j++) {
                if (graph[firstLocation][j].destination == secondLocation) {
                    graph[firstLocation][j].closed =!graph[firstLocation][j].closed;
                    break;
                }
            }
            for (size_t j = 0; j < graph[secondLocation].size(); j++) {
                if (graph[secondLocation][j].destination == firstLocation) {
                    graph[secondLocation][j].closed =!graph[secondLocation][j].closed;
                    break;
                }
            }
        }
        string extra;
        if (ss >> extra) {
            cout << "unsuccessful" << endl;
            return false;
        }
        cout << "successful" << endl;
        return true;
    }
    if (command.substr(0, 12) == "removeClass ") {
        stringstream ss(command);
        string commandName;
        string classCode;
        string extra;
        ss >> commandName >> classCode;

        if (commandName != "removeClass") {
            cout << "unsuccessful" << endl;
            return false;
        }
        if (!validClassCode(classCode)) {
            cout << "unsuccessful" << endl;
            return false;
        }
        if (ss >> extra) {
            cout << "unsuccessful" << endl;
            return false;
        }
        if (classLocations.find(classCode) == classLocations.end()) {
            cout << "unsuccessful" << endl;
            return false;
        }
        int removedCount = 0;
        vector<string> studentsToRemove;

        for (auto& studentPair : students) {
            string studentID = studentPair.first;

            for (size_t i = 0; i < studentPair.second.classes.size(); i++) {
                if (studentPair.second.classes[i] == classCode) {
                    studentPair.second.classes.erase(
                        studentPair.second.classes.begin() + i
                    );
                    removedCount++;
                    if (studentPair.second.classes.empty()) {
                        studentsToRemove.push_back(studentID);
                    }
                    break;
                }
            }
        }

        for (size_t i = 0; i < studentsToRemove.size(); i++) {
            students.erase(studentsToRemove[i]);
        }
        if (removedCount == 0) {
            cout << "unsuccessful" << endl;
            return false;
        }
        cout << removedCount << endl;
        return true;
    }
    if (command.substr(0, 13) == "replaceClass ") {
        stringstream ss(command);
        string commandName;
        string studentID;
        string oldClass;
        string newClass;
        string extra;
        ss >> commandName >> studentID >> oldClass >> newClass;

        if (commandName != "replaceClass") {
            cout << "unsuccessful" << endl;
            return false;
        }
        if (!validID(studentID)) {
            cout << "unsuccessful" << endl;
            return false;
        }
        if (!validClassCode(oldClass) || !validClassCode(newClass)) {
            cout << "unsuccessful" << endl;
            return false;
        }
        if (ss >> extra) {
            cout << "unsuccessful" << endl;
            return false;
        }
        if (students.find(studentID) == students.end()) {
            cout << "unsuccessful" << endl;
            return false;
        }
        if (classLocations.find(newClass) == classLocations.end()) {
            cout << "unsuccessful" << endl;
            return false;
        }
        bool hasOldClass = false;
        bool hasNewClass = false;

        for (size_t i = 0; i < students[studentID].classes.size(); i++) {
            if (students[studentID].classes[i] == oldClass) {
                hasOldClass = true;
            }
            if (students[studentID].classes[i] == newClass) {
                hasNewClass = true;
            }
        }

        if (!hasOldClass || hasNewClass) {
            cout << "unsuccessful" << endl;
            return false;
        }
        for (size_t i = 0; i < students[studentID].classes.size(); i++) {
            if (students[studentID].classes[i] == oldClass) {
                students[studentID].classes[i] = newClass;
                break;
            }
        }
        cout << "successful" << endl;
        return true;
    }
    if (command.substr(0, 10) == "dropClass ") {
        stringstream ss(command);
        string commandName;
        string studentID;
        string classCode;
        string extra;
        ss >> commandName >> studentID >> classCode;

        if (commandName != "dropClass") {
            cout << "unsuccessful" << endl;
            return false;
        }
        if (!validID(studentID)) {
            cout << "unsuccessful" << endl;
            return false;
        }
        if (!validClassCode(classCode)) {
            cout << "unsuccessful" << endl;
            return false;
        }
        if (ss >> extra) {
            cout << "unsuccessful" << endl;
            return false;
        }
        if (students.find(studentID) == students.end()) {
            cout << "unsuccessful" << endl;
            return false;
        }
        if (classLocations.find(classCode) == classLocations.end()) {
            cout << "unsuccessful" << endl;
            return false;
        }
        bool foundClass = false;
        for (size_t i = 0; i < students[studentID].classes.size(); i++) {
            if (students[studentID].classes[i] == classCode) {
                students[studentID].classes.erase(
                    students[studentID].classes.begin() + i
                );
                foundClass = true;
                break;
            }
        }

        if (!foundClass) {
            cout << "unsuccessful" << endl;
            return false;
        }
        if (students[studentID].classes.empty()) {
            students.erase(studentID);
        }
        cout << "successful" << endl;
        return true;
    }
    if (command.substr(0, 7) == "remove ") {
        stringstream ss(command);
        string commandName;
        string studentID;
        string extra;
        ss >> commandName >> studentID;
        if (commandName != "remove") {
            cout << "unsuccessful" << endl;
            return false;
        }
        if (!validID(studentID)) {
            cout << "unsuccessful" << endl;
            return false;
        }
        if (ss >> extra) {
            cout << "unsuccessful" << endl;
            return false;
        }
        if (students.find(studentID) == students.end()) {
            cout << "unsuccessful" << endl;
            return false;
        }
        students.erase(studentID);
        cout << "successful" << endl;
        return true;
    }
    if (command.substr(0, 6) != "insert") {
        cout << "unsuccessful" << endl;
        return false;
    }

    size_t firstQuote = command.find('"');
    size_t secondQuote = command.find('"', firstQuote + 1);

    if (firstQuote == string::npos || secondQuote == string::npos) {
        cout << "unsuccessful" << endl;
        return false;
    }

    string name = command.substr(firstQuote + 1, secondQuote - firstQuote - 1);
    string remaining = command.substr(secondQuote + 1);
    stringstream ss(remaining);
    string studentID;
    int residence;
    int numberOfClasses;

    if (!(ss >> studentID >> residence >> numberOfClasses)) {
        cout << "unsuccessful" << endl;
        return false;
    }
    if (!validName(name)) {
        cout << "unsuccessful" << endl;
        return false;
    }
    if (!validID(studentID)) {
        cout << "unsuccessful" << endl;
        return false;
    }
    if (students.find(studentID) != students.end()) {
        cout << "unsuccessful" << endl;
        return false;
    }

    if (numberOfClasses < 1 || numberOfClasses > 6) {
        cout << "unsuccessful" << endl;
        return false;
    }
    vector<string> classes;
    for (int i = 0; i < numberOfClasses; i++) {
        string classCode;

        if (!(ss >> classCode)) {
            cout << "unsuccessful" << endl;
            return false;
        }

        if (!validClassCode(classCode)) {
            cout << "unsuccessful" << endl;
            return false;
        }
        if (classLocations.find(classCode) == classLocations.end()) {
            cout << "unsuccessful" << endl;
            return false;
        }

        classes.push_back(classCode);
    }
    string extra;
    if (ss >> extra) {
        cout << "unsuccessful" << endl;
        return false;
    }
    Student student;
    student.name = name;
    student.id = studentID;
    student.residence = residence;
    student.classes = classes;
    students[studentID] = student;

    cout << "successful" << endl;
    return true;
}

bool CampusCompass::connected(int start, int end) {
    if (graph.find(start) == graph.end() ||
        graph.find(end) == graph.end()) {
        return false;
        }
    if (start == end) {
        return true;
    }
    vector<int> locationsToVisit;
    map<int, bool> visited;
    locationsToVisit.push_back(start);
    visited[start] = true;

    size_t currentIndex = 0;

    while (currentIndex < locationsToVisit.size()) {
        int currentLocation = locationsToVisit[currentIndex];
        currentIndex++;
        for (size_t i = 0; i < graph[currentLocation].size(); i++) {
            Edge currentEdge = graph[currentLocation][i];
            if (!currentEdge.closed &&
                !visited[currentEdge.destination]) {

                if (currentEdge.destination == end) {
                    return true;
                }
                visited[currentEdge.destination] = true;
                locationsToVisit.push_back(currentEdge.destination);
                }
        }
    }
    return false;
}