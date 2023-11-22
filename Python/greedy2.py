import numpy as np
from avl_tree import AVLTree
import time
from common import *

# N = 3
# setA = [(3,2),(4,1),(8,5)]
# setB = [(1,2),(3,6),(1,5)]
N = 5000
setA = np.random.uniform(0, 10, (N, 2))
setB = np.random.uniform(0, 10, (N, 2))

class greedy2:
    def __init__(self, sorted_cost_array):
        self.sorted_cost_array = sorted_cost_array
        self.N = N
    def greedy2(self):
        minumum_distance = 0
        x = AVLTree()
        y = AVLTree()
        # print(range(len(sorted_cost_array)))
        #for i in range(len(sorted_cost_array)):#takes O(n^2) time
        i = 0
        while self.N>0:
            # print(i)
            if not x.searchTree(sorted_cost_array[i][1]) and not y.searchTree(sorted_cost_array[i][2]): #takes O(2log n) time
                #print(sorted_cost_array[i][0], (sorted_cost_array[i][1],sorted_cost_array[i][2]))
                minumum_distance += sorted_cost_array[i][0]
                x.insert(sorted_cost_array[i][1]) #takes O(log n) time
                y.insert(sorted_cost_array[i][2]) #takes O(log n) time
                self.N = self.N - 1
            i =i+1
        return minumum_distance

first_start_time = time.time()
start_time = time.time()
pre = preprocess(setA,setB)
# n, cost_matrix = pre.cost_matrix()
cost_array = pre.cost_matrix()
execution_time = time.time() - start_time
print("Execution time for cost_matrix is " + str(execution_time) + " seconds" )
start_time = time.time()
sorted_cost_array = pre.sorted_array(cost_array)
execution_time = time.time() - start_time
print("Execution time for sorting is " + str(execution_time) + " seconds" )
start_time = time.time()
greedy = greedy2(sorted_cost_array)
minumum_distance = greedy.greedy2()
execution_time = time.time() - start_time
print("Execution time for minimum distance calculation is " + str(execution_time) + " seconds" )
print("Total Minimum distance:" + str(minumum_distance))
execution_time = time.time() - first_start_time
print("Total Execution time is " + str(execution_time) + " seconds" )