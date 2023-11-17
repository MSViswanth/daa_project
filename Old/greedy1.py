import copy
import math

setA = [(3,2),(4,1),(8,5)]
setB = [(1,2),(3,6),(1,5)]

# Cost matrix as provided in the research paper.
# cost_matrix_original = [
#     [19.1, 17.6, 24.7, 19.3, 18.7],
#     [7.0, 7.3, 1.0, 8.3, 5.3],
#     [17.2, 13.2, 9.9, 18.6, 13],
#     [6.6, 4.6, 3.7, 9.1, 2.7],
#     [23.6, 15.4, 19.8, 25.1, 17.3]
# ]
cost_matrix_original=[[0 for i in range(len(setA))] for j in range(len(setB))]
print(cost_matrix_original)
for i in range(len(setA)):
    for j in range(len(setB)):
        print(math.dist(setA[i],setB[j]))
        cost_matrix_original[i][j] = math.dist(setA[i],setB[j])
        print(cost_matrix_original[i][j])
print(cost_matrix_original)
# Set the value of n.
n = len(cost_matrix_original)

result_array = [[False for i in range(len(setA))] for j in range(len(setB))]
print(result_array)
# Create a new array with cost values and positions of each of the values
# [['cost value','measurement', 'track']] - format
cost_array = []

for i in range(0,n):
    for j in range(0,n):
        cost_array.append([cost_matrix_original[i][j],i,j])

# Print the generated cost array with values and positions.
print(cost_array)

# Sort the cost array in ascending order with respect to the first element in child lists.
sorted_cost_array = cost_array.copy()
sorted_cost_array.sort()

# Print the sorted cost array.
print(sorted_cost_array)


# Greedy Algorithm
greedy_array = copy.deepcopy(sorted_cost_array)
# print(greedy_array)
# print(len(greedy_array))

result_length = n


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
print(result_array)
# print(sorted_cost_array)
sumOfDistances = 0
for i in range(0, len(greedy_array)):
    if greedy_array[i][1] == -2 and greedy_array[i][2] == -2:
        # print((sorted_cost_array[i][1],sorted_cost_array[i][2]))
        sumOfDistances += greedy_array[i][0]

print(sumOfDistances)