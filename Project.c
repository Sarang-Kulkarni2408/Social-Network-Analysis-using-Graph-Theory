/* ============================================================================
 * Project.c
 *
 * A social-network graph engine built from scratch on top of custom
 * Stack / Queue ADTs and an adjacency-list graph representation.
 *
 * Since we model a *social network*, every edge represents a friendship
 * and therefore always carries weight = 1 (see addVertexWithWT).
 * ==========================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#include "Project.h"

/* The users of the social network. Index 0 is unused ("") so that vertex
 * numbers map 1:1 onto array indices — this keeps every algorithm below
 * consistently 1-indexed. */
char users[MAX_USERS + 1][MAX_NAME] = {
    "", "Raj", "Amit", "Yash", "Om",
    "Suraj", "Nilesh", "Prakash",
    "Ankit", "Amol", "Mayank"
};

/* ==========================================================================
 * Stack
 * ==========================================================================*/

void initStack(Stack *stack, int size)
{
    stack->top   = -1;
    stack->size  = size;
    stack->items = (int *)malloc(sizeof(int) * (size_t)size);
}

bool isEmptys(const Stack *stack) { return stack->top == -1; }
bool isFulls(const Stack *stack)  { return stack->top == stack->size - 1; }

void push(Stack *stack, int item)
{
    if (isFulls(stack)) return;
    stack->items[++(stack->top)] = item;
}

int pop(Stack *stack)
{
    if (isEmptys(stack)) return -1;
    return stack->items[(stack->top)--];
}

int peek(const Stack *stack)
{
    if (isEmptys(stack)) return -1;
    return stack->items[stack->top];
}

void freeStack(Stack *stack)
{
    free(stack->items);
    stack->items = NULL;
    stack->top = -1;
    stack->size = 0;
}

/* ==========================================================================
 * Queue (circular)
 * ==========================================================================*/

void initQueue(Queue *queue, int size)
{
    queue->size  = size;
    queue->items = (int *)malloc(sizeof(int) * (size_t)size);
    queue->front = -1;
    queue->rear  = -1;
}

bool isEmptyq(const Queue *queue) { return queue->front == -1; }
bool isFullq(const Queue *queue)  { return (queue->rear + 1) % queue->size == queue->front; }

void enqueue(Queue *queue, int item)
{
    if (isFullq(queue)) return;

    if (isEmptyq(queue)) queue->front = 0;

    queue->rear = (queue->rear + 1) % queue->size;
    queue->items[queue->rear] = item;
}

int dequeue(Queue *queue)
{
    if (isEmptyq(queue)) return -1;

    int item = queue->items[queue->front];
    if (queue->front == queue->rear)
        queue->front = queue->rear = -1;
    else
        queue->front = (queue->front + 1) % queue->size;

    return item;
}

void freeQueue(Queue *queue)
{
    free(queue->items);
    queue->items = NULL;
    queue->front = queue->rear = -1;
    queue->size = 0;
}

/* ==========================================================================
 * Graph construction / teardown
 * ==========================================================================*/

void initGraph(GraphList *g, int vertx)
{
    g->numVertices = vertx;
    g->arrOfLists = (Node **)malloc(sizeof(Node *) * (size_t)vertx);
    for (int i = 0; i < vertx; i++)
        g->arrOfLists[i] = NULL;
}

Node *createNodeWT(int index, int wt)
{
    Node *nn = (Node *)malloc(sizeof(Node));
    nn->ver  = index;
    nn->wt   = wt;
    nn->next = NULL;
    return nn;
}

bool isValidVertex(const GraphList *g, int v)
{
    return v >= 1 && v <= g->numVertices;
}

/* Returns true if scr-dess is already an edge (queried from the cost matrix). */
static bool edgeExists(int cm[MAX_USERS + 1][MAX_USERS + 1], int scr, int dess)
{
    return cm[scr][dess] == 1;
}

