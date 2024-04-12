# AStar Assignment: A Routing Problem

This assignment was completed as part of the Optimization course in the MSc for Modelling for Science and Engineering at Universitat Autònoma de Barcelona. The objective was to compute an optimal path from two points using an AStar algorithm.

## Introduction

Understanding pathfinding algorithms like AStar is essential for various applications, including navigation systems and route optimization. This assignment explores the implementation of the AStar algorithm to find the optimal path between two points on a map.

## Exercises

### Convert map files to binary
Write a code to convert the provided files into binary files, so that the reading time during execution is reduced. This code reads the .csv file of the map, and stores this information as a list of nodes that contains information about nodes, ways, and relations. This serves as the base program for subsequent exercises, implementing the AStar algorithm.

### Implement AStar Algorithm
Develop a function to implement the AStar algorithm for pathfinding between two points on the map. A first part reads the binary file and stores the information, followed by the AStar algorithm implementation and the output generation.

### Visualize Computed Path
Write a Python code to visualize the computed optimal path on a map, providing a graphical representation of the route.


## Files

1. **C Code Files:** 
   -  AStar.c implements the AStar algorithm, having as an argument a binary map.
   - ReadMap-WriteBin.c converts a .csv map to a binary file.
   
2. **PDF Files:**
    - Assignment-AStar.pdf explains the assignment in depth.
    - Report-AStar.pdf includes a summary of the algorithm used for the implementation of A*, code details of the implementation and compilation and execution instructions.
3. **Graphical representation File:**
    - mapplot.py graphically represents the computed optimal path on a map.
4. **Example File:**
    - OutputExampleCat.txt 

 ### Usage example
 It is important to note the .csv map file is not included in this repository.
 
```bash
gcc -o ReadMap-WriteBin ReadMap-WriteBin.c -lm
gcc -o AStar AStar.c -lm

./ReadMap-WriteBin map.csv
./AStar map.bin
# Choose option 1 to run the assignment or choose option 2 to manually input origin and destination node ids.
```

## Conclusion
The AStar assignment provided valuable insights into pathfinding algorithms and their applications in navigation and route optimization. By completing these exercises, we gained practical experience in implementing and analyzing AStar algorithm, which is fundamental to various fields such as computer science, transportation, and logistics.