import copy
import math
import csv

class preprocess:
    
    def __init__(self, setA=None, setB=None):
        self.setA = setA
        self.setB = setB

    def distance_matrix(self):
         """Generate distance matric from the input lists setA and setB"""
        distance_matrix=[[0 for i in range(len(self.setA))] for j in range(len(self.setB))]
        distance_array = []
        for i in range(len(self.setA)):
            for j in range(len(self.setB)):
                distance_matrix[i][j] = math.dist(self.setA[i],self.setB[j])
                distance_array.append([distance_matrix[i][j],i,j])
        n = len(distance_matrix)
        return distance_array

    def sorted_array(self, distance_array):
        distance_array = self.merge_sort(distance_array)
        return distance_array
    
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
        if len(array) <= 1: 
            return array
        half = len(array) // 2
        left = self.merge_sort(array[:half])
        right = self.merge_sort(array[half:])

        return self.merge(left, right)
    
    def read_csv(self, filename):
        """Read data from csv file"""
        points = []

        with open(filename, newline='') as csvfile:
            reader = csv.reader(csvfile)
            for row in reader:
                if len(row) == 2:
                    points.append((float(row[0]), float(row[1])))

        return points
