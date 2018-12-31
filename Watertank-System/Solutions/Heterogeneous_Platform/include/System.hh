#ifndef SYSTEM_HH
#define SYSTEM_HH

#include <systemc-ams>
#include "global.hh"
#include "Controller.hh"
#include "ControllerTransactor.hh"
#include "xtea_RTL.hh"
#include "ValveIface.hh"
#include "valve_tdf.hh"
#include "watertank_lsf.hh"
#include "WatertankTransactor.hh"

#define PERIOD 10.0


//class System : public sc_core::sc_module {
SC_MODULE(System) {

public:
    SC_HAS_PROCESS(System);
    System( sc_core::sc_module_name );
    ~System();

//protected:
    Controller           controller;
    ControllerTransactor controller_t;
    xtea_RTL             xtea;
    WatertankTransactor  watertank_t;
    valve_iface          valve_if;
    valve_tdf            valve;
    watertank_lsf        watertank;


    // CLOCK SIGNAL
    sc_signal< sc_dt::sc_logic > clk;

    void clk_gen() {
        while( true ) {
            clk.write( sc_dt::SC_LOGIC_1 );
            wait( PERIOD / 2, sc_core::SC_MS );
            clk.write( sc_dt::SC_LOGIC_0 );
            wait( PERIOD / 2, sc_core::SC_MS );
        }
    }

    // RESET SIGNAL
    sc_signal < bool >        rst;

    // OTHER SIGNALS
    sc_signal<uint32_t>       word00;
    sc_signal<uint32_t>       word01;
    sc_signal<int >           word1;
    sc_signal<sc_uint<32> >   k0, k1, k2, k3;
    sc_signal< bool >         mode;

    sc_signal<sc_uint<1> >    din_rdy;
    sc_signal<sc_uint<1> >    dout_rdy;

    sca_tdf::sca_signal< double > a_treshold_sig_tdf_fw;
    sca_tdf::sca_signal< int >    cmd_sig_tdf_fw;
    sc_core::sc_signal< int >     cmd_sig_rtl_fw;

};

#endif //SYSTEM_HH
