#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Node {
    vector<vector<int>> state;    // state is the coordinate in the 8 x 12 grid representing the boat
    Node* parent;
    string action;                // Action is the description of the moves we are making. (Left, right, up down)
    int cost;                     // accumulated cost(represented by manhattan distance)
    int heuristic;                // heuristic value (the calculated imbalance)
    int crane_row;                  // crane row position (0..7)
    int crane_col;                  // crane col position (0..11)

    Node(const vector<vector<int>>& s, int row = 0, int column = 7) {
        state = s;
        parent = nullptr;
        action = "";
        cost = 0;
        heuristic = 0;
        crane_row = row;
        crane_col = column;
    }

    Node(const vector<vector<int>>& s, Node* p, const string& a, int c, int h, int row, int column) {
        state = s;
        parent = p;
        action = a;
        cost = c;
        heuristic = h;
        crane_row = row;
        crane_col = column;
    }

    double total_cost() const {
        return cost + heuristic;
    }
};

#endif