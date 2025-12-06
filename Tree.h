#ifndef TREE_H
#define TREE_H

#include "Node.h"
#include "Problem.h"
#include <string>
#include <unordered_map>

using namespace std;

class Tree {
public:
    Problem* problem;                           //to generate the problem
    Node* root;
    unordered_map<string, Node*> all_nodes;     //tracks the states that have been created

    Tree(Problem* p) {                       //constructor
        problem = p;

        root = new Node(problem->initialState);     //create the root node to perform problem on

        string key = problem->toString(root->state);  //Converts 2D vector to string 
        all_nodes[key] = root;                  // Stores root node using the key
    }
};

#endif