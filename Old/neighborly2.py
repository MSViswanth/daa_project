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

setA = [(3,2),(4,1),(8,5)]
setB = [(1,2),(3,6),(1,5)]

# cost_matrix_original=[[0 for i in range(len(setA))] for j in range(len(setB))] # O(n^2)
cost_array = []
# # print(cost_matrix_original)
# for i in range(len(setA)):# O(n^2)
#     for j in range(len(setB)):
#         # print(math.dist(setA[i],setB[j]))
#         cost_matrix_original[i][j] = math.dist(setA[i],setB[j])
#         cost_array.append([cost_matrix_original[i][j],i,j])
#         # print(cost_matrix_original[i][j])
# # print(cost_matrix_original)
n = len(cost_matrix_original)

for i in range(n):# O(n^2)
 for j in range(n):
     cost_array.append([cost_matrix_original[i][j],i,j])
     
     

# Set the value of n.


# Create assignment matrix
# assignment_matrix = [[-1 for i in range(n)] for j in range(n)]
# print(assignment_matrix)

# Create a new array with cost values and positions of each of the values
# [['cost value','measurement', 'track']] - format
# Print the sorted cost array.
# print(sorted_cost_array)

def sort_by_index(listoflists,index, desc=False):#Implement merge sort
    if len(listoflists)>index:
        listoflists.sort(key=lambda x:x[index],reverse=desc)


# print(row_sorted_cost_array)
# print(sorted_cost_array)

def get_row_tracks(array):#O(n)
    """Square root of length of list array must be an integer.
    
    And input matrix must also be sorted along the rows."""
    # print (len(array))
    # print(int(math.sqrt(len(array))))
    return [array[i][2] for i in range(0,len(array), int(math.sqrt(len(array))))]

# [[1.0, 1, 2], [2.7, 3, 4], [3.7, 3, 2], [4.6, 3, 1], [5.3, 1, 4], [6.6, 3, 0], [7.0, 1, 0], [7.3, 1, 1], [8.3, 1, 3], [9.1, 3, 3], [9.9, 2, 2], [13, 2, 4], [13.2, 2, 1], [15.4, 4, 1], [17.2, 2, 0], [17.3, 4, 4], [17.6, 0, 1], [18.6, 2, 3], [18.7, 0, 4], [19.1, 0, 0], [19.3, 0, 3], [19.8, 4, 2], [23.6, 4, 0], [24.7, 0, 2], [25.1, 4, 3]]


# Neighborly Algorithm
# n squared
# Sort the cost array in ascending order with respect to the first element in child lists.
sorted_cost_array = cost_array.copy()
sorted_cost_array.sort()#Implement merge sort

def neighborly(cost_array,sumofDistances):
    # print("Length of cost array: " + str(len(cost_array)))
    # print("cost array: " + str(cost_array))
    print("cost_array")
    print(cost_array)
    row_sorted_cost_array = cost_array.copy()
    
    size = int(math.sqrt(len(row_sorted_cost_array)))
    sort_by_index(row_sorted_cost_array,1)
    print("row_sorted_cost_array")
    print(row_sorted_cost_array)
    row_track = get_row_tracks(row_sorted_cost_array)
    print("row_track: " + str(row_track))
    competition = []
    
    for i, j in enumerate(row_track):
        if cost_array[0][2] == j and i != cost_array[0][1]:
            competition.append(i)#Implement delta with in this for loop. Calculate the delta with first element
    
    # print("competition: " + str(competition))
    delta = [[cost_array[0][1], 1]]
    if size > 1:
        delta = [[cost_array[0][1],row_sorted_cost_array[(size*cost_array[0][1])+1][0]-row_sorted_cost_array[(size*cost_array[0][1])][0]]]
    for item in competition:
        delta.append([item,row_sorted_cost_array[(size*item)+1][0]-row_sorted_cost_array[(size*item)][0]])
    # print("delta: " + str(delta))
    print("Before sort delta")
    print(delta)
    sort_by_index(delta, 1, desc=True)
    # print("sorted delta: " + str(delta))
    print("After sort delta")
    print(delta)
    assignmentX = delta[0][0]
    assignmentY = cost_array[0][2]
    print("Assignment:" + str((assignmentX, assignmentY)))
    # print(str(setA[assignmentX]) + " --> " + str(setB[assignmentY]))
    # setA.pop(assignmentX)
    # setB.pop(assignmentY)
    new_cost_array = []
    for element in cost_array:
        if element[1] != assignmentX and element[2] != assignmentY:
            new_cost_array.append(element)

        if element[1]==assignmentX and element[2] == assignmentY:
            print("Cost: " + str(element[0]))
            sumofDistances+=element[0]
        # if element[1] > assignmentX:
        #     element[1]-=1
        # if element[2] > assignmentY:
        #     element[2]-=1
    for element in new_cost_array:
        if element[1] > assignmentX:
            element[1]-=1
        if element[2] > assignmentY:
            element[2]-=1
    
    # print(cost_array)
    print("Sum of distances till "+ str(n+1-size)+ " assignments: "+ str(sumofDistances))
    if len(new_cost_array) !=0:
        neighborly(new_cost_array, sumofDistances=sumofDistances)


neighborly(sorted_cost_array,sumofDistances=0)
