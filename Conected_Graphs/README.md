# Connected Graphs Exercise

This exercise was completed as part of the Optimization course in the MSc for Modelling for Science and Engineering at Universitat Autònoma de Barcelona. The objective was to develop code for various tasks related to connected graphs, with a focus on understanding graph connectivity and its applications in optimization.


## Introduction
Understanding connected graphs is crucial in various scientific and engineering disciplines, as it enables the modeling and analysis of interconnected systems. In this exercise, we explore algorithms for determining graph connectivity, identifying connected components, and detecting trees within a graph.

## Exercises

### Ex0: Load Graph Edges
Load the edges of a graph from a given file provided as an argument. Display information about the graph on the screen. This serves as the base program for subsequent exercises, implementing either BFS or DFS iteration to check graph connectivity.

### Ex1: Check Graph Connectivity
Implement a code that returns 1 if the graph is connected and 0 if it is not, based on the loaded graph from Ex0.

### Ex2: Identify Connected Components
Develop code to determine the number of connected components in a given graph, building upon the functionality of Ex0.

### Ex3: Detect Trees
Create a program that returns 1 if the given graph is a tree and 0 otherwise. This exercise extends the concepts learned in the previous exercises to identify tree structures within graphs.

## Files

1. **C Code for Exercises:** 
   Contains the C code files for the exercises.
   
2. **Example Graph Files (`GraphX.txt`):** 
   Included in the repository are example text files representing graphs. These files can be used as inputs for the exercises, providing sample data for testing and demonstration purposes.

### Usage example
```bash
gcc -o ex0 ex0.c
./ex0 Graph1.txt
```

   

## Conclusion
The connected graphs exercise provided valuable insights into graph connectivity algorithms and their applications. By completing these exercises, we gained practical experience in implementing and analyzing graph-related algorithms, which are fundamental to various scientific and engineering disciplines.