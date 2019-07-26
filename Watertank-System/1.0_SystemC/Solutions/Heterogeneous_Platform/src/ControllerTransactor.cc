#include "ControllerTransactor.hh"


// INTERFACE SIDE:
//****************
ControllerTransactor::ControllerTransactor(sc_module_name name_)
: sc_module(name_)
, target_socket("target_socket")
{

    target_socket(*this);

    SC_THREAD(WRITEPROCESS);
    sensitive << clk.pos();
}

void ControllerTransactor::b_transport(tlm::tlm_generic_payload& trans, sc_time& t) {
    cout<< "\t\t" << sc_time_stamp()<<" - [ControllerTransactor] - b_transport"<<endl;
    wait(1, SC_MS);
    tlm::tlm_command trans_command = trans.get_command();

    if (trans_command == tlm::TLM_WRITE_COMMAND) {
        ioDataStruct = *((iostruct*) trans.get_data_ptr());
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
        begin_write.notify();
        cout<< "\t\t" << sc_time_stamp()<<" - [ControllerTransactor] - write notified"<<endl;

        wait(end_write);
    }
    cout<< "\t\t" <<sc_time_stamp()<<" - [ControllerTransactor] - b_transport ended"<<endl;
}



void ControllerTransactor::WRITEPROCESS() {
    while (true) {
        wait(begin_write);
        cout<< "\t\t" <<sc_time_stamp()<<" - [ControllerTransactor] - begin_write notify received"<<endl;
        rst.write(1);
        //cout<< "\t\t" <<sc_time_stamp()<<" - [ControllerTransactor] - ioDataStruct.result0: " << ioDataStruct.result0 <<endl;
        splitterDataStruct.a_threshold = ioDataStruct.result0;
        word00_in.write(splitterDataStruct.a_threshold_me[0]);
        word01_in.write(splitterDataStruct.a_threshold_me[1]);
        word1_in.write(ioDataStruct.result1);
        key0_in.write(ioDataStruct.k0);
        key1_in.write(ioDataStruct.k1);
        key2_in.write(ioDataStruct.k2);
        key3_in.write(ioDataStruct.k3);
        mode_in.write(ioDataStruct.mode);
        din_rdy.write(1);

        wait();

        while(dout_rdy.read() != 1) {
            //cout<< "\t\t" <<sc_time_stamp()<<" - [ControllerTransactor] - dout_rdy.read() != 1"<<endl;
            wait();
        }

        end_of_elaboration();
        wait();

        end_write.notify();
    }
}



void ControllerTransactor :: end_of_elaboration() {
    reset();
}

void ControllerTransactor :: reset() {
    cout<< "\t\t" <<sc_time_stamp()<<" - [ControllerTransactor] - reset"<<endl;
    rst.write(0);
    din_rdy.write(0);
    // din.write(0);
}




bool ControllerTransactor::get_direct_mem_ptr(tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmi_data) {
    return false;
}

tlm::tlm_sync_enum ControllerTransactor::nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_time& t) {
    return tlm::TLM_UPDATED;
}

unsigned int ControllerTransactor::transport_dbg(tlm::tlm_generic_payload& trans) {
    return 0;
}
