#include "WatertankTransactor.hh"

WatertankTransactor::WatertankTransactor( sc_module_name name ) :
sc_module( name ),
target_socket( "target_socket" ),
pending_transaction( NULL ) {
    target_socket( *this );
    water_level_packet = 0.0;
}

WatertankTransactor::~WatertankTransactor() {}

void WatertankTransactor::b_transport(tlm::tlm_generic_payload& trans, sc_time & t ) {
    //implementation of the b_transport
    timing_annotation = SC_ZERO_TIME;
    water_level_packet = *((double*) trans.get_data_ptr());

    // we can only read from watertank
    if (trans.is_read()) {
        std::cout << "\t\t[WatertankTransactor] Received invocation of the b_transport primitive - read" << '\n';

        transduce();

        *((double*) trans.get_data_ptr()) = water_level_packet;
        std::cout << "\t\t[WatertankTransactor] Returning water_level: "  << water_level_packet << '\n';
    }

    t += timing_annotation;
}

bool WatertankTransactor::get_direct_mem_ptr(tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmi_data) {
    return false;
}

tlm::tlm_sync_enum WatertankTransactor::nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_time& t) {
    return tlm::TLM_COMPLETED;
}

unsigned int WatertankTransactor::transport_dbg(tlm::tlm_generic_payload& trans) {
    return 0;
}

void WatertankTransactor::transduce() {
    //reading from ams...
    water_level_packet = water_level_in.read();
}
