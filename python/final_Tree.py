import final_problem
import copy
import Node
from collections import deque


class Tree:                                                     #build the tree that we will run search algorithms on
    def __init__(self, problem):                                #constructor
        self.problem = problem
        self.root = Node.Node(None, problem.initial_state)           #creates root
        self.all_nodes = {problem.to_tuple(self.root.state): self.root} #tracks all nodes we have created
        self.build_tree(self.root)                              #builds the tree

    def build_tree(self, parent_node, depth_limit=100):            #expand the tree, can change the depth limit
        if parent_node.depth >= depth_limit:
            return
        
        for action in self.problem.actions(parent_node.state):            #go through each valid actiions
            new_state = self.problem.result(parent_node.state, action)
            new_state_tuple = self.problem.to_tuple(new_state)

            if new_state_tuple not in self.all_nodes:               #if its not a repeated state add the node to the tree
                child_node = Node.Node(parent = parent_node, state = new_state, depth = parent_node.depth + 1, cost = parent_node.cost + 1)       #create new child node

                parent_node.children.append(child_node)             #add the child node
                self.all_nodes[new_state_tuple] = child_node        #add the new node to the list of nodes(used when checking for duplicates)

                self.build_tree(child_node, depth_limit)            #recursive step


