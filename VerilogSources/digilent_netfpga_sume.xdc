################################################################################
# IO constraints
################################################################################
# cpu_reset_n:0
set_property LOC AR13 [get_ports {cpu_reset_n}]
set_property IOSTANDARD LVCMOS15 [get_ports {cpu_reset_n}]
set_property VCCAUX_IO NORMAL [get_ports {cpu_reset_n}]

# clk200:0.p
set_property LOC H19 [get_ports {clk200_p}]
set_property SLEW FAST [get_ports {clk200_p}]
set_property VCCAUX_IO NORMAL [get_ports {clk200_p}]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {clk200_p}]

# clk200:0.n
set_property LOC G18 [get_ports {clk200_n}]
set_property SLEW FAST [get_ports {clk200_n}]
set_property VCCAUX_IO NORMAL [get_ports {clk200_n}]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {clk200_n}]

# serial:0.rx
set_property LOC AY19 [get_ports {serial_rx}]
set_property IOSTANDARD LVCMOS15 [get_ports {serial_rx}]
set_property VCCAUX_IO NORMAL [get_ports {serial_rx}]

# serial:0.tx
set_property LOC BA19 [get_ports {serial_tx}]
set_property IOSTANDARD LVCMOS15 [get_ports {serial_tx}]
set_property VCCAUX_IO NORMAL [get_ports {serial_tx}]

# serial:0.rts
set_property LOC BB16 [get_ports {serial_rts}]
set_property IOSTANDARD LVCMOS15 [get_ports {serial_rts}]
set_property VCCAUX_IO NORMAL [get_ports {serial_rts}]

# serial:0.cts
set_property LOC BA16 [get_ports {serial_cts}]
set_property IOSTANDARD LVCMOS15 [get_ports {serial_cts}]
set_property VCCAUX_IO NORMAL [get_ports {serial_cts}]

# ddram:0.a
set_property LOC F35 [get_ports {ddram_a[0]}]
set_property SLEW FAST [get_ports {ddram_a[0]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_a[0]}]
set_property IOSTANDARD SSTL15 [get_ports {ddram_a[0]}]

# ddram:0.a
set_property LOC B36 [get_ports {ddram_a[1]}]
set_property SLEW FAST [get_ports {ddram_a[1]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_a[1]}]
set_property IOSTANDARD SSTL15 [get_ports {ddram_a[1]}]

# ddram:0.a
set_property LOC H33 [get_ports {ddram_a[2]}]
set_property SLEW FAST [get_ports {ddram_a[2]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_a[2]}]
set_property IOSTANDARD SSTL15 [get_ports {ddram_a[2]}]

# ddram:0.a
set_property LOC D32 [get_ports {ddram_a[3]}]
set_property SLEW FAST [get_ports {ddram_a[3]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_a[3]}]
set_property IOSTANDARD SSTL15 [get_ports {ddram_a[3]}]

# ddram:0.a
set_property LOC B34 [get_ports {ddram_a[4]}]
set_property SLEW FAST [get_ports {ddram_a[4]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_a[4]}]
set_property IOSTANDARD SSTL15 [get_ports {ddram_a[4]}]

# ddram:0.a
set_property LOC A36 [get_ports {ddram_a[5]}]
set_property SLEW FAST [get_ports {ddram_a[5]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_a[5]}]
set_property IOSTANDARD SSTL15 [get_ports {ddram_a[5]}]

# ddram:0.a
set_property LOC E33 [get_ports {ddram_a[6]}]
set_property SLEW FAST [get_ports {ddram_a[6]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_a[6]}]
set_property IOSTANDARD SSTL15 [get_ports {ddram_a[6]}]

# ddram:0.a
set_property LOC E32 [get_ports {ddram_a[7]}]
set_property SLEW FAST [get_ports {ddram_a[7]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_a[7]}]
set_property IOSTANDARD SSTL15 [get_ports {ddram_a[7]}]

