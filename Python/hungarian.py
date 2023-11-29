import numpy as np
from scipy.optimize import linear_sum_assignment
import math
import time
import csv
N = 1000
# setA = [(3,2),(4,1),(8,5)]
# setB = [(1,2),(3,6),(1,5)]
# setA = np.random.uniform(0, 10, (N, 2))
# setB = np.random.uniform(0, 10, (N, 2))
class hungarian_algorithm:
    
    def __init__(self, setA, setB):
        self.setA = setA
        self.setB = setB
    
    def cost_matrix(self):
        len_A, len_B = len(self.setA), len(self.setB)
        cost_matrix = np.zeros((len_A, len_B)) #takes O(n^2) time and O(n^2) space
        # Compute the cost matrix
        for i in range(len_A):
            for j in range(len_B):
                cost_matrix[i][j] = math.dist(setA[i],setB[j]) #takes O(n^2) time and O(n^2) space
        return cost_matrix

    def calDistance_MPairs(self, cost_matrix):
        row, col = linear_sum_assignment(cost_matrix)
        # Calculate the sum of minimum distances
        mimimum_distance = cost_matrix[row, col].sum() #takes O(n^3) time and O(1) space
        #print(cost_matrix[row, col])
        # Construct the matching pairs
        matching_pairs = [(row, col) for row, col in zip(row, col)] #takes O(n^2) time and O(1) space
        return mimimum_distance, matching_pairs




def read_csv(filename):
    points = []

    with open(filename, newline='') as csvfile:
        reader = csv.reader(csvfile)
        for row in reader:
            if len(row) == 2:
                points.append((float(row[0]), float(row[1])))

    return points

start_time = time.time()
setAfilename = "/Users/guna/Education/Masters/2ndSemester/COT6405_DAA/daa_project/Dataset/setA_2000.csv"
setBfilename = "/Users/guna/Education/Masters/2ndSemester/COT6405_DAA/daa_project/Dataset/setB_2000.csv"
setA = read_csv(setAfilename)
setB = read_csv(setBfilename)
hungarian =  hungarian_algorithm(setA,setB)
cost_matrix = hungarian.cost_matrix()
mimimum_distance, matching_pairs = hungarian.calDistance_MPairs(cost_matrix)
print("Total Minimum distance:", mimimum_distance)
# print("Matching pairs:", matching_pairs)
execution_time = time.time() - start_time
print("Execution time is " + str(execution_time) + " seconds")