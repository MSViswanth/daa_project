import copy
# from TreeAVL.AVL import AVL
from avl_tree import AVLTree
import math


# Cost matrix as provided in the research paper.
# cost_matrix_original = [
#     [19.1, 17.6, 24.7, 19.3, 18.7],
#     [7.0, 7.3, 1.0, 8.3, 5.3],
#     [17.2, 13.2, 9.9, 18.6, 13],
#     [6.6, 4.6, 3.7, 9.1, 2.7],
#     [23.6, 15.4, 19.8, 25.1, 17.3]
# ]

setA = [(3,2),(4,1),(8,5)]
setB = [(1,2),(3,6),(1,5)]

cost_matrix_original=[[0 for i in range(len(setA))] for j in range(len(setB))]
# print(cost_matrix_original)
for i in range(len(setA)):
    for j in range(len(setB)):
        # print(math.dist(setA[i],setB[j]))
        cost_matrix_original[i][j] = math.dist(setA[i],setB[j])
        # print(cost_matrix_original[i][j])
# print(cost_matrix_original)
# Set the value of n.
n = len(cost_matrix_original)

# Create assignment matrix
assignment_matrix = [[-1 for i in range(n)] for j in range(n)]
# print(assignment_matrix)

# Create a new array with cost values and positions of each of the values
# [['cost value','measurement', 'track']] - format
cost_array = []

for i in range(0,n):
    for j in range(0,n):
        cost_array.append([cost_matrix_original[i][j],i,j])


# n squared
# Sort the cost array in ascending order with respect to the first element in child lists.
sorted_cost_array = cost_array.copy()
sorted_cost_array.sort()

# Print the sorted cost array.
# print(sorted_cost_array)


# Greedy Algorithm
greedy_array = copy.deepcopy(sorted_cost_array)
# print(greedy_array)
# print(len(greedy_array))
# print(greedy_array)
# result_length = n
sumofDistances = 0
x = AVLTree()
y = AVLTree()

# x.insert(greedy_array[0][1])
# y.insert(greedy_array[0][2])

for i in range(len(greedy_array)):
    # print(str(greedy_array[i][0])+"--> "+str(greedy_array[i][1]) +" "+ str(greedy_array[i][2])+" "+str(x.searchTree(greedy_array[i][1])))
    if not x.searchTree(greedy_array[i][1]) and not y.searchTree(greedy_array[i][2]):
        print(greedy_array[i][0], (greedy_array[i][1],greedy_array[i][2]))
        sumofDistances += greedy_array[i][0]
        x.insert(greedy_array[i][1])
        y.insert(greedy_array[i][2])
        # x.prettyPrint
        # y.prettyPrint

print("Total Distance: " + str(sumofDistances))