class Node:                     #node class for the tree(helps build the tree)
    #Each node tracks its parent, current state, cost, depth, heuristic, and children.
    def __init__(self, parent, state, action, cost=0, heuristic=0):                #constructor
        self.parent = parent                                                        #parent pointer for backtracking
        self.state = state                                                          #Current State
        self.action = action                                                          #Depth in the tree
        self.cost = cost                                                            #g(n)
        self.heuristic = heuristic                                                  #default set to 0 for uniform cost search. h(n)
        self.children = []                                                          #keep track of children

    def total_cost(self):                                                           #total cost used in search algorithm
        return self.cost + self.heuristic                                           #f(n) = g(n) + h(n)

    def __lt__(self, other):                                                        #this will compare two nodes to see which has the least cost
        return self.total_cost() < other.total_cost()

    def path(self):                                                             #path to get to final solution. Uses backtracking
        currentNode = self
        path = []
        while currentNode is not None:
            path.append(currentNode.state)
            currentNode = currentNode.parent
        path.reverse()                  # because we collected states from goal to root
        return path
