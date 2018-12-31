#include "watertank_lsf.hh"
#include "global.hh"

watertank_lsf::watertank_lsf( sc_core::sc_module_name _name ) :
  sc_core::sc_module( _name ),
  sig_valve_aperture("sig_valve_aperture"),
  sig_water_level("sig_water_level"),
  tdf2lsf( "tdf2lsf" ),
  lsf2rtl( "lsf2rtl" ),
  int1("int1" ),
  gain1("gain1", 0.6),
  gain2("gain2", 0.03),
  sub1("sub1"),
  sig1("sig1"),
  sig2("sig2"),
  sig3("sig3")
{
  tdf2lsf.inp( valve_aperture );
  tdf2lsf.y( sig_valve_aperture );

  lsf2rtl.x( sig_water_level );
  lsf2rtl.outp( water_level );

  gain1.x( sig_valve_aperture );
  gain1.y( sig1 );
  //gain1.set_timestep( 1, sc_core::SC_MS );

  sub1.x1( sig1 );
  sub1.x2( sig3 );
  sub1.y( sig2 );

  int1.x( sig2 );
  int1.y( sig_water_level );

  gain2.x( sig_water_level );
  gain2.y( sig3 );
}
