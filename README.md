# Graph-Based Social Network Analysis

> A C-based project that uses graph data structures and algorithms to analyze social networks — identifying key influencers, communities, and network properties.

---

## 👨‍💻 Contributors

- Sarang Kulkarni – 612303099  
- Indranil Kavade – 612303090  
- Rajwardhan Kharade – 612303093  

---

## 📌 Project Overview

This project aims to develop a Social Network Analysis (SNA) tool using graph algorithms in C. It helps understand user interactions and community dynamics by modeling the network as a graph.

---

## 📥 Inputs

- **Graph Data**: Nodes (users) and Edges (relationships)
- **Adjacency Matrix**
- **Edge List**
- **Adjacency List**
- **Node/Edge Attributes** (e.g., user profiles, strength of relationships)

---

## 📤 Outputs

- **Centrality Measures**  
  - Degree Centrality  
  - Betweenness Centrality  
  - Closeness Centrality  

- **Community Detection**  
- **Path Analysis**  
- **Network Visualization (optional)**  
- **Link Prediction (optional)**  

---

## 🧠 Problem Statement

Traditional social network analysis tools fail to scale or provide deep insight into large datasets. This project leverages graph-based algorithms to visualize and interpret complex connections effectively.

---

## 📦 Data Structures Used

- **Adjacency List** – Efficient for sparse graphs  
- **Adjacency Matrix** – Suitable for dense graph operations  
- **Graph Nodes and Edges** – Represent users and interactions  
- **Priority Queue** – For Dijkstra’s algorithm  

---

## ⚙️ Algorithms Implemented

- **Dijkstra’s Algorithm** – Shortest path calculation  
- **Betweenness Centrality**  
- **Breadth-First Search (BFS)** – For traversal and community detection  
- **PageRank** – Importance ranking  
- **Louvain Method** – Community detection via modularity optimization  

---

## 🌍 Real-World Applications

- **Marketing**: Identify social media influencers  
- **Sociology**: Analyze community structures  
- **Enterprise**: Optimize communication within organizations  

---

## 🔮 Future Enhancements

- Integration of **Machine Learning** for predictive analytics  
- **Real-time** network updates and interactive **visualizations**  
- Handling even larger datasets with better memory optimization  

---

## 📷 Project Screenshots / Demo (optional)

> *(Include any command-line screenshots, visualizations, or sample outputs here)*

---

## 📂 How to Run

```bash
gcc -Wall -c Project_main.c Project.c
Use 'gcc Project_main.o Project.o -o Project' for VS Code and use 'cc Project_main.o Project.o -o Project' for Linux Ubuntu 
./Project
