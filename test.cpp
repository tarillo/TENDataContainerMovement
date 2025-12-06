// #include <iostream>
// #include <vector>
// #include <string>
// #include <algorithm>
// #include "Manifest.h"
// #include "Problem.h"
// #include "Node.h"
// #include "Tree.h"
// #include "astar.h"

// using namespace std;

// int main() {
//     // 1. Load the manifest
//     Manifest manifest;
//     manifest.loadManifest("manifest.txt");  // your input file
//     manifest.buildGrid();

//     // 2. Create problem instance
//     Problem problem(manifest.grid_to_vector());
//     problem.manifest = &manifest;   // set the manifest pointer

//     // 3. Build tree (optional, for your current Tree implementation)
//     Tree tree(&problem);

//     // 4. Run A* search
//     aStar astar_solver;
//     Node* start = tree.root;

//     // Priority queue and visited set (duplicating search to print moves as we expand)
//     struct CompareNodes {
//         bool operator()(Node* a, Node* b) const {
//             return a->total_cost() > b->total_cost();
//         }
//     };

//     priority_queue<Node*, vector<Node*>, CompareNodes> pq;
//     pq.push(start);
//     unordered_set<string> visited;

//     cout << "Expanding nodes:" << endl;

//     Node* goal = nullptr;
//     while (!pq.empty()) {
//         Node* current = pq.top();
//         pq.pop();

//         string key = problem.toString(current->state);
//         if (visited.find(key) != visited.end())
//             continue;

//         visited.insert(key);

//         // Print the move and total cost
//         if (!current->action.empty()) {
//             cout << "Move: " << current->action
//                  << " | Cost so far: " << current->cost
//                  << " | Total (cost + heuristic): " << current->total_cost() << endl;
//         }

//         if (problem.goal(current->state)) {
//             goal = current;
//             break;
//         }

//         for (Node* child : current->children) {
//             string child_key = problem.toString(child->state);
//             if (visited.find(child_key) == visited.end()) {
//                 child->heuristic = problem.heuristic(child->state);
//                 pq.push(child);
//             }
//         }
//     }

//     if (goal == nullptr) {
//         cout << "No solution found." << endl;
//         return 0;
//     }

//     // Print final solution summary
//     cout << "\nSolution found in " << goal->cost << " moves." << endl;

//     vector<string> steps = astar_solver.getSolutionPath(goal);
//     cout << "\nSteps in solution path:" << endl;
//     for (int i = 0; i < steps.size(); ++i) {
//         cout << i + 1 << ". " << steps[i] << endl;
//     }

//     return 0;
// }
#include <iostream>
#include <vector>
#include <string>
#include "Manifest.h"
#include "Problem.h"
#include "Node.h"
#include "Tree.h"
#include "astar.h"

using namespace std;

int main() {
    Manifest manifest;
    manifest.loadManifest("manifest.txt");
    manifest.buildGrid();

    Problem problem(manifest.grid_to_vector());
    problem.manifest = &manifest;

    Tree tree(&problem);

    aStar solver;
    cout << "Starting A*..." << endl;
    Node* goal = solver.search(&tree, &problem);

    if (goal == nullptr) {
        cout << "No solution found." << endl;
        return 0;
    }

    cout << "\nSolution found in cost " << goal->cost << " moves (total cost)." << endl;
    vector<string> steps = solver.getSolutionPath(goal);
    cout << "\nSteps in solution path:" << endl;
    for (int i = 0; i < (int)steps.size(); ++i) {
        cout << i + 1 << ". " << steps[i] << endl;
    }

    return 0;
}
