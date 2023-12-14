import numpy as np
import math
import time
import copy
import csv

class Hungarian:
    """
    Implementation of the Hungarian (Munkres) Algorithm

    Usage:
        To generate the distance_matrix and find the matching pairs with minimum distance invoke hungarian = Hungarian(setA, setB)
        and then invoke hungarian.find_pairs()

        To get matching_points(Pair Points) and minimum distance after find the pairs invoke hungarian.matching_points() and hungarian.minimum_distance()
    """

    def __init__(self, setA=None, setB=None):
        """
        setA and setB are two different list
        """
        self.setA = setA
        self.setB = setB
        if setA is not None and setB is not None:
            # Generate distance matrix
            distance_matrix = self.distance_matrix(setA, setB)
            
            # Copy distance matrix
            self.raw_distance_matrix = distance_matrix
            
            # Fetch required details from generated distance matrix
            self._maxColumn = distance_matrix.shape[1]
            self._maxRow = distance_matrix.shape[0]
            self._distance_matrix = distance_matrix.copy()
            self._size = len(distance_matrix)
            self.matrix_shape = distance_matrix.shape
            
            # Results from algorithm
            self.pair_points = []
            self.min_distance = 0
            
    def read_csv(self, filename):
        """Read data from csv file"""
        points = []

        with open(filename, newline='') as csvfile:
            reader = csv.reader(csvfile)
            for row in reader:
                if len(row) == 2:
                    points.append((float(row[0]), float(row[1])))

        return points
      
    def distance_matrix(self, setA, setB):
        """Generate distance matric from the input lists setA and setB"""
        len_A, len_B = len(setA), len(setB)
        distance_matrix = np.zeros((len_A, len_B)) 
        # Compute the euclidean distance between each points
        for i in range(len_A):
            for j in range(len_B):
                distance_matrix[i][j] = math.dist(setA[i],setB[j]) 
        return distance_matrix

    def find_pairs(self):
        """Function to find the pair points using the generated distance matrix"""
        working_distance_matrix = self._distance_matrix.copy()

        # Step 1: Subtract minimum value of row with each values in that particular row for each row
        for index, row in enumerate(working_distance_matrix):
            working_distance_matrix[index] -= row.min()

        # Step 2: Subtract minimum value of column with each values in that particular column for each column
        for index, column in enumerate(working_distance_matrix.T):
            working_distance_matrix[:, index] -= column.min()

        # Step 3: Use minimum number of lines required to cover all zeros in the matrix. If the total covered is not equal to the matrix size then adjust matrix and repeat
        total_covered = 0
        while total_covered < self._size:
            # Find minimum number of lines to cover all zeros in the matrix and find total covered rows and columns
            cover_zeros = CoverZeros(working_distance_matrix)
            covered_rows = cover_zeros.get_covered_rows()
            covered_columns = cover_zeros.get_covered_columns()
            total_covered = len(covered_rows) + len(covered_columns)

            # if the total covered is not equal to the matrix size then adjust it by min uncovered num of the matrix
            if total_covered < self._size:
                working_distance_matrix = self.adjust_matrix_by_min_uncovered_num(working_distance_matrix, covered_rows, covered_columns)

        # Step 4: Starting with the top row, work your way downwards as you make assignments. Find single zeros in rows or columns. Add them to final result and remove them and their associated row/column from the matrix.
        expectedpair_points = min(self._maxColumn, self._maxRow)
        zero_locations = (working_distance_matrix == 0)
        while len(self.pair_points) != expectedpair_points:

            # Find pair points and mark rows and columns for deletion
            matched_rows, matched_columns = self.find_matches(zero_locations)
            total_matched = len(matched_rows) + len(matched_columns)
            if total_matched == 0:
                matched_rows, matched_columns = self.select_arbitrary_match(zero_locations)

            # Delete rows and columns
            for row in matched_rows:
                zero_locations[row] = False
            for column in matched_columns:
                zero_locations[:, column] = False

            # Store pair points
            self.setpair_points(zip(matched_rows, matched_columns))

        # Calculate total minimum distance
        value = 0
        for row, column in self.pair_points:
            value += self.raw_distance_matrix[row, column]
        self.min_distance = value

    def adjust_matrix_by_min_uncovered_num(self, working_distance_matrix, covered_rows, covered_columns):
        """Subtract m from every uncovered number and add m to every element covered with two lines"""
        # Derive minimum uncovered number (m)
        elements = []
        for row_index, row in enumerate(working_distance_matrix):
            if row_index not in covered_rows:
                for index, element in enumerate(row):
                    if index not in covered_columns:
                        elements.append(element)
        min_uncovered_num = min(elements)

        # Add m to every covered element
        adjusted_matrix = working_distance_matrix
        for row in covered_rows:
            adjusted_matrix[row] += min_uncovered_num
        for column in covered_columns:
            adjusted_matrix[:, column] += min_uncovered_num

        # Subtract m from every element
        m_matrix = np.ones(self.matrix_shape, dtype=int) * min_uncovered_num
        adjusted_matrix -= m_matrix

        return adjusted_matrix

    def find_matches(self, zero_locations):
        """Returns rows and columns with matches in them"""
        marked_rows = np.array([], dtype=int)
        marked_columns = np.array([], dtype=int)

        # Mark rows and columns with matches by iterating over rows
        for index, row in enumerate(zero_locations):
            row_index = np.array([index])
            if np.sum(row) == 1:
                column_index, = np.where(row)
                marked_rows, marked_columns = self.mark_rows_and_columns(marked_rows, marked_columns, row_index,
                                                                           column_index)

        # Iterate over columns
        for index, column in enumerate(zero_locations.T):
            column_index = np.array([index])
            if np.sum(column) == 1:
                row_index, = np.where(column)
                marked_rows, marked_columns = self.mark_rows_and_columns(marked_rows, marked_columns, row_index,
                                                                           column_index)

        return marked_rows, marked_columns
    @staticmethod
    def mark_rows_and_columns(marked_rows, marked_columns, row_index, column_index):
        """Check if column or row is marked. If not marked then mark it."""
        newmarked_rows = marked_rows
        newmarked_columns = marked_columns
        if not (marked_rows == row_index).any() and not (marked_columns == column_index).any():
            newmarked_rows = np.insert(marked_rows, len(marked_rows), row_index)
            newmarked_columns = np.insert(marked_columns, len(marked_columns), column_index)
        return newmarked_rows, newmarked_columns

    @staticmethod
    def select_arbitrary_match(zero_locations):
        """Selects row column combination with minimum number of zeros in it."""
        # Count number of zeros in row and column combinations
        rows, columns = np.where(zero_locations)
        zero_count = []
        for index, row in enumerate(rows):
            total_zeros = np.sum(zero_locations[row]) + np.sum(zero_locations[:, columns[index]])
            zero_count.append(total_zeros)

        # Get the row column combination with the minimum number of zeros
        indices = zero_count.index(min(zero_count))
        row = np.array([rows[indices]])
        column = np.array([columns[indices]])

        return row, column

    def setpair_points(self, result_lists):
        """Set pair points"""
        # Check if results values are out of bound from input matrix (because of matrix being padded). Add pair points to results pair points
        for result in result_lists:
            row, column = result
            if row < self._maxRow and column < self._maxColumn:
                new_result = (int(row), int(column))
                self.pair_points.append(new_result)
                     
    def matching_points(self):
        """Returns pair points after finding the matching points with minimum distances"""
        return self.pair_points

    def minimum_distance(self):
        """Returns after finding the matching points with minimum distances"""
        return self.min_distance

