
#ifndef _ROLLBUFF_H_
#define _ROLLBUFF_H_


class RollBuff{

private:
    int     first;
    int     last;
    int     rollBack;
    static  const int ROLL_BUF_SIZE =4097;
    char buff[ROLL_BUF_SIZE];

public:
    RollBuff();

    /* reset */
    void ReInit();

    /* get the empty size of rollBuff */
    int GetEmptySize();

    /* Get the buff size of the data filled */
    int GetFilledSize();

    /* Get the perfect point of buffer, according to blocksize */
    char *GetPerfectReadBufPtr( int blockSize, int * rollBackPos );

    /* Get the point of buffer, to write */
    char *GetPerfectWriteBufPtr( int *writeSpan );

    /* set the lastIndex */
    void SetLastIndex( int last );

    /* update the lastIndex */
    void UpdateLastIndex( int incLast );

    /* trigger the rollback */
    void ReadTriggerRollBack();

    /* update the first index */
    void UpdateFirstIndex( int incStep );

    /* get first index */
    int GetFirstIndex();

    /* get last index */
    int GetLastIndex();
};








#endif
