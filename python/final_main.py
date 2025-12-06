import heapq #For the priority queue (heap) to implement Uniform Cost Search
from final_Tree import Tree
from final_problem import Problem

def aStar_euclidean(tree, problem):
    start = tree.root                                                           #get root node(initial state)
    start.heuristic = problem.euclidean_distance_heuristic(start.state)         #get the staright line distance for the starting node/state
    problem_queue = [start]                                                     #queue for the nodes we are tracking. begins with the root node
    heapq.heapify(problem_queue)                                                #turn heap into min heap so that shortest total cost is at the front
    visited = set()                                                             #track visited nodes
    max_queue_size = 0
    nodes_expanded = 0

    while problem_queue:
        max_queue_size = max(max_queue_size, len(problem_queue))

        current_node = heapq.heappop(problem_queue)                             #get shortest total cost

        print(f"The best state to expand with g(n) = ", current_node.cost, " and h(n) = ", current_node.heuristic, " is...")
        problem.display_state(current_node.state)
        print("Expanding this node...")

        if problem.is_goal(current_node.state):
            print("Goal!!!")
            print(f"To solve this problem the search algorithm expanded a total of ", nodes_expanded, " nodes.")
            print(f"The maximum number of nodes in the queue at any one time: ", max_queue_size, ".")
            print(f"The depth of the goal node was ", current_node.depth, ".")
            return current_node

        visited.add(problem.to_tuple(current_node.state))
        nodes_expanded += 1

        for child in current_node.children:
            if problem.to_tuple(child.state) not in visited:
                child.heuristic = problem.euclidean_distance_heuristic(child.state)                 #get the children of the node and get the euclidean value
                heapq.heappush(problem_queue, child)                                                #add the chidlren to the queue

    return None

def a_star_search_misplaced_tile(tree, problem):
    start = tree.root
    start.heuristic = problem.misplaced_tile_heuristic(start.state)  # Set heuristic at root
    problem_queue = [start]
    heapq.heapify(problem_queue)
    visited = set()
    max_queue_size = 0
    nodes_expanded = 0

    while problem_queue:
        max_queue_size = max(max_queue_size, len(problem_queue))    # Update the maximum queue size if necessary

        current_node = heapq.heappop(problem_queue)     # Get the node with the lowest f(n) = g(n) + h(n)

        print(f"The best state to expand with g(n) = {current_node.cost} and h(n) = {current_node.heuristic} is...")
        problem.display_state(current_node.state)
        print("Expanding this node...")

        if problem.is_goal(current_node.state):
            print("Goal!!!")
            print(f"To solve this problem the search algorithm expanded a total of {nodes_expanded} nodes.")
            print(f"The maximum number of nodes in the queue at any one time: {max_queue_size}.")
            print(f"The depth of the goal node was {current_node.depth}.")
            return current_node

        visited.add(problem.to_tuple(current_node.state))    # Mark the current state as visited
        nodes_expanded += 1    # Increment the number of nodes expanded

        for child in current_node.children:   # Expand all children of the current node
            if problem.to_tuple(child.state) not in visited:
                child.heuristic = problem.misplaced_tile_heuristic(child.state)  # <--- Add heuristic to child!
                heapq.heappush(problem_queue, child)
    
    return None

def is_solvable(state):
    # First, turn the 2D puzzle into a 1D list (but skip the blank tile, which is 0)
    flat_list = []
    for row in state:
        for num in row:
            if num != 0:
                flat_list.append(num)

    # Now, count how many times a bigger number comes before a smaller one
    inversions = 0
    for i in range(len(flat_list)):
        for j in range(i + 1, len(flat_list)):
            if flat_list[i] > flat_list[j]:
                inversions += 1

    # If the number of inversions is even, the puzzle is solvable
    if inversions % 2 == 0:
        return True
    else:
        return False

def main():
    print("Welcome to 862430896, 862420602 8 puzzle solver.")
    print("Type 1 to use a default puzzle, or 2 to enter your own puzzle.")
    
    choice = input()

    if choice == "1":     #Defaul puzzle
        initial_state = [[1, 2, 0], [4, 5, 3], [7, 8, 6]]
    elif choice == "2":   # User enters their own puzzle
        print("Enter your puzzle, use a zero to represent the blank")
        print("Enter the first row, use space or tabs between numbers")
        print("Enter the second rw, use space or tabs between numbers")
        print("Enter your puzzle, use a zero to represent blank")
        initial_state = []
        for i in range(1, 4):
            row = input()
            row_numbers = list(map(int, row.strip().split()))
            initial_state.append(row_numbers)
    else:
        print("Invalid input!")
        return
    
    if not is_solvable(initial_state):
        print("This puzzle is not solvable!")
        return  # Exit the program since the puzzle can't be solved

    # Create Problem and Tree objects
    problem = Problem(initial_state)
    tree = Tree(problem)

    print("Enter your choice of algorithm")
    print("1. Uniform Cost Search")
    print("2. A* withthe Misplaced Tile heuristic")
    print("3. A* with the Euclidean Distance heuristic")

    algorithm = input()

    if algorithm == "1":
        result = uniform_cost_search(tree, problem)
    if algorithm == "2":
        result = a_star_search_misplaced_tile(tree, problem)
    if algorithm == "3":
        result = aStar_euclidean(tree, problem)


if __name__ == "__main__":   #start the program
    main()
