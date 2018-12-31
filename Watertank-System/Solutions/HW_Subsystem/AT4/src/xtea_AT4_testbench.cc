#include "xtea_AT4_testbench.hh"

void xtea_AT4_testbench::invalidate_direct_mem_ptr(uint64 start_range, uint64 end_range){}

// initiator process
void xtea_AT4_testbench::run() {

    sc_time local_time = SC_ZERO_TIME;
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

    // set phase: begin request
    tlm::tlm_phase phase = tlm::BEGIN_REQ;

    payload.set_data_ptr((unsigned char*) &xtea_packet); // set payload data
    payload.set_address(0); // set address, 0 here since we have only 1 target and 1 initiator
    payload.set_write(); // write transaction

    printf("[TB:] Invoking the nb_transport primitive of xtea - write\n");
    tlm::tlm_sync_enum result = initiator_socket->nb_transport_fw(payload, phase, local_time);  // invoke the transport primitive
                                                                                                // pass also the phase

    if (result == tlm::TLM_COMPLETED) {
      // If the target immediately completes the transaction something
      // wrong happened. We should inspect the response status and
      // take appropriate actions. In this case we just stop the simulation.
      printf("[ERROR:] Transaction error on nb_transport_fw. Forcing simulation stop.\n");
      sc_stop();
    }

    // Phase must be an end request, used by the target to notify that
    // it accepted the request
    // else, there is something wrong in the protocol
    if (phase != tlm::END_REQ) {
        printf("[ERROR: ] Unexpected protocol phase on nb_transport_fw. Forcing simulation stop.\n");
      sc_stop();
    }

    printf("[TB:] Waiting for nb_transport_bw to be invoked\n");
    response_pending = true;  // we have one request pending
    wait(available_response); // thus we suspend until the protocol is finished

    // if I am here, then the target has invoked the backward primitive and the AT4 protocol is finished
    // I can issue a new request to get the result
    response_pending = false;

    phase = tlm::BEGIN_REQ;
    payload.set_data_ptr((unsigned char*) &xtea_packet); // set payload data
    payload.set_address(0); // set address, 0 here since we have only 1 target and 1 initiator
    payload.set_read();  // read transaction

    printf("[TB:] Invoking the nb_transport_fw primitive of xtea - read\n");
    result = initiator_socket->nb_transport_fw(payload, phase, local_time);

    if (result == tlm::TLM_COMPLETED) {
        printf("[ERROR:] Transaction error on nb_transport_fw. Forcing simulation stop.\n");
      sc_stop();
    }

    if (phase != tlm::END_REQ) {
      printf("[ERROR: ] Unexpected protocol phase on nb_transport_fw. Forcing simulation stop.\n");
      sc_stop();
    }

    printf("[TB:] Waiting for nb_transport_bw to be invoked\n");
    response_pending = true;
    wait(available_response);

    // if I am here, then the target has invoked the backward primitive and the AT4 protocol is finished
    // I can get the final result and print it
    response_pending = false;


    if(payload.get_response_status() == tlm::TLM_OK_RESPONSE) {
        // check that the protocol has been correctly implemented
        // and print the result
        printf("[TB:] TLM protocol correctly implemented\n");
        printf("is: %x, %x \n\n", uint32_t(xtea_packet.result0), uint32_t(xtea_packet.result1));
    }
    if((xtea_packet.result0 != 0x99bbb92b) || (xtea_packet.result1 != 0x3ebd1644))
        printf("Wrong result!");


// ----------------------------------------------------------------------------

        // send words, keys and mode - write invocation
        xtea_packet.word0 = xtea_packet.result0;
        xtea_packet.word1 = xtea_packet.result1;
        xtea_packet.k0 = 0x6a1d78c8;
        xtea_packet.k1 = 0x8c86d67f;
        xtea_packet.k2 = 0x2a65bfbe;
        xtea_packet.k3 = 0xb4bd6e46;
        xtea_packet.mode = 1; // decryption

        // Decryption
        printf("[TB:] Second invocation: \n");
        printf("[TB:]   - the decryption of %x and %x \n", uint32_t(xtea_packet.word0), uint32_t(xtea_packet.word1));
        printf("[TB:]   - with key %x%x%x%x \n", uint32_t(xtea_packet.k0), uint32_t(xtea_packet.k1), uint32_t(xtea_packet.k2), uint32_t(xtea_packet.k3));

        // set phase: begin request
        phase = tlm::BEGIN_REQ;

        payload.set_data_ptr((unsigned char*) &xtea_packet); // set payload data
        payload.set_address(0); // set address, 0 here since we have only 1 target and 1 initiator
        payload.set_write(); // write transaction

        printf("[TB:] Invoking the b_transport primitive of xtea - write\n");
        result = initiator_socket->nb_transport_fw(payload, phase, local_time);  // invoke the transport primitive
                                                                                 // pass also the phase

        if (result == tlm::TLM_COMPLETED) {
          // If the target immediately completes the transaction something
          // wrong happened. We should inspect the response status and
          // take appropriate actions. In this case we just stop the simulation.
          printf("[ERROR:] Transaction error on nb_transport_fw. Forcing simulation stop.\n");
          sc_stop();
        }

        // Phase must be an end request, used by the target to notify that
        // it accepted the request
        // else, there is something wrong in the protocol
        if (phase != tlm::END_REQ) {
            printf("[ERROR: ] Unexpected protocol phase on nb_transport_fw. Forcing simulation stop.\n");
          sc_stop();
        }

        printf("[TB:] Waiting for nb_transport_bw to be invoked\n");
        response_pending = true;  // we have one request pending
        wait(available_response); // thus we suspend until the protocol is finished

        // if I am here, then the target has invoked the backward primitive and the AT4 protocol is finished
        // I can issue a new request to get the result
        response_pending = false;

        phase = tlm::BEGIN_REQ;
        payload.set_data_ptr((unsigned char*) &xtea_packet); // set payload data
        payload.set_address(0); // set address, 0 here since we have only 1 target and 1 initiator
        payload.set_read();  // read transaction

        printf("[TB:] Invoking the nb_transport_fw primitive of xtea - read\n");
        result = initiator_socket->nb_transport_fw(payload, phase, local_time);

        if (result == tlm::TLM_COMPLETED) {
            printf("[ERROR:] Transaction error on nb_transport_fw. Forcing simulation stop.\n");
          sc_stop();
        }

        if (phase != tlm::END_REQ) {
          printf("[ERROR: ] Unexpected protocol phase on nb_transport_fw. Forcing simulation stop.\n");
          sc_stop();
        }

        printf("[TB:] Waiting for nb_transport_bw to be invoked\n");
        response_pending = true;
        wait(available_response);

        // if I am here, then the target has invoked the backward primitive and the AT4 protocol is finished
        // I can get the final result and print it
        response_pending = false;


        if(payload.get_response_status() == tlm::TLM_OK_RESPONSE) {
            // check that the protocol has been correctly implemented
            // and print the result
            printf("[TB:] TLM protocol correctly implemented\n");
            printf("is: %x, %x \n\n", uint32_t(xtea_packet.result0), uint32_t(xtea_packet.result1));
        }
        if((xtea_packet.result0 != 0x12345678) || (xtea_packet.result1 != 0x9abcdeff))
             printf("Wrong result!");


    printf("Done!!\n");

    sc_stop();
}

tlm::tlm_sync_enum xtea_AT4_testbench::nb_transport_bw(tlm::tlm_generic_payload &  trans, tlm::tlm_phase &  phase, sc_time &  t) {
    // If response_pending is true, then the initiator had made a request to the target
    // and this invocation is the answer
    if (!response_pending) {
      printf("[ERROR:] Unexpected state for nb_transport_be\n");
      trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
      return tlm::TLM_COMPLETED;
    }

    // The phase of the invocation must be begin response, or there is some error in the protocol
    if (phase != tlm::BEGIN_RESP) {
      printf("[ERROR:] Unexpected phase for nb_transport_be\n");
      trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
      return tlm::TLM_COMPLETED;
    }

    // reactivate the initiator process to go on with computation
    available_response.notify();

    // phase is updated to end response since the AT4 protocol is finished
    phase = tlm::END_RESP;

    return tlm::TLM_COMPLETED;
}

// constructor to initialize the TLM socket and state that run is a thread
xtea_AT4_testbench::xtea_AT4_testbench(sc_module_name name): sc_module(name), response_pending(false) {
    initiator_socket(*this); // initialize TLM socket
    SC_THREAD(run);          // run this thread
}