# ddram:0.a
set_property LOC C33 [get_ports {ddram_a[8]}]
set_property SLEW FAST [get_ports {ddram_a[8]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_a[8]}]
set_property IOSTANDARD SSTL15 [get_ports {ddram_a[8]}]

# ddram:0.a
set_property LOC F34 [get_ports {ddram_a[9]}]
set_property SLEW FAST [get_ports {ddram_a[9]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_a[9]}]
set_property IOSTANDARD SSTL15 [get_ports {ddram_a[9]}]

# ddram:0.a
set_property LOC F36 [get_ports {ddram_a[10]}]
set_property SLEW FAST [get_ports {ddram_a[10]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_a[10]}]
set_property IOSTANDARD SSTL15 [get_ports {ddram_a[10]}]

# ddram:0.a
set_property LOC B32 [get_ports {ddram_a[11]}]
set_property SLEW FAST [get_ports {ddram_a[11]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_a[11]}]
set_property IOSTANDARD SSTL15 [get_ports {ddram_a[11]}]

# ddram:0.a
set_property LOC C34 [get_ports {ddram_a[12]}]
set_property SLEW FAST [get_ports {ddram_a[12]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_a[12]}]
set_property IOSTANDARD SSTL15 [get_ports {ddram_a[12]}]

# ddram:0.a
set_property LOC E37 [get_ports {ddram_a[13]}]
set_property SLEW FAST [get_ports {ddram_a[13]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_a[13]}]
set_property IOSTANDARD SSTL15 [get_ports {ddram_a[13]}]

# ddram:0.a
set_property LOC F32 [get_ports {ddram_a[14]}]
set_property SLEW FAST [get_ports {ddram_a[14]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_a[14]}]
set_property IOSTANDARD SSTL15 [get_ports {ddram_a[14]}]

# ddram:0.a
set_property LOC G33 [get_ports {ddram_a[15]}]
set_property SLEW FAST [get_ports {ddram_a[15]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_a[15]}]
set_property IOSTANDARD SSTL15 [get_ports {ddram_a[15]}]

# ddram:0.ba
set_property LOC B37 [get_ports {ddram_ba[0]}]
set_property SLEW FAST [get_ports {ddram_ba[0]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_ba[0]}]
set_property IOSTANDARD SSTL15 [get_ports {ddram_ba[0]}]

# ddram:0.ba
set_property LOC A35 [get_ports {ddram_ba[1]}]
set_property SLEW FAST [get_ports {ddram_ba[1]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_ba[1]}]
set_property IOSTANDARD SSTL15 [get_ports {ddram_ba[1]}]

# ddram:0.ba
set_property LOC A34 [get_ports {ddram_ba[2]}]
set_property SLEW FAST [get_ports {ddram_ba[2]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_ba[2]}]
set_property IOSTANDARD SSTL15 [get_ports {ddram_ba[2]}]

# ddram:0.ras_n
set_property LOC D38 [get_ports {ddram_ras_n}]
set_property SLEW FAST [get_ports {ddram_ras_n}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_ras_n}]
set_property IOSTANDARD SSTL15 [get_ports {ddram_ras_n}]

# ddram:0.cas_n
set_property LOC D37 [get_ports {ddram_cas_n}]
set_property SLEW FAST [get_ports {ddram_cas_n}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_cas_n}]
set_property IOSTANDARD SSTL15 [get_ports {ddram_cas_n}]

# ddram:0.we_n
set_property LOC A37 [get_ports {ddram_we_n}]
set_property SLEW FAST [get_ports {ddram_we_n}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_we_n}]
set_property IOSTANDARD SSTL15 [get_ports {ddram_we_n}]

# ddram:0.dm
set_property LOC K28 [get_ports {ddram_dm[0]}]
set_property SLEW FAST [get_ports {ddram_dm[0]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dm[0]}]
set_property IOSTANDARD SSTL15 [get_ports {ddram_dm[0]}]

