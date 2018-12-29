/*        Queue Abstract Data Type Model
*    Able to hold any number of 64 bits simultaneously,
*    not constricted to type of 64 bit, but it cannot be
*    more than 64 bit. Implements given compare function to
*    compare elements in the array of contents which is 
*    inserted by the client. 
*
*    @author Carson Bloomingdale | ceb3745
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <assert.h>

#define	QUEUE_ALLOC_UNIT	8

typedef struct queueStruct {
    void **contents;
    size_t capacity;
    size_t num;
    int (*type)();
} *QueueADT;

#define _QUEUE_IMPL_

#include "queueADT.h"

//Takes in the arguement: function pointer and returns a pointer to the created Queue
QueueADT que_create(  int (*cmp)(const void * a, const void * b)  ) {
    QueueADT new;
    if (*cmp == NULL){
        new = (QueueADT) malloc( sizeof(struct queueStruct) );
	      if( new != 0 ) {
  		      new->contents = 0;
  	  	    new->capacity = 0;
	    	    new->num = 0;
            new->type = NULL;
	      }
        return( new );
    }else{
        new = (QueueADT) malloc( sizeof(struct queueStruct) );
	      if( new != 0 ) {
  		      new->contents = 0;
  	  	    new->capacity = 0;
	    	    new->num = 0;
            new->type = cmp;
	      }
        return( new );
    }
}

//Takes in the arguement: QueueADT and frees the contents of the que and the que simultaneously
void que_destroy( QueueADT que ) {
	  assert( que != 0 );

	  if( que->contents != 0 ) {
	  	  free( que->contents );
	  }

	  free( que );
}

//Takes in the arguement: QueueADT and frees the que contents and resets it to 0 for future use
//then resets the parameters num and capacity.
void que_clear( QueueADT que ) {
	  if( que->contents != 0 ) {
		    free( que->contents );
		    que->contents = 0;
	  }
	  que->capacity = 0;
	  que->num = 0;
}

//Takes in the arguement: QueueADT and the data to be added. Then determines
//if the compare function exists and implements FIFO else. 
void que_insert( QueueADT que, void *data ) {
    int compare = 1;
    int n = que->num;
	  if (que->type != NULL){
        if( que->contents == 0 ) {
    	    	que->contents = malloc( sizeof(void *) * QUEUE_ALLOC_UNIT );
    	    	assert( que->contents != 0 );
    	    	que->capacity = QUEUE_ALLOC_UNIT;
      	}
      	if( que->num >= que->capacity ) {
    	    	void *tmp;
    	    	tmp = realloc( que->contents,
    	    	  	sizeof(void *) * (que->capacity + QUEUE_ALLOC_UNIT) );
    	    	assert( tmp != 0 );
    	    	que->contents = tmp;
    	    	que->capacity += QUEUE_ALLOC_UNIT;
        }
        if (n != 0){
            while(true){
                if(n==0){
                    for(int i=(int)(que->num); i>n ;i--){
                        que->contents[i] = que->contents[i-1];
                    }
                    que->contents[n] = data;
                    que->num += 1;
                    break;
                }
                compare = que->type(que->contents[n-1], data);
                if( compare <= 0){
                    for(int i=(int)(que->num); i>n-1 ;i--){
                        que->contents[i] = que->contents[i-1];
                    }
                    que->contents[n] = data;
                    que->num += 1;
                    break;
                }else{
                    n = ( n-1 );
                }
            }
        }
        else{
            que->contents[que->num] = data;
            que->num += 1;
        }
   }else{
       if( que->contents == 0 ) {
           que->contents = malloc( sizeof(void *) * QUEUE_ALLOC_UNIT );
           assert( que->contents != 0 );
           que->capacity = QUEUE_ALLOC_UNIT;
       }
       if( que->num >= que->capacity ) {
           void *tmp;
           tmp = realloc( que->contents,
               sizeof(void *) * (que->capacity + QUEUE_ALLOC_UNIT) );
           assert( tmp != 0 );
           que->contents = tmp;
           que->capacity += QUEUE_ALLOC_UNIT;
       }
         
       que->contents[que->num] = data;
       que->num += 1;
   }

}

//Takes in the QueueADT and removes the first element from the array of contents
//that it holds, by moving everything over one to the left. 
void *que_remove( QueueADT que ) {
  	void *data;

  	assert( que->num > 0 );
   
    data = que->contents[0];
    
    for(int i=0;i<(int)(que->num - 1);i++){
        que->contents[i] = que->contents[i+1];
    }

	  que->num -= 1;
  
  	return( data );
}

void *get(QueueADT que, int index){
    void *data;

  	assert( que->num > 0 );
   
    data = que->contents[index];
  
  	return( data );
}

//returns whether or not the number of queue elements equals 0 which would make
//the queue empty
bool que_empty( QueueADT que ) {
	  return( que->num == 0 );
}

int getnum(QueueADT que){
    return(que->num);
}
