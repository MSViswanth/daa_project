# COT6405 - Design and Analysis of Algorithms - Term Project

# Overview
We implemented below algorithms in pyhton and C++ to address the Euclidean Bichromatic Matching problem. 

| Algorithm | Time Complexity | Space Complexity |
| --- | --- | --- |
| Hungarian | $O(N^3)$ | $O(N^2)$ |
| Greedy | $O(N^2 \log N)$ | $O(N^2)$ |
| Neighbourly | $O(N^2 \log N)$| $O(N^2)$ |

# Dependencies

- GCC Compiler to run C++ programs
- Latest version of Python &ge; 3.7 to run python programs

# Folder Structure

 DAA_PROJECT
    ├── C++
        ├── hungarian.cpp          # C++ implementation for Hungarian Algorithm
        ├── greedy.cpp             # C++ implementation for Greedy Algorithm
        ├── neighbourly.cpp        # C++ implementation for Neighbourly Algorithm
    ├── Dataset                    # Contains different size of dataset for setA and SetB input ranging from 10 to 30000 records
    ├── Python
        ├── hungarian.py           # Pyhton implementation for Hungarian Algorithm
        ├── greedy.py              # Pyhton implementation for Greedy Algorithm
        ├── generateData.py        # Pyhton implementation to generate synthetic data points for set A and set B that as be fed as input to test implemented algorithms
    └── README.md

# Usage

1. Download the DAA_Project folder to your local machine
2. Install the dependencies
3. To run the program follow below steps
    a. For C++ programs
        - Open the respective C++ file in any IDE or text editor
        - Modify the input file path defined in setAfilename and setAfilename variable in main() function to the path of desired input size from the dataset. You can also generate your own dataset using generateData.py by specifying the input size N
        - Save the file
        - Compile the file using the command 'g++ -o <MyProgram> Filename.cpp'
        - Run the program ./MyProgram
    b. For C++ programs
        - Open the respective C++ file in any IDE or text editor
        - Modify the input file path defined in setAfilename and setAfilename variable to the path of desired input size from the dataset. You can also generate your own dataset using generateData.py by specifying the input size N
        - Save the file
        - Run the python file using the command python MyProgram.py   