# COT6405 - Design and Analysis of Algorithms - Term Project

# Overview
We implemented the below algorithms in Python and C++ to address the Euclidean Bichromatic Matching problem. 

| Algorithm | Time Complexity | Space Complexity |
| --- | --- | --- |
| Hungarian | $O(N^3)$ | $O(N^2)$ |
| Greedy | $O(N^2 \log N)$ | $O(N^2)$ |
| Neighbourly | $O(N^2 \log N)$| $O(N^2)$ |

# Dependencies

- Clang &ge; 16 compiler to run C++ programs.
- Latest version of Python &ge; 3.7 to run Python programs.

# Folder Structure
```bash
 DAA_PROJECT
    ├── C++
        ├── hungarian.cpp          # C++ implementation for Hungarian Algorithm
        ├── greedy.cpp             # C++ implementation for Greedy Algorithm
        ├── neighbourly.cpp        # C++ implementation for Neighbourly Algorithm
        ├── avl_tree.h             # AVL tree implementation
    ├── Dataset                    # Contains different sizes of the dataset for setA and SetB input ranging from 10 to 30000 records
        ├── setA                   # CSV files for setA inputs
        ├── setB                   # CSV files for setB inputs
    ├── Python
        ├── hungarian.py           # Pyhton implementation for Hungarian Algorithm
        ├── greedy.py              # Pyhton implementation for Greedy Algorithm
        ├── generateData.py        # Pyhton implementation to generate synthetic data points for set A and set B that as be fed as input to test implemented algorithms
        ├── avl_tree.py            # AVL tree implementation
    └── README.md
```


# Usage

1. Download the DAA_Project folder to your local machine.
2. Install the dependencies.
3. To run the program follow the below steps:

   a. For C++ programs

        i. Open the respective C++ file in any IDE or text editor.
        ii. Modify the input file path defined in the setAfilename and setBfilename variable in the 'main()' function. to the path of the desired input size from the dataset. You can also generate your own dataset using 'generateData.py' in the Python folder by specifying the input size N.
        iii. Save the file.
        iv. Compile and execute the file using the command 'clang++ -o <MyProgram> Filename.cpp'(Change MyProgam to a desired output file name).
        v. Open the program output file to view the output using the command './MyProgram.out'.

   b. For Python programs

        i. Open the respective Python file in any IDE or text editor.
        ii. Modify the input file path defined in setAfilename and setBfilename variable to the path of desired input size from the dataset. You can also generate your own dataset using 'generateData.py' in the Python folder by specifying the input size N.
        iii. Save the file.
        iv. Run the Python file using the command python 'MyProgram.py'(Change MyProgam to the respective file name).   

>**Note:** 
> We benchmarked the hungarian implementation in C++ with 2000 record(i.e. N = 2000). Greedy and neighborly C++ implementation was benchmarked till 15000 records(i.e. N = 15000).
