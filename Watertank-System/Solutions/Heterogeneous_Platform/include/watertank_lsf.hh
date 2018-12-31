#ifndef WATERTANK_LSF_HH
#define WATERTANK_LSF_HH

#include <systemc-ams>
#include "global.hh"

SC_MODULE(watertank_lsf){

public:

    //sc_in_clk clock;

    sca_lsf::sca_signal sig_valve_aperture;
    sca_lsf::sca_signal sig_water_level;

    sca_lsf::sca_tdf::sca_source tdf2lsf; // get values from tdf
    sca_lsf::sca_de::sca_sink lsf2rtl;    // get signal lsf and output value rtl

    sca_lsf::sca_integ int1;
    sca_lsf::sca_gain gain1, gain2;
    sca_lsf::sca_sub sub1;

    SC_CTOR( watertank_lsf );

private:
    sca_lsf::sca_signal sig1, sig2, sig3;

};

#endif
