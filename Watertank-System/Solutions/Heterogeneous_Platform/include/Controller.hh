#ifndef CONTROLLER_HH
#define CONTROLLER_HH

#include <systemc.h>
#include <tlm.h>
#include <tlm_utils/tlm_quantumkeeper.h>
#include "global.hh"


class Controller: public sc_core::sc_module, public virtual tlm::tlm_bw_transport_if<> {

  private:
    virtual void invalidate_direct_mem_ptr(uint64 start_range, uint64 end_range );
    virtual tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_core::sc_time& t);

    void run();
    void xtea_function();

    tlm_utils::tlm_quantumkeeper m_qk;
    sc_time timing_annotation;

    double current_threshold;
    double water_level_packet;
    int command;

  public:
    SC_HAS_PROCESS( Controller );
    Controller( sc_module_name name );
    tlm::tlm_initiator_socket<> initiator_socket_to_controller_t; // initiator for xtea
    tlm::tlm_initiator_socket<> initiator_socket_to_watertank_t;  // initiator for watertank

    iostruct xtea_packet;
    type_splitter splitter;

};

#endif //CONTROLLER_TRANSACTOR_HH
