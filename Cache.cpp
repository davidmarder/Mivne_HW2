//
// Created by davidelonez on 5/20/2021.
//

#include "Cache.h"
#include <math.h>

Cache::Cache(int size_l, int block_size_l, int ways_l, int num_of_cycle,bool write_allocate){
    size_l=size_l;
    block_size_l=block_size_l;
    ways_l=ways_l;
    size=pow(2,size_l);//build pow function
    block_size=pow(2,block_size_l);
    ways=pow(2,ways_l);
    num_of_cycle=num_of_cycle;
    write_allocate=write_allocate;
    rows=(size/(block_size*ways);
    cache_unit matrix=new cache_unit[rows];
    for (int i=0;i<rows;i++){
        matrix[i]=new cache_unit[ways];
        }
}
Cache::~Cache(){
    for(int i=0;i<rows;i++){
        delete [] matrix[i];
    }
    delete [] matrix;
}
/// Transfer the Cpu command to the cache. Num of access is incremented. If the Cache succed to execute the command by himself, it returns true.
/// If it doesn't it return false and num of miss is incremented
bool Cache::ExecuteCommand(char op, unsigned long int addr) {
    num_of_access++;
    time_counter++;
    int tag = calc_tag(addr);//TODO
    int set = calc_set(addr);//TODO
    for (int i = 0; i < ways; i++) {
        if (matrix[set][i].tag == tag) {
            matrix[set][i].last_access = time_counter;
            //TODO enum
            if (op== WRITE){
                matrix[set][i].dirty = 1;
            }
            return true;
        }
    }
    num_of_miss++;
    return false;
}
/// Called when we want to get the block from the under level memory. return 1 if there was space for the new block (in this case the block is put in the cache).
/// If there was no space for the new block, return the address of the block that has to be removed.(in this cache the block is not put in the cache)
unsigned long int Cache::GetBlockFromUnderMem(unsigned long int addr){//When L2 remove someone, we will call L1.remove(address) in mem file
    int res=1;
    int tag = calc_tag(addr);
    int set = calc_set(addr);
    int LRU_index,min_time=matrix[set][0].last_access;
    for(int i=0; i<ways;i++){
        if (matrix[set][i].tag==-1){
            matrix[set][i].tag=tag;
            matrix[set][i].last_access=time_counter;
            matrix[set][LRU_index].address = addr;
            return res;//return 1 if update didn't erase anything important in the cache
        }
        if(matrix[set][i].last_access < min_time && matrix[set][i].tag!=-1){
            min_time=matrix[set][i].last_access;
            LRU_index=i;
        }
    }
    cout << " no empty slots during update" << endl;
    //No empty slots find
    res = matrix[set][LRU_index].address;
    return res;
    }
}
/// Remove a block from the cache by the address given
///Return true if evicted is done
/// Return false if we didn't find the block to erase
bool Cache::Evict(unsigned long int addr)
{
    int tag = calc_tag(addr);
    int set = calc_set(addr);
    for(int i=0; i<ways;i++) {
        if (matrix[set][i].tag == tag) {
            matrix[set][i].tag = -1;
            matrix[set][i].last_access = 0;
            matrix[set][i].dirty = 0;
            return true;
        }
    }
    return false;
}
bool Cache::GetDirty(unsigned long int addr){
    int tag = calc_tag(addr);
    int set = calc_set(addr);
    for(int i=0; i<ways;i++) {
        if (matrix[set][i].tag == tag) {
            return matrix[set][i].dirty;
        }
    }
    return false; ///Should not get here
}

bool Cache::DirtyTheBlock(unsigned long int addr){
    int tag = calc_tag(addr);
    int set = calc_set(addr);
    for(int i=0; i<ways;i++) {
        if (matrix[set][i].tag == tag) {
            matrix[set][i].dirty = 1;
        }
    }
    return true;
}


int Cache::calc_set(unsigned long int addr) {
    int offset_bits = block_size_l;
    int set_bits = log2(rows);
    unsigned long int val= pow(2, set_bits+offset_bits);
    unsigned long int mask = val - 1;
    unsigned long int tmp=pow(2, offset_bits);
    unsigned long int res = (mask & addr) / tmp;
    return res;
}

int Cache::calc_tag(unsigned long int addr){
    int offset_bits = block_size_l;
    int set_bits= log2(rows);
    unsigned long int val = pow(2,(set_bits+offset_bits));
    unsigned long int mask=(0xffffffff-(val-1);
    unsigned long int res=(mask & addr)/val));
    return res;
}
