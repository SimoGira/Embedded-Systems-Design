#include "System.hh"


System::System( sc_core::sc_module_name ) :
  controller("controller"),
  valve("valve"),
  watertank("watertank")
{
    // Valve Connection
    valve.aperture_threshold_in(a_treshold_sig_tdf_fw);
    valve.command_in(cmd_sig_tdf_fw);
    valve.valve_aperture_out(valve_aperture);

    // Controller Connection
    controller.water_level_in(water_level);
    controller.aperture_threshold_out(a_treshold_sig_tdf_fw);
    controller.command_out(cmd_sig_tdf_fw);
}

System::~System() {}
