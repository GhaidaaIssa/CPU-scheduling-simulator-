#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <cstdlib>

using namespace std;

struct Process {
    string id;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int priority;
    int completionTime;
    int turnaroundTime;
    int waitingTime;
    int startTime;
    bool isCompleted;

    Process() {
        id = "";
        arrivalTime = 0;
        burstTime = 0;
        remainingTime = 0;
        priority = 0;
        completionTime = 0;
        turnaroundTime = 0;
        waitingTime = 0;
        startTime = -1;
        isCompleted = false;
    }

    Process(string pId, int at, int bt, int prio = 0) {
        id = pId;
        arrivalTime = at;
        burstTime = bt;
        remainingTime = bt;
        priority = prio;
        completionTime = 0;
        turnaroundTime = 0;
        waitingTime = 0;
        startTime = -1;
        isCompleted = false;
    }
};

struct GanttBlock {
    string processId;
    int startTime;
    int endTime;
};

template <typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;
        Node(T val) : data(val), next(nullptr) {}
    };
    Node* head;
    Node* tail;
    int size;

public:
    LinkedList() : head(nullptr), tail(nullptr), size(0) {}
    ~LinkedList() { clear(); }

    void clear() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head = tail = nullptr;
        size = 0;
    }

    void pushBack(T val) {
        Node* newNode = new Node(val);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        size++;
    }

    void pushFront(T val) {
        Node* newNode = new Node(val);
        if (!head) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head = newNode;
        }
        size++;
    }

    T popFront() {
        if (isEmpty()) throw runtime_error("List is empty");
        Node* temp = head;
        T val = temp->data;
        head = head->next;
        if (!head) tail = nullptr;
        delete temp;
        size--;
        return val;
    }

    T& get(int index) {
        Node* curr = head;
        for (int i = 0; i < index; i++) {
            curr = curr->next;
        }
        return curr->data;
    }

    int getSize() const { return size; }
    bool isEmpty() const { return size == 0; }
};

template <typename T>
class Queue {
private:
    LinkedList<T> list;

public:
    void enqueue(T item) { list.pushBack(item); }
    T dequeue() { return list.popFront(); }
    T& front() { return list.get(0); }
    bool isEmpty() const { return list.isEmpty(); }
    int size() const { return list.getSize(); }
};

template <typename T>
class Stack {
private:
    LinkedList<T> list;

public:
    void push(T item) { list.pushFront(item); }
    T pop() { return list.popFront(); }
    T& top() { return list.get(0); }
    bool isEmpty() const { return list.isEmpty(); }
    int size() const { return list.getSize(); }
};