# ddram:0.dm
set_property LOC G24 [get_ports {ddram_dm[1]}]
set_property SLEW FAST [get_ports {ddram_dm[1]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dm[1]}]
set_property IOSTANDARD SSTL15 [get_ports {ddram_dm[1]}]

# ddram:0.dm
set_property LOC L24 [get_ports {ddram_dm[2]}]
set_property SLEW FAST [get_ports {ddram_dm[2]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dm[2]}]
set_property IOSTANDARD SSTL15 [get_ports {ddram_dm[2]}]

# ddram:0.dm
set_property LOC N26 [get_ports {ddram_dm[3]}]
set_property SLEW FAST [get_ports {ddram_dm[3]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dm[3]}]
set_property IOSTANDARD SSTL15 [get_ports {ddram_dm[3]}]

# ddram:0.dm
set_property LOC W30 [get_ports {ddram_dm[4]}]
set_property SLEW FAST [get_ports {ddram_dm[4]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dm[4]}]
set_property IOSTANDARD SSTL15 [get_ports {ddram_dm[4]}]

# ddram:0.dm
set_property LOC M31 [get_ports {ddram_dm[5]}]
set_property SLEW FAST [get_ports {ddram_dm[5]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dm[5]}]
set_property IOSTANDARD SSTL15 [get_ports {ddram_dm[5]}]

# ddram:0.dm
set_property LOC J32 [get_ports {ddram_dm[6]}]
set_property SLEW FAST [get_ports {ddram_dm[6]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dm[6]}]
set_property IOSTANDARD SSTL15 [get_ports {ddram_dm[6]}]

# ddram:0.dm
set_property LOC L31 [get_ports {ddram_dm[7]}]
set_property SLEW FAST [get_ports {ddram_dm[7]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dm[7]}]
set_property IOSTANDARD SSTL15 [get_ports {ddram_dm[7]}]

# ddram:0.dq
set_property LOC K27 [get_ports {ddram_dq[0]}]
set_property SLEW FAST [get_ports {ddram_dq[0]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[0]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[0]}]

# ddram:0.dq
set_property LOC K24 [get_ports {ddram_dq[1]}]
set_property SLEW FAST [get_ports {ddram_dq[1]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[1]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[1]}]

# ddram:0.dq
set_property LOC J27 [get_ports {ddram_dq[2]}]
set_property SLEW FAST [get_ports {ddram_dq[2]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[2]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[2]}]

# ddram:0.dq
set_property LOC J28 [get_ports {ddram_dq[3]}]
set_property SLEW FAST [get_ports {ddram_dq[3]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[3]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[3]}]

# ddram:0.dq
set_property LOC K25 [get_ports {ddram_dq[4]}]
set_property SLEW FAST [get_ports {ddram_dq[4]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[4]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[4]}]

# ddram:0.dq
set_property LOC J25 [get_ports {ddram_dq[5]}]
set_property SLEW FAST [get_ports {ddram_dq[5]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[5]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[5]}]

# ddram:0.dq
set_property LOC G28 [get_ports {ddram_dq[6]}]
set_property SLEW FAST [get_ports {ddram_dq[6]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[6]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[6]}]

# ddram:0.dq
set_property LOC G29 [get_ports {ddram_dq[7]}]
set_property SLEW FAST [get_ports {ddram_dq[7]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[7]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[7]}]

# ddram:0.dq
set_property LOC H21 [get_ports {ddram_dq[8]}]
set_property SLEW FAST [get_ports {ddram_dq[8]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[8]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[8]}]

# ddram:0.dq
set_property LOC H24 [get_ports {ddram_dq[9]}]
set_property SLEW FAST [get_ports {ddram_dq[9]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[9]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[9]}]

# ddram:0.dq
set_property LOC H23 [get_ports {ddram_dq[10]}]
set_property SLEW FAST [get_ports {ddram_dq[10]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[10]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[10]}]

