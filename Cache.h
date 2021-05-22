//
// Created by davidelonez on 5/20/2021.
//

#ifndef UNTITLED1_CACHE_H
#define UNTITLED1_CACHE_H

typedef struct cache_unit {
    int tag=-1;
    bool dirty=0;
    int last_access=0;
    unsigned long int address =0;
}cache_unit;
class Cache {
public:
    Cache(int size_l, int block_size_l, int ways_l, int num_of_cycle,bool write_allocate);
    int size, block_size, ways, num_of_cycle,rows;
    int size_l, block_size_l, ways_l;
    int num_of_access = 0 , num_of_miss =0 ;
    int time_counter = 0;
    bool write_allocate;
    cache_unit[][];//TODO check
    ~Cache();

    bool ExecuteCommand(char op, unsigned long int addr);
    unsigned long int GetBlockFromUnderMem(unsigned long int addr);
    bool Evict(unsigned long int addr);
    int GetNumOfAccess();
    int GetNumOfMiss();
    bool GetDirty(unsigned long int addr);
    bool DirtyTheBlock(unsigned long int addr);
    bool FindBlock(unsigned long int addr);




};

#endif //UNTITLED1_CACHE_H
