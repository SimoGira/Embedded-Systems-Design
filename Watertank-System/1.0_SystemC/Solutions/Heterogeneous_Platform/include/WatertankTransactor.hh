#ifndef WATERTANK_TRANSACTOR_HH
#define WATERTANK_TRANSACTOR_HH


#include <tlm.h>
#include <systemc.h>

class WatertankTransactor : public sc_module, public virtual tlm::tlm_fw_transport_if<> {
  public:
    tlm::tlm_target_socket<> target_socket;

    virtual void b_transport( tlm::tlm_generic_payload& trans, sc_time& t );
    virtual bool get_direct_mem_ptr(tlm::tlm_generic_payload& trans, tlm::tlm_dmi & dmi_data );
    virtual unsigned int transport_dbg( tlm::tlm_generic_payload& trans );

    virtual tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& trans,tlm::tlm_phase& phase,sc_time& t);

    double water_level_packet;

    tlm::tlm_generic_payload* pending_transaction;

    sc_core::sc_time timing_annotation;

    void transduce();

    SC_HAS_PROCESS(WatertankTransactor);

    WatertankTransactor(sc_module_name name);
    ~WatertankTransactor();

    // RTL Interface
    sc_in< double > water_level_in;
};


#endif //WATERTANK_TRANSACTOR_HH
