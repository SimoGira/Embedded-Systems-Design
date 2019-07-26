#include "xtea_LT_testbench.hh"

void xtea_LT_testbench::invalidate_direct_mem_ptr(uint64 start_range, uint64 end_range){}
tlm::tlm_sync_enum xtea_LT_testbench::nb_transport_bw(tlm::tlm_generic_payload &  trans, tlm::tlm_phase &  phase, sc_time &  t) {
    return tlm::TLM_COMPLETED;
}


void xtea_LT_testbench::run() {

    std::cout << sc_simulation_time() << " - " << name() << " - run" << std::endl;
    sc_time local_time = m_qk.get_local_time();
    iostruct xtea_packet;
    tlm::tlm_generic_payload payload;

    // send words, keys and mode - write invocation
    xtea_packet.word0 = 0x12345678;
    xtea_packet.word1 = 0x9abcdeff;
    xtea_packet.k0 = 0x6a1d78c8;
    xtea_packet.k1 = 0x8c86d67f;
    xtea_packet.k2 = 0x2a65bfbe;
    xtea_packet.k3 = 0xb4bd6e46;
    xtea_packet.mode = 0; // encryption

    // Encryption
    printf("[TB:] First invocation: \n");
    printf("[TB:]   - the encryption of %x and %x \n", uint32_t(xtea_packet.word0), uint32_t(xtea_packet.word1));
    printf("[TB:]   - with key %x%x%x%x \n", uint32_t(xtea_packet.k0), uint32_t(xtea_packet.k1), uint32_t(xtea_packet.k2), uint32_t(xtea_packet.k3));
    payload.set_data_ptr((unsigned char*) &xtea_packet); // set payload data
    payload.set_address(0); // set address, 0 here since we have only 1 target and 1 initiator
    payload.set_write(); // write transaction

    // update the local time variable to send it to the target
    local_time = m_qk.get_local_time();

    printf("[TB:] Invoking the b_transport primitive - write\n");
    initiator_socket->b_transport(payload, local_time); // invoke the transport primitive

    if(payload.get_response_status() == tlm::TLM_OK_RESPONSE) {
        // check that the protocol has been correctly implemented
        // and print the result
        printf("[TB:] TLM protocol correctly implemented\n");
        printf("is: %x, %x \n\n", uint32_t(xtea_packet.result0), uint32_t(xtea_packet.result1));
    }
    if((xtea_packet.result0 != 0x99bbb92b) || (xtea_packet.result1 != 0x3ebd1644))
        printf("Wrong result!");

    // temporal decoupling> get time and check if we have to synchronize with the target
    std::cout << "Time: " << sc_time_stamp() << " + " << local_time << std::endl;
    m_qk.set(local_time);
    if (m_qk.need_sync()) {
      // synchronize simulation time
      cout << "SYNCHRONIZING" << endl;
      m_qk.sync();
      cout << "#####################" << endl;
    }

    // Decryption
    printf("[TB:] Second invocation: \n");
    xtea_packet.mode = 1; //decryption

    xtea_packet.word0 = xtea_packet.result0;
    xtea_packet.word1 = xtea_packet.result1;
    xtea_packet.k0 = 0x6a1d78c8;
    xtea_packet.k1 = 0x8c86d67f;
    xtea_packet.k2 = 0x2a65bfbe;
    xtea_packet.k3 = 0xb4bd6e46;

    printf("[TB:]   - the decryption of %x and %x \n", uint32_t(xtea_packet.word0), uint32_t(xtea_packet.word1));
    printf("[TB:]   - with key %x%x%x%x \n", uint32_t(xtea_packet.k0), uint32_t(xtea_packet.k1), uint32_t(xtea_packet.k2), uint32_t(xtea_packet.k3));
    payload.set_data_ptr((unsigned char*) &xtea_packet); // set payload data
    payload.set_address(0); // set address, 0 here since we have only 1 target and 1 initiator
    payload.set_write(); // write transaction

    // update the local time variable to send it to the target
    local_time = m_qk.get_local_time();

    printf("[TB:] Invoking the b_transport primitive - write\n");
    initiator_socket->b_transport(payload, local_time); // invoke the transport primitive

    if(payload.get_response_status() == tlm::TLM_OK_RESPONSE) {
        // check that the protocol has been correctly implemented
        // and print the result
        printf("[TB:] TLM protocol correctly implemented\n");
        printf("is: %x, %x \n\n", uint32_t(xtea_packet.result0), uint32_t(xtea_packet.result1));
    }
    if((xtea_packet.result0 != 0x12345678) || (xtea_packet.result1 != 0x9abcdeff))
        printf("Wrong result!");

    // temporal decoupling> get time and check if we have to synchronize with the target
    std::cout << "Time: " << sc_time_stamp() << " + " << local_time << std::endl;
    m_qk.set(local_time);
    if (m_qk.need_sync()) {
      // synchronize simulation time
      cout << "SYNCHRONIZING" << endl;
      m_qk.sync();
      cout << "#####################" << endl;
    }

    printf("Done!!\n");

    sc_stop();
}

// constructor to initialize the TLM socket and the main thread
xtea_LT_testbench::xtea_LT_testbench(sc_module_name name): sc_module(name) {
    initiator_socket(*this); // initialize TLM socket
    SC_THREAD(run);         // run this thread

    // set parameters for temporal decoupling
    // one quantum is made of 500 ns
    m_qk.set_global_quantum(sc_time(500, SC_NS));
    m_qk.reset();
}
