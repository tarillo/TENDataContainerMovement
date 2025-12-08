#ifndef UNIFORMCOST_H
#define UNIFORMCOST_H

#include <iostream>
#include <queue>
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <climits>
#include "Node.h"
#include "Tree.h"
#include "Problem.h"

using namespace std;

class uniformCost {
public:
    struct SearchStats {
        int nodesExpanded = 0;
        int maxQueueSize = 0;
        int solutionDepthMoves = 0;
        int solutionDepthMinutes = 0;
    };

    struct NodeCompare { 
        bool operator()(Node* a, Node* b) const {
            return a->cost > b->cost;
        }
    };

    Node* search(Tree* tree, Problem* problem, SearchStats& stats);  
    vector<string> getSolutionPath(Node* goal); 
};

Node* uniformCost::search(Tree* tree, Problem* problem, SearchStats& stats) {

    priority_queue<Node*, vector<Node*>, NodeCompare> frontier;
    unordered_map<string, int> gscore;

    Node* start = tree->root;
    start->cost = 0;

    frontier.push(start);
    gscore[problem->toString(start->state)] = 0;

    // BEST FALLBACK NODE (not equal to start)
    Node* bestNode = nullptr;
    int bestImbalance = INT_MAX;
    int bestCost = INT_MAX;

    while (!frontier.empty()) {

        stats.maxQueueSize = max(stats.maxQueueSize, (int)frontier.size());

        Node* current = frontier.top();
        frontier.pop();

        stats.nodesExpanded++;

        string key = problem->toString(current->state);

        // Skip stale frontier nodes
        if (gscore.find(key) != gscore.end() && current->cost > gscore[key])
            continue;

        // Compute imbalance for this node
        int currImbalance = problem->compute_imbalance(current->state);

        // 🔥 FIXED: Initialize bestNode ONLY AFTER first non-start node appears
        if (bestNode == nullptr && current != start) {
            bestNode = current;
            bestImbalance = currImbalance;
            bestCost = current->cost;
        }

        // 🔥 FIXED: Update best fallback only using successor states, not root
        if (current != start) {
            if (currImbalance < bestImbalance ||
                (currImbalance == bestImbalance && current->cost < bestCost)) {

                bestImbalance = currImbalance;
                bestCost = current->cost;
                bestNode = current;
            }
        }

        // Perfect goal reached
        if (problem->goal(current->state)) {

            stats.solutionDepthMinutes = current->cost;
            stats.solutionDepthMoves = getSolutionPath(current).size();

            cout << "\n GOAL FOUND\n";
            cout << "Total Cost (Minutes): " << current->cost << endl;
            cout << "Solution Depth (Moves): " << stats.solutionDepthMoves << endl;

            return current;
        }

        // Expand successors
        vector<pair<pair<int,int>, pair<int,int>>> acts = problem->actions(current->state);

        for (auto& action : acts) {

            vector<vector<int>> newState = problem->result(current->state, action);

            int step_cost = problem->path_length(current->state, action.first, action.second);
            if (step_cost <= 0) continue;

            int newCost = current->cost + step_cost;
            string childKey = problem->toString(newState);

            // Standard UCS pruning
            if (gscore.find(childKey) != gscore.end() && newCost >= gscore[childKey])
                continue;

            gscore[childKey] = newCost;

            int sr = action.first.first;
            int sc = action.first.second;
            int dr = action.second.first;
            int dc = action.second.second;

            string moveDesc =
                "Move from [" + to_string(sr) + "," + to_string(sc) +
                "] to [" + to_string(dr) + "," + to_string(dc) + "]";

            Node* child = new Node(newState, current, moveDesc, newCost, 0, dr, dc);
            frontier.push(child);
        }
    }

    // If no perfect balance was found, return the best fallback
    cout << "No perfect solution found. Using best imbalance fallback.\n";
    cout << "Best imbalance = " << bestImbalance << " with cost = " << bestCost << endl;

    return bestNode;  // → guaranteed not to be the start node now
}


vector<string> uniformCost::getSolutionPath(Node* goal) {
    vector<string> steps;
    Node* currNode = goal;

    while (currNode != nullptr && currNode->parent != nullptr) {
        steps.push_back(currNode->action);
        currNode = currNode->parent;
    }

    reverse(steps.begin(), steps.end());
    return steps;
}

#endif
