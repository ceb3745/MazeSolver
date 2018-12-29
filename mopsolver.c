/*   Finding a path to the end of a maze input
*    and finding the shortest path to complete
*    the maze in the shortest time possible.
*      @author  Carson Bloomingdale | ceb3745
*/

#define BUF 256
#define TOT 1024

#include<stdio.h>
#include<ctype.h>
#include<unistd.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include<errno.h>
#include<limits.h>

#include "queueADT.h"

extern int errno;

typedef struct qnode_s {   
   	size_t x;     // using separate x/y coordinates
	  size_t y;
    int value; 
} * QNode;

typedef struct pathStruct{
    QNode **pathway;
} * pathPoint;

int sizePub;

//Function to find the node in the board given the row and column indices.
QNode findNode(QNode board[], int row, int col, int size){
    for(int i=0; i<size;i++){
        if( board[i]->x == row && board[i]->y == col){
            return board[i];        
        }
    }
    return NULL;
  
}

//Function to find the index of such node in the board array given the
//node itself and the board to search. 
int getIndex(QNode board[], QNode node, int size){
    for(int i=0; i<size;i++){
        if( board[i]->x == node->x && board[i]->y == node->y){
            return i;        
        }
    }
    return 0;
  
}

//Take in the board and the node on the board and return a queue of the 
//found neighbors. Find them using the cells up, down, left, and right of
//the root node. 
QueueADT findNeighbors(QNode node, QNode board[], bool seen[] , int size){
    int leftNum = 0;
    int rightNum = 0;
    int upNum = 0;
    int downNum = 0;
    int leftIndex = -1;
    int rightIndex = -1;
    int upIndex = -1;
    int downIndex = -1;
    int neighborCount = 0;
    QueueADT neighbors = que_create(NULL);
    QNode left, right, up, down;
    left = findNode(board, (node->x)-1, (node->y), size);
    if(left != NULL){
        leftIndex = getIndex(board, left, size);
    }
    right = findNode(board, (node->x)+1, (node->y), size);
    if( right != NULL){
        rightIndex = getIndex(board, right, size);
    }
    up = findNode(board, (node->x), (node->y)-1, size);
    if( up != NULL){
        upIndex = getIndex(board, up, size);
    }
    down = findNode(board, (node->x), (node->y)+1, size);
    if( down != NULL){
        downIndex = getIndex(board, down, size);
    }
    for( int i=0; i < size; i++){
        if(leftIndex != -1){
            if(leftIndex == i){
                if(seen[i] == 1){
                    leftNum++;
                }
            }
        }
        if(rightIndex != -1){
            if(rightIndex == i){
                if(seen[i] == 1){
                    rightNum++;
                }
            }
        }
        if(upIndex != -1){
            if(upIndex == i){
                if(seen[i] == 1){
                    upNum++;
                }
            }
        }
        if(downIndex != -1){
            if(downIndex == i){
                if(seen[i] == 1){
                    downNum++;
                }
            }
        }
    }
    if(down != NULL){
        if(downNum == 0 && down->value == 0){
             que_insert( neighbors, (void *) down);
             neighborCount++;
        }
    }
    if(right != NULL){
        if(rightNum == 0 && right->value == 0){
             que_insert( neighbors, (void *) right);
             neighborCount++;
        }
    }
    if(left != NULL){
        if(leftNum == 0 && left->value == 0){
            que_insert( neighbors, (void *) left);
            neighborCount++;
        }
    }
    if(up != NULL){
        if(upNum == 0 && up->value == 0){
             que_insert( neighbors, (void *) up);
             neighborCount++;
        }
    }
    
    if(neighborCount == 0){
        return NULL;
    }
    
    return neighbors;
    
    que_destroy(neighbors);
    
}

//Finds the value given inside path x and returns it's indice else return -1
int findVal(int path[], int size, int val){
    int index = -1;
    for( int i=0; i<size; i++){
        if( path[i] == val ){
            index = i;
        }
    } 
    return index;
}

//Prints out the solution path to defined output given the dimensions, mainBoard 
//and path on the board
void solPrint(QNode mainBoard[], int xSize, int ySize, FILE* out, int path[]){
    size_t xnum;
    size_t ynum;
    bool sol = 1;
    int intxnum;
    int intynum;
    int count = 0;
    int boardCount = 0;
    int boardSize = (xSize * ySize);
    int j=0;
    for(int i=0; i<((xSize*2)+3); i++){
        fprintf(out,"O");
    }
    printf("\n");
    for(int i=0;i<ySize;i++){
        if(i == 0){
            fprintf(out, "+ ");
        }
        else{
            fprintf(out, "O ");
        }
        for( int i=0; i<xSize; i++){
            if(mainBoard[boardCount]->value == 1){
                fprintf(out,"O ");
            }
            else if(path[boardCount] > 0){
                fprintf(out,"+ ");
            }
            else{
                fprintf(out,"  ");
            }
            count++;
            boardCount++;
            
        }
        if( sol == 0){
            if(mainBoard[i]->value == 1){
                fprintf(out,"O ");
            }
            else{
                fprintf(out,"  ");
            }
        }
        if(i == ySize - 1){
            fprintf(out,"+");
        }
        else{
            fprintf(out,"O");
        }
        fprintf(out,"\n");
    }
    for(int i=0; i<((xSize*2)+3); i++){
        fprintf(out,"O");
    }
    fprintf(out,"\n");
}

