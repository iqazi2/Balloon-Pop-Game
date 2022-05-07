// file:  bpgame.c
/* 'Author: Sohum Bhole, Ibrahim Qazi UIC, Fall 'Bhole and Qazi - Functions that ensure an effortless user experience while playing Balloon Pop Game.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bpgame.h"

/** #include statements... **/


/** TYPEDEFS / STRUCTs HERE ***/
//typedef struct bpgame BPGame;


struct bpgame {
   char ** board;
   int nrows;
   int ncols;
   int points;
   int numSpaces;
   struct bpgame *next;
   // Variables go inside struct (no functions)
   // YOU DECIDE WHAT TO PUT IN HERE TO CAPTURE THE STATE OF
   //   A "RUN" OF THE GAME.....

};



void push_list(BPGame** head, BPGame *b)
{
    BPGame* node = (BPGame*)malloc(sizeof(BPGame));
    node->points = b->points;
    node->nrows = b->nrows;
    node->ncols = b->ncols;
    node->numSpaces = b->numSpaces;
    node->board = (char**)malloc(b->nrows*sizeof(char*));

    for(int i=0; i<b->nrows; i++)
    {
        node->board[i] = (char*) malloc(sizeof(char)*b->ncols);
    }
    for(int i=0; i<b->nrows; i++)
    {
        for(int j=0; j<b->ncols; j++)
        {
            node->board[i][j] = b->board[i][j];
        }
    }
    
    node->next = *head;
    *head = node;
}
 

void pop_list(BPGame** headRef)
{
    if (*headRef != NULL) {
 
        BPGame* head = *headRef;
        (*headRef) = (*headRef)->next;
        
        for (int k = 0; k < head->nrows; k++)
        {
            free (head->board[k]);
        }
     
        free(head);
    }
 }



/*** IMPLEMENTATION OF bp_XXXX FUNCTIONS HERE  ****/

BPGame *bp_create(int nrows, int ncols)
{
   if (nrows > MAX_ROWS || ncols > MAX_COLS)
   {
       fprintf(stderr, "ERROR");
       return NULL;
   }

   char ** board;
   BPGame *boardPointer = malloc(sizeof(BPGame));

   board = (char**) malloc(nrows*sizeof(char*));

   for(int i=0; i<nrows; i++)
   {
       board[i] = (char*) malloc(sizeof(char)*ncols);
   }

   srand(time(NULL));

   for (int i=0; i<nrows; i++)
   {
       for (int j = 0; j<ncols; j++)
       {
           int determiner = (rand() % 4);
           if (determiner == 0)
           {
               board[i][j] = Red;
           }
           else if (determiner == 1)
           {
               board[i][j] = Blue;
           }
           else if (determiner == 2)
           {
               board[i][j] = Green;
           }
           else if (determiner == 3)
           {
               board[i][j] = Yellow;
           }
       }
   }

   boardPointer->board = board;
   boardPointer->nrows = nrows;
   boardPointer->ncols = ncols;
   boardPointer->numSpaces = 0;
   boardPointer->points = 0;
    boardPointer->next = NULL;

   return boardPointer;
}

BPGame * bp_create_from_mtx(char mtx[][MAX_COLS], int nrows, int ncols)
{
   if (nrows > MAX_ROWS || ncols > MAX_COLS)
   {
       fprintf(stderr, "ERROR");
       return NULL;
   }

   char ** board;
   BPGame *boardPointer = malloc(sizeof(BPGame));

   board = (char**) malloc(nrows*sizeof(char*));

   for(int i=0; i<nrows; i++)
   {
       board[i] = (char*) malloc(sizeof(char)*ncols);
   }

   for (int i=0; i<nrows; i++)
   {
       for (int j = 0; j<ncols; j++)
       {
           if (mtx[i][j] != Red && mtx[i][j] != Blue && mtx[i][j] != Green && mtx[i][j] != Yellow && mtx[i][j] != None)
           {
               fprintf(stderr, "ERROR..");
               return NULL;
           }
           else
           {
               board[i][j] = mtx[i][j];
           }
       }
   }
   boardPointer->board = board;
   boardPointer->nrows = nrows;
   boardPointer->ncols = ncols;
    boardPointer->numSpaces = 0;
    boardPointer->points = 0;
    boardPointer->next = NULL;
    
   return boardPointer;
}

extern void bp_destroy(BPGame * b)
{
    while(b->next != NULL){
        pop_list(&b);
    }
}

extern void bp_display(BPGame * b)
{
   int i;
   int j;
   printf("  +");
   for (i = 0; i < (b->ncols*2)+1; i++)
   {
       printf("-");
   }
   printf("+\n");

   for (i=0; i<b->nrows; i++)
   {
       if (i<10)
       {
           printf("%i |", i);
       }
       else
       {
           printf("%i|", i);
       }
       for (j=0; j<b->ncols; j++)
       {
           printf(" %c", b->board[i][j]);
       }
       printf(" |\n");
   }

   printf("  +");
   for (i = 0; i < (b->ncols*2)+1; i++)
   {
       printf("-");
   }
   printf("+\n");
   printf("    ");
   for (j = 0; j < b->ncols; j++)
   {
       printf("%i ",j);
   }
   printf("\n");
}

//
// CALCULATE BALLOONS POPPED
int amountPopped(BPGame * b) {
   int spacesOnBoard = b->numSpaces;
   int spacesFound = 0;

   for (int i = 0; i < b->nrows; ++i) {
       for (int j = 0; j < b->ncols; ++j) {
           if (b->board[i][j] == '.') {
               spacesFound++;
           }
       }
   }
   b->numSpaces = spacesFound;
   return (spacesFound - spacesOnBoard);
}





