#ifndef VALVE_TDF_HH
#define VALVE_TDF_HH

#include "systemc.h"
#include "systemc-ams.h"
#include "global.hh"

SCA_TDF_MODULE(valve_tdf) {

    // port declarations
    sca_tdf::sca_in<int> command_in;
    sca_tdf::sca_in<double> aperture_threshold_in;

    sca_tdf::sca_out<double> valve_aperture_out;    // input for watertank

    SCA_CTOR(valve_tdf) {
        // set_attributes();
        // initialize();
    }

    double a_threshold;

    // module and ports attributes
    void set_attributes();

    // initial values of ports with a delay
    void initialize();

    // time-domain signal behavior or algorithm
    void processing();
};

#endif
