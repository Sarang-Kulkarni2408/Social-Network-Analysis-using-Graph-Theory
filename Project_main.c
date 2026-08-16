#include "Project.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/*
    Our social network graph looks like -
                  Raj
               /   |   \
           Ankit - Amit - Yash
          /         |         \
         Amol     Suraj       Om
          |
       Prakash
      /       \
    Nilesh   Mayank
*/

/* Reads an int robustly; on bad input it clears the offending token from
 * stdin and returns false instead of leaving scanf in a broken state
 * (the original code never checked scanf's return value at all). */
static bool readInt(int *out)
{
    int result = scanf("%d", out);
    if (result == EOF) {
        /* Input stream closed (e.g. piped input ran out) - exit cleanly
         * instead of spinning forever re-prompting for input. */
        printf("\nInput ended - exiting.\n\n");
        exit(0);
    }
    if (result != 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { /* discard the bad token */ }
        printf("Please enter a valid number.\n\n");
        return false;
    }
    return true;
}

static void printMenu(void)
{
    printf("\nMenu : \n");
    printf("    1.  Remove a connection between 2 users\n");
    printf("    2.  Free the graph of users\n");
    printf("    3.  Print the graph of users (adjacency list)\n");
    printf("    4.  Find BFS of the graph of users\n");
    printf("    5.  Find DFS of the graph of users\n");
    printf("    6.  Detect a cycle anywhere in the graph of users\n");
    printf("    7.  Find minimum spanning tree using Prim's Algorithm\n");
    printf("    8.  Find shortest distance from a user to all other users (Dijkstra)\n");
    printf("    9.  Find shortest path between two users\n");
    printf("    10. Find mutual friends of two users\n");
    printf("    11. Find first-level friends of a user\n");
    printf("    12. Display friend recommendations for a user\n");
    printf("    13. Print the adjacency matrix\n");
    printf("    14. Show degree centrality (most popular users)\n");
    printf("    15. Check if the whole network is connected\n");
    printf("Enter 0 to exit\n\n");
}

int main(void)
{
    const int numVertices = MAX_USERS;
    GraphList G;
    initGraph(&G, numVertices);

    int cm[MAX_USERS + 1][MAX_USERS + 1] = { 0 }; /* cost / adjacency matrix, zero-initialised */

    printf("\nThe users of this social network and their respective numbers are :\n");
    for (int i = 1; i <= numVertices; i++)
        printf("    %d. %s\n", i, users[i]);
    printf("\n");

    /* Seed the network with the friendships shown in the diagram above. */
    addVertexWithWT(&G, 1, 3, 1, cm);
    addVertexWithWT(&G, 8, 9, 1, cm);
    addVertexWithWT(&G, 2, 5, 1, cm);
    addVertexWithWT(&G, 1, 8, 1, cm);
    addVertexWithWT(&G, 2, 8, 1, cm);
    addVertexWithWT(&G, 1, 2, 1, cm);
    addVertexWithWT(&G, 6, 7, 1, cm);
    addVertexWithWT(&G, 2, 3, 1, cm);
    addVertexWithWT(&G, 4, 3, 1, cm);
    addVertexWithWT(&G, 9, 7, 1, cm);
    addVertexWithWT(&G, 10, 7, 1, cm);

    bool running = true;
    printMenu();

    while (running) {
        printf("Enter your choice: ");
        int choice;
        if (!readInt(&choice)) continue;

        switch (choice) {
        case 0:
            printf("\nThank You for visiting!\n\n");
            running = false;
            break;

        case 1: {
            int scr, dess;
            printf("\nEnter the vertices to remove (src and dest): ");
            if (!readInt(&scr) || !readInt(&dess)) break;
            removeVertex(&G, scr, dess, cm);
            break;
        }

        case 2:
            freeGraph(&G);
            printf("\nThe graph is cleared successfully\n\n");
            break;

        case 3:
            printf("\nThe graph structure will be : \n");
            printGraph(G);
            break;

        case 4: {
            int startbfs;
            printf("\nEnter the vertex from which you want to find BFS: ");
            if (!readInt(&startbfs)) break;
            BFS(G, startbfs);
            break;
        }

        case 5: {
            int startdfs;
            printf("\nEnter the vertex from which you want to find DFS: ");
            if (!readInt(&startdfs)) break;
            DFS(G, startdfs);
            break;
        }

        case 6:
            if (checkIfCircular(G))
                printf("\nThe graph contains a cycle\n\n");
            else
                printf("\nThe graph doesn't contain a cycle\n\n");
            break;

        case 7: {
            int startPrims;
            printf("\nEnter the vertex from which to build the minimum spanning tree: ");
            if (!readInt(&startPrims)) break;
            prims(G, startPrims);
            break;
        }

        case 8: {
            int startDij;
            printf("\nEnter the vertex from which to find shortest distances to all users: ");
            if (!readInt(&startDij)) break;
            dijkstras(G, startDij);
            break;
        }

        case 9: {
            int src, dest;
            printf("\nEnter the vertices to find the shortest path (src and dest): ");
            if (!readInt(&src) || !readInt(&dest)) break;
            printShortPath(G, src, dest);
            break;
        }

        case 10: {
            int s1, s2;
            printf("\nEnter the vertices to find mutual friends (s1 and s2): ");
            if (!readInt(&s1) || !readInt(&s2)) break;
            mutualFriends(G, cm, s1, s2);
            break;
        }

        case 11: {
            int user;
            printf("\nEnter the vertex to find its first-level friends: ");
            if (!readInt(&user)) break;
            firstLevelFriends(G, cm, user);
            break;
        }

        case 12: {
            int user;
            printf("\nEnter the vertex to find its recommendations: ");
            if (!readInt(&user)) break;
            recommendations(G, cm, user);
            break;
        }

        case 13:
            printf("\n");
            printAdjacencyMatrix(G, cm);
            break;

        case 14:
            printf("\n");
            degreeCentrality(G);
            break;

        case 15:
            printf("\n");
            if (isConnected(G))
                printf("The network is fully connected - everyone can reach everyone else.\n\n");
            else
                printf("The network is NOT fully connected - it has isolated groups.\n\n");
            break;

        default:
            printf("\nInvalid choice!\n\n");
        }
    }

    freeGraph(&G); /* make sure nothing leaks even if the user never picked option 2 */
    return 0;
}