//
// POP
int valid_bp_pop(BPGame * b, int r, int c ) {
   if(r>-1 && r<b->nrows ){
       if(c>-1 && c<b->ncols ){
           return 1;
       }
   }
   return 0;
}

void pop(BPGame * b, int r, int c){
    
   char Ballon;
   int check = 0;
   Ballon = bp_get_balloon(b, r, c);

   check=valid_bp_pop(b,r+1,c);
   if(check == 1){
       if(Ballon == b->board[r+1][c]){
           b->board[r][c] = '.';
           pop(b, r+1, c);
           b->board[r+1][c] = '.';
       }
   }

   check=valid_bp_pop(b,r-1,c);
   if(check == 1){
       if(Ballon == b->board[r-1][c]){
           b->board[r][c] = '.';
           pop(b, r-1, c);
           b->board[r-1][c] = '.';
       }
   }

   check=valid_bp_pop(b,r,c+1);
   if(check == 1){
       if(Ballon == b->board[r][c+1]){
           b->board[r][c] = '.';
           pop(b, r, c+1);
           b->board[r][c+1] = '.';
       }
   }

   check=valid_bp_pop(b,r,c-1);
   if(check == 1){
       if(Ballon == b->board[r][c-1]){
           b->board[r][c] = '.';
           pop(b, r, c-1);
           b->board[r][c-1] = '.';
       }
   }
}


extern int bp_pop(BPGame * b, int r, int c) {
   if(r > b->nrows || c > b->ncols || b->board[r][c] == None){
       return 0;
   }
    char Ballon;
   int check = 0;
   Ballon = bp_get_balloon(b, r, c);

   check=valid_bp_pop(b,r+1,c);
   if(check == 1){
       if(Ballon == b->board[r+1][c]){
           // printf("need to pop up\n");
           b->board[r][c] = '.';
           pop(b, r+1, c);
           b->board[r+1][c] = '.';
       }
   }

   check=valid_bp_pop(b,r-1,c);
   if(check == 1){
       if(Ballon == b->board[r-1][c]){
           //printf("need to pop down\n");
           b->board[r][c] = '.';
           pop(b, r-1, c);
           b->board[r-1][c] = '.';
       }
   }

   check=valid_bp_pop(b,r,c+1);
   if(check == 1){
       if(Ballon == b->board[r][c+1]){
           //printf("need to pop right\n");
           b->board[r][c] = '.';
           pop(b, r, c+1);
           b->board[r][c+1] = '.';
       }
   }

   check=valid_bp_pop(b,r,c-1);
   if(check == 1){
       if(Ballon == b->board[r][c-1]){
           //printf("need to pop left\n");
           b->board[r][c] = '.';
           pop(b, r, c-1);
           b->board[r][c-1] = '.';
       }
   }

    int balloonsPopped = amountPopped(b);
    b->points += balloonsPopped*(balloonsPopped-1);
   return balloonsPopped;
}


extern int bp_is_compact(BPGame * b)
{
   for (int i = 1; i < b->nrows; i++) {
       for (int j = 0; j < b->ncols; j++) {
           if (b->board[i-1][j]==None && b->board[i][j]!=None){
               return 0;
           }
       }
   }
   return 1;
}

void bp_float_one_step(BPGame * b)
{
   char temp;
   for (int i = 1; i < b->nrows; i++)
   {
       for (int j = 0; j < b->ncols; j++)
       {
           if (b->board[i-1][j]==None && b->board[i][j]!=None)
           {
               temp = b->board[i][j];
               b->board[i][j] = None;
               b->board[i-1][j] = temp;
           }
       }
   }
}


extern int bp_score(BPGame * b)
{
   return b->points;
}


extern int bp_get_balloon(BPGame * b, int r, int c)
{

   if(r > b->nrows || c > b->ncols)
   {
       return -1;
   }
    
    if(b->board[r][c] == None)
    {
        return None;
    }
    else if(b->board[r][c] == Red)
    {
        return Red;
    }
    else if(b->board[r][c] == Blue)
    {
        return Blue;
    }
    else if(b->board[r][c] == Yellow)
    {
        return Yellow;
    }
    else if(b->board[r][c] == Green)
    {
        return Green;
    }
   return -1;
}

extern int bp_can_pop(BPGame * b)
{
   int i,j;
   for (i=0;i<b->nrows;i++)
   {
       for (j=0;j<b->ncols;j++)
       {
           if (b->board[i][j] == None)
           {
               continue;
           }
           if ((i-1)>=0 && b->board[i][j] == b->board[i-1][j])
           {
               return 1;
           }
           else if ((i+1)<b->nrows && b->board[i][j] == b->board[i+1][j])
           {
               return 1;
           }
           else if ((j-1)>=0 && b->board[i][j] == b->board[i][j-1])
           {
               return 1;
           }
           else if ((j+1)<b->ncols && b->board[i][j] == b->board[i][j+1])
           {
               return 1;
           }
       }
   }
   return 0;
}

extern int bp_undo(BPGame * b)
{
    if(b->next == NULL){
        return 0;
    }
    pop_list(&b);

   return 1;
}

void bp_display_STD(BPGame *b) {

   for (int i =0; i<b->nrows; i++)
   {
       for (int j =0; j<b->ncols; j++) {
           printf("%c", b->board[i][j]);
       }
       printf("\n");
   }
}
