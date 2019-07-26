#include "valve_tdf.hh"

void valve_tdf::set_attributes() {
    set_timestep(1.0, SC_MS);          // time between activations
    // command_in.set_timestep(1.0, SC_SEC);
    // command_in.set_rate(1);
    // aperture_threshold_in.set_timestep(1.0, SC_SEC);
    // aperture_threshold_in.set_rate(1);
    // //
    // valve_aperture_out.set_timestep(1.0, SC_SEC);
    // valve_aperture_out.set_rate(1);
}

void valve_tdf::initialize() {  // initialize delay buffer
}

void valve_tdf::processing() {  // executed at each activation
    double delta = 0.25 * get_timestep().to_seconds();

    switch (command_in.read()) {
        case IDLE:
        valve_aperture_out.write(a_threshold);
        break;
        case OPEN:
        a_threshold = aperture_threshold_in.read() + delta;
        valve_aperture_out.write(a_threshold);
        break;
        case CLOSE:
        a_threshold = aperture_threshold_in.read() - delta;
        valve_aperture_out.write(a_threshold);
        break;
        default:
        break;
    }
    //std::cout << sc_time_stamp() << " - [VALVE] - command: " << command_in.read() << '\n';
}
