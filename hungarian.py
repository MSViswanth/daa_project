import numpy as np
from scipy.optimize import linear_sum_assignment
import math
import time

setA = [(3,2),(4,1),(8,5)]
setB = [(1,2),(3,6),(1,5)]
class hungarian:
    def cost_matrix(setA, setB):
        len_A, len_B = len(setA), len(setB)
        cost_matrix = np.zeros((len_A, len_B)) #takes O(n^2) time and O(n^2) space
        # Compute the cost matrix
        for i in range(len_A):
            for j in range(len_B):
                cost_matrix[i][j] = math.dist(setA[i],setB[j]) #takes O(n^2) time and O(n^2) space
        return cost_matrix

    def hungarian(cost_matrix):
        row, col = linear_sum_assignment(cost_matrix)
        # Calculate the sum of minimum distances
        mimimum_distance = cost_matrix[row, col].sum() #takes O(n^3) time and O(1) space
        print(cost_matrix[row, col])
        # Construct the matching pairs
        matching_pairs = [(row, col) for row, col in zip(row, col)] #takes O(n^2) time and O(1) space
        return mimimum_distance, matching_pairs


start_time = time.time()
cost_matrix = hungarian.cost_matrix(setA, setB)
mimimum_distance, matching_pairs = hungarian.hungarian(cost_matrix)
print("minimum distance:", mimimum_distance)
print("Matching pairs:", matching_pairs)
execution_time = time.time() - start_time
print("Execution time is " + str(execution_time) + " seconds" )