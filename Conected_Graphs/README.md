# Connected Graphs Exercise

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

## Usage
1. Clone the repository to your local machine.
2. Navigate to the directory containing the C code files.
3. Compile each code file using a C compiler (e.g., gcc).
4. Run the compiled executable file with the appropriate arguments as specified in the exercise descriptions.

### Example
```bash
gcc -o ex0 ex0.c
./ex0 graph.txt
```

## Example Graph Files
Included in the repository are example text files representing graphs (`graph.txt`). These files can be used as inputs for Ex0 and other exercises requiring graph data.

## Additional Notes
- Ensure that the graph files are formatted correctly to match the expected input structure.
- Consider exploring additional graph algorithms and optimizations beyond the scope of these exercises to deepen your understanding.

## Conclusion
The connected graphs exercise provided valuable insights into graph connectivity algorithms and their applications. By completing these exercises, you've gained practical experience in implementing and analyzing graph-related algorithms, which are fundamental to various scientific and engineering disciplines.