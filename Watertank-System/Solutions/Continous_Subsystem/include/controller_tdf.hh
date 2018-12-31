#ifndef CONTROLLER_TDF_HH
#define CONTROLLER_TDF_HH

#include "systemc.h"
#include "systemc-ams.h"
#include "global.hh"
#include "params.hh"

SCA_TDF_MODULE(controller_tdf) {

    // port declarations
    sca_tdf::sca_in<double> water_level_in;

    sca_tdf::sca_out<int> command_out;         // input for valve
    sca_tdf::sca_out<double> aperture_threshold_out;    // input for valve

    //sca_tdf::sca_signal<double> current_threshold;
    double current_threshold;

    // module and ports attributes
    void set_attributes();

    // initial values of ports with a delay
    void initialize();

    // time-domain signal behavior or algorithm
    void processing();

    SCA_CTOR(controller_tdf) {
        // set_attributes();    <-- vengono chiatate automaticamente dal kernel di simulazione
        // initialize();
    }
};

#endif // CONTROLLER_TDF_HH
