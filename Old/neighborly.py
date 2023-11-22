import copy
# from TreeAVL.AVL import AVL
from avl_tree import AVLTree
import math


cost_matrix_original = [
    [19.1, 17.6, 24.7, 19.3, 18.7],
    [7.0, 7.3, 1.0, 8.3, 5.3],
    [17.2, 13.2, 9.9, 18.6, 13],
    [6.6, 4.6, 3.7, 9.1, 2.7],
    [23.6, 15.4, 19.8, 25.1, 17.3]
]


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

def sort_by_index(listoflists,index, desc=False):
    listoflists.sort(key=lambda x:x[index],reverse=desc)

row_sorted_cost_array = sorted_cost_array.copy()
sort_by_index(row_sorted_cost_array,1)
# print(row_sorted_cost_array)
# print(sorted_cost_array)

def get_row_tracks(array):
    """Square root of length of list array must be an integer.
    
    And input matrix must also be sorted along the rows."""
    return [array[i][2] for i in range(0,len(array), int(math.sqrt(len(array))))]

# [[1.0, 1, 2], [2.7, 3, 4], [3.7, 3, 2], [4.6, 3, 1], [5.3, 1, 4], [6.6, 3, 0], [7.0, 1, 0], [7.3, 1, 1], [8.3, 1, 3], [9.1, 3, 3], [9.9, 2, 2], [13, 2, 4], [13.2, 2, 1], [15.4, 4, 1], [17.2, 2, 0], [17.3, 4, 4], [17.6, 0, 1], [18.6, 2, 3], [18.7, 0, 4], [19.1, 0, 0], [19.3, 0, 3], [19.8, 4, 2], [23.6, 4, 0], [24.7, 0, 2], [25.1, 4, 3]]


# Neighborly Algorithm

# Start with the sorted_cost_array
# Check the first element's position and compare with other rows.
result_size = n
nsquared = len(sorted_cost_array)
while result_size > 0:

    size = int(math.sqrt(len(row_sorted_cost_array[0:(n*n)-(n-1)*(n-1)])))
    row_track = get_row_tracks(row_sorted_cost_array[0:(n*n)-(n-1)*(n-1)])
    print(sorted_cost_array[0])
    competition = []
    print(row_track)
    for i, j in enumerate(row_track):
        if sorted_cost_array[0][2] == j and i != sorted_cost_array[0][1]:
            competition.append(i)

    print(competition)
    delta = [[sorted_cost_array[0][1],row_sorted_cost_array[(size*sorted_cost_array[0][1])+1][0]-row_sorted_cost_array[(size*sorted_cost_array[0][1])][0]]]
    for item in competition:
        delta.append([item,row_sorted_cost_array[(size*item)+1][0]-row_sorted_cost_array[(size*item)][0]])

    sort_by_index(delta, 1, desc=True)
    assignmentX = delta[0][0]
    assignmentY = sorted_cost_array[0][2]
    # print(assignmentX, assignmentY, cost_matrix_original[assignmentX][assignmentY])
    for k in range(0,size):
        assignment_matrix[assignmentX][k] = 0
        assignment_matrix[k][assignmentY] = 0
    assignment_matrix[assignmentX][assignmentY] = 1
    for element in sorted_cost_array:
        if element[1] == assignmentX or element[2] == assignmentY:
            sorted_cost_array.remove(element)
            sorted_cost_array.insert(nsquared-1, element)

    for element in row_sorted_cost_array:
        if element[1] == assignmentX or element[2] == assignmentY:
            row_sorted_cost_array.remove(element)
            row_sorted_cost_array.insert(nsquared-1, element)
    result_size-=1

print(assignment_matrix)