class CoverZeros:
    """
    Use minimum number of lines to cover all zeros in the matrix
    """

    def __init__(self, matrix):
        
        # Find zeros in matrix
        self.zero_locations = (matrix == 0)
        self.matrix_shape = matrix.shape

        # Choices starts without any choices made.
        self.choices = np.zeros(self.matrix_shape, dtype=bool)

        self.marked_rows = []
        self.marked_columns = []

        # marks rows and columns
        self.calculate_minlines()

        # Draw lines through all unmarked rows and all marked columns.
        self._covered_rows = list(set(range(self.matrix_shape[0])) - set(self.marked_rows))
        self._covered_columns = self.marked_columns

    def calculate_minlines(self):
        """
        Calculates minimum number of lines necessary to cover all zeros in a matrix.
        """
        while True:
            # Erase all marks
            self.marked_rows = []
            self.marked_columns = []

            # Mark all rows in which no choice has been made
            for index, row in enumerate(self.choices):
                if not row.any():
                    self.marked_rows.append(index)

            # If no marked rows then finish
            if not self.marked_rows:
                return True

            # Mark all columns not already marked which have zeros in marked rows
            nummarked_columns = self.mark_new_columns_with_zeros_unmarked_rows()

            # If no new marked columns then finish
            if nummarked_columns == 0:
                return True

            # While there is some choice in every marked column
            while self.choice_in_allmarked_columns():

                # Mark all rows not already marked which have choices in marked columns.
                nummarked_rows = self.mark_new_rows_withchoices_unmarked_columns()

                # If no new marks then Finish
                if nummarked_rows == 0:
                    return True

                # Mark all columns not already marked which have zeros in marked rows
                nummarked_columns = self.mark_new_columns_with_zeros_unmarked_rows()

                # If no new marked columns then finish
                if nummarked_columns == 0:
                    return True

            # No choice in one or more marked columns
            # Find a marked column that does not have a choice
            choice_column_index = self.find_marked_column_without_anychoice()

            while choice_column_index is not None:
                # Find a zero in the column indexed that does not have a row with a choice
                choice_row_index = self.find_row_without_anychoice(choice_column_index)

                # Check if an available row was found
                new_choice_column_index = None
                if choice_row_index is None:
                    # Find a good row to accomodate swap. Find its column pair
                    choice_row_index, new_choice_column_index = \
                        self.find_best_choice_row_and_new_column(choice_column_index)

                    # Delete old choice
                    self.choices[choice_row_index, new_choice_column_index] = False

                # Set zero to choice
                self.choices[choice_row_index, choice_column_index] = True

                # Loop again if choice is added to a row with a choice already in it
                choice_column_index = new_choice_column_index

    def mark_new_columns_with_zeros_unmarked_rows(self):
        """Mark all columns not already marked which have zeros in marked rows"""
        nummarked_columns = 0
        for index, column in enumerate(self.zero_locations.T):
            if index not in self.marked_columns:
                if column.any():
                    row_indices, = np.where(column)
                    zeros_inmarked_rows = (set(self.marked_rows) & set(row_indices)) != set([])
                    if zeros_inmarked_rows:
                        self.marked_columns.append(index)
                        nummarked_columns += 1
        return nummarked_columns

    def mark_new_rows_withchoices_unmarked_columns(self):
        """Mark all rows not already marked which have choices in marked columns"""
        nummarked_rows = 0
        for index, row in enumerate(self.choices):
            if index not in self.marked_rows:
                if row.any():
                    column_index, = np.where(row)
                    if column_index in self.marked_columns:
                        self.marked_rows.append(index)
                        nummarked_rows += 1
        return nummarked_rows

    def choice_in_allmarked_columns(self):
        """Return Boolean True if there is a choice in all marked columns. Returns boolean False otherwise"""
        for column_index in self.marked_columns:
            if not self.choices[:, column_index].any():
                return False
        return True

    def find_marked_column_without_anychoice(self):
        """Find a marked column that does not have a choice."""
        for column_index in self.marked_columns:
            if not self.choices[:, column_index].any():
                return column_index

    def find_row_without_anychoice(self, choice_column_index):
        """Find a row without a choice in it for the column indexed. If a row does not exist then return None"""
        row_indices, = np.where(self.zero_locations[:, choice_column_index])
        for row_index in row_indices:
            if not self.choices[row_index].any():
                return row_index

        # All rows have choices. Return None.
        return None

    def find_best_choice_row_and_new_column(self, choice_column_index):
        """Find the best choice of row and column to get the optimal solution"""
        row_indices, = np.where(self.zero_locations[:, choice_column_index])
        for row_index in row_indices:
            column_indices, = np.where(self.choices[row_index])
            column_index = column_indices[0]
            if self.find_row_without_anychoice(column_index) is not None:
                return row_index, column_index

        # Cannot find optimal row and column. Return a random row and column
        from random import shuffle

        shuffle(row_indices)
        column_index, = np.where(self.choices[row_indices[0]])
        return row_indices[0], column_index[0]
    
    def get_covered_rows(self):
        return self._covered_rows

    def get_covered_columns(self):
        return self._covered_columns




#Input to the algorithm. Change the file path to desired dataset file path in dataset folder
setAfilename = "../Dataset/setA/setA_10.csv"
setBfilename = "../Dataset/setB/setB_10.csv"
hungarian = Hungarian()
setA = hungarian.read_csv(setAfilename)
setB = hungarian.read_csv(setBfilename)

start_time = time.time()
hungarian = Hungarian(setA, setB)
hungarian.find_pairs()
print("Total minimum distance:\t", hungarian.minimum_distance())
# print("Pair Points:\t", hungarian.matching_points())
execution_time = time.time() - start_time
print("Execution time for minimum distance calculation is " + str(execution_time) + " seconds" )