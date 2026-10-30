# CPU Scheduling Simulator 🚀

A comprehensive C++ implementation of core Operating System CPU scheduling algorithms, built from scratch with custom data structures and a dynamic web GUI visualization.

## 🛠️ Features & Algorithms
- **First Come First Serve (FCFS)**
- **Shortest Job First (SJF)**
- **Shortest Remaining Time First (SRTF)** (Preemptive)
- **Priority Scheduling**
- **Round Robin (RR)** (Preemptive with Time Quantum)

## 📦 Custom Data Structures
Built without relying on heavy external standard libraries for core structures:
- `LinkedList<T>`
- `Queue<T>`
- `Stack<T>`
- `PriorityQueue<T>`

## 💻 How to Run
1. Compile the code using a C++ compiler:
   ```bash
   g++ -std=c++11 CPU.cpp -o cpu_simulator
