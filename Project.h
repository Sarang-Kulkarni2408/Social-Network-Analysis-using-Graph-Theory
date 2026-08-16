#ifndef PROJECT_H
#define PROJECT_H

#include <stdbool.h>
#include <limits.h>

/* ----------------------------------------------------------------------
 * Configuration
 * -------------------------------------------------------------------- */
#define MAX_USERS   10          /* number of registered users (index 1..MAX_USERS) */
#define MAX_NAME    16          /* max length of a user name incl. null terminator */

/* ----------------------------------------------------------------------
 * Data structures
 * -------------------------------------------------------------------- */

/* Stack (array based, used for DFS / cycle-detection / path reconstruction) */
typedef struct {
    int *items;
    int top;
    int size;
} Stack;

/* Circular Queue (array based, used for BFS) */
typedef struct {
    int *items;
    int size;
    int front, rear;
} Queue;

/* Adjacency-list node */
typedef struct node {
    int ver, wt;
    struct node *next;
} Node;

/* Graph represented as an array of adjacency lists (1-indexed vertices) */
typedef struct GraphList {
    int numVertices;
    Node **arrOfLists;
} GraphList;

/* ----------------------------------------------------------------------
 * Global user table (defined once in Project.c)
 * -------------------------------------------------------------------- */
extern char users[MAX_USERS + 1][MAX_NAME];

/* ----------------------------------------------------------------------
 * Stack functions
 * -------------------------------------------------------------------- */
void initStack(Stack *stack, int size);
bool isEmptys(const Stack *stack);
bool isFulls(const Stack *stack);
void push(Stack *stack, int item);
int  pop(Stack *stack);
int  peek(const Stack *stack);
void freeStack(Stack *stack);

/* ----------------------------------------------------------------------
 * Queue functions
 * -------------------------------------------------------------------- */
void initQueue(Queue *queue, int size);
bool isEmptyq(const Queue *queue);
bool isFullq(const Queue *queue);
void enqueue(Queue *queue, int item);
int  dequeue(Queue *queue);
void freeQueue(Queue *queue);

/* ----------------------------------------------------------------------
 * Graph construction / teardown
 * -------------------------------------------------------------------- */
void  initGraph(GraphList *g, int vertx);
Node *createNodeWT(int index, int wt);
bool  addVertexWithWT(GraphList *g, int scr, int dess, int wt, int cm[MAX_USERS + 1][MAX_USERS + 1]);
bool  removeVertex(GraphList *g, int scr, int dess, int cm[MAX_USERS + 1][MAX_USERS + 1]);
void  freeGraph(GraphList *g);
void  printGraph(GraphList g);
void  printList(const Node *p);
bool  isValidVertex(const GraphList *g, int v);

/* ----------------------------------------------------------------------
 * Core algorithms
 * -------------------------------------------------------------------- */
void BFS(GraphList g, int ver);
void DFS(GraphList g, int ver);
bool checkIfCircular(GraphList g);                       /* checks the WHOLE graph, all components */
void prims(GraphList g, int ver);
void dijkstras(GraphList g, int ver);
void printShortPath(GraphList g, int scr, int dess);

/* ----------------------------------------------------------------------
 * Social-network features
 * -------------------------------------------------------------------- */
void mutualFriends(GraphList G, int cm[MAX_USERS + 1][MAX_USERS + 1], int s1, int s2);
void firstLevelFriends(GraphList G, int cm[MAX_USERS + 1][MAX_USERS + 1], int user);
void recommendations(GraphList G, int cm[MAX_USERS + 1][MAX_USERS + 1], int ver);

/* ----------------------------------------------------------------------
 * Extra analytics (new, "worth noticing" additions)
 * -------------------------------------------------------------------- */
void printAdjacencyMatrix(GraphList g, int cm[MAX_USERS + 1][MAX_USERS + 1]);
void degreeCentrality(GraphList g);
bool isConnected(GraphList g);

#endif /* PROJECT_H */
