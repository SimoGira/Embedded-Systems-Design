#include "xtea_RTL_testbench.hh"
#include <stdio.h>

void xtea_RTL_testbench::run() {
    sc_uint<32> word0, word1, k0, k1, k2, k3;
    sc_uint<32> result0, result1;
    bool mode;

    // encipher test
    mode = 0;
    word0 = 0x12345678;
    word1 = 0x9abcdeff;
    k0 = 0x6a1d78c8;
    k1 = 0x8c86d67f;
    k2 = 0x2a65bfbe;
    k3 = 0xb4bd6e46;

    // Encipher test ----------------------------------------------------------
    printf("First invocation: \n");
    printf("   - the encryption of %x and %x \n", uint32_t(word0), uint32_t(word1));
    printf("   - with key %x%x%x%x \n", uint32_t(k0), uint32_t(k1), uint32_t(k2), uint32_t(k3));

    rst.write(1);
    word0_out.write(word0);
    word1_out.write(word1);
    key0_out.write(k0);
    key1_out.write(k1);
    key2_out.write(k2);
    key3_out.write(k3);
    mode_out.write(mode);
    din_rdy.write(1);          // input for xtea module

    wait();

    while(dout_rdy.read() != 1)
        wait();
    result0 = result0_in.read();
    result1 = result1_in.read();
    // rst.write(0);
    // din_rdy.write(0);
    // wait();

    printf("is: %x, %x \n\n", uint32_t(result0), uint32_t(result1));
    if((result0 != 0x99bbb92b) || (result1 != 0x3ebd1644))
      printf("Wrong result!");

    // Decipher test ----------------------------------------------------------
    printf("Second invocation: \n");
    mode = 1;           //decryption mode
    word0 = result0;
    word1 = result1;

    printf("   - the decryption of %x and %x \n", uint32_t(word0), uint32_t(word1));
    printf("   - with key %x%x%x%x \n", uint32_t(k0), uint32_t(k1), uint32_t(k2), uint32_t(k3));

    rst.write(1);
    word0_out.write(word0);
    word1_out.write(word1);
    mode_out.write(mode);
    din_rdy.write(1);          // input for xtea module

    wait();

    while(dout_rdy.read() != 1)
        wait();
    result0 = result0_in.read();
    result1 = result1_in.read();

    printf("is: %x, %x \n\n", uint32_t(result0), uint32_t(result1));
    if((result0 != 0x12345678) || (result1 != 0x9abcdeff))
      printf("Wrong result!");


    printf("Done!!\n");
    rst.write(0);
    din_rdy.write(0);

    sc_stop();
}
