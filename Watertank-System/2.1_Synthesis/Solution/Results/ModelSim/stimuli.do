add wave *
add wave /xtea/mode
add wave /xtea/i
add wave /xtea/v0
add wave /xtea/v1

force clk 1 10 ns, 0 20 ns -repeat 20
force rst 1 0

#force word0_in <> 0
force word0_in 0x12345678 0
force word1_in 0x9abcdeff 0

# enchipered words ##############################
#force word0_in <> 0
#force word1_in <> 0
#################################################
force key0_in  0x6a1d78c8 0
force key1_in  0x8c86d67f 0
force key2_in  0x2a65bfbe 0
force key3_in  0xb4bd6e46 0
force din_rdy  1 10, 0 40
force mode_in  0 0

# dechiper mode ##
#force mode_in  1 0
##################

run 10000

#quit

