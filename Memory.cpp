//
// Created by davidelonez on 5/22/2021.
//

#include "Memory.h"
Cache(int size_l, int block_size_l, int ways_l, int num_of_cycle,bool write_allocate);

Memory::Memory(int MemCyc, int Bsize, int WrAlloc, int L1Size, int L1Assoc, int L1Cyc, int L2Size, int L2Assoc,
               int L2Cyc): MemCyc(MemCyc),Bsize(Bsize),WrAlloc(WrAlloc),L1Size(L1Size),  L1Assoc(L1Assoc),L1Cyc(L1Cyc), L2Size(L2Size),L2Assoc(L2Assoc),L2Cyc(L2Cyc) {
    this->L1= Cache(L1Size , Bsize, L1Assoc,L1Cyc,WrAlloc);
    this->L2= Cache(L2Size , Bsize, L2Assoc,L2Cyc,WrAlloc);
}


void Memory::execute(char op, int addr) {


    if (L1.ExecuteCommand(op, addr) == TRUE)return;///L1 read/write the block, if write makes dirty
    if (op == READ || ((op == WRITE) && (write_allocate == true))) {/// We want in this case to get the block to L1
        tmp = L2.FindBlock(op, addr);
        if (tmp == TRUE) {///L2 has the block
            tmp1 = L1.GetBlockFromUnderMem(op, addr);
            if (tmp1 != 1) {///L1 need to make some space to add the new block
                if (L1.GetDirty(tmp1) == true)
                    L2.DirtyTheBlock(tmp1);///L2 update dirty bit of the block that L1 will evict THIS IS WB
                L1.evict(tmp1);///L1 make space for the new block
                L1.GetBlockFromUnderMem(op, addr);///L1 will add the new block for sure from L2
            }
        } else {///L2 has not the block
            tmp2 = L2.GetBlockFromUnderMem(op, addr);///HERE WE ACCESS TO THE MEMORY
            AccessMemory();//read
            if (tmp2 != 1) {///L2 has not enough space for a new block and add it
                if (L1.FindBlock(tmp2) == true) {/// L1 has the block that L2 wants to throw away
                    if (L1.GetDirty(tmp2) == true) {///The block that we want to throw away in L1 is dirty
                        L2.DirtyTheBlock(tmp2);///Put dirty on the block we want to throw away in L2
                    }
                    L1.Evict(tmp2);///Throw away the block in L1
                }
                if (L2.GetDirty(tmp2) == true) AccessMemory(); //write
                L2.Evict(tmp2);///Throw away the block in L2L2.Evict(tmp2);///Throw away the block in L2
                L2.GetBlockFromUnderMem(op, addr);///L2 will add the new block for sure.
            }
        }///new Block is in L2
        tmp3 = L1.GetBlockFromUnderMem(op, addr); /// if L1 has space it will add the new block
        if (tmp3 != 1) {///L1 need to make some space to add the new block
            if (L1.GetDirty(tmp3) == true)
                L2.DirtyTheBlock(tmp3);///L2 update dirty bit of the block that L1 will evict THIS IS WB
            L1.evict(tmp3);///L1 make space for the new block
            L1.GetBlockFromUnderMem(op, addr);///L1 will add the new block for sure from L2
        }
        return;
    } else {/// We don't need to get the block to L1. Here we are in the case that we want to right but the block isn't in L1 and we are in No Write Allocate
        L2.ExecuteCommand(op, addr);///If L2 has the block it will dirty it, if not we wrote directly to the memory
        ///Maybe here I have to add an access to the memory. access memory ++
        AccessMemory();//write
        return;
    }
}

double Memory::GetL1MissRate() const {
    return L1.num_of_miss / L1.num_of_access;//TODO
}

double Memory::GetL2MissRate() const {
    return L2.num_of_miss/ L2.num_of_access;//TODO
}

double Memory::GetAvgAccTime() const {
    return 0;//TODO
}