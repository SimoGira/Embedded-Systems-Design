#ifndef CONTROLLER_TRANSACTOR_HH
#define CONTROLLER_TRANSACTOR_HH

#include <systemc.h>
#include <tlm.h>
#include <tlm_utils/tlm_quantumkeeper.h>
#include "global.hh"


class ControllerTransactor
  : public sc_module
  , public virtual tlm::tlm_fw_transport_if<>
{

 public:

   //TLM side

  tlm::tlm_target_socket<> target_socket;
  iostruct  ioDataStruct;
  type_splitter splitterDataStruct;
  tlm::tlm_generic_payload* pending_transaction;
  sc_time timing_annotation;
  sc_event begin_write, end_write;

  //TLM interfaces
  virtual void b_transport(tlm::tlm_generic_payload& trans, sc_time& t);
  virtual bool get_direct_mem_ptr(tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmi_data);
  virtual tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_time& t);
  virtual unsigned int transport_dbg(tlm::tlm_generic_payload& trans);


  //RTL side

  //RTL ports
  sc_in< sc_dt::sc_logic > clk;
  sc_in<sc_uint<1> >  	dout_rdy;    // xtea notify that the result is ready for valve
  sc_out<sc_uint<1> >  	din_rdy;
  sc_out<uint32_t> 	    word00_in;
  sc_out<uint32_t> 	    word01_in;
  sc_out<int >          word1_in;
  sc_out<sc_uint<32> >  key0_in, key1_in, key2_in, key3_in;
  sc_out<bool>          mode_in;
  sc_out<bool> 		    rst;
  //sc_signal<bool > rst;


  //processes
  void WRITEPROCESS();

  //mandatory for TLM
  //void sync();
  void end_of_elaboration();
  void reset();

  SC_HAS_PROCESS(ControllerTransactor);
  ControllerTransactor(sc_module_name name_);

};

#endif //CONTROLLER_TRANSACTOR_HH
