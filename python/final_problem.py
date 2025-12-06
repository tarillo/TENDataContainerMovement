
import final_Tree
from copy import deepcopy       #makes a full copy of the puzzle board so we dont modify initial state
import math                     #used for euclidean distance calculation

class Problem:                  #problem class
    def __init__(self, initial_state, goal_state=None):         #constructor
        self.initial_state = initial_state
        self.goal_state = [[1,2,3],[4,5,6],[7,8,0]]
        self.size = 3
        
    def get_blank_position(self, state):                        #finds the blank 0 tile
        for i in range(self.size):                              #matrix/array
            for j in range(self.size):
                if state[i][j] == 0:                
                    return i, j

    def actions(self, state):                                   #which moves are legal in the current state
        row, col = self.get_blank_position(state)               #gets blank tiles position
        moves = []
        if col > 0: moves.append('Left')
        if col < self.size - 1: moves.append('Right')
        if row > 0: moves.append('Up')
        if row < self.size - 1: moves.append('Down')
        return moves

    def result(self, state, action):
        row, col = self.get_blank_position(state)               #blank tile position
        new_state = deepcopy(state)                             #copy of previous state

        if action == 'Up':                                      #does all the actions
            new_state[row][col], new_state[row - 1][col] = new_state[row - 1][col], new_state[row][col]
        elif action == 'Down':
            new_state[row][col], new_state[row + 1][col] = new_state[row + 1][col], new_state[row][col]
        elif action == 'Left':
            new_state[row][col], new_state[row][col - 1] = new_state[row][col - 1], new_state[row][col]
        elif action == 'Right':
            new_state[row][col], new_state[row][col + 1] = new_state[row][col + 1], new_state[row][col]
        return new_state                                        #returns the new state

    def is_goal(self, state):                                   #checks for goal state
        return state == self.goal_state

    def to_tuple(self, state):                                  #converts state into tuple
        return tuple(map(tuple, state))

    def misplaced_tile_heuristic(self, state):                  #how many tiles are misplaced, used for A*
        return sum(state[i][j] != 0 and state[i][j] != self.goal_state[i][j]        #checks each tile with goal state
                   for i in range(self.size) for j in range(self.size))

    def euclidean_distance_heuristic(self, state):              #calculates exact euclidean distance
        total = 0
        for i in range(self.size):                              #calculates straight line distance
            for j in range(self.size):
                val = state[i][j]
                if val != 0:
                    goal_i = (val - 1) // self.size
                    goal_j = (val - 1) % self.size
                    total += math.sqrt((i - goal_i) ** 2 + (j - goal_j) ** 2)
        return total

    def display_state(self, state):                             #prints the state(board)
        for row in state:
            print(" ".join(str(n) if n != 0 else "b" for n in row))
        print()