//use the path given to print out whether the solution if found dependant
// on the solution bool and print out the final path dependant on another bool
void findsolution(int path[], QNode board[], int size, bool solution,
                     bool pathway, int xSize, int ySize, FILE* out, bool found){
    int index = 0;
    int length = path[size-1];
    int pathcount = 0;
    
    
    if(solution == 1){
        if(found == 0){
            fprintf(out, "No solution.\n");
        }else{ 
            fprintf(out, "Solution in %i steps.\n", length);
        }
    }
    if(pathway == 1){
        solPrint(board,xSize, ySize, out, path);
    }
}

//find the pathway that needed to take by searching possible neighbor paths and discluding
// anything previously visited by using the seen[] array to check values. 
QueueADT findplace(QNode curr, QNode board[], bool seen[], int size, int path[],
                   int count, bool solution, bool pathway, int xSize, int ySize, FILE* out, int indexofbreak){
    seen[getIndex(board, curr, size)] = 1;
    path[getIndex(board, curr, size)] = count;
    count++;
    bool found = 0;
    QueueADT neighbors;
    int index;
    int sizeofbad;
    neighbors = (QueueADT) malloc(sizeof(*neighbors)); 
    
    if(curr == board[size -1]){
        found = 1;
        findsolution(path, board, size, solution, pathway, xSize, ySize, out, found);       
    }

    
    neighbors = findNeighbors(curr, board, seen, size);
    if(neighbors != NULL){
        if(getnum(neighbors) == 1){
            QNode neighbor;
            neighbor = (QNode) get( neighbors, 0 );
            index = -1;
            findplace(neighbor, board, seen, size, path, count, solution, pathway, xSize, ySize, out, index);
        }
        for( int i = 0; i < getnum(neighbors); i++){
            QNode neighbor;
            neighbor = (QNode) get( neighbors, i );
            index = count;
            findplace(neighbor, board, seen, size, path, count, solution, pathway, xSize, ySize, out, index);
            
        }
    }
    
    return neighbors;
}

//pretty print the initial array in given format
void prettyPrint(QNode mainBoard[], int xSize, int ySize, FILE* out ){
    size_t xnum;
    size_t ynum;
    bool sol = 1;
    int intxnum;
    int intynum;
    int count = 0;
    int boardCount = 0;
    int boardSize = (xSize * ySize);
    int j=0;
    for(int i=0; i<((xSize*2)+3); i++){
        fprintf(out,"O");
    }
    printf("\n");
    for(int i=0;i<ySize;i++){
        if(i == 0){
            fprintf(out, "  ");
        }
        else{
            fprintf(out, "O ");
        }
        for( int i=0; i<xSize; i++){
            if(mainBoard[boardCount]->value == 1){
                fprintf(out,"O ");
            }
            else{
                fprintf(out,"  ");
            }
            count++;
            boardCount++;
            
        }
        if( sol == 0){
            if(mainBoard[i]->value == 1){
                fprintf(out,"O ");
            }
            else{
                fprintf(out,"  ");
            }
        }
        if(i == ySize - 1){
            fprintf(out," ");
        }
        else{
            fprintf(out,"O");
        }
        fprintf(out,"\n");
    }
    for(int i=0; i<((xSize*2)+3); i++){
        fprintf(out,"O");
    }
    fprintf(out,"\n");
}

//print the initial matrix to given output
void printmatrix(QNode mainBoard[], FILE* out, int xSize, int boardSize){
    int count = 0;
    fprintf(out, "Read This Matrix: \n");
    count = 0;
  	for( int i = 0; i < boardSize; ++i ) {
        if(count == xSize){
            fprintf(out, "\n");
            count = 0;
        }   
 		    fprintf(out, "%i ", mainBoard[i]->value );
        count++;
  	}
    fprintf(out,"\n");
}

