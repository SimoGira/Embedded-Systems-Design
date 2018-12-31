#ifndef SYSTEM_HH
#define SYSTEM_HH

#include <systemc-ams>
#include "global.hh"
#include "valve_tdf.hh"
#include "watertank_lsf.hh"
#include "controller_tdf.hh"


class System : public sc_core::sc_module {

public:
    System( sc_core::sc_module_name );
    ~System();

//protected:
    controller_tdf controller;
    valve_tdf valve;
    watertank_lsf watertank;


    //sc_clock clock( "clock", 0.5, SC_SEC );

    sca_tdf::sca_signal< double > a_treshold_sig_tdf_fw;
    sca_tdf::sca_signal< int > cmd_sig_tdf_fw;
    sc_core::sc_signal< int > cmd_sig_tlm_fw;

};

#endif //SYSTEM_HH
