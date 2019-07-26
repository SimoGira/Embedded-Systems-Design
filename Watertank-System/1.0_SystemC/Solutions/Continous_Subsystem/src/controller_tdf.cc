#include "controller_tdf.hh"

void controller_tdf::set_attributes() {
    set_timestep(5.0, SC_SEC);          // time between activations (5s from specification)
    water_level_in.set_delay(5000);
    water_level_in.set_rate(5000);

    aperture_threshold_out.set_timestep(0.001, SC_SEC);
    aperture_threshold_out.set_rate(5000);
    command_out.set_timestep(0.001, SC_SEC);
    command_out.set_rate(5000);
}

void controller_tdf::initialize() {  //initialize delay buffer
    current_threshold = 0.7;
    for (int i = 0; i < 5000; i++) {
        water_level_in.initialize(0.0, i);
    }
}

void controller_tdf::processing() {  // executed at each activation

    //std::cout << sc_time_stamp() << " - [CONTROLLER] - water_level: " << water_level_in.read(5-1) << '\n';

    // calculate the new aperture_threshold
    if (water_level_in.read(5000-1) > 8.8) {
        current_threshold *= 0.7;
        for (int i = 0; i < 5000; i++) {
            aperture_threshold_out.write(current_threshold, i);
            command_out.write(CLOSE, i);
            //std::cout << sc_time_stamp() << " - [CONTROLLER] - command: " << CLOSE << '\n';
        }
    }
    else if (water_level_in.read(5000-1) < 5) {
        current_threshold *= 1.1;
        for (int i = 0; i < 5000; i++) {
            aperture_threshold_out.write(current_threshold, i);
            command_out.write(OPEN, i);
            //std::cout << sc_time_stamp() << " - [CONTROLLER] - command: " << OPEN << '\n';
        }
    }
    else{
        for (int i = 0; i < 5000; i++) {
            aperture_threshold_out.write(current_threshold, i);
            command_out.write(IDLE, i);
            //std::cout << sc_time_stamp() << " - [CONTROLLER] - command: " << IDLE << '\n';
        }
    }
    //std::cout << sc_time_stamp() << " - [CONTROLLER] - current_threshold: " << current_threshold << '\n';

}