# ddram:0.dq
set_property LOC G21 [get_ports {ddram_dq[11]}]
set_property SLEW FAST [get_ports {ddram_dq[11]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[11]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[11]}]

# ddram:0.dq
set_property LOC J21 [get_ports {ddram_dq[12]}]
set_property SLEW FAST [get_ports {ddram_dq[12]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[12]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[12]}]

# ddram:0.dq
set_property LOC G22 [get_ports {ddram_dq[13]}]
set_property SLEW FAST [get_ports {ddram_dq[13]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[13]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[13]}]

# ddram:0.dq
set_property LOC G26 [get_ports {ddram_dq[14]}]
set_property SLEW FAST [get_ports {ddram_dq[14]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[14]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[14]}]

# ddram:0.dq
set_property LOC G27 [get_ports {ddram_dq[15]}]
set_property SLEW FAST [get_ports {ddram_dq[15]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[15]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[15]}]

# ddram:0.dq
set_property LOC M21 [get_ports {ddram_dq[16]}]
set_property SLEW FAST [get_ports {ddram_dq[16]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[16]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[16]}]

# ddram:0.dq
set_property LOC M24 [get_ports {ddram_dq[17]}]
set_property SLEW FAST [get_ports {ddram_dq[17]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[17]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[17]}]

# ddram:0.dq
set_property LOC J22 [get_ports {ddram_dq[18]}]
set_property SLEW FAST [get_ports {ddram_dq[18]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[18]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[18]}]

# ddram:0.dq
set_property LOC J23 [get_ports {ddram_dq[19]}]
set_property SLEW FAST [get_ports {ddram_dq[19]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[19]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[19]}]

# ddram:0.dq
set_property LOC K22 [get_ports {ddram_dq[20]}]
set_property SLEW FAST [get_ports {ddram_dq[20]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[20]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[20]}]

# ddram:0.dq
set_property LOC K23 [get_ports {ddram_dq[21]}]
set_property SLEW FAST [get_ports {ddram_dq[21]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[21]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[21]}]

# ddram:0.dq
set_property LOC L25 [get_ports {ddram_dq[22]}]
set_property SLEW FAST [get_ports {ddram_dq[22]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[22]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[22]}]

# ddram:0.dq
set_property LOC L26 [get_ports {ddram_dq[23]}]
set_property SLEW FAST [get_ports {ddram_dq[23]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[23]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[23]}]

# ddram:0.dq
set_property LOC P21 [get_ports {ddram_dq[24]}]
set_property SLEW FAST [get_ports {ddram_dq[24]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[24]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[24]}]

# ddram:0.dq
set_property LOC P26 [get_ports {ddram_dq[25]}]
set_property SLEW FAST [get_ports {ddram_dq[25]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[25]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[25]}]

# ddram:0.dq
set_property LOC N23 [get_ports {ddram_dq[26]}]
set_property SLEW FAST [get_ports {ddram_dq[26]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[26]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[26]}]

# ddram:0.dq
set_property LOC N24 [get_ports {ddram_dq[27]}]
set_property SLEW FAST [get_ports {ddram_dq[27]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[27]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[27]}]

# ddram:0.dq
set_property LOC P25 [get_ports {ddram_dq[28]}]
set_property SLEW FAST [get_ports {ddram_dq[28]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[28]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[28]}]

# ddram:0.dq
set_property LOC N25 [get_ports {ddram_dq[29]}]
set_property SLEW FAST [get_ports {ddram_dq[29]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[29]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[29]}]

# ddram:0.dq
set_property LOC L27 [get_ports {ddram_dq[30]}]
set_property SLEW FAST [get_ports {ddram_dq[30]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[30]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[30]}]

# ddram:0.dq
set_property LOC M27 [get_ports {ddram_dq[31]}]
set_property SLEW FAST [get_ports {ddram_dq[31]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[31]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[31]}]

# ddram:0.dq
set_property LOC U29 [get_ports {ddram_dq[32]}]
set_property SLEW FAST [get_ports {ddram_dq[32]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[32]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[32]}]