bool addVertexWithWT(GraphList *g, int scr, int dess, int wt, int cm[MAX_USERS + 1][MAX_USERS + 1])
{
    if (!isValidVertex(g, scr) || !isValidVertex(g, dess) || scr == dess) {
        printf("Invalid edge (%d, %d) - skipped.\n", scr, dess);
        return false;
    }
    if (edgeExists(cm, scr, dess)) {
        printf("Edge between %s and %s already exists - skipped.\n", users[scr], users[dess]);
        return false;
    }

    Node *forScr = createNodeWT(dess, wt);
    forScr->next = g->arrOfLists[scr - 1];
    g->arrOfLists[scr - 1] = forScr;
    cm[scr][dess] = 1;

    Node *forDess = createNodeWT(scr, wt);
    forDess->next = g->arrOfLists[dess - 1];
    g->arrOfLists[dess - 1] = forDess;
    cm[dess][scr] = 1;

    return true;
}

/* Removes the node with vertex id `d` from the list pointed to by *headAD.
 * Safe against "not found" (does nothing) and empty-list cases. */
static void deleteNode(Node **headAD, int d)
{
    Node *cur = *headAD, *prev = NULL;

    while (cur != NULL && cur->ver != d) {
        prev = cur;
        cur = cur->next;
    }

    if (cur == NULL) return; /* not found - nothing to remove */

    if (prev == NULL)
        *headAD = cur->next;
    else
        prev->next = cur->next;

    free(cur);
}

bool removeVertex(GraphList *g, int scr, int dess, int cm[MAX_USERS + 1][MAX_USERS + 1])
{
    if (!isValidVertex(g, scr) || !isValidVertex(g, dess)) {
        printf("Invalid vertex/vertices supplied.\n");
        return false;
    }
    if (!edgeExists(cm, scr, dess)) {
        printf("No edge exists between %s and %s.\n\n", users[scr], users[dess]);
        return false;
    }

    deleteNode(&(g->arrOfLists[scr - 1]), dess);
    cm[scr][dess] = 0;

    deleteNode(&(g->arrOfLists[dess - 1]), scr);
    cm[dess][scr] = 0;               /* BUG FIX: this used to be set to 1 */

    printf("The edge from %s to %s removed successfully\n\n", users[scr], users[dess]);
    return true;
}

void freeGraph(GraphList *g)
{
    for (int i = 0; i < g->numVertices; i++) {
        Node *p = g->arrOfLists[i];
        while (p) {
            Node *tmp = p->next;
            free(p);
            p = tmp;
        }
        g->arrOfLists[i] = NULL;
    }

    free(g->arrOfLists);
    g->arrOfLists = NULL;
    g->numVertices = 0;
}

/* ==========================================================================
 * Traversals
 * ==========================================================================*/

void BFS(GraphList g, int ver)
{
    if (!isValidVertex(&g, ver)) { printf("Invalid vertex.\n\n"); return; }

    Queue q;
    initQueue(&q, g.numVertices);

    int *visited = (int *)calloc((size_t)g.numVertices, sizeof(int));
    int *bfs     = (int *)malloc(sizeof(int) * (size_t)g.numVertices);
    int sbfs = 0;

    enqueue(&q, ver);
    visited[ver - 1] = 1;

    while (!isEmptyq(&q)) {
        int n = dequeue(&q);
        bfs[sbfs++] = n;

        for (Node *p = g.arrOfLists[n - 1]; p != NULL; p = p->next) {
            if (!visited[p->ver - 1]) {
                enqueue(&q, p->ver);
                visited[p->ver - 1] = 1;
            }
        }
    }

    printf("The BFS traversal for the given graph starting from %s will be : \n", users[bfs[0]]);
    for (int i = 0; i < sbfs; i++)
        printf("%s%s", users[bfs[i]], (i < sbfs - 1) ? " -> " : "");
    printf("\n\n");

    free(visited);
    free(bfs);
    freeQueue(&q);
}

void DFS(GraphList g, int ver)
{
    if (!isValidVertex(&g, ver)) { printf("Invalid vertex.\n\n"); return; }

    Stack s;
    initStack(&s, g.numVertices);

    int *visited = (int *)calloc((size_t)g.numVertices, sizeof(int));
    int *dfs     = (int *)malloc(sizeof(int) * (size_t)g.numVertices);
    int sdfs = 0;

    push(&s, ver);
    visited[ver - 1] = 1;

    while (!isEmptys(&s)) {
        int n = pop(&s);
        dfs[sdfs++] = n;

        for (Node *p = g.arrOfLists[n - 1]; p != NULL; p = p->next) {
            if (!visited[p->ver - 1]) {
                push(&s, p->ver);
                visited[p->ver - 1] = 1;
            }
        }
    }

    printf("The DFS traversal for the given graph starting from %s will be : \n", users[dfs[0]]);
    for (int i = 0; i < sdfs; i++)
        printf("%s%s", users[dfs[i]], (i < sdfs - 1) ? " -> " : "");
    printf("\n\n");

    free(visited);
    free(dfs);
    freeStack(&s);
}

