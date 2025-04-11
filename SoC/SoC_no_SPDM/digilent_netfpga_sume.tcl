
# Create Project

create_project -force -name digilent_netfpga_sume -part xc7vx690tffg1761-3
set_msg_config -id {Common 17-55} -new_severity {Warning}

# Add project commands


# Add Sources

read_verilog {src/freechips.rocketchip.system.LitexConfig_linux_1_8.v}
read_verilog {src/freechips.rocketchip.system.LitexConfig_linux_1_8.behav_srams.v}
read_verilog {src/plusarg_reader.v}
read_verilog {src/AsyncResetReg.v}
read_verilog {src/EICG_wrapper.v}
read_verilog {src/digilent_netfpga_sume.v}

# Add EDIFs


# Add IPs


# Add constraints

read_xdc src/digilent_netfpga_sume.xdc
set_property PROCESSING_ORDER EARLY [get_files src/digilent_netfpga_sume.xdc]

# Add pre-synthesis commands


# Synthesis

synth_design -directive default -top digilent_netfpga_sume -part xc7vx690tffg1761-3

# Synthesis report

report_timing_summary -file digilent_netfpga_sume_timing_synth.rpt
report_utilization -hierarchical -file digilent_netfpga_sume_utilization_hierarchical_synth.rpt
report_utilization -file digilent_netfpga_sume_utilization_synth.rpt
write_checkpoint -force digilent_netfpga_sume_synth.dcp

# Add pre-optimize commands


# Optimize design

opt_design -directive default

# Add pre-placement commands


# Placement

place_design -directive default

# Placement report

report_utilization -hierarchical -file digilent_netfpga_sume_utilization_hierarchical_place.rpt
report_utilization -file digilent_netfpga_sume_utilization_place.rpt
report_io -file digilent_netfpga_sume_io.rpt
report_control_sets -verbose -file digilent_netfpga_sume_control_sets.rpt
report_clock_utilization -file digilent_netfpga_sume_clock_utilization.rpt
write_checkpoint -force digilent_netfpga_sume_place.dcp

# Add pre-routing commands


# Routing

route_design -directive default
phys_opt_design -directive default
write_checkpoint -force digilent_netfpga_sume_route.dcp

# Routing report

report_timing_summary -no_header -no_detailed_paths
report_route_status -file digilent_netfpga_sume_route_status.rpt
report_drc -file digilent_netfpga_sume_drc.rpt
report_timing_summary -datasheet -max_paths 10 -file digilent_netfpga_sume_timing.rpt
report_power -file digilent_netfpga_sume_power.rpt

# Bitstream generation

write_bitstream -force digilent_netfpga_sume.bit 

# End