# ddram:0.dq
set_property LOC V30 [get_ports {ddram_dq[33]}]
set_property SLEW FAST [get_ports {ddram_dq[33]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[33]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[33]}]

# ddram:0.dq
set_property LOC U31 [get_ports {ddram_dq[34]}]
set_property SLEW FAST [get_ports {ddram_dq[34]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[34]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[34]}]

# ddram:0.dq
set_property LOC V31 [get_ports {ddram_dq[35]}]
set_property SLEW FAST [get_ports {ddram_dq[35]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[35]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[35]}]

# ddram:0.dq
set_property LOC Y29 [get_ports {ddram_dq[36]}]
set_property SLEW FAST [get_ports {ddram_dq[36]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[36]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[36]}]

# ddram:0.dq
set_property LOC V29 [get_ports {ddram_dq[37]}]
set_property SLEW FAST [get_ports {ddram_dq[37]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[37]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[37]}]

# ddram:0.dq
set_property LOC W31 [get_ports {ddram_dq[38]}]
set_property SLEW FAST [get_ports {ddram_dq[38]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[38]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[38]}]

# ddram:0.dq
set_property LOC Y30 [get_ports {ddram_dq[39]}]
set_property SLEW FAST [get_ports {ddram_dq[39]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[39]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[39]}]

# ddram:0.dq
set_property LOC N29 [get_ports {ddram_dq[40]}]
set_property SLEW FAST [get_ports {ddram_dq[40]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[40]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[40]}]

# ddram:0.dq
set_property LOC N30 [get_ports {ddram_dq[41]}]
set_property SLEW FAST [get_ports {ddram_dq[41]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[41]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[41]}]

# ddram:0.dq
set_property LOC R30 [get_ports {ddram_dq[42]}]
set_property SLEW FAST [get_ports {ddram_dq[42]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[42]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[42]}]

# ddram:0.dq
set_property LOC N31 [get_ports {ddram_dq[43]}]
set_property SLEW FAST [get_ports {ddram_dq[43]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[43]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[43]}]

# ddram:0.dq
set_property LOC R28 [get_ports {ddram_dq[44]}]
set_property SLEW FAST [get_ports {ddram_dq[44]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[44]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[44]}]

# ddram:0.dq
set_property LOC N28 [get_ports {ddram_dq[45]}]
set_property SLEW FAST [get_ports {ddram_dq[45]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[45]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[45]}]

# ddram:0.dq
set_property LOC P28 [get_ports {ddram_dq[46]}]
set_property SLEW FAST [get_ports {ddram_dq[46]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[46]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[46]}]

# ddram:0.dq
set_property LOC P30 [get_ports {ddram_dq[47]}]
set_property SLEW FAST [get_ports {ddram_dq[47]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[47]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[47]}]

# ddram:0.dq
set_property LOC J35 [get_ports {ddram_dq[48]}]
set_property SLEW FAST [get_ports {ddram_dq[48]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[48]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[48]}]

# ddram:0.dq
set_property LOC K35 [get_ports {ddram_dq[49]}]
set_property SLEW FAST [get_ports {ddram_dq[49]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[49]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[49]}]

# ddram:0.dq
set_property LOC L35 [get_ports {ddram_dq[50]}]
set_property SLEW FAST [get_ports {ddram_dq[50]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[50]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[50]}]

# ddram:0.dq
set_property LOC M34 [get_ports {ddram_dq[51]}]
set_property SLEW FAST [get_ports {ddram_dq[51]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[51]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[51]}]

# ddram:0.dq
set_property LOC M33 [get_ports {ddram_dq[52]}]
set_property SLEW FAST [get_ports {ddram_dq[52]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[52]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[52]}]

# ddram:0.dq
set_property LOC L34 [get_ports {ddram_dq[53]}]
set_property SLEW FAST [get_ports {ddram_dq[53]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[53]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[53]}]

