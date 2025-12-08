#ifndef ASTAR_H
#define ASTAR_H
#include <iostream>
#include <queue>
#include <unordered_set>
#include <string>
#include "Node.h"
#include "Tree.h"
#include "Problem.h"

using namespace std;

class aStar {
public:
    struct NodeCompare { //Comparison so that nodes with smaller cost + heuristic have higher priority
        bool operator()(Node* a, Node* b) const {
            return (a->cost + a->heuristic) > (b->cost + b->heuristic);
        }
    };
    struct SearchStats {
        int nodesExpanded = 0;
        int maxQueueSize = 0;
        int solutionDepthMoves = 0;
        int solutionDepthMinutes = 0;
    };

    vector<Node*> getSolutionPath(Node* goal); //Reconstructs solution path from goal node back to start
    Node* search(Tree* tree, Problem* problem, SearchStats& stats);  //Main A* Search function
    vector<pair<vector<vector<int>>, int>> generate_successors(const Problem& prob, const vector<vector<int>>& state);
};


Node* aStar::search(Tree* tree, Problem* problem, SearchStats& stats) {
    priority_queue<Node*, vector<Node*>, NodeCompare> frontier;
    unordered_map<string,int> gscore; //Keeps track of the lowest cost found so far for each state

    //Initializing the start node.
    Node* start = tree->root;
    start->cost = 0;
    start->heuristic = problem->heuristic(start->state);

    frontier.push(start);
    gscore[problem->toString(start->state)] = 0;


    Node* bestNode = start;
    int bestImbalance = start->heuristic;   //Best heuristic (imbalance) found so far

    int expansions = 0;
 
    while (!frontier.empty()) {  //A* Search Loop
        if (frontier.size() > stats.maxQueueSize) {
            stats.maxQueueSize = frontier.size();
        }
        Node* current = frontier.top();  //Node with smallest total cost
        frontier.pop();
        
        stats.nodesExpanded++;


        string key = problem->toString(current->state);

        unordered_map<string, int>::iterator gscoreIterator = gscore.find(key); //Look up the current state's cost in gscore map

        // If the state exists in the map and the current path cost is higher than the recorded one, skip it
        if (gscoreIterator != gscore.end() && current->cost > gscoreIterator->second) {
            continue;
        }

        int imbalance = problem->heuristic(current->state);  //Update the best node if current heuristic is lower
        if (imbalance < bestImbalance) {
            bestImbalance = imbalance;
            bestNode = current;
        }

        if (problem->goal(current->state)) { // Test to see if we reach goal. If so, we return it
             Node* temp = current;
            int depthMoves = 0;
            int depthMinutes = 0;

            while (temp->parent != nullptr) {
                depthMoves++;
                depthMinutes += temp->cost - temp->parent->cost; // accumulate step costs
                temp = temp->parent;
            }

            stats.solutionDepthMoves = depthMoves;
            stats.solutionDepthMinutes = depthMinutes;
            cout << "Goal found with imbalance = " << imbalance << " cost = " << current->cost << endl;
            return current;
        }

        vector<pair<pair<int,int>, pair<int,int>>> acts = problem->actions(current->state); //Generate all possible moves/actions from current state

        for (int i = 0; i < acts.size(); i++) {
            pair<pair<int,int>, pair<int,int>> action = acts[i]; //Each action is a pair of coords
            vector<vector<Container>> newState = problem->result(current->state, action); //Generate new state after applying this action

            int step_cost = problem->path_length(current->state, action.first, action.second);
            
            if (step_cost <= 0){ //Prevents negative costs
                continue;
            } 

            int newCost = current->cost + step_cost; //Total cost to reach new state
            string childKey = problem->toString(newState);

            unordered_map<string, int>::iterator gscoreIt = gscore.find(childKey);  // Check if we already have a cheaper path to this state

            if (gscoreIt != gscore.end() && newCost >= gscoreIt->second) {
                continue; // Skip this child because we found a better path before
            }
            
            gscore[childKey] = newCost;

            int newHeuristic = problem->heuristic(newState);
            
            //Extract the source and destination coordinates
            int sourceRow = action.first.first;
            int SourceColumn = action.first.second;
            int destinationRow = action.second.first;
            int destinationColumn = action.second.second;
            
            // the string description of the move to make, with color coding for terminal

            int SR = sourceRow + 1;
            int SC = SourceColumn + 1;
            int DR   = destinationRow + 1;
            int DC   = destinationColumn + 1;
            string moveDesc = "Move from \x1b[32m[" + to_string(SR) + "," + to_string(SC) +"]\x1b[0m to \x1b[31m[" + to_string(DR) + "," + to_string(DC) + "]\x1b[0m";


            Node* child = new Node(newState, current, moveDesc, newCost, newHeuristic, destinationRow, destinationColumn);

            frontier.push(child);
        }
    }

    
    cout << "No 10% solution. Best imbalance = " << bestImbalance << endl; // If not exact goal found, return best solution seen (least imbalance
    return bestNode;
}

vector<Node*> aStar::getSolutionPath(Node* goal) { //Returns sequence of actions from start node to the goal state
    vector<Node*> steps;
    Node* currNode = goal; //Starts from the goal

    while (currNode != nullptr && currNode->parent != nullptr) { //Traverse from goal node back to the root node
        steps.push_back(currNode);
        currNode = currNode->parent;
    }

    reverse(steps.begin(), steps.end());
    return steps;
}

#endif
