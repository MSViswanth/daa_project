import numpy as np
import copy
import time
import common


N = 5000
# setA = [(3,2),(4,1),(8,5)]
# setB = [(1,2),(3,6),(1,5)]
setA = np.random.uniform(0, 10, (N, 2))
setB = np.random.uniform(0, 10, (N, 2))

setA = [(3,2),(4,1),(8,5)]
setB = [(1,2),(3,6),(1,5)]
class greedy1:
    def greedy1(n, sorted_cost_array): 
        greedy_array = copy.deepcopy(sorted_cost_array)
        result_length = n
        result_array = [[False for i in range(len(setA))] for j in range(len(setB))]
        for i in range(0, len(greedy_array)):
            if greedy_array[i][1] != -1 and greedy_array[i][2] != -1:
                print(greedy_array[i])
                for j in range(i+1, len(greedy_array)):
                    if greedy_array[j][1] == greedy_array[i][1] or greedy_array[j][2] == greedy_array[i][2]:
                        greedy_array[j][1] = -1
                        greedy_array[j][2] = -1
                result_array[greedy_array[i][1]][greedy_array[i][2]] = True
                greedy_array[i][1] = -2
                greedy_array[i][2] = -2            
                result_length -= 1        
        minumum_distance = 0
        for i in range(0, len(greedy_array)):
            if greedy_array[i][1] == -2 and greedy_array[i][2] == -2:
                minumum_distance += greedy_array[i][0]
        return minumum_distance

start_time = time.time()
n, cost_matrix = common.preprocess.cost_matrix(setA,setB)
sorted_cost_array = common.preprocess.sorted_array(n, cost_matrix)
minumum_distance = greedy1.greedy1(n, sorted_cost_array)
print("Total Minimum distance:" +str(minumum_distance))
execution_time = time.time() - start_time
print("Execution time is " + str(execution_time) + " seconds" )