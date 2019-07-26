#include <iostream>
//#include <fstream>  <--- questa non serve
#include <stdint.h>

/**********************************************************************

     eXtended Tiny Encryption Algorithm (XTEA)
        algorithm:        en.wikipedia.org/wiki/XTEA
        course:           Progettazione di Sistemi Embedded (2018-19)
        code provided by: Sara Vinco

**********************************************************************/

using namespace std;

void logo(){
  //printf("\n     (((       \n");
  //printf("     ((((     XTEA - eXtended Tiny Encryption Algorithm\n");
  //printf("     ))))       * Design of Embedded Systems Course\n");
  //printf("  _ .----.      * 2018-2019\n");
  //printf("  ( |`---'|     \n");
  //printf("    |     |\n");
  //printf("   : .___, :\n");
  //printf("    `-----'\n\n\n");

}

void xtea (uint32_t word0, uint32_t word1, uint32_t key0,
          uint32_t key1, uint32_t key2, uint32_t key3,
          bool mode,
          uint32_t * result0, uint32_t * result1) {

  uint64_t sum;
  uint32_t i, delta, v0, v1, temp;
  v0 = word0;
  v1 = word1;
  sum = 0;
  *result0 = 0;
  *result1 = 0;

  if(mode == 0) {
    // encipher
    delta=0x9e3779b9;
    for (i=0; i < 32; i++) {
      //std::cout << "i: " << i << '\n';
      if((sum & 3) == 0) {
        temp = key0;
      }
      else if((sum & 3) == 1)
        temp = key1;
      else if ((sum & 3) == 2)
        temp = key2;
      else temp = key3;

      // std::cout << "\tv0: " << v0 << '\n';
      // std::cout << "\tv1: " << v1 << '\n';
      // std::cout << "\tsum: " << sum << '\n';
      // std::cout << "\ttemp: " << temp << '\n';
      // std::cout << "\t(v1 << 4): " << (v1 << 4) << '\n';
      // std::cout << "\t(v1 >> 5): " << (v1 >> 5) << '\n';
      // std::cout << "\t((v1 << 4) ^ (v1 >> 5)): " << ((v1 << 4) ^ (v1 >> 5)) << '\n';
      // std::cout << "\t((v1 << 4) ^ (v1 >> 5)) + v1): " << (((v1 << 4) ^ (v1 >> 5)) + v1) << '\n';
      // std::cout << "\t(v0 + (((v1 << 4) ^ (v1 >> 5)) + v1)): " << v0 + (((v1 << 4) ^ (v1 >> 5)) + v1) << '\n';
      // std::cout << "\tv0 = v0 + (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + temp): " << v0 + ((((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + temp)) << endl;

      v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + temp);
      sum += delta;

      // std::cout << "\t -- v0:" << v0 << '\n';
      // std::cout << "\t -- sum:" << sum << '\n';

      if(((sum>>11) & 3) == 0)
        temp = key0;
      else if(((sum>>11) & 3) == 1)
        temp = key1;
      else if (((sum>>11) & 3) == 2)
        temp = key2;
      else temp = key3;

      v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + temp);
    }
  }

  else if (mode == 1) {
    // decipher
    delta = 0x9e3779b9;
    sum = delta*32;
    for (i=0; i<32; i++) {

      if(((sum>>11) & 3) == 0)
        temp = key0;
      else if(((sum>>11) & 3) == 1)
        temp = key1;
      else if (((sum>>11) & 3) == 2)
        temp = key2;
      else temp = key3;

      v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + temp);
      sum -= delta;

      if((sum & 3) == 0)
        temp = key0;
      else if((sum & 3) == 1)
        temp = key1;
      else if ((sum & 3) == 2)
        temp = key2;
      else temp = key3;

      v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + temp);

    }
  }
  *result0 = v0;
  *result1 = v1;
}


int main(int argc, char *argv[])
{
  uint32_t word0, word1, k0, k1, k2, k3;
  bool mode;
  uint32_t result0, result1;

  logo();

  mode = 0;
  word0 = 0x12345678;
  word1 = 0x9abcdeff;
  k0 = 0x6a1d78c8;
  k1 = 0x8c86d67f;
  k2 = 0x2a65bfbe;
  k3 = 0xb4bd6e46;
  xtea(word0, word1, k0, k1, k2, k3, mode, &result0, &result1);
  //printf("First invocation: \n");
  //printf("   - the encryption of %x and %x \n", word0, word1);
  //printf("   - with key %x%x%x%x \n", k0, k1, k2, k3);
  //printf("is: %x, %x \n\n", result0, result1);
  if((result0 != 0x99bbb92b) || (result1 != 0x3ebd1644))
    //printf("Wrong result!");

  //printf("Second invocation: \n");
  mode = 1; //decryption

  word0 = result0;
  word1 = result1;

  k0 = 0x6a1d78c8;
  k1 = 0x8c86d67f;
  k2 = 0x2a65bfbe;
  k3 = 0xb4bd6e46;

  xtea(word0, word1, k0, k1, k2, k3, mode, &result0, &result1);
  //printf("   - the decryption of %x and %x \n", word0, word1);
  //printf("   - with key %x%x%x%x \n", k0, k1, k2, k3);
  //printf("is: %x, %x \n\n", result0, result1);

  if((result0 != 0x12345678) || (result1 != 0x9abcdeff))
    //printf("Wrong result!");

  //printf("Done!!\n");
  return(0);

}
