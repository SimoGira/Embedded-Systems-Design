#include "xtea_RTL_testbench.hh"
#include "xtea_RTL.hh"

void logo() {
  printf("\n     (((       \n");
  printf("     ((((     XTEA - eXtended Tiny Encryption Algorithm\n");
  printf("     ))))       * Design of Embedded Systems Course\n");
  printf("  _ .----.      * 2018-2019\n");
  printf("  ( |`---'|     \n");
  printf("    |     |\n");
  printf("   : .___, :\n");
  printf("    `-----'\n\n\n");
}

int sc_main(int argc, char* argv[]) {

    // Declaration of signals connection of modules
    //sc_clock                  CLOCK("clock", 2);  <--- deprecated
    sc_clock                  CLOCK;
    sc_signal < bool >        rst;

    sc_signal<sc_uint<32> >   word0, word1;
    sc_signal<sc_uint<32> >   k0, k1, k2, k3;
    sc_signal< bool >         mode;

    sc_signal<sc_uint<1> >    din_rdy;
    sc_signal<sc_uint<1> >    dout_rdy;
    sc_signal<sc_uint<32> >   result0, result1;

    // Allocation of xtea_RTL_testbench module
    xtea_RTL_testbench        tb("tb");             // source module
    tb.clk(CLOCK);
    tb.rst(rst);
    tb.result0_in(result0);
    tb.result1_in(result1);
    tb.din_rdy(din_rdy);

    tb.word0_out(word0);
    tb.word1_out(word1);
    tb.key0_out(k0);
    tb.key1_out(k1);
    tb.key2_out(k2);
    tb.key3_out(k3);
    tb.mode_out(mode);
    tb.dout_rdy(dout_rdy);              // node: dout_rdy is an input port for testbench

    // Allocation of xtea_RTL module
    xtea_RTL                  xtea("xtea");         // xtea RTL module
    xtea.clk(CLOCK);
    xtea.rst(rst);
    xtea.din_rdy(din_rdy);
    xtea.word0_in(word0);
    xtea.word1_in(word1);
    xtea.key0_in(k0);
    xtea.key1_in(k1);
    xtea.key2_in(k2);
    xtea.key3_in(k3);
    xtea.mode_in(mode);

    xtea.result0_out(result0);
    xtea.result1_out(result1);
    xtea.dout_rdy(dout_rdy);

    // Activation of signals tracking
    sc_trace_file *tf = sc_create_vcd_trace_file("xtea");
    sc_trace(tf, CLOCK, "clock");
    sc_trace(tf, rst, "reset");
    sc_trace(tf, mode, "mode");
    sc_trace(tf, din_rdy, "in_rdy");
    sc_trace(tf, dout_rdy, "out_rdy");
    sc_trace(tf, xtea.STATUS, "STATUS");

    logo();

    // start simulation
    sc_start();

    //sc_close_vcd_file(tf);

    return 0;
};