int main(int argc, char *argv[]){

    float size;
    char line [TOT][BUF];
    int count = 1;
    int index = 0;
    QNode curr = NULL;
    char helpMsg[] = "USAGE: mopsolver [-bspmh] [-i INFILE] [-o OUTFILE]\n "
                      "Options:\n"
                      "-h Print this helpful message to stdout.\n"
                      "-b Add borders and pretty print.  (Default off.)\n"
                      "-s Print shortest solution steps. (Default off.)\n"
                      "-m Print matrix after reading.    (Default off.)\n"
                      "-p Print solution with path.      (Default off.)\n"
                      "-i INFILE Read maze from INFILE.  (Default stdin.)\n"
                      "-o OUTFILE Write maze to OUTFILE. (Default stdout.)\n";
                      
    bool pretty = 0;
    bool shortest = 0;
    bool print = 0;
    bool pathprint = 0;
    bool inFile = 0;
    char *infileName = NULL;
    bool outFile = 0;
    char *outfileName = NULL;
    int c;

    opterr = 0;
    
    //switch based on optional arguement and flags using getopt
    while ((c = getopt (argc, argv, "hbsmpi:o:")) != -1){
        switch (c)
        {
            case 'h':
                  printf("%s",helpMsg);
                  return 1;
            case 'b':
                  pretty = 1;
                  break;
            case 's':
                  shortest = 1;
                  break;
            case 'm':
                  print = 1;
                  break;
            case 'p':
                  pathprint = 1;
                  break;
            case'i':
                inFile = 1;
                infileName = optarg;
                break;
            case 'o':
                outFile = 1;
                outfileName = optarg;
                break;
            case '?':
                  if (optopt == 'i')
                    fprintf (stderr, "Option -%i requires an argument.\n", optopt);
                  else if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%i'.\n", optopt);
                  else
                    fprintf (stderr,
                             "Unknown option character `\\x%x'.\n",
                             optopt);
                  
                  if (optopt == 'o')
                    fprintf (stderr, "Option -%o requires an argument.\n", optopt);
                  else if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%o'.\n", optopt);
                  else
                    fprintf (stderr,
                             "Unknown option character `\\x%x'.\n",
                             optopt);
                  return 1;
            
            default:
              abort ();
        }
    }
    char *ptr = NULL;
    char *ptrTwo;
    QueueADT input = que_create(NULL);
    int i=0;
    long entry;
    long value;
    int intval;
    int xSize = 0;
    int ySize = 0;
    char ch;
    int counter;
    long temp;
    
    //get input based on if a inFile was defined else, use stdin
    if(inFile  == 0){
        while(ch != EOF){
             ch = fgetc(stdin);
             if( ch != ' ' ){
                 if (ch == '\n'){
                     xSize = counter;
                     counter = 0;
                     ySize++;
                 }else{
                     if(ch == '0'){
                         temp = 0;
                         que_insert(input, (void*) temp);
                         counter++;
                     }else if(ch == '1'){
                         temp = 1;
                         que_insert(input, (void*) temp);
                         counter++;
                     }
                 }
             }
        }   
    }else{
        FILE *fp = fopen(infileName, "r");
        while(ch != EOF){
             ch = fgetc(fp);
             if( ch != ' ' ){
                 if (ch == '\n'){
                     xSize = counter;
                     counter = 0;
                     ySize++;
                 }else{
                     if(ch == '0'){
                         temp = 0;
                         que_insert(input, (void*) temp);
                         counter++;
                     }else if(ch == '1'){
                         temp = 1;
                         que_insert(input, (void*) temp);
                         counter++;
                     }
                 }
             }
        }    
    }
    int boardSize = getnum(input);
    
    //define the mainBoard and malloc space for the QNodes
    QNode mainBoard[boardSize];
    for( int i=0; i<boardSize; i++){
        mainBoard[i] = malloc(sizeof(struct qnode_s));
    }
    count = 0;
    int k,l = 0;
    for(k=0; k < ySize; k++){
        for(l=0; l < xSize; l++){
            if(curr == NULL){
                curr = &mainBoard[k][l];
            }
            mainBoard[count]->x = k;
            mainBoard[count]->y = l;
            value = (long) get(input, count);
            intval = (int) value;
            mainBoard[count]->value = intval;
            count++;    
        }
    }
    
    //print the matrix dependant on the print bool
    if(print == 1){
        if(outFile == 1){
            FILE *ofp = fopen(outfileName, "w");
            printmatrix(mainBoard, ofp, xSize, boardSize);
            fclose(ofp);
        }else{
            printmatrix(mainBoard, stdout, xSize, boardSize);
        }  
    }
    
    //pretty print the matrix dependant on the pretty bool
    if(pretty == 1){
        if(outFile == 1){
            FILE *ofp = fopen(outfileName, "w");
            prettyPrint(mainBoard, xSize, ySize, ofp);
            fclose(ofp);
        }
        else{
            prettyPrint(mainBoard, xSize, ySize, stdout);
        }
    }
    //create the seen array which will contain bools parallel to nodes in
    //the mainBoard array. 
    bool seen[boardSize];
    for( int k=0; k<boardSize; k++){
        seen[k] = 0;
    }
    //create the path array with contains numbers parallel to nodes in
    //the mainBoard array that give the length of the path as current point
    int path[boardSize];
    for( int j=0; j<boardSize; j++){
        path[j] = 0;
    }
    int pathCount = 1;
    
    void* neighbors;
    //call findplace function to do work and define the ofp if an outputfile was defined
    if(outFile == 1){
        FILE *ofp = fopen(outfileName, "w");
        neighbors = findplace(mainBoard[0], mainBoard, seen, boardSize, path, pathCount, shortest, pathprint, xSize, ySize, ofp, index);
        fclose(ofp);
    }else{
        neighbors = findplace(mainBoard[0], mainBoard, seen, boardSize, path, pathCount, shortest, pathprint, xSize, ySize, stdout, index);
    }
    
    //free the mainBoard
    for( int i=0; i<boardSize; i++){
        free(mainBoard[i] );
    }
    
    free(neighbors);
    
    return 0;
}