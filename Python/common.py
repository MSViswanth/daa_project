import copy
import math

class preprocess:

    def cost_matrix(setA, setB):
        cost_matrix=[[0 for i in range(len(setA))] for j in range(len(setB))]
        for i in range(len(setA)):
            for j in range(len(setB)):
                cost_matrix[i][j] = math.dist(setA[i],setB[j])
        # Set the value of n.
        n = len(cost_matrix)
        return n, cost_matrix

    def sorted_array(n, cost_matrix):
        # Create a new array with cost values and positions of each of the values
        # [['cost value','measurement', 'track']] - format
        cost_array = []
        for i in range(0,n):
            for j in range(0,n):
                cost_array.append([cost_matrix[i][j],i,j])

        # Sort the cost array in ascending order with respect to the first element in child lists.
        sorted_cost_array = cost_array.copy()
        sorted_cost_array.sort()

        return sorted_cost_array
