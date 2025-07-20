#include <stdbool.h>
#include <limits.h>

// Stack
typedef struct {
    int *items;
    int top;
    int size;
} Stack;

// Queue
typedef struct {
    int *items;
    int size;
    int front, rear;
} Queue;

// Node structure for adjacency list
typedef struct node {
    int ver, wt;
    struct node *next;
} Node;

// Graph structure
typedef struct GraphList {
    int numVertices;
    Node **arrOfLists;
} GraphList;

// Stack Functions

void initStack(Stack *stack, int size);

bool isEmptys(Stack *stack);

bool isFulls(Stack *stack);

void push(Stack *stack, int item);

int pop(Stack *stack);

int peek(Stack *stack);

// Queue Functions

void initQueue(Queue *queue, int size);

bool isEmptyq(Queue *queue);

bool isFullq(Queue *queue);

void enqueue(Queue *queue, int item);

int dequeue(Queue *queue);

// Graph Functions

void initGraph(GraphList *g, int vertx);

Node *createnodeWT(int index, int wt);

void addvertexwithWT(GraphList *g, int scr, int dess, int wt , int cm[15][15]);

void removeVertex(GraphList *g, int scr, int dess , int cm[15][15]);

void freeGraph(GraphList *g);

void printgraph(GraphList g);

void printlist(Node *p);

// Algorithms

void BFS(GraphList g, int ver);

void DFS(GraphList g, int ver);

int Checkifcircular(GraphList g, int ver);

void prims(GraphList g, int ver);

void Dijkstras(GraphList g, int ver);

void PrintShortPath(GraphList g, int scr, int dess);

void mutual_friends(GraphList G, int cm[15][15], int s1, int s2);

void first_level_friends(GraphList G , int cm[15][15] , int user);

void recommendations(GraphList G , int cm[15][15] , int ver);