class PriorityQueue {
private:
    struct PQNode {
        Process proc;
        PQNode* next;
        PQNode(Process p) : proc(p), next(nullptr) {}
    };
    PQNode* head;
    int count;

public:
    PriorityQueue() : head(nullptr), count(0) {}
    ~PriorityQueue() {
        while (head) {
            PQNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void enqueue(Process p, int mode) {
        PQNode* newNode = new PQNode(p);
        count++;

        if (!head) {
            head = newNode;
            return;
        }

        bool insertBeforeHead = false;
        if (mode == 1) { // SJF / SRTF
            if (p.remainingTime < head->proc.remainingTime ||
               (p.remainingTime == head->proc.remainingTime && p.arrivalTime < head->proc.arrivalTime)) {
                insertBeforeHead = true;
            }
        } else if (mode == 2) { // Priority
            if (p.priority < head->proc.priority ||
               (p.priority == head->proc.priority && p.arrivalTime < head->proc.arrivalTime)) {
                insertBeforeHead = true;
            }
        }

        if (insertBeforeHead) {
            newNode->next = head;
            head = newNode;
            return;
        }

        PQNode* curr = head;
        while (curr->next) {
            bool insertHere = false;
            if (mode == 1) {
                if (p.remainingTime < curr->next->proc.remainingTime ||
                   (p.remainingTime == curr->next->proc.remainingTime && p.arrivalTime < curr->next->proc.arrivalTime)) {
                    insertHere = true;
                }
            } else if (mode == 2) {
                if (p.priority < curr->next->proc.priority ||
                   (p.priority == curr->next->proc.priority && p.arrivalTime < curr->next->proc.arrivalTime)) {
                    insertHere = true;
                }
            }

            if (insertHere) break;
            curr = curr->next;
        }

        newNode->next = curr->next;
        curr->next = newNode;
    }

    Process dequeue() {
        if (!head) throw runtime_error("Priority Queue is empty");
        PQNode* temp = head;
        Process p = temp->proc;
        head = head->next;
        delete temp;
        count--;
        return p;
    }

    Process& top() { return head->proc; }
    bool isEmpty() const { return head == nullptr; }
    int size() const { return count; }
};

void printGanttChart(LinkedList<GanttBlock>& gantt) {
    cout << "\n--- Gantt Chart Execution Order ---\n|";
    for (int i = 0; i < gantt.getSize(); i++) {
        cout << "  " << gantt.get(i).processId << "  |";
    }
    cout << "\n";
    
    if (gantt.getSize() > 0) {
        cout << gantt.get(0).startTime;
        for (int i = 0; i < gantt.getSize(); i++) {
            cout << setw(6) << gantt.get(i).endTime;
        }
    }
    cout << "\n\n";
}

void printResults(Process procs[], int n, LinkedList<GanttBlock>& gantt, const string& algoName) {
    cout << "========================================================\n";
    cout << "            " << algoName << " RESULTS\n";
    cout << "========================================================\n";
    cout << left << setw(10) << "Process" 
         << setw(15) << "Arrival Time" 
         << setw(13) << "Burst Time" 
         << setw(11) << "Priority" 
         << setw(17) << "Completion Time" 
         << setw(17) << "Turnaround Time" 
         << setw(15) << "Waiting Time" << endl;
    cout << string(98, '-') << endl;

    double totalWT = 0, totalTAT = 0;
    for (int i = 0; i < n; i++) {
        cout << left << setw(10) << procs[i].id
             << setw(15) << procs[i].arrivalTime
             << setw(13) << procs[i].burstTime
             << setw(11) << procs[i].priority
             << setw(17) << procs[i].completionTime
             << setw(17) << procs[i].turnaroundTime
             << setw(15) << procs[i].waitingTime << endl;
        
        totalWT += procs[i].waitingTime;
        totalTAT += procs[i].turnaroundTime;
    }

    cout << string(98, '-') << endl;
    cout << fixed << setprecision(2);
    cout << "Average Waiting Time    : " << (totalWT / n) << " ms\n";
    cout << "Average Turnaround Time : " << (totalTAT / n) << " ms\n";

    printGanttChart(gantt);
}

void resetProcesses(Process input[], Process output[], int n) {
    for (int i = 0; i < n; i++) {
        output[i] = input[i];
        output[i].remainingTime = input[i].burstTime;
        output[i].isCompleted = false;
        output[i].completionTime = 0;
        output[i].turnaroundTime = 0;
        output[i].waitingTime = 0;
    }
}

void openGUI() {
    string fileName = "index.html";
#ifdef _WIN32
    // Windows command
    string command = "start \"\" \"" + fileName + "\"";
#elif __APPLE__
    // macOS command
    string command = "open \"" + fileName + "\"";
#else
    // Linux/Unix command
    string command = "xdg-open \"" + fileName + "\"";
#endif

    int result = system(command.c_str());
    if (result != 0) {
        cerr << "Note: Could not open " << fileName << " automatically.\n";
    }
}

void runFCFS(Process inputProcs[], int n) {
    Process procs[100];
    resetProcesses(inputProcs, procs, n);

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (procs[j].arrivalTime > procs[j + 1].arrivalTime) {
                swap(procs[j], procs[j + 1]);
            }
        }
    }

    LinkedList<GanttBlock> gantt;
    int currentTime = 0;

    for (int i = 0; i < n; i++) {
        if (currentTime < procs[i].arrivalTime) {
            currentTime = procs[i].arrivalTime;
        }
        
        GanttBlock block;
        block.processId = procs[i].id;
        block.startTime = currentTime;
        
        currentTime += procs[i].burstTime;
        block.endTime = currentTime;
        gantt.pushBack(block);

        procs[i].completionTime = currentTime;
        procs[i].turnaroundTime = procs[i].completionTime - procs[i].arrivalTime;
        procs[i].waitingTime = procs[i].turnaroundTime - procs[i].burstTime;
    }

