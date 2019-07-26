#include "System.hh"


System::System( sc_core::sc_module_name ) :
controller("controller"),
controller_t("controller_t"),
xtea("xtea"),
valve_if("valve_if"),
valve("valve"),
watertank("watertank"),
watertank_t("watertank_t")
{

    // RTL design <-> Transactor binding (RTL interfaces)
    xtea.clk(clk);
    xtea.rst(rst);
    xtea.din_rdy(din_rdy);
    xtea.word00_in(word00);
    xtea.word01_in(word01);
    xtea.word1_in(word1);
    xtea.key0_in(k0);
    xtea.key1_in(k1);
    xtea.key2_in(k2);
    xtea.key3_in(k3);
    xtea.mode_in(mode);

    xtea.result00_out(a_threshold_0);
    xtea.result01_out(a_threshold_1);
    xtea.result1_out(cmd_sig_rtl_fw);
    xtea.dout_rdy(dout_rdy);

    controller_t.clk(clk);
    controller_t.rst(rst);
    controller_t.din_rdy(din_rdy);
    controller_t.dout_rdy(dout_rdy);
    controller_t.word00_in(word00);
    controller_t.word01_in(word01);
    controller_t.word1_in(word1);
    controller_t.key0_in(k0);
    controller_t.key1_in(k1);
    controller_t.key2_in(k2);
    controller_t.key3_in(k3);
    controller_t.mode_in(mode);



    // Valve Connection
    valve.aperture_threshold_in(a_treshold_sig_tdf_fw);
    valve.command_in(cmd_sig_tdf_fw);
    valve.valve_aperture_out(valve_aperture);


    // Controller Interface Side
    valve_if.dout_rdy(dout_rdy);
    valve_if.command_in(cmd_sig_rtl_fw);
    valve_if.aperture_threshold_in_0(a_threshold_0);
    valve_if.aperture_threshold_in_1(a_threshold_1);
    valve_if.aperture_threshold_out(a_treshold_sig_tdf_fw);
    valve_if.command_out(cmd_sig_tdf_fw);


    // Transactors Connections
    controller.initiator_socket_to_watertank_t( watertank_t.target_socket );
    controller.initiator_socket_to_controller_t( controller_t.target_socket );


    // Watertank Interface Transactor Side
    watertank_t.water_level_in(water_level);

    SC_THREAD( clk_gen );
}

System::~System() {}