# ddram:0.dq
set_property LOC J33 [get_ports {ddram_dq[54]}]
set_property SLEW FAST [get_ports {ddram_dq[54]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[54]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[54]}]

# ddram:0.dq
set_property LOC H34 [get_ports {ddram_dq[55]}]
set_property SLEW FAST [get_ports {ddram_dq[55]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[55]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[55]}]

# ddram:0.dq
set_property LOC J31 [get_ports {ddram_dq[56]}]
set_property SLEW FAST [get_ports {ddram_dq[56]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[56]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[56]}]

# ddram:0.dq
set_property LOC K30 [get_ports {ddram_dq[57]}]
set_property SLEW FAST [get_ports {ddram_dq[57]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[57]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[57]}]

# ddram:0.dq
set_property LOC L32 [get_ports {ddram_dq[58]}]
set_property SLEW FAST [get_ports {ddram_dq[58]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[58]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[58]}]

# ddram:0.dq
set_property LOC M32 [get_ports {ddram_dq[59]}]
set_property SLEW FAST [get_ports {ddram_dq[59]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[59]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[59]}]

# ddram:0.dq
set_property LOC K29 [get_ports {ddram_dq[60]}]
set_property SLEW FAST [get_ports {ddram_dq[60]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[60]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[60]}]

# ddram:0.dq
set_property LOC H30 [get_ports {ddram_dq[61]}]
set_property SLEW FAST [get_ports {ddram_dq[61]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[61]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[61]}]

# ddram:0.dq
set_property LOC J30 [get_ports {ddram_dq[62]}]
set_property SLEW FAST [get_ports {ddram_dq[62]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[62]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[62]}]

# ddram:0.dq
set_property LOC H31 [get_ports {ddram_dq[63]}]
set_property SLEW FAST [get_ports {ddram_dq[63]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dq[63]}]
set_property IOSTANDARD SSTL15_T_DCI [get_ports {ddram_dq[63]}]

# ddram:0.dqs_p
set_property LOC H28 [get_ports {ddram_dqs_p[0]}]
set_property SLEW FAST [get_ports {ddram_dqs_p[0]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dqs_p[0]}]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddram_dqs_p[0]}]

# ddram:0.dqs_p
set_property LOC H25 [get_ports {ddram_dqs_p[1]}]
set_property SLEW FAST [get_ports {ddram_dqs_p[1]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dqs_p[1]}]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddram_dqs_p[1]}]

# ddram:0.dqs_p
set_property LOC M22 [get_ports {ddram_dqs_p[2]}]
set_property SLEW FAST [get_ports {ddram_dqs_p[2]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dqs_p[2]}]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddram_dqs_p[2]}]

# ddram:0.dqs_p
set_property LOC P22 [get_ports {ddram_dqs_p[3]}]
set_property SLEW FAST [get_ports {ddram_dqs_p[3]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dqs_p[3]}]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddram_dqs_p[3]}]

# ddram:0.dqs_p
set_property LOC T29 [get_ports {ddram_dqs_p[4]}]
set_property SLEW FAST [get_ports {ddram_dqs_p[4]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dqs_p[4]}]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddram_dqs_p[4]}]

# ddram:0.dqs_p
set_property LOC M28 [get_ports {ddram_dqs_p[5]}]
set_property SLEW FAST [get_ports {ddram_dqs_p[5]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dqs_p[5]}]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddram_dqs_p[5]}]

# ddram:0.dqs_p
set_property LOC K33 [get_ports {ddram_dqs_p[6]}]
set_property SLEW FAST [get_ports {ddram_dqs_p[6]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dqs_p[6]}]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddram_dqs_p[6]}]

# ddram:0.dqs_p
set_property LOC L29 [get_ports {ddram_dqs_p[7]}]
set_property SLEW FAST [get_ports {ddram_dqs_p[7]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dqs_p[7]}]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddram_dqs_p[7]}]

