#ifndef VALVE_IFACE_HH
#define VALVE_IFACE_HH

#include "systemc-ams.h"
#include "global.hh"


SCA_TDF_MODULE( valve_iface ) {
  public:
    sca_tdf::sca_out< int > command_out;                    // input for valve
    sca_tdf::sca_out< double > aperture_threshold_out;      // input for valve

    sca_tdf::sca_de::sca_in< int > command_in;
    sca_tdf::sca_de::sca_in< uint32_t > aperture_threshold_in_0;
    sca_tdf::sca_de::sca_in< uint32_t > aperture_threshold_in_1;
    sca_tdf::sca_de::sca_in<sc_uint<1> >dout_rdy;

    valve_iface( sc_core::sc_module_name );

    ~valve_iface();

    type_splitter splitter;

    void set_attribute();
    void processing();
};

#endif //CONTROLLER_IFACE_HH
