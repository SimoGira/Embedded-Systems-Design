#include "System.hh"

int sc_main( int ac, char *av[] ) {

  System system( "system" );

  sca_util::sca_trace_file* atf = sca_util::sca_create_vcd_trace_file( "trace.vcd" );
  sca_util::sca_trace( atf, system.clk, "clock" );
  sca_util::sca_trace( atf, system.cmd_sig_rtl_fw, "controller_cmd" );
  sca_util::sca_trace( atf, system.a_treshold_sig_tdf_fw, "aperture_treshold" );
  sca_util::sca_trace( atf, valve_aperture, "valve_aperture" );
  sca_util::sca_trace( atf, water_level, "water_level" );

  sc_start( 1000, SC_SEC );

  sca_util::sca_close_vcd_trace_file( atf );

  return 0;

}