/* ==========================================================================
 * Cycle detection
 *
 * Iterative stack-based DFS gives false positives on undirected graphs
 * (a node can be pushed twice before being marked visited). We instead
 * use recursive DFS with an explicit parent, which is the textbook-correct
 * way to detect a cycle in an undirected graph, and we scan every
 * connected component so disconnected graphs are handled correctly too.
 * ==========================================================================*/

static bool cycleDFS(GraphList *g, int cur, int parent, int *visited)
{
    visited[cur] = 1;
    for (Node *p = g->arrOfLists[cur - 1]; p != NULL; p = p->next) {
        if (!visited[p->ver]) {
            if (cycleDFS(g, p->ver, cur, visited)) return true;
        } else if (p->ver != parent) {
            return true; /* back-edge to an already-visited, non-parent vertex */
        }
    }
    return false;
}

bool checkIfCircular(GraphList g)
{
    int *visited = (int *)calloc((size_t)g.numVertices + 1, sizeof(int));
    bool found = false;

    for (int v = 1; v <= g.numVertices && !found; v++) {
        if (!visited[v])
            found = cycleDFS(&g, v, 0, visited);
    }

    free(visited);
    return found;
}

/* ==========================================================================
 * Prim's Algorithm (Minimum Spanning Tree)
 * ==========================================================================*/

void prims(GraphList g, int ver)
{
    if (!isValidVertex(&g, ver)) { printf("Invalid vertex.\n\n"); return; }

    int totalver = g.numVertices;
    if (totalver < 2) { printf("Need at least 2 users to build a spanning tree.\n\n"); return; }

    printf("The minimum spanning tree starting from %s will be : \n", users[ver]);

    int *vis  = (int *)calloc((size_t)totalver, sizeof(int));
    int *key  = (int *)malloc(sizeof(int) * (size_t)totalver); /* cheapest edge weight into the MST so far */
    int *from = (int *)malloc(sizeof(int) * (size_t)totalver); /* the MST vertex that offers that cheapest edge */

    for (int i = 0; i < totalver; i++) { key[i] = INT_MAX; from[i] = -1; }

    int start = ver - 1;
    key[start] = 0;
    int edgesAdded = 0;

    for (int iter = 0; iter < totalver; iter++) {
        int u = -1, best = INT_MAX;
        for (int j = 0; j < totalver; j++) {
            if (!vis[j] && key[j] < best) { best = key[j]; u = j; }
        }
        if (u == -1) break; /* remaining vertices are unreachable -> disconnected graph */

        vis[u] = 1;
        if (from[u] != -1) {
            printf("%s -> %s\n", users[from[u] + 1], users[u + 1]);
            edgesAdded++;
        }

        for (Node *p = g.arrOfLists[u]; p != NULL; p = p->next) {
            int v = p->ver - 1;
            if (!vis[v] && p->wt < key[v]) { key[v] = p->wt; from[v] = u; }
        }
    }

    if (edgesAdded < totalver - 1)
        printf("(Graph is disconnected from %s - only a partial spanning forest was built.)\n", users[ver]);

    printf("\n");
    free(vis);
    free(key);
    free(from);
}

/* ==========================================================================
 * Dijkstra's Algorithm
 * ==========================================================================*/

/* Returns the unvisited vertex (0-indexed) with the smallest tentative
 * distance, or -1 if none remain reachable. */
static int findMinFromDist(int numVertices, const int *vis, const int *dist)
{
    int min = INT_MAX, v = -1;
    for (int i = 0; i < numVertices; i++) {
        if (!vis[i] && dist[i] < min) { min = dist[i]; v = i; }
    }
    return v;
}

