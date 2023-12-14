# COT6405 - Design and Analysis of Algorithms - Term Project

# Overview
We implemented below algorithms in python and C++ to address the Euclidean Bichromatic Matching problem. 

| Algorithm | Time Complexity | Space Complexity |
| --- | --- | --- |
| Hungarian | $O(N^3)$ | $O(N^2)$ |
| Greedy | $O(N^2 \log N)$ | $O(N^2)$ |
| Neighbourly | $O(N^2 \log N)$| $O(N^2)$ |

# Dependencies

- Clang compiler to run C++ programs
- Latest version of Python &ge; 3.7 to run python programs

# Folder Structure
'''bash
 DAA_PROJECT
    ├── C++
        ├── hungarian.cpp          # C++ implementation for Hungarian Algorithm
        ├── greedy.cpp             # C++ implementation for Greedy Algorithm
        ├── neighbourly.cpp        # C++ implementation for Neighbourly Algorithm
        ├── avl_tree.h             # AVL tree implementation
    ├── Dataset                    # Contains different size of dataset for setA and SetB input ranging from 10 to 30000 records
    ├── Python
        ├── hungarian.py           # Pyhton implementation for Hungarian Algorithm
        ├── greedy.py              # Pyhton implementation for Greedy Algorithm
        ├── generateData.py        # Pyhton implementation to generate synthetic data points for set A and set B that as be fed as input to test implemented algorithms
        ├── avl_tree.py             # AVL tree implementation
    └── README.md
'''
# Usage

1. Download the DAA_Project folder to your local machine
2. Install the dependencies
3. To run the program follow below steps
    a. For C++ programs
        - Open the respective C++ file in any IDE or text editor
        - Modify the input file path defined in setAfilename and setBfilename variable in main() function to the path of desired input size from the dataset. You can also generate your own dataset using generateData.py in python folder by specifying the input size N
        - Save the file
        - Compile the file using the command 'g++ -o <MyProgram> Filename.cpp'
        - Run the program ./MyProgram
    b. For python programs
        - Open the respective python file in any IDE or text editor
        - Modify the input file path defined in setAfilename and setBfilename variable to the path of desired input size from the dataset. You can also generate your own dataset using generateData.py in python folder by specifying the input size N
        - Save the file
        - Run the python file using the command python MyProgram.py   

>**Note** 
> We benchmarked the hungarian implementation in C++ with 2000 record(i.e N = 2000). Greedy and neighbourly C++ implementation was benchmarked till 15000 records(i.e N = 15000) 