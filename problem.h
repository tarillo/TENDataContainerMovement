#ifndef PROBLEM_H
#define PROBLEM_H

#include <vector>
#include <cmath>
#include <string>
#include <queue>
#include <algorithm>
#include <numeric>   // for accumulate
#include "Manifest.h"

using namespace std;

class Problem {
public:
    vector<vector<Container>> initialState;       //initial state of the grid/ship
    int row = 8;                        
    int col = 12;  
    Manifest* manifest;      

    Problem()   {                                                       //constructor
        initialState = vector<vector<Container>>(8, vector<Container>(12));       //all values set to zero
    }
    Problem(const vector<vector<Container>>& initial) {                       //constructer with passed in initial value
        initialState = initial;
    }

    bool is_solvable(const vector<vector<Container>>& state) const;
    bool goal(const vector<vector<Container>>& state) const;
    string toString(const vector<vector<Container>>& state) const;
    int heuristic(const vector<vector<Container>>& state) const;
    vector<pair<pair<int,int>, pair<int,int>>> actions(const vector<vector<Container>>& state) const;
    vector<vector<Container>> result(const vector<vector<Container>>& state, pair<pair<int,int>, pair<int,int>> action) const;
    int path_length(const vector<vector<Container>>& state, pair<int,int> start, pair<int,int> goal) const;

   
    int compute_imbalance(const vector<vector<Container>>& state) const;
};


bool Problem::is_solvable(const vector<vector<Container>>& state) const {
    int portSide = 0;
    int starSide = 0;

    for (int i = 0; i < row; ++i)    {                  //get weight of each side
        for (int j = 0; j < col; ++j)   {
            int weight = state[i][j].weight;
            if(weight <= 0)  {
                continue;
            }
            if(j < col/2)   {
                portSide += weight;
            }
            else {
                starSide += weight;
            }
        }
    }

    int totalWeight = portSide + starSide;

    if(totalWeight <= 1)  {
        return true;
    }

    int balance = abs(portSide - starSide);             //check if ship is balanced
    if(balance <= totalWeight * 0.10)   {
        return true;
    }

    int heavier = max(portSide,starSide);                     //check if we can balance the ship
    int maxAllowed = int(totalWeight *0.55);
    if(heavier <= maxAllowed)  {
        return true;
    }

    return false;
}


bool Problem::goal(const vector<vector<Container>>& state) const {
    int portSide = 0;
    int starSide = 0;

    for (int i = 0; i < row; ++i)    {                  //get weight of each side
        for (int j = 0; j < col; ++j)   {
            int weight = state[i][j].weight;
            if(weight <= 0)  {
                continue;
            }
            if(j < col/2)   {
                portSide += weight;
            }
            else {
                starSide += weight;
            }
        }
    }

    

    int balance  = abs(portSide - starSide);
    int limit = (portSide + starSide) * 0.10;

    return balance <= limit;                            //return if its balanced or not
}

string Problem::toString(const vector<vector<Container>>& state) const {      //turns the state into string that can be used in aStar to check if this state is already visited
    string currState;                                                   
    for (int i = 0; i < row; ++i)
        for (int j = 0; j < col; ++j)
            currState += to_string(state[i][j].weight) + ",";
    return currState;
}


int Problem::heuristic(const vector<vector<Container>>& state) const {            //get the heuristic value
    int portSide = 0;
    int starSide = 0;

    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            int weight = state[i][j].weight;
            if (weight <= 0)    {
                continue;
            }
            if (j < col/2)  {
                portSide += weight;
            }
            else    {
                starSide += weight;
            }
        }
    }
    return abs(portSide - starSide);
}

int Problem::compute_imbalance(const vector<vector<Container>>& state) const {
    return heuristic(state);
}

