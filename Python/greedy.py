import numpy as np
from avl_tree import AVLTree
import time
from common import *

class Greedy_algorithm:
    def __init__(self, N, sorted_distance_array):
        self.sorted_distance_array = sorted_distance_array
        self.N = N
    
    def greedy(self):
        """Identify the pair points and return the minimum distance"""        
        minumum_distance = 0
        x = AVLTree()
        y = AVLTree()
        i = 0
        while self.N>0:
            if not x.searchTree(sorted_distance_array[i][1]) and not y.searchTree(sorted_distance_array[i][2]): 
                minumum_distance += sorted_distance_array[i][0]
                x.insert(sorted_distance_array[i][1])
                y.insert(sorted_distance_array[i][2])
                self.N = self.N - 1
            i =i+1
        return minumum_distance

#Input to the algorithm. Change the file path top desired dataset file path in dataset folder
setAfilename = "../Dataset/setA_10.csv"
setBfilename = "../Dataset/setB_10.csv"

#Read the data from csv file and store it in setA and setB respectively
readdata = preprocess()
setA = readdata.read_csv(setAfilename)
setB = readdata.read_csv(setBfilename)

#Calculate distancematrix and cover the matrix as sorted array using merge sort
start_time = time.time()
pre = preprocess(setA,setB)
distance_array = pre.distance_matrix()
N = len(setA)
sorted_distance_array = pre.sorted_array(distance_array)

#Invoke greedy algorithm to find the minimum distance between matching pair points 
greedy = Greedy_algorithm(N, sorted_distance_array)
minumum_distance = greedy.greedy()
print("Total Minimum distance:" + str(minumum_distance))
execution_time = time.time() - start_time
print("Total Execution time is " + str(execution_time) + " seconds" )