# ddram:0.dqs_n
set_property LOC H29 [get_ports {ddram_dqs_n[0]}]
set_property SLEW FAST [get_ports {ddram_dqs_n[0]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dqs_n[0]}]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddram_dqs_n[0]}]

# ddram:0.dqs_n
set_property LOC H26 [get_ports {ddram_dqs_n[1]}]
set_property SLEW FAST [get_ports {ddram_dqs_n[1]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dqs_n[1]}]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddram_dqs_n[1]}]

# ddram:0.dqs_n
set_property LOC L22 [get_ports {ddram_dqs_n[2]}]
set_property SLEW FAST [get_ports {ddram_dqs_n[2]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dqs_n[2]}]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddram_dqs_n[2]}]

# ddram:0.dqs_n
set_property LOC P23 [get_ports {ddram_dqs_n[3]}]
set_property SLEW FAST [get_ports {ddram_dqs_n[3]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dqs_n[3]}]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddram_dqs_n[3]}]

# ddram:0.dqs_n
set_property LOC T30 [get_ports {ddram_dqs_n[4]}]
set_property SLEW FAST [get_ports {ddram_dqs_n[4]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dqs_n[4]}]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddram_dqs_n[4]}]

# ddram:0.dqs_n
set_property LOC M29 [get_ports {ddram_dqs_n[5]}]
set_property SLEW FAST [get_ports {ddram_dqs_n[5]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dqs_n[5]}]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddram_dqs_n[5]}]

# ddram:0.dqs_n
set_property LOC K34 [get_ports {ddram_dqs_n[6]}]
set_property SLEW FAST [get_ports {ddram_dqs_n[6]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dqs_n[6]}]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddram_dqs_n[6]}]

# ddram:0.dqs_n
set_property LOC L30 [get_ports {ddram_dqs_n[7]}]
set_property SLEW FAST [get_ports {ddram_dqs_n[7]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_dqs_n[7]}]
set_property IOSTANDARD DIFF_SSTL15_T_DCI [get_ports {ddram_dqs_n[7]}]

# ddram:0.clk_p
set_property LOC C35 [get_ports {ddram_clk_p[0]}]
set_property SLEW FAST [get_ports {ddram_clk_p[0]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_clk_p[0]}]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {ddram_clk_p[0]}]

# ddram:0.clk_p
set_property LOC D35 [get_ports {ddram_clk_p[1]}]
set_property SLEW FAST [get_ports {ddram_clk_p[1]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_clk_p[1]}]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {ddram_clk_p[1]}]

# ddram:0.clk_n
set_property LOC C36 [get_ports {ddram_clk_n[0]}]
set_property SLEW FAST [get_ports {ddram_clk_n[0]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_clk_n[0]}]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {ddram_clk_n[0]}]

# ddram:0.clk_n
set_property LOC D36 [get_ports {ddram_clk_n[1]}]
set_property SLEW FAST [get_ports {ddram_clk_n[1]}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_clk_n[1]}]
set_property IOSTANDARD DIFF_SSTL15 [get_ports {ddram_clk_n[1]}]

# ddram:0.cke
set_property LOC D33 [get_ports {ddram_cke}]
set_property SLEW FAST [get_ports {ddram_cke}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_cke}]
set_property IOSTANDARD SSTL15 [get_ports {ddram_cke}]

# ddram:0.odt
set_property LOC B39 [get_ports {ddram_odt}]
set_property SLEW FAST [get_ports {ddram_odt}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_odt}]
set_property IOSTANDARD SSTL15 [get_ports {ddram_odt}]

# ddram:0.cs_n
set_property LOC A39 [get_ports {ddram_cs_n}]
set_property SLEW FAST [get_ports {ddram_cs_n}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_cs_n}]
set_property IOSTANDARD SSTL15 [get_ports {ddram_cs_n}]

