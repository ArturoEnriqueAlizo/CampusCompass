CampusCompass
CampusCompass is a C++ campus navigation and scheduling system that models campus locations as a weighted graph. The application calculates efficient routes between locations, handles temporary road or pathway closures, and determines whether students have enough time to travel between scheduled classes.

Features
- Calculates shortest routes between campus locations
- Supports dynamically opening and closing graph edges
- Checks whether locations remain connected after closures
- Verifies whether a student's class schedule is physically feasible
- Stores and manages student and class information
- Processes campus graph and class information from CSV files
- Calculates a minimum-cost network connecting locations relevant to a student

Algorithms & Data Structures
The project implements several graph algorithms and data structures:
- Dijkstra's Algorithm — calculates shortest travel paths between campus locations
- Breadth-First Search (BFS) — determines graph connectivity while accounting for closed edges
- Minimum Spanning Tree (MST) — calculates the minimum network required to connect relevant locations
- Kruskal's Algorithm
- Union-Find / Disjoint Set Union (DSU)
- Adjacency Lists
- Priority Queues
- Hash Maps and Sets

Technologies
- C++
- Standard Template Library (STL)
- Graph Algorithms
- CSV Data Processing
- Object-Oriented Programming

How It Works
Campus locations are represented as vertices and paths between locations are represented as weighted edges.
The program loads campus information from CSV files and builds an internal graph representation.

Example:
CampusCompass compass;
compass.ParseCSV("data/edges.csv", "data/classes.csv");

The application then processes commands to perform operations such as route calculation, schedule verification, student management, and road closure handling.

Route Optimization
CampusCompass uses Dijkstra's shortest-path algorithm to determine the fastest route between two locations.
Closed edges are excluded from route calculations, allowing the graph to respond dynamically to temporary campus closures.

Schedule Verification
The system can analyze a student's class schedule by:
1. Sorting classes by start time
2. Calculating the shortest travel time between consecutive classes
3. Comparing the travel time with the available time between classes
4. Determining whether the schedule is feasible

Student Zone Calculation
CampusCompass identifies the network of campus locations relevant to a student's schedule and computes a minimum-cost connection between them using Minimum Spanning Tree techniques and Union-Find.

Skills Demonstrated
- Graph representation and traversal
- Dijkstra's shortest-path algorithm
- Breadth-First Search
- Minimum Spanning Trees
- Kruskal's Algorithm
- Union-Find / Disjoint Set Union
- Dynamic graph updates
- File and CSV parsing
- C++ STL containers
- Object-Oriented Programming
- Input validation
- Command processing

Author
Arturo Alizo
Computer Science
University of Florida