    printResults(procs, n, gantt, "First Come First Serve (FCFS)");
}

void runSJFNonPreemptive(Process inputProcs[], int n) {
    Process procs[100];
    resetProcesses(inputProcs, procs, n);

    LinkedList<GanttBlock> gantt;
    int completed = 0, currentTime = 0;

    while (completed < n) {
        PriorityQueue pq;
        for (int i = 0; i < n; i++) {
            if (procs[i].arrivalTime <= currentTime && !procs[i].isCompleted) {
                pq.enqueue(procs[i], 1);
            }
        }

        if (pq.isEmpty()) {
            currentTime++;
            continue;
        }

        Process currentP = pq.dequeue();
        int idx = -1;
        for (int i = 0; i < n; i++) {
            if (procs[i].id == currentP.id) {
                idx = i;
                break;
            }
        }

        GanttBlock block;
        block.processId = procs[idx].id;
        block.startTime = currentTime;
        currentTime += procs[idx].burstTime;
        block.endTime = currentTime;
        gantt.pushBack(block);

        procs[idx].completionTime = currentTime;
        procs[idx].turnaroundTime = procs[idx].completionTime - procs[idx].arrivalTime;
        procs[idx].waitingTime = procs[idx].turnaroundTime - procs[idx].burstTime;
        procs[idx].isCompleted = true;
        completed++;
    }

    printResults(procs, n, gantt, "Shortest Job First (SJF Non-Preemptive)");
}

void runSJFPreemptive(Process inputProcs[], int n) {
    Process procs[100];
    resetProcesses(inputProcs, procs, n);

    LinkedList<GanttBlock> gantt;
    int completed = 0, currentTime = 0;
    string prevProcess = "";
    int blockStart = 0;

    while (completed < n) {
        PriorityQueue pq;
        for (int i = 0; i < n; i++) {
            if (procs[i].arrivalTime <= currentTime && procs[i].remainingTime > 0) {
                pq.enqueue(procs[i], 1);
            }
        }

        if (pq.isEmpty()) {
            if (prevProcess != "") {
                gantt.pushBack({prevProcess, blockStart, currentTime});
                prevProcess = "";
            }
            currentTime++;
            continue;
        }

        Process currentP = pq.top();
        int idx = -1;
        for (int i = 0; i < n; i++) {
            if (procs[i].id == currentP.id) {
                idx = i;
                break;
            }
        }

        if (prevProcess != procs[idx].id) {
            if (prevProcess != "") {
                gantt.pushBack({prevProcess, blockStart, currentTime});
            }
            prevProcess = procs[idx].id;
            blockStart = currentTime;
        }

        procs[idx].remainingTime--;
        currentTime++;

        if (procs[idx].remainingTime == 0) {
            procs[idx].completionTime = currentTime;
            procs[idx].turnaroundTime = procs[idx].completionTime - procs[idx].arrivalTime;
            procs[idx].waitingTime = procs[idx].turnaroundTime - procs[idx].burstTime;
            procs[idx].isCompleted = true;
            completed++;
            gantt.pushBack({procs[idx].id, blockStart, currentTime});
            prevProcess = "";
        }
    }

    printResults(procs, n, gantt, "Shortest Remaining Time First (SRTF)");
}