# ddram:0.reset_n
set_property LOC B33 [get_ports {ddram_reset_n}]
set_property SLEW FAST [get_ports {ddram_reset_n}]
set_property VCCAUX_IO NORMAL [get_ports {ddram_reset_n}]
set_property IOSTANDARD LVCMOS15 [get_ports {ddram_reset_n}]

# sfp:0.txp
set_property LOC A6 [get_ports {sfp_txp}]

# sfp:0.txn
set_property LOC A5 [get_ports {sfp_txn}]

# sfp:0.rxn
set_property LOC B3 [get_ports {sfp_rxn}]

# sfp:0.rxp
set_property LOC B4 [get_ports {sfp_rxp}]

# sfp_tx_disable_n:0
set_property LOC M18 [get_ports {sfp_tx_disable_n}]
set_property IOSTANDARD LVCMOS15 [get_ports {sfp_tx_disable_n}]

# user_led:0
set_property LOC AR22 [get_ports {user_led0}]
set_property IOSTANDARD LVCMOS15 [get_ports {user_led0}]
set_property VCCAUX_IO NORMAL [get_ports {user_led0}]

# user_led:1
set_property LOC AR23 [get_ports {user_led1}]
set_property IOSTANDARD LVCMOS15 [get_ports {user_led1}]
set_property VCCAUX_IO NORMAL [get_ports {user_led1}]

################################################################################
# Design constraints
################################################################################

set_property CFGBVS GND [current_design]

set_property CONFIG_VOLTAGE 1.8 [current_design]

set_property BITSTREAM.GENERAL.COMPRESS TRUE [current_design]

set_property INTERNAL_VREF 0.750 [get_iobanks 34]

set_property INTERNAL_VREF 0.750 [get_iobanks 35]

set_property INTERNAL_VREF 0.750 [get_iobanks 36]

set_property INTERNAL_VREF 0.750 [get_iobanks 37]

set_property INTERNAL_VREF 0.750 [get_iobanks 38]

set_property INTERNAL_VREF 0.750 [get_iobanks 39]

set_property SEVERITY {Warning} [get_drc_checks UCIO-1]

set_property SEVERITY {Warning} [get_drc_checks REQP-44]

################################################################################
# Clock constraints
################################################################################


create_clock -name clk200_p -period 5.0 [get_ports clk200_p]

create_clock -name eth_tx_clk -period 8.0 [get_nets eth_tx_clk]

create_clock -name eth_rx_clk -period 8.0 [get_nets eth_rx_clk]

set_clock_groups -group [get_clocks -include_generated_clocks -of [get_nets sys_clk]] -group [get_clocks -include_generated_clocks -of [get_nets main_crg_clkin]] -asynchronous

set_clock_groups -group [get_clocks -include_generated_clocks -of [get_nets sys_clk]] -group [get_clocks -include_generated_clocks -of [get_nets eth_tx_clk]] -asynchronous

set_clock_groups -group [get_clocks -include_generated_clocks -of [get_nets sys_clk]] -group [get_clocks -include_generated_clocks -of [get_nets eth_rx_clk]] -asynchronous

set_clock_groups -group [get_clocks -include_generated_clocks -of [get_nets eth_rx_clk]] -group [get_clocks -include_generated_clocks -of [get_nets eth_tx_clk]] -asynchronous

################################################################################
# False path constraints
################################################################################


set_false_path -quiet -through [get_nets -hierarchical -filter {mr_ff == TRUE}]

set_false_path -quiet -to [get_pins -filter {REF_PIN_NAME == PRE} -of_objects [get_cells -hierarchical -filter {ars_ff1 == TRUE || ars_ff2 == TRUE}]]

set_max_delay 2 -quiet -from [get_pins -filter {REF_PIN_NAME == C} -of_objects [get_cells -hierarchical -filter {ars_ff1 == TRUE}]] -to [get_pins -filter {REF_PIN_NAME == D} -of_objects [get_cells -hierarchical -filter {ars_ff2 == TRUE}]]