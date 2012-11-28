
#include "RollBuff.h"
#include <stddef.h>
#include "ESLog.h"

/* constructor */
RollBuff::RollBuff(){
    first = 0; // the last read position, next is first+1;
    last = 1; //the next write poistion 
    rollBack = this->ROLL_BUF_SIZE;
}

/* reset */
void RollBuff::ReInit(){

    this->first = 0; 
    this->last = 1; 
    this->rollBack = this->ROLL_BUF_SIZE;
}

/* get the empty size of rollBuff */
int RollBuff::GetEmptySize(){

    //condition start
    if( this->first == this->last  ) {
      //return this->ROLL_BUF_SIZE;
      return 0;
    }
    else if( this->first > this->last ) {
        return this->first-this->last; //qu diao 1
    }
    else{
        return this->ROLL_BUF_SIZE - this->last + this->first; //qu diao 1
    }
}

/* Get the buff size of the data filled */
int RollBuff::GetFilledSize(){

    //condition start 
    if( this->last == this->first) {
        //return 0;
        //return this->ROLL_BUF_SIZE;
        this->rollBack-this->first+this->last-1;
    }
    else if( this->last > this->first ) {
        return this->last-this->first-1;
    }
    else{
        return this->rollBack - this->first + this->last-1;
    }
}


/* Get the perfect directive span buffer ptr of roll , according the blocksize */
char *RollBuff::GetPerfectReadBufPtr( int blockSize, int *rollBackPos ){//empty size 

    //ESLog::ISLog("GET PERFECT READ BUF PTR START\n", __FILE__, __LINE__);

    // condition two
    if( this->first == this->last ) {
        //if( this->ROLL_BUF_SIZE -1 < blockSize ) {
        //    return NULL;
        //}

        //return &(this->buff[this->last+1]);
        //ESLog::ISLog("roll Buff error!\n", __FILE__, __LINE__);
        return NULL;
    }
    else if( this->first < this->last ){
        // first
        if( this->ROLL_BUF_SIZE - this->last < blockSize ) {

            // secondary
            if( this->first < blockSize ) { // qu diao +1
                return NULL;
            }

            *rollBackPos = this->last;  // the rollback positon have not write
            return &(this->buff[0]); // rollback. remember the point .
        }

        return &(this->buff[this->last]);
    }
    else{
        if( this->first-this->last < blockSize ) { //qu diao +1
            return NULL;
        }

        return &(this->buff[this->last]);
    }

     //ESLog::ISLog("GET PERFECT READ BUF PTR END\n", __FILE__, __LINE__);

    return NULL;
}


/* Get the perfect buffer ptr of roll, to write */
char *RollBuff::GetPerfectWriteBufPtr( int *writeSpan ){ //filled size
    
    // start point 
    if( this->first == this->last ) {
        //writeSpan = 0;
        //return NULL;
        //*writeSpan = this->rollBack-this->first-1;
        //return &(this->buff[this->first+1]);
        //ESLog::ISLog("roll buff error!\n", __FILE__, __LINE__ );
        *writeSpan = this->rollBack-this->first-1;
        if( *writeSpan == 0 ) { //rollback
            *writeSpan = this->last;
            return &(buff[0]);
        }
        return &(buff[this->first+1]);
    }
    else if( this->first < this->last ) {
        *writeSpan = this->last - this->first-1 ;
        if(*writeSpan == 0 ) {
            return NULL;
        }
        return  &(this->buff[this->first+1]);                                               
    }
    else{
        
        if( this->first +1 == this->rollBack ) { //rollback
            *writeSpan = this->last;
            return &(this->buff[0]); 
        }

        *writeSpan = this->rollBack-this->first-1;
        return &(this->buff[this->first+1]); 
    }
}


/* update the last index */
void RollBuff::UpdateLastIndex( int incStep )
{
    

    if( this->rollBack == this->last  ) {
        this->last = incStep;
        return;
        //this->last = 0;
    }

    this->last += incStep;

   
}

/* trigger the rollBack */
void RollBuff::ReadTriggerRollBack(){
    this->rollBack = this->last;
}


/* update the first index */
void RollBuff::UpdateFirstIndex( int incStep ){

    

    if( this->first+1 == this->rollBack  ) {
        //this->first = incStep-1;
        //this->first = incStep-1;
        //this->first = 0;
        this->first = incStep -1;
        return;
    }

    this->first += incStep;

}

/* get first index */
int RollBuff::GetFirstIndex(){
    return this->first;
}

/* get last index */
int RollBuff::GetLastIndex(){
    return this->last;
}
