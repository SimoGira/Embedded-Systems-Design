#include "global.hh"

#include <iostream>
#include <math.h>
#include <sstream>
#include <climits>
#include <bitset>
#include <cstring>

sca_tdf::sca_signal< double >  valve_aperture;
sc_core::sc_signal< double >   water_level;
sc_core::sc_signal< uint32_t > a_threshold_0;
sc_core::sc_signal< uint32_t > a_threshold_1;
