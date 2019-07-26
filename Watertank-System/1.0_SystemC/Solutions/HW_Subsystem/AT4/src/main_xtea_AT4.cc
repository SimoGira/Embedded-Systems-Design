#include "xtea_AT4.hh"
#include "xtea_AT4_testbench.hh"

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

class xtea_top : public sc_module {

private:
    xtea_AT4  m_target;
    xtea_AT4_testbench m_initiator;


public:
    xtea_top(sc_module_name name) : sc_module(name), m_target("target"), m_initiator("initiator") {
        // bind TLM ports
        m_initiator.initiator_socket(m_target.target_socket);
    }
};

int main(int argc, char* argv[]) {
    xtea_top top("top");
    logo();
    sc_start();

    return 0;
}
