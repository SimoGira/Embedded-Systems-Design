#include "ValveIface.hh"

valve_iface::valve_iface( sc_core::sc_module_name ) :
    command_in( "command_in" ),
    command_out( "command_out" ),
    aperture_threshold_in_0( "aperture_threshold_in_0" ),
    aperture_threshold_in_1( "aperture_threshold_in_1" ),
    aperture_threshold_out( "aperture_threshold_out" )
{}

    valve_iface::~valve_iface(){}

    void valve_iface::set_attribute() {}

    void valve_iface::processing() {

        if (dout_rdy.read() == 1) {
            splitter.a_threshold_me[0] = aperture_threshold_in_0.read();
            splitter.a_threshold_me[1] = aperture_threshold_in_1.read();

            aperture_threshold_out.write(splitter.a_threshold);
            command_out.write( command_in.read() );
        }
    }
