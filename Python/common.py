import copy
import math

class preprocess:
    
    def __init__(self, setA, setB):
        self.setA = setA
        self.setB = setB

    def cost_matrix(self):
        cost_matrix=[[0 for i in range(len(self.setA))] for j in range(len(self.setB))]
        cost_array = []
        for i in range(len(self.setA)):
            for j in range(len(self.setB)):
                cost_matrix[i][j] = math.dist(self.setA[i],self.setB[j])
                cost_array.append([cost_matrix[i][j],i,j])
        # Set the value of n.
        n = len(cost_matrix)
        return cost_array

    def sorted_array(self, cost_array):
        # Create a new array with cost values and positions of each of the values
        # [['cost value','measurement', 'track']] - format
        # cost_array = []
        # for i in range(0,n):
        #     for j in range(0,n):
        #         cost_array.append([cost_matrix[i][j],i,j])

        # Sort the cost array in ascending order with respect to the first element in child lists.
        #sorted_cost_array = cost_array.copy()
        #cost_array.sort()
        cost_array = self.merge_sort(cost_array)
        #print()
        return cost_array
    
    def merge(self, left, right):
        """Merge sort merging function."""

        left_index, right_index = 0, 0
        result = []
        while left_index < len(left) and right_index < len(right):
            if left[left_index] < right[right_index]:
                result.append(left[left_index])
                left_index += 1
            else:
                result.append(right[right_index])
                right_index += 1

        result += left[left_index:]
        result += right[right_index:]
        return result

    def merge_sort(self, array):
        """Merge sort algorithm implementation."""
        if len(array) <= 1:  # base case
            return array

        # divide array in half and merge sort recursively
        half = len(array) // 2
        left = self.merge_sort(array[:half])
        right = self.merge_sort(array[half:])

        return self.merge(left, right)
