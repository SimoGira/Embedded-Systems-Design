import numpy as np 
from pyfmi import load_fmu
from pylab import *
import random
import math
import sys
from  constants import *
import matplotlib.pyplot as plt




#------------------------------------------------------------------------------
# ACCELEROMETER VALUE NORMALIZER FUNCTION
#------------------------------------------------------------------------------
def accelerometer_normalizer(accelerometer):

	cap1 = accelerometer.get_real( ACCELEROMETER_CAP1 )
	cap2 = accelerometer.get_real( ACCELEROMETER_CAP2 )
	cap5 = accelerometer.get_real( ACCELEROMETER_CAP5 )
	cap7 = accelerometer.get_real( ACCELEROMETER_CAP7 )

	final_cap = (int(math.floor((((cap1+cap2+cap5+cap7)/4)*1e22))+20) & 0xffff)

	return final_cap

#------------------------------------------------------------------------------
# DATA EXCHANGE BETWEEN THE MODELS
#------------------------------------------------------------------------------
def data_exchange(m6502,memory,sine1_y,sine2_y,sine3_y,accelerometer,gain,clk,i):


	#----------------------------------------------------------------------------
	# ACCELEROMETER SETTING
	#----------------------------------------------------------------------------

	accelerometer.set_boolean(ACCELEROMETER_CLK,	clk )

	# set with the value only new sin values occures
	if i==1:
		accelerometer.set_real(ACCELEROMETER_AVX,		sine1_y	)
		accelerometer.set_real(ACCELEROMETER_AVY,		sine2_y	)
		accelerometer.set_real(ACCELEROMETER_AVZ,		sine3_y	)	


	#----------------------------------------------------------------------------
	# MEMORY SETTING
	#----------------------------------------------------------------------------
	memory.set_boolean(MEMORY_CLK,	clk )
	# From Cpu
	memory.set_integer(MEMORY_ADDR,				m6502.get_integer(M6502_ADDR) 	)	# addr
	memory.set_integer(MEMORY_DATAO,			m6502.get_integer(M6502_DATAO)	)	# datao
	memory.set_boolean(MEMORY_OEB,				m6502.get_boolean(M6502_OEB)	)	# oeb
	memory.set_boolean(MEMORY_SYNC,				m6502.get_boolean(M6502_SYNC)	)	# sync
	memory.set_boolean(MEMORY_VPAB,				m6502.get_boolean(M6502_VPAB)	)	# vpab
	memory.set_boolean(MEMORY_WE_N,				m6502.get_boolean(M6502_WE_N)	)	# we_n
	memory.set_integer(MEMORY_ACCELEROMETER,  	accelerometer_normalizer(accelerometer)  ) # value from accelerometer
	
	# From gain
	memory.set_integer(MEMORY_RESULT, 		gain.get_integer(GAIN_RESULT) )     # result
	memory.set_boolean(MEMORY_RESULT_RDY, 	gain.get_boolean(GAIN_RESULT_RDY) ) # result_rdy

	#----------------------------------------------------------------------------
	# M6502 SETTING
	#----------------------------------------------------------------------------
	
	m6502.set_boolean(M6502_CLK,	clk 								)	# clk
	m6502.set_integer(M6502_DATAI,	memory.get_integer(MEMORY_DATAI)	)	# datai
	m6502.set_boolean(M6502_IRQ_N,	memory.get_boolean(MEMORY_IRQ_N)	)	# irq_n
	m6502.set_boolean(M6502_NMI_N,	memory.get_boolean(MEMORY_NMI_N)	)	# nmi_n
	m6502.set_boolean(M6502_RES_N,	memory.get_boolean(MEMORY_RES_N)	)	# res_n
	m6502.set_boolean(M6502_RDY,	memory.get_boolean(MEMORY_RDY)		)	# rdy
	m6502.set_boolean(M6502_SOB_N,	memory.get_boolean(MEMORY_SOB_N)	)	# sob_n
	

	##---------------------------------------
	## GAIN SETTING
	##---------------------------------------

	gain.set_integer(GAIN_DATA,		memory.get_integer(MEMORY_DATA)		)
	gain.set_boolean(GAIN_DATA_RDY,	memory.get_boolean(MEMORY_DATA_RDY)	)