void dijkstras(GraphList g, int ver)
{
    if (!isValidVertex(&g, ver)) { printf("Invalid vertex.\n\n"); return; }

    int totalver = g.numVertices;
    int *dist = (int *)malloc(sizeof(int) * (size_t)totalver);
    int *vis  = (int *)calloc((size_t)totalver, sizeof(int));

    for (int i = 0; i < totalver; i++) dist[i] = INT_MAX;
    dist[ver - 1] = 0;

    for (int done = 0; done < totalver; done++) {
        int u = findMinFromDist(totalver, vis, dist);
        if (u == -1) break; /* remaining vertices are unreachable */
        vis[u] = 1;

        for (Node *p = g.arrOfLists[u]; p != NULL; p = p->next) {
            if (!vis[p->ver - 1] && dist[u] != INT_MAX && dist[p->ver - 1] > dist[u] + p->wt)
                dist[p->ver - 1] = dist[u] + p->wt;
        }
    }

    printf("The shortest distance from %s is : \n", users[ver]);
    for (int i = 0; i < totalver; i++) {
        if (dist[i] == INT_MAX)
            printf("%s -> unreachable\n", users[i + 1]);
        else
            printf("%s -> %d\n", users[i + 1], dist[i]);
    }
    printf("\n");

    free(dist);
    free(vis);
}

void printList(const Node *p)
{
    while (p) {
        printf("%s ", users[p->ver]);
        if (p->next != NULL) printf("-> ");
        p = p->next;
    }
}

void printGraph(GraphList g)
{
    for (int i = 0; i < g.numVertices; i++) {
        printf("%s ==> ", users[i + 1]);
        printList(g.arrOfLists[i]);
        printf("\n");
    }
    printf("\n");
}

void printShortPath(GraphList g, int scr, int dess)
{
    if (!isValidVertex(&g, scr) || !isValidVertex(&g, dess)) { printf("Invalid vertex/vertices.\n\n"); return; }

    int totalver = g.numVertices;
    int *dist   = (int *)malloc(sizeof(int) * (size_t)totalver);
    int *vis    = (int *)calloc((size_t)totalver, sizeof(int));
    int *parent = (int *)calloc((size_t)totalver + 1, sizeof(int));

    for (int i = 0; i < totalver; i++) dist[i] = INT_MAX;
    dist[scr - 1] = 0;

    for (int done = 0; done < totalver; done++) {
        int u = findMinFromDist(totalver, vis, dist);
        if (u == -1) break;
        vis[u] = 1;

        for (Node *p = g.arrOfLists[u]; p != NULL; p = p->next) {
            if (!vis[p->ver - 1] && dist[u] != INT_MAX && dist[p->ver - 1] > dist[u] + p->wt) {
                dist[p->ver - 1] = dist[u] + p->wt;
                parent[p->ver] = u + 1;
            }
        }
    }

    if (dist[dess - 1] == INT_MAX) {
        printf("There is no path between %s and %s.\n\n", users[scr], users[dess]);
        free(dist); free(vis); free(parent);
        return;
    }

    Stack s;
    initStack(&s, totalver + 1);
    for (int v = dess; v != 0; v = parent[v])
        push(&s, v);

    printf("The shortest path between %s and %s is : \n", users[scr], users[dess]);
    while (!isEmptys(&s)) {
        printf("%s ", users[pop(&s)]);
        if (!isEmptys(&s)) printf("-> ");
    }
    printf("\n\n");

    free(dist);
    free(vis);
    free(parent);
    freeStack(&s);
}

/* ==========================================================================
 * Social-network features
 * ==========================================================================*/

void mutualFriends(GraphList G, int cm[MAX_USERS + 1][MAX_USERS + 1], int s1, int s2)
{
    if (!isValidVertex(&G, s1) || !isValidVertex(&G, s2)) { printf("Invalid vertex/vertices.\n\n"); return; }

    printf("The mutual friends of %s and %s are : \n", users[s1], users[s2]);
    int count = 0;
    for (int i = 1; i <= G.numVertices; i++) {
        if (i != s1 && i != s2 && cm[s1][i] == 1 && cm[s2][i] == 1) {
            printf("%s\n", users[i]);
            count++;
        }
    }
    if (count == 0) printf("The given users have no mutual friends.\n");
    printf("\n");
}

void firstLevelFriends(GraphList G, int cm[MAX_USERS + 1][MAX_USERS + 1], int user)
{
    if (!isValidVertex(&G, user)) { printf("Invalid vertex.\n\n"); return; }

    printf("The first-level friends of %s are :\n", users[user]);
    int count = 0;
    for (int i = 1; i <= G.numVertices; i++) {
        if (cm[user][i] == 1) { printf("%s\n", users[i]); count++; }
    }
    if (count == 0) printf("%s has no friends yet.\n", users[user]);
    printf("\n");
}

