#ifndef __xtea_LT_HPP__
#define __xtea_LT_HPP__

#include <systemc.h>
#include <tlm.h>
#include "define_LT.hh" // definizione del payload

class xtea_LT : public sc_module, public virtual tlm::tlm_fw_transport_if<> {

public:
    // TLM socket
    tlm::tlm_target_socket<> target_socket;

    // blocking transport primitive
    virtual void b_transport(tlm::tlm_generic_payload& trans, sc_time& t);

    // functions that must be implemented to be compliant with the standard
    // not used here
    virtual tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_time& t);
    virtual bool get_direct_mem_ptr(tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmi_data);
    virtual unsigned int transport_dbg(tlm::tlm_generic_payload& trans);

    void end_of_elaboration();
    void reset();

    // internal data
    iostruct  ioDataStruct;
    tlm::tlm_generic_payload* pending_transaction;

    sc_uint<32> tmp_result0, tmp_result1;
    sc_uint<32> tmp_word0, tmp_word1;
    sc_uint<32> delta;
    sc_uint<32> temp;
    sc_uint<64> sum;

    sc_time timing_annotation;

    // elaboration function
    void xtea_function();

    // constructors
    SC_HAS_PROCESS(xtea_LT);
    xtea_LT(sc_module_name name_);

};

#endif