int Problem::path_length(const vector<vector<Container>>& state, pair<int,int> start, pair<int,int> goal) const {            //get the length of the path the crane needs to take

    int R = row;
    int C = col;
    int startR = start.first; 
    int startC = start.second;
    int goalR = goal.first;
    int goalC = goal.second;

    if (startR < 0 || startR >= R || startC < 0 || startC >= C)     {       //make sure our initial state and goal state is within the bounds of the grid(valid)
        return -1;
    }

    if (goalR < 0 || goalR >= R || goalC < 0 || goalC >= C)     {
        return -1;
    }

    if (state[startR][startC].weight <= 0) return -1; //nothing to move
    if (startR == goalR && startC == goalC) return -1;        //make sure we are not moving to the same place

    
    for (int r = startR + 1; r < R; ++r) {      //making sure there is nothing above the container we wish to lift
        if (state[r][startC].weight > 0)   {
            return -1;
        }
    }

    
    vector<vector<int>> visited(R, vector<int>(C, 0));      //BFS setup, keeps track of visited cells
    queue<pair<pair<int,int>, int>> q;                      //queue to keep track of cost
    q.push(make_pair(make_pair(startR, startC), 0));
    visited[startR][startC] = 1;

    int x[4] = {1, -1, 0, 0};               //can move up or down in x direction
    int y[4] = {0, 0, -1, 1};               //can move side to side in y direction

    while (!q.empty()) {                    //make sure queue is not empty
        pair<pair<int,int>, int> front = q.front(); q.pop();
        pair<int,int> curr = front.first;
        int dist = front.second;            //cost to get to curr
        int r = curr.first;
        int c = curr.second;

        
        if (r == goalR && c == goalC) {     //check if goal is reached
            if (goalR == 0)    {            //check if its on bottom row
                return dist; 
            }
            if (goalR - 1 == startR && goalC == startC){        //check for floating container
                return -1;
            }

            if (state[goalR - 1][goalC].weight > 0) {          //container beneath
                return dist;
            }
            return -1;
        }

        
        for (int k = 0; k < 4; ++k) {                   //checking neighbors
            int newRow = r + x[k];
            int newCol = c + y[k];

            if (newRow < 0 || newRow >= R || newCol < 0 || newCol >= C){        //checking if its in the ship
                continue;
            } 

            if (visited[newRow][newCol]) {                      //check if its already visited
                continue;
            }

            int cellVal = state[newRow][newCol].weight;
            bool isSourceCell = (newRow == startR && newCol == startC);

            if (!isSourceCell && cellVal != 0) continue;

            visited[newRow][newCol] = 1;
            q.push(make_pair(make_pair(newRow, newCol), dist + 1));         //push to queue to continue the BFS      
        }
    }

    return -1;           //no path
}



vector<pair<pair<int,int>, pair<int,int>>> Problem::actions(const vector<vector<Container>>& state) const {
    vector<pair<pair<int,int>, pair<int,int>>> moves;

    for (int i = 0; i < col; ++i) {
        int sRow = -1;
        for (int j = row - 1; j >= 0; --j) {            //get top conatiner in the specified column
            if (state[j][i].weight > 0) { 
                sRow = j; 
                break; 
            }
        }
        if (sRow == -1){  //column is empty
            continue;
        }  

        int sCol = i;


        for (int k = 0; k < col; ++k) {                 //move container to other columns
            if (k == sCol) continue;
            int c = k;
            int newRow = -1;
            for (int rr = 0; rr < row; ++rr) {
                if (state[rr][c].weight != 0) continue;        //cannot move here
                if (rr == 0) {                          //bottom row
                    newRow = rr; 
                    break; 
                }
                if (state[rr - 1][c].weight > 0) {             //conatainer is below
                    newRow = rr; 
                    break; 
                }
                
            }
            if (newRow == -1) continue;                      //cannot place in this column

            
            if (newRow == sRow && c == sCol) continue;       

            
            int dist = path_length(state, make_pair(sRow, sCol), make_pair(newRow, c));      //check if we can legally move container to the spot
            if (dist <= 0) continue; 

            moves.push_back(make_pair(make_pair(sRow, sCol), make_pair(newRow, c)));     //add the move
        }
    }

    return moves;
}



vector<vector<Container>> Problem::result(const vector<vector<Container>>& state, pair<pair<int,int>,pair<int,int>> action) const {
    vector<vector<Container>> newState = state;           //make sure we dont overwrite the original state

    int sRow = action.first.first;                  //get the action we eed to perform
    int sCol = action.first.second;
    int gRow = action.second.first;
    int gCol = action.second.second;

    int weight = newState[sRow][sCol].weight;                   //store the weight
    newState[sRow][sCol].weight = 0;                       //remove container from where it originally was
    newState[gRow][gCol].weight = weight;                       //container is moved to its new location
    newState[gRow][gCol].isEmpty = false;
    newState[sRow][sCol].isEmpty = true;
    newState[gRow][gCol].description = newState[sRow][sCol].description;
    newState[sRow][sCol].description = "UNUSED";
    return newState;
}

#endif

