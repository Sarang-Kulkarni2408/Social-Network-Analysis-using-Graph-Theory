# Social Network Graph Engine (C)

A Data Structures & Algorithms mini-project that models a small social
network as a graph, built from scratch on top of custom **Stack** and
**Queue** ADTs and an **adjacency-list** graph representation — no STL,
no external libraries, pure C.

Every "friendship" is an undirected, unit-weight edge. On top of the
classic graph algorithms, the project layers a few social-network-style
features (mutual friends, friend recommendations, network analytics).

## Features

**Core ADTs**
- Array-based `Stack` and circular `Queue`, used internally by the
  traversal / pathfinding algorithms.

**Graph algorithms**
- BFS and DFS traversal
- Cycle detection (correct recursive parent-tracking DFS, checks every
  connected component)
- Minimum Spanning Tree via **Prim's Algorithm**
- Single-source shortest distances via **Dijkstra's Algorithm**
- Shortest path reconstruction between any two users

**Social-network features**
- Mutual friends of two users
- First-level (direct) friends of a user
- Friend recommendations (users exactly 2 hops away — i.e. "friend of a
  friend")

**Extra analytics (new)**
- Adjacency matrix printout
- Degree centrality ranking ("who's the most popular user")
- Whole-network connectivity check

## What was fixed / hardened vs. the original version

- Fixed a cost-matrix bug where removing an edge set the reverse entry to
  `1` instead of `0`.
- Fixed a crash when trying to remove an edge that doesn't exist
  (NULL-pointer dereference in the linked-list delete helper).
- Replaced the iterative stack-based cycle check (which could report
  false positives) with a correct recursive DFS that also checks
  disconnected components.
- Fixed several `-Wmaybe-uninitialized` warnings (uninitialized
  candidate vertex in Prim's/Dijkstra's when a graph is disconnected).
- Fixed memory leaks: Stack/Queue backing arrays are now always freed.
- Added input validation everywhere (invalid vertex numbers, non-numeric
  menu input, duplicate edges) instead of trusting raw `scanf` results.
- The whole codebase now compiles **warning-free** with
  `-Wall -Wextra -Wpedantic` and is clean under AddressSanitizer /
  UndefinedBehaviorSanitizer.
- Removed duplicated/conflicting global state (the user list used to be
  declared twice).

## Project structure

```
.
├── Project.h        # Data structures, constants, function declarations
├── Project.c        # Implementation of ADTs, graph, and algorithms
├── Project_main.c   # Interactive menu-driven driver program
└── .gitignore
```

## How to run

Requires `gcc` (or any C11-compliant compiler).

```bash
# Clone the repo
git clone <your-repo-url>
cd <repo-folder>

# Build
gcc -std=c11 -Wall -Wextra -O2 -o project Project.c Project_main.c

# Run
./project
```

### Sample menu

```
Menu :
    1.  Remove a connection between 2 users
    2.  Free the graph of users
    3.  Print the graph of users (adjacency list)
    4.  Find BFS of the graph of users
    5.  Find DFS of the graph of users
    6.  Detect a cycle anywhere in the graph of users
    7.  Find minimum spanning tree using Prim's Algorithm
    8.  Find shortest distance from a user to all other users (Dijkstra)
    9.  Find shortest path between two users
    10. Find mutual friends of two users
    11. Find first-level friends of a user
    12. Display friend recommendations for a user
    13. Print the adjacency matrix
    14. Show degree centrality (most popular users)
    15. Check if the whole network is connected
Enter 0 to exit
```

## Complexity notes

| Operation                  | Time complexity          |
|-----------------------------|---------------------------|
| BFS / DFS                   | O(V + E)                  |
| Cycle detection              | O(V + E)                  |
| Prim's MST                  | O(V²) (array-based, no heap — fine at this scale) |
| Dijkstra's shortest path    | O(V²) (array-based, no heap) |
| Mutual / first-level friends | O(V)                      |

> Note: Prim's and Dijkstra's use a simple O(V²) array scan for the
> minimum-key vertex rather than a binary/Fibonacci heap. That's the
> right trade-off for a ~10-user demo graph; swapping in a min-heap
> would bring both down to O(E log V) and is a natural "next step" if
> you want to extend this project.

## Possible extensions

- Swap the priority selection in Prim's/Dijkstra's for a binary heap.
- Allow adding/removing *users* (not just edges) at runtime.
- Persist the graph to/from a file so the network survives restarts.
- Weighted "closeness" edges instead of a fixed weight of 1.
