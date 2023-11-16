# Cost matrix as provided in the research paper.
cost_matrix_original = [
    [19.1, 17.6, 24.7, 19.3, 18.7],
    [7.0, 7.3, 1.0, 8.3, 5.3],
    [17.2, 13.2, 9.9, 18.6, 13],
    [6.6, 4.6, 3.7, 9.1, 2.7],
    [23.6, 15.4, 19.8, 25.1, 17.3]
]

# Set the value of n.
n = len(cost_matrix_original)

# Create a new array with cost values and positions of each of the values
cost_array = []

for i in range(0,n):
    for j in range(0,n):
        cost_array.append([cost_matrix_original[i][j],i,j])

# Print the generated cost array with values and positions.
# print(cost_array)

# Sort the cost array in ascending order with respect to the first element in child lists.
sorted_cost_array = cost_array.copy()
sorted_cost_array.sort()

# Print the sorted cost array.
print(sorted_cost_array)