/* A user `j` is recommended to `ver` if their shortest-path distance is
 * exactly 2 (i.e. they share at least one mutual friend but are not
 * already friends). Reuses the same Dijkstra logic as `dijkstras`. */
void recommendations(GraphList G, int cm[MAX_USERS + 1][MAX_USERS + 1], int ver)
{
    (void)cm; /* kept in the signature for API symmetry with the other social features */
    if (!isValidVertex(&G, ver)) { printf("Invalid vertex.\n\n"); return; }

    int totalver = G.numVertices;
    int *dist = (int *)malloc(sizeof(int) * (size_t)totalver);
    int *vis  = (int *)calloc((size_t)totalver, sizeof(int));

    for (int i = 0; i < totalver; i++) dist[i] = INT_MAX;
    dist[ver - 1] = 0;

    for (int done = 0; done < totalver; done++) {
        int u = findMinFromDist(totalver, vis, dist);
        if (u == -1) break;
        vis[u] = 1;

        for (Node *p = G.arrOfLists[u]; p != NULL; p = p->next) {
            if (!vis[p->ver - 1] && dist[u] != INT_MAX && dist[p->ver - 1] > dist[u] + p->wt)
                dist[p->ver - 1] = dist[u] + p->wt;
        }
    }

    printf("The recommendations for %s are :\n", users[ver]);
    int count = 0;
    for (int i = 0; i < totalver; i++) {
        if (dist[i] == 2) { printf("%s\n", users[i + 1]); count++; }
    }
    if (count == 0) printf("No recommendations available right now.\n");
    printf("\n");

    free(dist);
    free(vis);
}

/* ==========================================================================
 * Extra analytics - new "worth noticing" additions
 * ==========================================================================*/

void printAdjacencyMatrix(GraphList g, int cm[MAX_USERS + 1][MAX_USERS + 1])
{
    printf("Adjacency matrix:\n\n%8s", "");
    for (int j = 1; j <= g.numVertices; j++) printf("%-8s", users[j]);
    printf("\n");

    for (int i = 1; i <= g.numVertices; i++) {
        printf("%-8s", users[i]);
        for (int j = 1; j <= g.numVertices; j++)
            printf("%-8d", cm[i][j]);
        printf("\n");
    }
    printf("\n");
}

/* Degree centrality: how many direct friends each user has, sorted
 * descending - a quick "who is the most popular user" view. */
void degreeCentrality(GraphList g)
{
    typedef struct { int ver, degree; } DegEntry;
    DegEntry *entries = (DegEntry *)malloc(sizeof(DegEntry) * (size_t)g.numVertices);

    for (int i = 0; i < g.numVertices; i++) {
        int degree = 0;
        for (Node *p = g.arrOfLists[i]; p != NULL; p = p->next) degree++;
        entries[i].ver = i + 1;
        entries[i].degree = degree;
    }

    /* simple insertion sort - fine for MAX_USERS-scale input */
    for (int i = 1; i < g.numVertices; i++) {
        DegEntry key = entries[i];
        int j = i - 1;
        while (j >= 0 && entries[j].degree < key.degree) {
            entries[j + 1] = entries[j];
            j--;
        }
        entries[j + 1] = key;
    }

    printf("Degree centrality (most connected users first):\n");
    for (int i = 0; i < g.numVertices; i++)
        printf("    %-10s %d connection(s)\n", users[entries[i].ver], entries[i].degree);
    printf("\n");

    free(entries);
}

/* Whether the whole social network forms a single connected component. */
bool isConnected(GraphList g)
{
    if (g.numVertices == 0) return true;

    int *visited = (int *)calloc((size_t)g.numVertices, sizeof(int));
    Queue q;
    initQueue(&q, g.numVertices);

    enqueue(&q, 1);
    visited[0] = 1;
    int seen = 1;

    while (!isEmptyq(&q)) {
        int n = dequeue(&q);
        for (Node *p = g.arrOfLists[n - 1]; p != NULL; p = p->next) {
            if (!visited[p->ver - 1]) {
                visited[p->ver - 1] = 1;
                seen++;
                enqueue(&q, p->ver);
            }
        }
    }

    free(visited);
    freeQueue(&q);
    return seen == g.numVertices;
}
