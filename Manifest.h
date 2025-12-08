#ifndef MANIFEST_H
#define MANIFEST_H

#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cctype>
#include <string>

using namespace std;

struct Container {       //used to keep track each cell in the grid
    int x;               // row index (0 = bottom, 7 = top)
    int y;               // column index (0 = left, 11 = right)
    int weight;
    string description;  //used to check for NAN, UNUSED, and what the container holds
    bool isEmpty;        //keeps track of if a cell can have a container moved to it
    bool isIllegal;      //This is if the cell is NAN
};

class Manifest {
private:
    vector<Container> containers;   

public:
    Container* grid[8][12] = {nullptr};

    void loadManifest(const string& filename);
    void buildGrid();
    void clearContainers() { containers.clear(); }
    int getContainerCount() { return containers.size(); } // returns number of containers in manifest (will be used for log)
    Container getCurrContainer(int index) { return containers[index]; } // returns container at specified index
    vector<pair<int,int>> movable_boxes();
    vector<vector<int>> grid_to_vector() const;
    vector<vector<Container>> get() const;
};
vector<vector<Container>> Manifest::get() const {
    vector<vector<Container>> gridState;

    for(int i = 0; i < 8; i++) {
        vector<Container> row;
        for(int j = 0; j < 12; j++) {
            if (grid[i][j] != nullptr) {
                row.push_back(*grid[i][j]);
            } else {
                Container emptyBox;
                emptyBox.x = i;
                emptyBox.y = j;
                emptyBox.weight = 0;
                emptyBox.description = "UNUSED";
                emptyBox.isEmpty = true;
                emptyBox.isIllegal = false;
                row.push_back(emptyBox);
            }
        }
        gridState.push_back(row);
    }
    return gridState;
}

void Manifest::loadManifest(const string& filename) {                       //getting the data from the manifest file
    ifstream inFS(filename);

    if (!inFS.is_open()) {                                                  //error handling for file
        cout << "Error: Cannot open Manifest file: " << filename << endl;
        exit(1);
    }

    containers.clear();
    string line;
    int lineNumber = 0;

    while (getline(inFS, line)) {
        lineNumber++;

        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        bool allws = true;
        for (char ch : line)
            if (!isspace((unsigned char)ch)) { allws = false; break; }
        if (allws) continue;

        size_t rowStart = line.find('[');
        size_t comma = line.find(',', rowStart);
        size_t rowEnd = line.find(']', comma);
        size_t weightStart = line.find('{', rowEnd);
        size_t weightEnd = line.find('}', weightStart);
        size_t descStart = line.find("},", weightEnd);

         if (rowStart == string::npos || comma == string::npos || rowEnd == string::npos || weightStart == string::npos || weightEnd == string::npos || descStart == string::npos) {
            cout << "Warning: malformed line " << lineNumber << endl;
            continue;
        }

        Container box;

        string rowStr = line.substr(rowStart + 1, comma - (rowStart + 1));
        string colStr = line.substr(comma + 1, rowEnd - (comma + 1));

        auto trim = [](string &s) {
            size_t a = 0;
            while (a < s.size() && isspace((unsigned char)s[a])) a++;
            size_t b = s.size();
            while (b > a && isspace((unsigned char)s[b-1])) b--;
            s = s.substr(a, b - a);
        };
        trim(rowStr);
        trim(colStr);

        try {
            box.x = stoi(rowStr) - 1;  //Changes row/column value to index value (1 --> 0)
            box.y = stoi(colStr) - 1;  //Stoi used to convert string "01" to integer 1
        }
        catch (...) { //If any errors occur when trying to change the string to an integer this catch will output a warning
            cout << "Warning: bad coordinates line " << lineNumber << endl; 
            continue;
        }

        string weightStr = line.substr(weightStart + 1, weightEnd - (weightStart + 1));
        trim(weightStr);

        int weightValue = 0;
        bool weightValid = true; //Tells us if the weight we read from file is valid (only non-negative digits)

        
        string digits = "";
        for (int i = 0; i < weightStr.length(); i++){ //iterates through each character in the weight
            char c = weightStr[i];
            if (c >= '0' && c <= '9') { //Checks for valid weights values.
                digits.push_back(c);
            }   
        }
        if (digits.empty()) {
            weightValid = false;
        } else {
            weightValue = stoi(digits);
            if (weightValue < 0) {       // Checks if weight is negative
                cout << "Warning: negative weight found, setting to 0" << endl;
                weightValue = 0;
            }
        }
        

        string description = line.substr(descStart + 3); //Extract our description of what is in the container
        trim(description);

        if (!weightValid){ //Invalid weights are changed to 0
            box.weight = 0;
        } else { //Valid weights are assigned to the container
            box.weight = weightValue;
        } 
        box.description = description;
        box.isEmpty = false;
        box.isIllegal = false;

        if (description == "UNUSED") { //If the cell is unused we mark it as empty and weight = 0
            box.isEmpty = true;
            box.weight = 0;
        }
        else if (description == "NAN") { //If the cell is NAN we mark it as illegal so that we can't move containers there
            box.isIllegal = true;
            box.weight = 0;
        }

        containers.push_back(box); //Add cell to the list of containers
    }

    inFS.close();
}


void Manifest::buildGrid() {
    for (int row = 0; row < 8; ++row){
        for (int col = 0; col < 12; ++col){
            grid[row][col] = nullptr; //Initializes all of our grid positions to nullptr (empty)
        }
    }
        
    for (int i = 0; i < containers.size(); i++) { //Place each container in the grid
        Container& box = containers[i];  //references the current contianer
        if (box.x >= 0 && box.x < 8 && box.y >= 0 && box.y < 12) {
            grid[box.x][box.y] = &box;    // store a pointer to the container in the grid
        }
    }
}

vector<pair<int,int>> Manifest::movable_boxes() { //To prevent moving boxes from under another box
    vector<pair<int,int>> boxes;

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 12; ++col) {

            Container* cell = grid[row][col]; // gets pointer to the current container

            if (cell == nullptr || cell->isEmpty || cell->isIllegal){ // Skip the cell if it is empty or illegal
                continue;
            }
                

            bool blockedAbove = false;

            if (row < 7) { //Check for all except for the top row (top row can't have a container above)
                Container* above = grid[row+1][col]; //Container above current box
                if (above != nullptr && !above->isEmpty && !above->isIllegal){ // If there is a real container above, mark as blocked. 
                    blockedAbove = true;
                }         
            }

            if (!blockedAbove){ //If there it is not blocked above, the box can be moves
                boxes.push_back({row, col});
            }
        }
    }

    return boxes;
}


vector<vector<int>> Manifest::grid_to_vector() const {
    vector<vector<int>> result(8, vector<int>(12, 0)); //Creates a 2D vector 8x12

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 12; ++col) {
            if (grid[row][col] != nullptr) { //Checks if there is a container in the position
                if (grid[row][col]->isIllegal){   //Checks if this cell is illegal
                    result[row][col] = -1;
                } else if (grid[row][col]->isEmpty){ //Checks if the cell is used 
                    result[row][col] = 0;
                } else {
                    result[row][col] = grid[row][col]->weight; //Stores the weight of the container
                }
            }
        }
    }
    return result;
}

#endif
