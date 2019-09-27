#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct vert {
    int vertex;
    //int color;
    struct vert* next;
} Vertex;

typedef struct node {
    Vertex* list;
} Node;


//Function
void printAdjArray(Node ptr[], int size);
int findSize(FILE* fPtr);
void graphSq(Node array[], int size, FILE* output);
void printToFile(Node ptr[], int size, FILE* output);

//Main function

int main(int argc, char* argv[]) {
    if (argc == 3) {
        //Code goes here
        FILE* fPtr = fopen(argv[1], "r");
        FILE* oPtr = fopen(argv[2], "w");
        
        if (fPtr == NULL) {
            printf("File could not be opened\n");
        }
        if (oPtr == NULL) {
            printf("File could not be opened\n");
        }

        
        //Adjacency list double array with Nodes pointing to vertexes
        //Edges in list are of form (u,v) where u is the index of the adjArray(such as u=1 is adjArray[1] and u=2 is adjArray[2]) and v is a Vertex in the list of adjArray
        
        int size = findSize(fPtr);
        printf("Size is %d\n", size);
        
        //Node 1 = vertex u=1
        Node adjArray[size];
        
        int i;
        for(i=0;i<=size;i++) {
            adjArray[i].list = NULL;
        }
        
        int u;    
        int v;

        while (fscanf(fPtr, "%d %d", &u, &v) == 2) {
            
            
            //Makes sure we are not at EOF before we store it in our array(just in case)
            if (!feof(fPtr)) {

                //Find the right Node in adjArray
                
                Vertex* currentVertex = adjArray[u].list;

                //Now decide where to add the vertex v

                if (currentVertex == NULL) {
                    //add new vertex to p->vertex
                    Vertex* hold = malloc(sizeof(Vertex));
                    hold->vertex = v;
                    hold->next = NULL;
                    adjArray[u].list = hold;
                }
                if (currentVertex != NULL) {
                    while(currentVertex->next != NULL) {
                        currentVertex = currentVertex->next;
                    }
                    
                    //Finishes with currentVertex as the last vertex in the vertex list
                    Vertex* hold = malloc(sizeof(Vertex));
                    hold->vertex = v;
                    hold->next = NULL;
                    currentVertex->next = hold;
                }
                
                //Debugging print statement:
                //printf("Edge (%d,%d) added to adj list\n", u, v);

                
            }//end if !=EOF

            else {
                printf("You are at the end of the file\n");

            }
        }//end while != EOF

        printAdjArray(adjArray, size);


        graphSq(adjArray, size, oPtr);

        
    } //end if check for correct # of arguments
    else {
        printf("Please include file in command line and try again.\n");
    }

} //end main

void printAdjArray(Node ptr[], int size) {
    //Runs through and prints every Node in the array ptr and every Vertex list of every Node
    printf("Printing array:\n\n");
    
    Vertex* v;
    int i;

    for (i=1; i<=size; i++) {
        v = ptr[i].list;
        printf("%d: ", i);
        while (v != NULL) {
            printf("%d, ", v->vertex);
            v = v->next;
        }
        printf("\n");
    }
}//end function print adj array

int findSize(FILE* fPtr) {

    int largest = 0;
    
    int u;
    int v;

    while (fscanf(fPtr, "%d %d", &u, &v) == 2) {

        //a is either a number or the EOF
        if (!feof(fPtr)) {
            //If u is larger than the current largest, make u the new largest
            if (u > largest) {
                largest = u;
            }

            //If v is larger than the current largest, make v the new largest
            if (v > largest) {
                largest = v;
            }
        } //end if != EOF

    }//end while != EOF

    rewind(fPtr);
    return largest;
} //end function findSize


void graphSq(Node array[], int size, FILE* output) {

    printf("Now running graph Squared\n\n");

    Vertex* v;
    Vertex* w;
    int vertexV;
    int vertexW;
    Vertex* currentVertex;

    //make the graph sq array
    Node newArr[size];

    int i;

    for (i=0;i<=size;i++) {
        newArr[i].list = NULL;
    }

    //For all edges u:
    for (i=1; i<=size; i++) {

        v = array[i].list;

        //for all vertices v in the adj list of u:
        while (v != NULL) {
            
            vertexV = v->vertex;

            //w = the adj list of the current v vertex
            w = array[vertexV].list;

            while (w != NULL) {

                //if color of w is white and it isn't a back edge
                if (w->vertex != i && w->vertex != vertexV) {

                    vertexW = w->vertex;
                    
                    currentVertex = newArr[i].list;
                    if (currentVertex == NULL) {
                        Vertex* hold = malloc(sizeof(Vertex));
                        hold->vertex = vertexW;
                        hold->next = NULL;
                        newArr[i].list = hold;
                        
                    }
                    else {
                        
                        while (currentVertex->next != NULL) {
                            if (currentVertex->vertex == vertexW) {
                                break;
                            }
                            currentVertex = currentVertex->next;
                        }
                        if (currentVertex->next == NULL) {
                            Vertex* hold = malloc(sizeof(Vertex));
                            hold->vertex = vertexW;
                            hold->next = NULL;
                            currentVertex->next = hold;
                        }
                    }

                    //Debugging print statement:
                    //printf("Edge (%d, %d) added to list\n", i, vertexW);

                }//end if edge is not a back edge
                
                w = w->next;
            }//end while w!=NULL


            v = v->next;

        }//end while v!=NULL

    } //end for all edges u


    printf("Graph squraed function is over\n\n");

    printToFile(newArr, size, output);
}//end function graphSq

void printToFile(Node ptr[], int size, FILE* output) {
    printf("Printing array to file\n\n");

    Vertex* v;
    int i;

    for (i=1;i<=size;i++) {
        v = ptr[i].list;
        fprintf(output, "%d: ", i);
        while (v != NULL) {
            fprintf(output, "%d, ", v->vertex);
            v = v->next;
        }//end while v!=N
        fprintf(output, "\n");
    }
}//end function print to file
