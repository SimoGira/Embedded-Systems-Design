#ifndef xtea_RTL_TESTBENCH_H
#define xtea_RTL_TESTBENCH_H

#include <systemc.h>

SC_MODULE(xtea_RTL_testbench) {

    private:
        void run();

    public:
        sc_in_clk               clk;
        sc_out< bool > 		    rst;

        sc_in<sc_uint<1> >  	dout_rdy;         // output from xtea module
        sc_in<sc_uint<32> > 	result0_in;
        sc_in<sc_uint<32> > 	result1_in;

        sc_out<sc_uint<1> >  	din_rdy;          // input for xtea module
        sc_out<sc_uint<32> > 	word0_out;
        sc_out<sc_uint<32> > 	word1_out;
        sc_out<sc_uint<32> > 	key0_out;
        sc_out<sc_uint<32> > 	key1_out;
        sc_out<sc_uint<32> > 	key2_out;
        sc_out<sc_uint<32> > 	key3_out;
        sc_out< bool > 		    mode_out;


        SC_CTOR(xtea_RTL_testbench) {
            SC_THREAD(run);
            sensitive << clk.pos();
        }
};

#endif
