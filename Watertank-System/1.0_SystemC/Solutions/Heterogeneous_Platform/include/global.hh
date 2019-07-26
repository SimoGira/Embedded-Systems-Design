#ifndef GLOBAL_HH
#define GLOBAL_HH

#include <systemc-ams.h>

struct iostruct {
    bool        mode;
    double      word0;                  // threshold
    int         word1;                  // command
    sc_uint<32> k0, k1, k2, k3;
    double      result0;                // encrypted/decrypted threshold
    int         result1;                // encrypted/decrypted command
};

union type_splitter {
    double a_threshold;
    uint32_t a_threshold_me[2];
};

typedef enum { IDLE, OPEN, CLOSE } command_type;

extern sca_tdf::sca_signal< double >  valve_aperture;
extern sc_core::sc_signal< double >   water_level;
extern sc_core::sc_signal< uint32_t > a_threshold_0;
extern sc_core::sc_signal< uint32_t > a_threshold_1;


#endif
