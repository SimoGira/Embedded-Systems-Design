onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate /xtea/clk
add wave -noupdate /xtea/rst
add wave -noupdate /xtea/din_rdy
add wave -noupdate /xtea/word0_in
add wave -noupdate /xtea/word1_in
add wave -noupdate /xtea/key0_in
add wave -noupdate /xtea/key1_in
add wave -noupdate /xtea/key2_in
add wave -noupdate /xtea/key3_in
add wave -noupdate /xtea/mode_in
add wave -noupdate -color Magenta /xtea/result0_out
add wave -noupdate -color Magenta /xtea/result1_out
add wave -noupdate /xtea/dout_rdy
add wave -noupdate /xtea/STATUS
add wave -noupdate /xtea/NEXT_STATUS
add wave -noupdate /xtea/mode
add wave -noupdate /xtea/i
add wave -noupdate /xtea/delta
add wave -noupdate /xtea/v0
add wave -noupdate /xtea/v1
add wave -noupdate /xtea/temp
add wave -noupdate /xtea/key0
add wave -noupdate /xtea/key1
add wave -noupdate /xtea/key2
add wave -noupdate /xtea/key3
add wave -noupdate /xtea/sum
add wave -noupdate /xtea/v64
add wave -noupdate /xtea/mode
add wave -noupdate /xtea/i
add wave -noupdate /xtea/v0
add wave -noupdate /xtea/v1
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {2630 ns} 0}
quietly wave cursor active 1
configure wave -namecolwidth 171
configure wave -valuecolwidth 159
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ns
update
WaveRestoreZoom {2594 ns} {2666 ns}
