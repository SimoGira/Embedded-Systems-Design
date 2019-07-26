#include "xtea_AT4.hh"

// constructor
// initialize the TLM socket and set ioprocess as a thread
xtea_AT4::xtea_AT4(sc_module_name name_) : sc_module(name_), target_socket("target_socket"), pending_transaction(NULL) {
    target_socket(*this);
    SC_THREAD(IOPROCESS);
}

// transport function invoked by the initiator
tlm::tlm_sync_enum xtea_AT4::nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_time& t) {
  // If there is already a pending transaction, we refuse to
  // serve the request because something went wrong
  if (pending_transaction != NULL) {
    trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return tlm::TLM_COMPLETED;
  }

  // Phase must be BEGIN_REQ (begin request), otherwise we set an
  // error response and complete the transaction.
  if (phase != tlm::BEGIN_REQ) {
    trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return tlm::TLM_COMPLETED;
  }

  printf("\t\t[XTEA:] Received invocation of the nb_transport_fw primitive\n");
  printf("\t\t[XTEA:] Activating the IOPROCESS\n");
  pending_transaction = &trans; // download the payload
  ioDataStruct = *((iostruct*) trans.get_data_ptr()); // get the data

  phase = tlm::END_REQ; // pahse is end request now, target accepted to process the data

  // activate the ioprocess process to elaborate data and to
  io_event.notify();

  // return control
  printf("\t\t[XTEA:] End of the nb_transport_fw primitive\n");
  return tlm::TLM_UPDATED;
}

void xtea_AT4::IOPROCESS() {

  sc_time timing_annotation;

  while (true) {
    wait(io_event);
    // if I am here, then the initiator has invoked the forward transport primitive to issue a request

    printf("\t\t[XTEA:] IOPROCESS has been activated\n");

    // Wait 100ns to model the delay required to
    // process the request - simulate advamncement of time

    wait(100, SC_NS);

    if (pending_transaction->is_write()) {
      // write request: elaborate the square xtea and return
      printf("\t\t[XTEA:] Invoking the xtea_function to encipher/decipher the input words\n");
      xtea_function();
    }

    // read transaction: return the result to the initiator
    else {
        printf("\t\t[XTEA:] Returning result0: %x, result1: %x\n", uint32_t(tmp_result0), uint32_t(tmp_result1));
    }

    ioDataStruct.result0 = tmp_result0;
    ioDataStruct.result1 = tmp_result1;

    // transaction went on correctly
    pending_transaction->set_response_status(tlm::TLM_OK_RESPONSE);

    // upload data and set phase to begin response
    *((iostruct*) pending_transaction->get_data_ptr()) = ioDataStruct;
    tlm::tlm_phase phase = tlm::BEGIN_RESP;

    printf("\t\t[XTEA:] Invoking the nb_transport_bw primitive - write\n");
    target_socket->nb_transport_bw(*pending_transaction, phase, timing_annotation); // transport primitive in the initiator - ends the AT4 protocol

    pending_transaction = NULL;
  }
}



void xtea_AT4:: xtea_function() {
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






void xtea_AT4::b_transport(tlm::tlm_generic_payload& trans, sc_time& t) {}
bool xtea_AT4::get_direct_mem_ptr(tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmi_data) {
    return false;
}

unsigned int xtea_AT4::transport_dbg(tlm::tlm_generic_payload& trans) {
    return 0;
}