#----------------------------------------------------------------------------------------------------
#----------------------------------------------------------------------------------------------------
#- MAIN - MAIN - MAIN - MAIN - MAIN - MAIN - MAIN - MAIN - MAIN - MAIN - MAIN - MAIN - MAIN - MAIN --
#- MAIN - MAIN - MAIN - MAIN - MAIN - MAIN - MAIN - MAIN - MAIN - MAIN - MAIN - MAIN - MAIN - MAIN --
#- MAIN - MAIN - MAIN - MAIN - MAIN - MAIN - MAIN - MAIN - MAIN - MAIN - MAIN - MAIN - MAIN - MAIN --
#----------------------------------------------------------------------------------------------------
#----------------------------------------------------------------------------------------------------


#----------------------------------------------------------------------------------------------------
# 1. FMUs LOADING
#----------------------------------------------------------------------------------------------------

# log_level is used to set the fmu's  log level. 
# NOTHING: do not use log  
# VERBOSE: log everything in file. <fmuname>_log.txt

memory 			= load_fmu( './fmus/memory.fmu',			log_level = NOTHING )
m6502  			= load_fmu( './fmus/m6502.fmu',				log_level = NOTHING )
accelerometer 	= load_fmu( './fmus/accelerometer.fmu',		log_level = NOTHING )
sine1 			= load_fmu( './fmus/Sine_1.fmu',			log_level = NOTHING )
sine2 			= load_fmu( './fmus/Sine_2.fmu',			log_level = NOTHING )
sine3 			= load_fmu( './fmus/Sine_3.fmu',			log_level = NOTHING )

# Missing gain FMU....
gain 			= load_fmu( './fmus/gain.fmu', 				log_level = NOTHING )


#----------------------------------------------------------------------------------------------------
# 2. FMUs INITIALIZATION
#----------------------------------------------------------------------------------------------------

# Initialize method call fmi2SetupExperiment
accelerometer.initialize()
m6502.initialize()
memory.initialize()
sine1.initialize()
sine2.initialize()
sine3.initialize()

# Missing gain initialization....
gain.initialize()

# Global time and  different simulation steps
time = 0.

digital_step = 20e-6    # 20mu
analog_step  = 40e-6    # 40mu

# initial value of clock for digital models
clk=False


t= []
y= []
y2= []

sine1_y = 0.
sine2_y = 0.
sine3_y = 0.


#----------------------------------------------------------------------------------------------------
# 3. MAIN ROUTINE
#----------------------------------------------------------------------------------------------------

while time<1. :	


		#--------------------------------
		# 3.1 SIMULATION DIGITAL STEP
		#--------------------------------

		#this loop simulates the digital part TWICE than the continuous part. 
		for i in range(1,3):

			#  Digital Simulation Step
			memory.do_step(			current_t = time, step_size = digital_step )
			m6502.do_step(			current_t = time, step_size = digital_step )
			accelerometer.do_step(	current_t = time, step_size = digital_step )
			
			# Missing gain simulation .....
			gain.do_step(           current_t = time, step_size = digital_step )

			# Tracing some values
			t.append( time )						  # trace Time	
			y.append( gain.get_integer(GAIN_RESULT) ) # trace values from the Gain
			y2.append( accelerometer.get("Cap1") )	  # trace one value from the accelerometer	

			# Digital clock update
			clk = not clk

			# Data Exchange between models  		(GAIN DOES NOT EXIST YET)
			data_exchange( m6502, memory, sine1_y, sine2_y, sine3_y, accelerometer, gain, clk, i )
			
			# Update the global time
			time = time + digital_step
		

		#----------------------------------
		# 3.2 SIMULATION CONTINUOUS STEP
		#----------------------------------

		# Sine simulation
		sine1.do_step( current_t = time, step_size = analog_step )
		sine2.do_step( current_t = time, step_size = analog_step )
		sine3.do_step( current_t = time, step_size = analog_step )

		# Retrieve the value of the waves
		sine1_y = sine1.get_real( SINE1_Y )
		sine2_y = sine2.get_real( SINE2_Y )
		sine3_y = sine3.get_real( SINE3_Y )




f1 = plt.figure()
f2 = plt.figure()

ax2 = f2.add_subplot(111)
ax2.plot(t,y2)
ax1 = f1.add_subplot(111)
ax1.plot(t,y)
grid(True)
show()	


