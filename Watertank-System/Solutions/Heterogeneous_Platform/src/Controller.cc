#include "Controller.hh"

void Controller::invalidate_direct_mem_ptr(uint64 start_range, uint64 end_range){}
tlm::tlm_sync_enum Controller::nb_transport_bw(tlm::tlm_generic_payload &  trans, tlm::tlm_phase &  phase, sc_time &  t) {
    return tlm::TLM_COMPLETED;
}


void Controller::run() {
    sc_time local_time;
    tlm::tlm_generic_payload payload;

    while (true) {
        local_time = m_qk.get_local_time();

        // --------------------------------------------------------------------
        // get current water level from WatertankTransactor
        // --------------------------------------------------------------------

        payload.set_data_ptr((unsigned char*) &water_level_packet); // set payload data
        payload.set_address(0); // set address to watertank_t tlm socket
        payload.set_read();     // read transaction

        // update the local time variable to send it to the target
        local_time = m_qk.get_local_time();

        std::cout << sc_time_stamp() << " - [Controller] Invoking the b_transport primitive - read: " << '\n';
        initiator_socket_to_watertank_t->b_transport(payload, local_time); // invoke the transport primitive
        //std::cout << sc_time_stamp() << " - [Controller] water_level_packet: " << water_level_packet << '\n';

        // --------------------------------------------------------------------
        // calculate the new aperture_threshold
        // --------------------------------------------------------------------

        if (water_level_packet > 8.8) {
            current_threshold *= 0.7;
            command = CLOSE;
        }
        else if (water_level_packet < 5) {
            current_threshold *= 1.1;
            command = OPEN;
        }
        else{
            command = IDLE;
        }

        // --------------------------------------------------------------------
        // prepare packet for xtea
        // --------------------------------------------------------------------

        xtea_packet.word0 = current_threshold;
        xtea_packet.word1 = command;
        xtea_packet.k0 = 0x6a1d78c8;
        xtea_packet.k1 = 0x8c86d67f;
        xtea_packet.k2 = 0x2a65bfbe;
        xtea_packet.k3 = 0xb4bd6e46;
        xtea_packet.mode = 1; // decryption

        //std::cout << sc_time_stamp() << " - [Controller] before xtea_function xtea_packet.word0: " << xtea_packet.word0 << '\n';
        std::cout << sc_time_stamp() << " - [Controller] Encrypting the data ..." << '\n';
        xtea_function();
        //std::cout << sc_time_stamp() << " - [Controller] after xtea_function xtea_packet.result0 : " << xtea_packet.result0 << '\n';

        payload.set_data_ptr((unsigned char*) &xtea_packet); // set payload data
        payload.set_address(0); // set address to controller_t tlm socket
        payload.set_write();    // write transaction

        // update the local time variable to send it to the target
        local_time = m_qk.get_local_time();

        std::cout << sc_time_stamp() << " - [Controller] Invoking the b_transport primitive - write: " << '\n';
        initiator_socket_to_controller_t->b_transport(payload, local_time); // invoke the transport primitive

        // check that the protocol has been correctly implemented
        if(payload.get_response_status() == tlm::TLM_OK_RESPONSE) {
            std::cout << sc_time_stamp() << " - [Controller] TLM protocol correctly implemented" << '\n';
        }

        //cout << "Time: " << sc_time_stamp() << " + " << local_time << endl;
        m_qk.set(local_time);
        if (m_qk.need_sync()) {
          //cout << "SYNCHRONIZING" << endl;
          m_qk.sync();
          //cout << "#####################" << endl;
        }

        wait(5, sc_core::SC_SEC);
    }
}

void Controller::xtea_function() {

    splitter.a_threshold = xtea_packet.word0;               // save word0 to a_threshold to the union
    uint32_t    tmp_word0 = splitter.a_threshold_me[0];     // read first 32bits of a_threshold
    int         tmp_word1 = xtea_packet.word1;
    sc_uint<32> delta = 0x9e3779b9;
    sc_uint<32> temp;
    sc_uint<64> sum = 0;

    // encipher
    for (int i = 0; i < 32; i++) {
        if ((sum & 3) == 0)
            temp = xtea_packet.k0;
        else if ((sum & 3) == 1)
            temp = xtea_packet.k1;
        else if ((sum & 3) == 2)
            temp = xtea_packet.k2;
        else
            temp = xtea_packet.k3;

        tmp_word0 += (((tmp_word1 << 4) ^ (tmp_word1 >> 5)) + tmp_word1) ^ (sum + temp);
        sum += delta;

        if (((sum >> 11) & 3) == 0)
            temp = xtea_packet.k0;
        else if (((sum >> 11) & 3) == 1)
            temp = xtea_packet.k1;
        else if (((sum >> 11) & 3) == 2)
            temp = xtea_packet.k2;
        else
            temp = xtea_packet.k3;

        //tmp_word1 += (((tmp_word0 * 16) ^ (tmp_word0 / 32)) + tmp_word0) ^ (sum + temp);
        tmp_word1 += (((tmp_word0 << 4) ^ (tmp_word0 >> 5)) + tmp_word0) ^ (sum + temp);
    }

    splitter.a_threshold_me[0] = tmp_word0;
    xtea_packet.result0 = splitter.a_threshold;;   // enchiper current_threshold
    xtea_packet.result1 = tmp_word1;                                          // enchiper command

    timing_annotation += sc_time(100, SC_NS);
}

// constructor to initialize the TLM socket and the main thread
Controller::Controller(sc_module_name name): sc_module(name) {
    current_threshold = 0.7;
    initiator_socket_to_watertank_t(*this); // initialize TLM socket
    initiator_socket_to_controller_t(*this);
    SC_THREAD(run);          // run this thread
    //sensitive << water_level;

    // set parameters for temporal decoupling
    // one quantum is made of 500 ms
    m_qk.set_global_quantum(sc_time(500, SC_MS));
    m_qk.reset();
}
