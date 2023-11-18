from avl_tree import AVLTree
import time
import common

setA = [(3,2),(4,1),(8,5)]
setB = [(1,2),(3,6),(1,5)]

class greedy2:
    def greedy2(sorted_cost_array):
        minumum_distance = 0
        x = AVLTree()
        y = AVLTree()
        for i in range(len(sorted_cost_array)):#takes O(n^2) time
            if not x.searchTree(sorted_cost_array[i][1]) and not y.searchTree(sorted_cost_array[i][2]): #takes O(2log n) time
                print(sorted_cost_array[i][0], (sorted_cost_array[i][1],sorted_cost_array[i][2]))
                minumum_distance += sorted_cost_array[i][0]
                x.insert(sorted_cost_array[i][1])
                y.insert(sorted_cost_array[i][2])
        return minumum_distance

start_time = time.time()
n, cost_matrix = common.preprocess.cost_matrix(setA, setB)
sorted_cost_array = common.preprocess.sorted_array(n, cost_matrix)
minumum_distance = greedy2.greedy2(sorted_cost_array)
print("Total Minimum distance:" + str(minumum_distance))
execution_time = time.time() - start_time
print("Execution time is " + str(execution_time) + " seconds" )