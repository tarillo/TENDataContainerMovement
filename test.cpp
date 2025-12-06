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
