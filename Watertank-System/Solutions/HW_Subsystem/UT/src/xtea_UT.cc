#include "xtea_UT.hh"

// constructor
// initialize the TLM socket
// no process, target works with function invocations from the initiator
xtea_UT::xtea_UT(sc_module_name name_) : sc_module(name_), target_socket("target_socket"), pending_transaction(NULL) {
    target_socket(*this);
}

void xtea_UT::b_transport(tlm::tlm_generic_payload& trans, sc_time& t) {
    // function invoked by the initiator
    // download data from the payload
    ioDataStruct = *((iostruct*) trans.get_data_ptr());   // restituisce il puntatore a memoria di dove ....

    if (trans.is_write()) {
        printf("\t\t[XTEA:] Received invocation of the b_transport primitive - write\n");
        printf("\t\t[XTEA:] Invoking the xtea_function to encipher/decipher the input words\n");

        // start elaboration
        // compute the functionality
        xtea_function();

        ioDataStruct.result0 = tmp_result0;
        ioDataStruct.result1 = tmp_result1;

        // and load it on the payload
        *((iostruct*) trans.get_data_ptr()) = ioDataStruct;
        printf("\t\t[XTEA:] Returning result0: %x, result1: %x\n", uint32_t(tmp_result0), uint32_t(tmp_result1));
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
    }
    else if (trans.is_read()) {
        printf("\t\t[XTEA:] Received invocation of the b_transport primitive - read\n");

        // return the result
        ioDataStruct.result0 = tmp_result0;
        ioDataStruct.result1 = tmp_result1;

        // and load it on the payload
        *((iostruct*) trans.get_data_ptr()) = ioDataStruct;
        printf("\t\t[XTEA:] Returning result0: %x, result1: %x\n", uint32_t(tmp_result0), uint32_t(tmp_result1));
    }
}

void xtea_UT:: xtea_function() {
    printf("\t\t[XTEA:] Calculating xtea_function ... \n");

    sum = 0;
    tmp_result0 = 0;
    tmp_result1 = 0;
    tmp_word0 = ioDataStruct.word0;
    tmp_word1 = ioDataStruct.word1;

    if(ioDataStruct.mode == 0) {
        // encipher
        delta=0x9e3779b9;
        for (int i = 0; i < 32; i++) {
            if ((sum & 3) == 0)
                temp = ioDataStruct.k0;
            else if ((sum & 3) == 1)
                temp = ioDataStruct.k1;
            else if ((sum & 3) == 2)
                temp = ioDataStruct.k2;
            else
                temp = ioDataStruct.k3;

            tmp_word0 += (((tmp_word1 << 4) ^ (tmp_word1 >> 5)) + tmp_word1) ^ (sum + temp);
            sum += delta;

            if (((sum >> 11) & 3) == 0)
                temp = ioDataStruct.k0;
            else if (((sum >> 11) & 3) == 1)
                temp = ioDataStruct.k1;
            else if (((sum >> 11) & 3) == 2)
                temp = ioDataStruct.k2;
            else
                temp = ioDataStruct.k3;

            tmp_word1 += (((tmp_word0 << 4) ^ (tmp_word0 >> 5)) + tmp_word0) ^ (sum + temp);
        }
    }
    else if (ioDataStruct.mode == 1) {
        // decipher
        delta = 0x9e3779b9;
        sum = delta*32;
        for (int i = 0; i < 32; i++) {
            if (((sum >> 11) & 3) == 0)
                temp = ioDataStruct.k0;
            else if (((sum >> 11) & 3) == 1)
                temp = ioDataStruct.k1;
            else if (((sum >> 11) & 3) == 2)
                temp = ioDataStruct.k2;
            else
                temp = ioDataStruct.k3;

            tmp_word1 -= (((tmp_word0 << 4) ^ (tmp_word0 >> 5)) + tmp_word0) ^ (sum + temp);
            sum -= delta;

            if((sum & 3) == 0)
                temp = ioDataStruct.k0;
            else if ((sum & 3) == 1)
                temp = ioDataStruct.k1;
            else if ((sum & 3) == 2)
                temp = ioDataStruct.k2;
            else
                temp = ioDataStruct.k3;

            tmp_word0 -= (((tmp_word1 << 4) ^ (tmp_word1 >> 5)) + tmp_word1) ^ (sum + temp);
        }
    }
    tmp_result0 = tmp_word0;
    tmp_result1 = tmp_word1;
}



// must be implemented to be compliant with the interface
// not significant in this code
bool xtea_UT::get_direct_mem_ptr(tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmi_data) {
    return false;
}

tlm::tlm_sync_enum xtea_UT::nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_time& t) {
    return tlm::TLM_COMPLETED;
}

unsigned int xtea_UT::transport_dbg(tlm::tlm_generic_payload& trans) {
    return 0;
}
