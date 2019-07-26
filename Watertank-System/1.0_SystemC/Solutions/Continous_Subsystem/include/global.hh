#ifndef GLOBAL_HH
#define GLOBAL_HH

#include <systemc-ams.h>

typedef enum { IDLE, OPEN, CLOSE } command_type;


//extern sc_core::sc_signal< double > valve_aperture;
extern sca_tdf::sca_signal< double > valve_aperture;
extern sca_tdf::sca_signal< double > water_level;
//extern sc_core::sc_signal< double > a_threshold;


#endif
