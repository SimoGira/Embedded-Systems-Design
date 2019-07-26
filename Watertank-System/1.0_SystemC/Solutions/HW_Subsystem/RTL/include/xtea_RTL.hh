#ifndef xtea_RTL_H
#define xtea_RTL_H

//systemc include
#include "systemc.h"

SC_MODULE(xtea_RTL) {
    public:
        // input ports
        sc_in<sc_uint<1> >    din_rdy;
        sc_in<sc_uint<32> >   word0_in, word1_in;
        sc_in<sc_uint<32> >   key0_in, key1_in, key2_in, key3_in;
        sc_in<bool>           mode_in;

        // output ports
        sc_out<sc_uint<32> >  result0_out, result1_out;
        sc_out<sc_uint<1> >   dout_rdy;

        // clock e reset ports
        sc_in<bool>              rst;
        sc_in_clk                clk;

        typedef enum {Reset_ST, ST_0, ST_1, ST_2, ST_3, ST_4, ST_5, ST_6, ST_7, ST_8, ST_9, ST_10, ST_11, ST_12} STATES;
        sc_signal<int> STATUS, NEXT_STATUS;

        SC_CTOR(xtea_RTL) {
            SC_METHOD(datapath);
            sensitive << rst.neg();
            sensitive << clk.pos();

            SC_METHOD(fsm);
            sensitive << STATUS << din_rdy << mode_in << i;
        }
        ~xtea_RTL();

    protected:
        sc_signal<sc_uint<32> > i, delta, v0, v1, temp;
        sc_signal<sc_uint<32> > key0, key1, key2, key3;
        sc_signal<sc_uint<64> > sum;
        sc_signal<bool>         mode;

        // process declariation
        void fsm(void);
        void datapath(void);
};

#endif
