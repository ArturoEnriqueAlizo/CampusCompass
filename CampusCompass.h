#pragma once
#include <string>
#include <vector>
#include <map>

using namespace std;

struct Student {
    string name;
    string id;
    int residence;
    vector<string> classes;
};
struct Edge {
    int destination;
    int time;
    bool closed;
};
class CampusCompass {
private:
    map<int, vector<Edge>> graph;
    map<string, Student> students;
    map<string, int> classLocations;
public:
    CampusCompass();
    bool ParseCSV(const string& edges_filepath, const string& classes_filepath);
    bool ParseCommand(const string& command);
    bool validName(string name);
    bool validID(string id);
    bool validClassCode(string code);
    bool connected(int start, int end);
    int shortestTime(int start, int end);
    int studentZoneCost(Student student);
    vector<int> shortestPath(int start, int end);
};