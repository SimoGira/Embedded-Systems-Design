#ifndef __define_AT4_h__
#define __define_AT4_h__

#include <systemc.h>

struct iostruct {
    bool mode;
    sc_uint<32> word0, word1;
    sc_uint<32> k0, k1, k2, k3;
    sc_uint<32> result0, result1;
};

#define ADDRESS_TYPE int    // utile in casi di lettura da indirizzo di memoria
#define DATA_TYPE iostruct

#endif
