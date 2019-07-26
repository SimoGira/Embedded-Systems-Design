#include "xtea_RTL.hh"
#include <iostream>


xtea_RTL::~xtea_RTL() {}

void xtea_RTL::datapath(void) {
    if (rst.read() == 0) {
        STATUS = Reset_ST;
    }
    else if (clk.read() == 1) {
        STATUS = NEXT_STATUS;

        switch(STATUS) {
            case Reset_ST:
                result00_out.write(0);
                result01_out.write(0);
                result1_out.write(0);
                dout_rdy.write(0);
                break;
            case ST_0:
                result00_out.write(0);
                result01_out.write(0);
                result1_out.write(0);
                dout_rdy.write(0);

                i.write(0);
                temp.write(0);
                delta.write(0);
                sum.write(0);
                break;
            case ST_1:
                //cout<<sc_time_stamp()<<" - [XTEA] - word00_in.read(): " << word00_in.read() << '\n';
                v0 = word00_in.read();
                v1 = word1_in.read();
                key0 = key0_in.read();
                key1 = key1_in.read();
                key2 = key2_in.read();
                key3 = key3_in.read();
                mode = mode_in.read();
                break;
            case ST_2:
                delta.write(0x9e3779b9);

                // Operations to determine the value of temp
                if((sum.read() & 3) == 0)
                  temp.write(key0.read());
                else if((sum.read() & 3) == 1)
                  temp.write(key1.read());
                else if ((sum.read() & 3) == 2)
                  temp.write(key2.read());
                else temp.write(key3.read());
                break;
            case ST_3:
                v0.write(v0.read() + ((((v1.read() << 4) ^ (v1.read() >> 5)) + v1.read()) ^ (sum.read() + temp.read())));
                sum.write(sum.read() + delta.read());
                break;
            case ST_4:
                if(((sum.read() >> 11) & 3) == 0)
                  temp.write(key0.read());
                else if(((sum.read() >> 11) & 3) == 1)
                  temp.write(key1.read());
                else if (((sum.read() >> 11) & 3) == 2)
                  temp.write(key2.read());
                else temp.write(key3.read());
                break;
            case ST_5:
                //v1.write(v1.read() + ((sc_dt::sc_uint<32>(sc_dt::sc_uint<32>(v0.read() << 4) ^ sc_dt::sc_uint<32>(v0.read() >> 5)) + v0.read()) ^ sc_dt::sc_uint<32>(sum.read() + temp.read())));
                v1.write(v1.read() + ((((v0.read() << 4) ^ (v0.read() >> 5)) + v0.read()) ^ (sum.read() + temp.read())));
                i.write(i.read() + 1);
                break;
            case ST_6:
                result00_out.write(v0.read());
                result01_out.write(word01_in.read());
                result1_out.write(v1.read());
                dout_rdy.write(1);
                break;
            case ST_7:
                delta.write(0x9e3779b9);
                break;
            case ST_8:
                sum.write(delta.read() * 32);
                break;
            case ST_9:
                if(((sum.read() >> 11) & 3) == 0)
                  temp.write(key0.read());
                else if(((sum.read() >> 11) & 3) == 1)
                  temp.write(key1.read());
                else if (((sum.read() >> 11) & 3) == 2)
                  temp.write(key2.read());
                else temp.write(key3.read());
                break;
            case ST_10:
                v1.write(v1.read() - ((((v0.read() << 4) ^ (v0.read() >> 5)) + v0.read()) ^ (sum.read() + temp.read())));
                sum.write(sum.read() - delta.read());
                break;
            case ST_11:
                if((sum.read() & 3) == 0)
                  temp.write(key0.read());
                else if((sum.read() & 3) == 1)
                  temp.write(key1.read());
                else if ((sum.read() & 3) == 2)
                  temp.write(key2.read());
                else temp.write(key3.read());
                break;
            case ST_12:
                v0.write(v0.read() - ((((v1.read() << 4) ^ (v1.read() >> 5)) + v1.read()) ^ (sum.read() + temp.read())));
                i.write(i.read() + 1);
                break;
        }
    }
}


void xtea_RTL::fsm(void) {
    NEXT_STATUS = STATUS;
    //cout<<sc_time_stamp()<<" - [XTEA] - din_rdy.read() value: " << din_rdy.read() << '\n';


    switch(STATUS) {
        case Reset_ST:
            NEXT_STATUS = ST_0;
            break;
        case ST_0:                      // waiting for a new input
            if (din_rdy.read() == 1) {
                //cout<<sc_time_stamp()<<" - [XTEA] - din_rdy.read() == 1 " << '\n';
                NEXT_STATUS = ST_1;
            }
            else /*if (din_rdy.read() == 0)*/{
                //cout<<sc_time_stamp()<<" - [XTEA] - din_rdy.read() == 0 " << '\n';
                NEXT_STATUS = ST_0;
            }
            break;
        case ST_1:                      // new input is ready
            if (mode_in.read() == 0) {     // encipher
                NEXT_STATUS = ST_2;
            }
            else if (mode_in.read() == 1){ // decipher
                NEXT_STATUS = ST_7;
            }
            break;
        case ST_2:                      // ready to encipher v0
            if (i.read() < 32) {
                NEXT_STATUS = ST_3;
            }
            else {
                NEXT_STATUS = ST_6;
            }
            break;
        case ST_3:                      // v0 is enciphered
            NEXT_STATUS = ST_4;
            break;
        case ST_4:                      // ready to encipher v1
            NEXT_STATUS = ST_5;
            break;
        case ST_5:                      // v1 is enciphered
            NEXT_STATUS = ST_2;
            break;
        case ST_6:                      // encipher or decipher completed
            NEXT_STATUS = ST_0;
            break;
        case ST_7:                      // assign delta
            NEXT_STATUS = ST_8;
            break;
        case ST_8:                      // assign sum
            NEXT_STATUS = ST_9;
            break;
        case ST_9:
            if (i.read() < 32) {
                NEXT_STATUS = ST_10;
            }
            else {
                NEXT_STATUS = ST_6;
            }
            break;
        case ST_10:
            NEXT_STATUS = ST_11;
            break;
        case ST_11:
            NEXT_STATUS = ST_12;
            break;
        case ST_12:
            NEXT_STATUS = ST_9;
            break;
    }
}
