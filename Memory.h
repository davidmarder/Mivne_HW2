//
// Created by davidelonez on 5/22/2021.
//

#ifndef MAIN_CPP_MEMORY_H
#define MAIN_CPP_MEMORY_H

#include <cmath>
#include <iostream>
#include "Cache.h"

class Memory {
public:
    Memory(int MemCyc, int Bsize, int WrAlloc,int L1Size,int L1Assoc,int L1Cyc,int L2Size,
           int L2Assoc,int L2Cyc);
    void execute(char op, int addr);

    double GetL1MissRate() const;
    double GetL2MissRate() const;
    double GetAvgAccTime() const;
    void stats();
private:
    int Bsize=0;
    int L2Size=0;
    int L1Cyc=0;
    int L1Size=0;
    int L1Assoc=0;
    int L2Assoc=0;
    int L2Cyc=0;
    int WrAlloc=0;
    int MemCyc=0;

    int num_access_to_memory=0;

    Cache L1;
    Cache L2;
};



#endif //MAIN_CPP_MEMORY_H