void runPriorityNonPreemptive(Process inputProcs[], int n) {
    Process procs[100];
    resetProcesses(inputProcs, procs, n);

    LinkedList<GanttBlock> gantt;
    int completed = 0, currentTime = 0;

    while (completed < n) {
        PriorityQueue pq;
        for (int i = 0; i < n; i++) {
            if (procs[i].arrivalTime <= currentTime && !procs[i].isCompleted) {
                pq.enqueue(procs[i], 2);
            }
        }

        if (pq.isEmpty()) {
            currentTime++;
            continue;
        }

        Process currentP = pq.dequeue();
        int idx = -1;
        for (int i = 0; i < n; i++) {
            if (procs[i].id == currentP.id) {
                idx = i;
                break;
            }
        }

        GanttBlock block;
        block.processId = procs[idx].id;
        block.startTime = currentTime;
        currentTime += procs[idx].burstTime;
        block.endTime = currentTime;
        gantt.pushBack(block);

        procs[idx].completionTime = currentTime;
        procs[idx].turnaroundTime = procs[idx].completionTime - procs[idx].arrivalTime;
        procs[idx].waitingTime = procs[idx].turnaroundTime - procs[idx].burstTime;
        procs[idx].isCompleted = true;
        completed++;
    }

    printResults(procs, n, gantt, "Priority Scheduling (Non-Preemptive)");
}

void runRoundRobin(Process inputProcs[], int n, int quantum) {
    Process procs[100];
    resetProcesses(inputProcs, procs, n);

    Queue<int> readyQueue;
    LinkedList<GanttBlock> gantt;
    bool inQueue[100] = {false};

    int currentTime = 0, completed = 0;

    for (int i = 0; i < n; i++) {
        if (procs[i].arrivalTime == 0) {
            readyQueue.enqueue(i);
            inQueue[i] = true;
        }
    }

    while (completed < n) {
        if (readyQueue.isEmpty()) {
            currentTime++;
            for (int i = 0; i < n; i++) {
                if (procs[i].arrivalTime <= currentTime && !inQueue[i] && procs[i].remainingTime > 0) {
                    readyQueue.enqueue(i);
                    inQueue[i] = true;
                }
            }
            continue;
        }

        int idx = readyQueue.dequeue();
        int execTime = min(quantum, procs[idx].remainingTime);

        GanttBlock block;
        block.processId = procs[idx].id;
        block.startTime = currentTime;
        currentTime += execTime;
        block.endTime = currentTime;
        gantt.pushBack(block);

        procs[idx].remainingTime -= execTime;

        for (int i = 0; i < n; i++) {
            if (i != idx && procs[i].arrivalTime <= currentTime && !inQueue[i] && procs[i].remainingTime > 0) {
                readyQueue.enqueue(i);
                inQueue[i] = true;
            }
        }

        if (procs[idx].remainingTime > 0) {
            readyQueue.enqueue(idx);
        } else {
            procs[idx].completionTime = currentTime;
            procs[idx].turnaroundTime = procs[idx].completionTime - procs[idx].arrivalTime;
            procs[idx].waitingTime = procs[idx].turnaroundTime - procs[idx].burstTime;
            procs[idx].isCompleted = true;
            completed++;
        }
    }

    printResults(procs, n, gantt, "Round Robin (RR, Quantum = " + to_string(quantum) + ")");
}

int main() {
    int numProcesses;
    cout << "========================================================\n";
    cout << "    CPU SCHEDULING SIMULATOR - INTERACTIVE INPUT DATA    \n";
    cout << "========================================================\n";

    cout << "Enter total number of processes: ";
    cin >> numProcesses;

    Process userProcesses[100];

    for (int i = 0; i < numProcesses; i++) {
        string pid;
        int at, bt, prio;
        cout << "\n--- Process #" << (i + 1) << " ---\n";
        cout << "Enter Process ID (e.g. P1): ";
        cin >> pid;
        cout << "Enter Arrival Time: ";
        cin >> at;
        cout << "Enter Burst Time: ";
        cin >> bt;
        cout << "Enter Priority (lower number = higher priority): ";
        cin >> prio;

        userProcesses[i] = Process(pid, at, bt, prio);
    }

    int quantum;
    cout << "\nEnter Time Quantum for Round Robin Algorithm: ";
    cin >> quantum;

    cout << "\nRunning all scheduling algorithms using your custom inputs...\n\n";

    runFCFS(userProcesses, numProcesses);
    runSJFNonPreemptive(userProcesses, numProcesses);
    runSJFPreemptive(userProcesses, numProcesses);
    runPriorityNonPreemptive(userProcesses, numProcesses);
    runRoundRobin(userProcesses, numProcesses, quantum);

    cout << "\nLaunching Graphical User Interface in your default browser...\n";
    
    openGUI();

    return 0;
}