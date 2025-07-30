
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
read_vhdl    {src/mux_2to1.vhd}
read_vhdl    {src/bit_mux_2to1.vhd}

# Add EDIFs

# Add IPs

# Add constraints

read_xdc {src/digilent_netfpga_sume.xdc}
set_property PROCESSING_ORDER EARLY [get_files src/digilent_netfpga_sume.xdc]

# Set the reference directory for source file relative paths (by default the value is script directory path)
set origin_dir ""

# Use origin directory path location variable, if specified in the tcl shell
if { [info exists ::origin_dir_loc] } {
  set origin_dir $::origin_dir_loc
}

proc cr_bd_processor4 { parentCell } {
# The design that will be created by this Tcl proc contains the following 
# module references:
# bit_mux_2to1, bit_mux_2to1, mux_2to1, bit_mux_2to1



  # CHANGE DESIGN NAME HERE
  set design_name processor4

  common::send_gid_msg -ssname BD::TCL -id 2010 -severity "INFO" "Currently there is no design <$design_name> in project, so creating one..."

  create_bd_design $design_name

  set bCheckIPsPassed 1
  ##################################################################
  # CHECK IPs
  ##################################################################
  set bCheckIPs 1
  if { $bCheckIPs == 1 } {
     set list_check_ips "\ 
  xilinx.com:ip:microblaze:11.0\
  xilinx.com:ip:mdm:3.2\
  xilinx.com:ip:proc_sys_reset:5.0\
  xilinx.com:ip:axi_gpio:2.0\
  xilinx.com:ip:fifo_generator:13.2\
  xilinx.com:ip:lmb_v10:3.0\
  xilinx.com:ip:lmb_bram_if_cntlr:4.0\
  xilinx.com:ip:blk_mem_gen:8.4\
  "

   set list_ips_missing ""
   common::send_gid_msg -ssname BD::TCL -id 2011 -severity "INFO" "Checking if the following IPs exist in the project's IP catalog: $list_check_ips ."

   foreach ip_vlnv $list_check_ips {
      set ip_obj [get_ipdefs -all $ip_vlnv]
      if { $ip_obj eq "" } {
         lappend list_ips_missing $ip_vlnv
      }
   }

   if { $list_ips_missing ne "" } {
      catch {common::send_gid_msg -ssname BD::TCL -id 2012 -severity "ERROR" "The following IPs are not found in the IP Catalog:\n  $list_ips_missing\n\nResolution: Please add the repository containing the IP(s) to the project." }
      set bCheckIPsPassed 0
   }

  }

  ##################################################################
  # CHECK Modules
  ##################################################################
  set bCheckModules 1
  if { $bCheckModules == 1 } {
     set list_check_mods "\ 
  bit_mux_2to1\
  bit_mux_2to1\
  mux_2to1\
  bit_mux_2to1\
  "

   set list_mods_missing ""
   common::send_gid_msg -ssname BD::TCL -id 2020 -severity "INFO" "Checking if the following modules exist in the project's sources: $list_check_mods ."

   foreach mod_vlnv $list_check_mods {
      if { [can_resolve_reference $mod_vlnv] == 0 } {
         lappend list_mods_missing $mod_vlnv
      }
   }

   if { $list_mods_missing ne "" } {
      catch {common::send_gid_msg -ssname BD::TCL -id 2021 -severity "ERROR" "The following module(s) are not found in the project: $list_mods_missing" }
      common::send_gid_msg -ssname BD::TCL -id 2022 -severity "INFO" "Please add source files for the missing module(s) above."
      set bCheckIPsPassed 0
   }
}

  if { $bCheckIPsPassed != 1 } {
    common::send_gid_msg -ssname BD::TCL -id 2023 -severity "WARNING" "Will not continue with creation of design due to the error(s) above."
    return 3
  }

  
# Hierarchical cell: microblaze_0_local_memory
proc create_hier_cell_microblaze_0_local_memory { parentCell nameHier } {

  variable script_folder

  if { $parentCell eq "" || $nameHier eq "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2092 -severity "ERROR" "create_hier_cell_microblaze_0_local_memory() - Empty argument(s)!"}
     return
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2090 -severity "ERROR" "Unable to find parent cell <$parentCell>!"}
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2091 -severity "ERROR" "Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."}
     return
  }

  # Save current instance; Restore later
  set oldCurInst [current_bd_instance .]

  # Set parent object as current
  current_bd_instance $parentObj

  # Create cell and set as current instance
  set hier_obj [create_bd_cell -type hier $nameHier]
  current_bd_instance $hier_obj

  # Create interface pins
  create_bd_intf_pin -mode MirroredMaster -vlnv xilinx.com:interface:lmb_rtl:1.0 DLMB

  create_bd_intf_pin -mode MirroredMaster -vlnv xilinx.com:interface:lmb_rtl:1.0 ILMB


  # Create pins
  create_bd_pin -dir I -type clk Clk
  create_bd_pin -dir I -type rst SYS_Rst

  # Create instance: dlmb_v10, and set properties
  set dlmb_v10 [ create_bd_cell -type ip -vlnv xilinx.com:ip:lmb_v10:3.0 dlmb_v10 ]

  # Create instance: ilmb_v10, and set properties
  set ilmb_v10 [ create_bd_cell -type ip -vlnv xilinx.com:ip:lmb_v10:3.0 ilmb_v10 ]

  # Create instance: dlmb_bram_if_cntlr, and set properties
  set dlmb_bram_if_cntlr [ create_bd_cell -type ip -vlnv xilinx.com:ip:lmb_bram_if_cntlr:4.0 dlmb_bram_if_cntlr ]
  set_property CONFIG.C_ECC {0} $dlmb_bram_if_cntlr


  # Create instance: ilmb_bram_if_cntlr, and set properties
  set ilmb_bram_if_cntlr [ create_bd_cell -type ip -vlnv xilinx.com:ip:lmb_bram_if_cntlr:4.0 ilmb_bram_if_cntlr ]
  set_property CONFIG.C_ECC {0} $ilmb_bram_if_cntlr


  # Create instance: lmb_bram, and set properties
  set lmb_bram [ create_bd_cell -type ip -vlnv xilinx.com:ip:blk_mem_gen:8.4 lmb_bram ]
  set_property -dict [list \
    CONFIG.Memory_Type {True_Dual_Port_RAM} \
    CONFIG.use_bram_block {BRAM_Controller} \
  ] $lmb_bram


  # Create interface connections
  connect_bd_intf_net -intf_net microblaze_0_dlmb [get_bd_intf_pins dlmb_v10/LMB_M] [get_bd_intf_pins DLMB]
  connect_bd_intf_net -intf_net microblaze_0_dlmb_bus [get_bd_intf_pins dlmb_v10/LMB_Sl_0] [get_bd_intf_pins dlmb_bram_if_cntlr/SLMB]
  connect_bd_intf_net -intf_net microblaze_0_dlmb_cntlr [get_bd_intf_pins dlmb_bram_if_cntlr/BRAM_PORT] [get_bd_intf_pins lmb_bram/BRAM_PORTA]
  connect_bd_intf_net -intf_net microblaze_0_ilmb [get_bd_intf_pins ilmb_v10/LMB_M] [get_bd_intf_pins ILMB]
  connect_bd_intf_net -intf_net microblaze_0_ilmb_bus [get_bd_intf_pins ilmb_v10/LMB_Sl_0] [get_bd_intf_pins ilmb_bram_if_cntlr/SLMB]
  connect_bd_intf_net -intf_net microblaze_0_ilmb_cntlr [get_bd_intf_pins ilmb_bram_if_cntlr/BRAM_PORT] [get_bd_intf_pins lmb_bram/BRAM_PORTB]

  # Create port connections
  connect_bd_net -net SYS_Rst_1 [get_bd_pins SYS_Rst] [get_bd_pins dlmb_v10/SYS_Rst] [get_bd_pins dlmb_bram_if_cntlr/LMB_Rst] [get_bd_pins ilmb_v10/SYS_Rst] [get_bd_pins ilmb_bram_if_cntlr/LMB_Rst]
  connect_bd_net -net microblaze_0_Clk [get_bd_pins Clk] [get_bd_pins dlmb_v10/LMB_Clk] [get_bd_pins dlmb_bram_if_cntlr/LMB_Clk] [get_bd_pins ilmb_v10/LMB_Clk] [get_bd_pins ilmb_bram_if_cntlr/LMB_Clk]

  # Restore current instance
  current_bd_instance $oldCurInst
}
  variable script_folder

  if { $parentCell eq "" } {
     set parentCell [get_bd_cells /]
  }

  # Get object for parentCell
  set parentObj [get_bd_cells $parentCell]
  if { $parentObj == "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2090 -severity "ERROR" "Unable to find parent cell <$parentCell>!"}
     return
  }

  # Make sure parentObj is hier blk
  set parentType [get_property TYPE $parentObj]
  if { $parentType ne "hier" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2091 -severity "ERROR" "Parent <$parentObj> has TYPE = <$parentType>. Expected to be <hier>."}
     return
  }

  # Save current instance; Restore later
  set oldCurInst [current_bd_instance .]

  # Set parent object as current
  current_bd_instance $parentObj


  # Create interface ports
  set gpio_rtl_2 [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:gpio_rtl:1.0 gpio_rtl_2 ]

  set gpio_rtl_3 [ create_bd_intf_port -mode Master -vlnv xilinx.com:interface:gpio_rtl:1.0 gpio_rtl_3 ]


  # Create ports
  set Clk [ create_bd_port -dir I -type clk -freq_hz 125000000 Clk ]
  set reset_rtl_0 [ create_bd_port -dir I -type rst reset_rtl_0 ]
  set_property -dict [ list \
   CONFIG.POLARITY {ACTIVE_LOW} \
 ] $reset_rtl_0
  set fifo_input [ create_bd_port -dir I -from 31 -to 0 fifo_input ]
  set fifo_write_en [ create_bd_port -dir I fifo_write_en ]
  set clk_fifo [ create_bd_port -dir I clk_fifo ]
  set fifo_output [ create_bd_port -dir O -from 31 -to 0 fifo_output ]
  set data_mux [ create_bd_port -dir I data_mux ]
  set fifo_read_en [ create_bd_port -dir I fifo_read_en ]
  set Write_mux [ create_bd_port -dir I Write_mux ]
  set clk_mux [ create_bd_port -dir I clk_mux ]
  set Read_mux [ create_bd_port -dir I Read_mux ]
  set spdm_wait [ create_bd_port -dir I spdm_wait ]
  set spdm_wait_requester [ create_bd_port -dir O -from 0 -to 0 spdm_wait_requester ]

  # Create instance: microblaze_0, and set properties
  set microblaze_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:microblaze:11.0 microblaze_0 ]
  set_property -dict [list \
    CONFIG.C_DATA_SIZE {32} \
    CONFIG.C_DEBUG_ENABLED {1} \
    CONFIG.C_D_AXI {1} \
    CONFIG.C_D_LMB {1} \
    CONFIG.C_I_LMB {1} \
  ] $microblaze_0


  # Create instance: microblaze_0_local_memory
  create_hier_cell_microblaze_0_local_memory [current_bd_instance .] microblaze_0_local_memory

  # Create instance: mdm_1, and set properties
  set mdm_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:mdm:3.2 mdm_1 ]

  # Create instance: rst_Clk_100M, and set properties
  set rst_Clk_100M [ create_bd_cell -type ip -vlnv xilinx.com:ip:proc_sys_reset:5.0 rst_Clk_100M ]

  # Create instance: axi_gpio_0, and set properties
  set axi_gpio_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 axi_gpio_0 ]
  set_property -dict [list \
    CONFIG.C_ALL_INPUTS {1} \
    CONFIG.C_ALL_OUTPUTS_2 {1} \
    CONFIG.C_GPIO2_WIDTH {32} \
    CONFIG.C_INTERRUPT_PRESENT {0} \
    CONFIG.C_IS_DUAL {1} \
  ] $axi_gpio_0


  # Create instance: microblaze_0_axi_periph, and set properties
  set microblaze_0_axi_periph [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_interconnect:2.1 microblaze_0_axi_periph ]
  set_property -dict [list \
    CONFIG.NUM_MI {6} \
    CONFIG.NUM_SI {2} \
  ] $microblaze_0_axi_periph


  # Create instance: axi_gpio_1, and set properties
  set axi_gpio_1 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 axi_gpio_1 ]
  set_property -dict [list \
    CONFIG.C_ALL_INPUTS {1} \
    CONFIG.C_ALL_OUTPUTS_2 {1} \
    CONFIG.C_GPIO2_WIDTH {32} \
    CONFIG.C_GPIO_WIDTH {32} \
    CONFIG.C_IS_DUAL {1} \
  ] $axi_gpio_1


  # Create instance: fifo_generator_0, and set properties
  set fifo_generator_0 [ create_bd_cell -type ip -vlnv xilinx.com:ip:fifo_generator:13.2 fifo_generator_0 ]
  set_property -dict [list \
    CONFIG.Almost_Empty_Flag {false} \
    CONFIG.Almost_Full_Flag {false} \
    CONFIG.Data_Count {false} \
    CONFIG.Fifo_Implementation {Common_Clock_Distributed_RAM} \
    CONFIG.INTERFACE_TYPE {Native} \
    CONFIG.Input_Data_Width {32} \
    CONFIG.Performance_Options {First_Word_Fall_Through} \
    CONFIG.Reset_Pin {false} \
  ] $fifo_generator_0


  # Create instance: axi_gpio_3, and set properties
  set axi_gpio_3 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 axi_gpio_3 ]
  set_property -dict [list \
    CONFIG.C_ALL_INPUTS {0} \
    CONFIG.C_ALL_OUTPUTS {1} \
    CONFIG.C_ALL_OUTPUTS_2 {1} \
    CONFIG.C_GPIO2_WIDTH {1} \
    CONFIG.C_GPIO_WIDTH {1} \
    CONFIG.C_IS_DUAL {1} \
  ] $axi_gpio_3


  # Create instance: axi_gpio_4, and set properties
  set axi_gpio_4 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 axi_gpio_4 ]
  set_property -dict [list \
    CONFIG.C_ALL_INPUTS {0} \
    CONFIG.C_ALL_OUTPUTS {1} \
    CONFIG.C_GPIO_WIDTH {1} \
    CONFIG.C_IS_DUAL {0} \
  ] $axi_gpio_4


  # Create instance: bit_mux_2to1_0, and set properties
  set block_name bit_mux_2to1
  set block_cell_name bit_mux_2to1_0
  if { [catch {set bit_mux_2to1_0 [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2095 -severity "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   } elseif { $bit_mux_2to1_0 eq "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2096 -severity "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   }
  
  # Create instance: bit_mux_2to1_1, and set properties
  set block_name bit_mux_2to1
  set block_cell_name bit_mux_2to1_1
  if { [catch {set bit_mux_2to1_1 [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2095 -severity "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   } elseif { $bit_mux_2to1_1 eq "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2096 -severity "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   }
  
  # Create instance: mux_2to1_0, and set properties
  set block_name mux_2to1
  set block_cell_name mux_2to1_0
  if { [catch {set mux_2to1_0 [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2095 -severity "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   } elseif { $mux_2to1_0 eq "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2096 -severity "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   }
  
  # Create instance: bit_mux_2to1_2, and set properties
  set block_name bit_mux_2to1
  set block_cell_name bit_mux_2to1_2
  if { [catch {set bit_mux_2to1_2 [create_bd_cell -type module -reference $block_name $block_cell_name] } errmsg] } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2095 -severity "ERROR" "Unable to add referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   } elseif { $bit_mux_2to1_2 eq "" } {
     catch {common::send_gid_msg -ssname BD::TCL -id 2096 -severity "ERROR" "Unable to referenced block <$block_name>. Please add the files for ${block_name}'s definition into the project."}
     return 1
   }
  
  # Create instance: axi_gpio_5, and set properties
  set axi_gpio_5 [ create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 axi_gpio_5 ]
  set_property -dict [list \
    CONFIG.C_ALL_INPUTS {1} \
    CONFIG.C_ALL_OUTPUTS_2 {1} \
    CONFIG.C_GPIO2_WIDTH {1} \
    CONFIG.C_GPIO_WIDTH {1} \
    CONFIG.C_IS_DUAL {1} \
  ] $axi_gpio_5


  # Create interface connections
  connect_bd_intf_net -intf_net axi_gpio_1_GPIO [get_bd_intf_ports gpio_rtl_2] [get_bd_intf_pins axi_gpio_1/GPIO]
  connect_bd_intf_net -intf_net axi_gpio_1_GPIO2 [get_bd_intf_ports gpio_rtl_3] [get_bd_intf_pins axi_gpio_1/GPIO2]
  connect_bd_intf_net -intf_net microblaze_0_M_AXI_DP [get_bd_intf_pins microblaze_0/M_AXI_DP] [get_bd_intf_pins microblaze_0_axi_periph/S00_AXI]
  connect_bd_intf_net -intf_net microblaze_0_axi_periph_M00_AXI [get_bd_intf_pins microblaze_0_axi_periph/M00_AXI] [get_bd_intf_pins axi_gpio_0/S_AXI]
  connect_bd_intf_net -intf_net microblaze_0_axi_periph_M01_AXI [get_bd_intf_pins microblaze_0_axi_periph/M01_AXI] [get_bd_intf_pins axi_gpio_1/S_AXI]
  connect_bd_intf_net -intf_net microblaze_0_axi_periph_M03_AXI [get_bd_intf_pins microblaze_0_axi_periph/M03_AXI] [get_bd_intf_pins axi_gpio_3/S_AXI]
  connect_bd_intf_net -intf_net microblaze_0_axi_periph_M04_AXI [get_bd_intf_pins microblaze_0_axi_periph/M04_AXI] [get_bd_intf_pins axi_gpio_4/S_AXI]
  connect_bd_intf_net -intf_net microblaze_0_axi_periph_M05_AXI [get_bd_intf_pins microblaze_0_axi_periph/M05_AXI] [get_bd_intf_pins axi_gpio_5/S_AXI]
  connect_bd_intf_net -intf_net microblaze_0_debug [get_bd_intf_pins mdm_1/MBDEBUG_0] [get_bd_intf_pins microblaze_0/DEBUG]
  connect_bd_intf_net -intf_net microblaze_0_dlmb_1 [get_bd_intf_pins microblaze_0/DLMB] [get_bd_intf_pins microblaze_0_local_memory/DLMB]
  connect_bd_intf_net -intf_net microblaze_0_ilmb_1 [get_bd_intf_pins microblaze_0/ILMB] [get_bd_intf_pins microblaze_0_local_memory/ILMB]

  # Create port connections
  connect_bd_net -net Clk_1 [get_bd_ports Clk] [get_bd_pins axi_gpio_0/s_axi_aclk] [get_bd_pins microblaze_0_axi_periph/M00_ACLK] [get_bd_pins axi_gpio_1/s_axi_aclk] [get_bd_pins microblaze_0_axi_periph/M01_ACLK] [get_bd_pins microblaze_0_axi_periph/M02_ACLK] [get_bd_pins axi_gpio_3/s_axi_aclk] [get_bd_pins microblaze_0_axi_periph/M03_ACLK] [get_bd_pins axi_gpio_4/s_axi_aclk] [get_bd_pins microblaze_0_axi_periph/M04_ACLK] [get_bd_pins microblaze_0/Clk] [get_bd_pins microblaze_0_local_memory/Clk] [get_bd_pins microblaze_0_axi_periph/S00_ACLK] [get_bd_pins microblaze_0_axi_periph/ACLK] [get_bd_pins rst_Clk_100M/slowest_sync_clk] [get_bd_pins microblaze_0_axi_periph/S01_ACLK] [get_bd_pins axi_gpio_5/s_axi_aclk] [get_bd_pins microblaze_0_axi_periph/M05_ACLK]
  connect_bd_net -net Read_mux_1 [get_bd_ports Read_mux] [get_bd_pins bit_mux_2to1_1/selector]
  connect_bd_net -net Write_mux_1 [get_bd_ports Write_mux] [get_bd_pins bit_mux_2to1_0/selector]
  connect_bd_net -net axi_gpio_0_gpio2_io_o [get_bd_pins axi_gpio_0/gpio2_io_o] [get_bd_pins mux_2to1_0/B]
  connect_bd_net -net axi_gpio_3_gpio2_io_o [get_bd_pins axi_gpio_3/gpio2_io_o] [get_bd_pins bit_mux_2to1_1/B]
  connect_bd_net -net axi_gpio_3_gpio_io_o [get_bd_pins axi_gpio_3/gpio_io_o] [get_bd_pins bit_mux_2to1_0/B]
  connect_bd_net -net axi_gpio_4_gpio_io_o [get_bd_pins axi_gpio_4/gpio_io_o] [get_bd_pins bit_mux_2to1_2/B]
  connect_bd_net -net axi_gpio_5_gpio2_io_o [get_bd_pins axi_gpio_5/gpio2_io_o] [get_bd_ports spdm_wait_requester]
  connect_bd_net -net bit_mux_2to1_0_output [get_bd_pins bit_mux_2to1_0/mux_output] [get_bd_pins fifo_generator_0/wr_en]
  connect_bd_net -net bit_mux_2to1_1_output [get_bd_pins bit_mux_2to1_1/mux_output] [get_bd_pins fifo_generator_0/rd_en]
  connect_bd_net -net bit_mux_2to1_2_mux_output [get_bd_pins bit_mux_2to1_2/mux_output] [get_bd_pins fifo_generator_0/clk]
  connect_bd_net -net clk_fifo_1 [get_bd_ports clk_fifo] [get_bd_pins bit_mux_2to1_2/A]
  connect_bd_net -net clk_mux_1 [get_bd_ports clk_mux] [get_bd_pins bit_mux_2to1_2/selector]
  connect_bd_net -net data_mux_1 [get_bd_ports data_mux] [get_bd_pins mux_2to1_0/selector]
  connect_bd_net -net fifo_generator_0_dout [get_bd_pins fifo_generator_0/dout] [get_bd_pins axi_gpio_0/gpio_io_i] [get_bd_ports fifo_output]
  connect_bd_net -net fifo_input_1 [get_bd_ports fifo_input] [get_bd_pins mux_2to1_0/A]
  connect_bd_net -net fifo_read_en_1 [get_bd_ports fifo_read_en] [get_bd_pins bit_mux_2to1_1/A]
  connect_bd_net -net fifo_write_en_1 [get_bd_ports fifo_write_en] [get_bd_pins bit_mux_2to1_0/A]
  connect_bd_net -net mdm_1_debug_sys_rst [get_bd_pins mdm_1/Debug_SYS_Rst] [get_bd_pins rst_Clk_100M/mb_debug_sys_rst]
  connect_bd_net -net mux_2to1_0_output [get_bd_pins mux_2to1_0/mux_output] [get_bd_pins fifo_generator_0/din]
  connect_bd_net -net reset_rtl_0_1 [get_bd_ports reset_rtl_0] [get_bd_pins rst_Clk_100M/ext_reset_in]
  connect_bd_net -net rst_Clk_100M_bus_struct_reset [get_bd_pins rst_Clk_100M/bus_struct_reset] [get_bd_pins microblaze_0_local_memory/SYS_Rst]
  connect_bd_net -net rst_Clk_100M_mb_reset [get_bd_pins rst_Clk_100M/mb_reset] [get_bd_pins microblaze_0/Reset]
  connect_bd_net -net rst_Clk_100M_peripheral_aresetn [get_bd_pins rst_Clk_100M/peripheral_aresetn] [get_bd_pins microblaze_0_axi_periph/S00_ARESETN] [get_bd_pins axi_gpio_0/s_axi_aresetn] [get_bd_pins microblaze_0_axi_periph/M00_ARESETN] [get_bd_pins microblaze_0_axi_periph/ARESETN] [get_bd_pins axi_gpio_1/s_axi_aresetn] [get_bd_pins microblaze_0_axi_periph/M01_ARESETN] [get_bd_pins microblaze_0_axi_periph/M02_ARESETN] [get_bd_pins microblaze_0_axi_periph/M03_ARESETN] [get_bd_pins axi_gpio_3/s_axi_aresetn] [get_bd_pins microblaze_0_axi_periph/S01_ARESETN] [get_bd_pins axi_gpio_4/s_axi_aresetn] [get_bd_pins microblaze_0_axi_periph/M04_ARESETN] [get_bd_pins axi_gpio_5/s_axi_aresetn] [get_bd_pins microblaze_0_axi_periph/M05_ARESETN]
  connect_bd_net -net spdm_wait_1 [get_bd_ports spdm_wait] [get_bd_pins axi_gpio_5/gpio_io_i]

  # Create address segments
  assign_bd_address -offset 0x40000000 -range 0x00010000 -target_address_space [get_bd_addr_spaces microblaze_0/Data] [get_bd_addr_segs axi_gpio_0/S_AXI/Reg] -force
  assign_bd_address -offset 0x40010000 -range 0x00010000 -target_address_space [get_bd_addr_spaces microblaze_0/Data] [get_bd_addr_segs axi_gpio_1/S_AXI/Reg] -force
  assign_bd_address -offset 0x40030000 -range 0x00010000 -target_address_space [get_bd_addr_spaces microblaze_0/Data] [get_bd_addr_segs axi_gpio_3/S_AXI/Reg] -force
  assign_bd_address -offset 0x40040000 -range 0x00010000 -target_address_space [get_bd_addr_spaces microblaze_0/Data] [get_bd_addr_segs axi_gpio_4/S_AXI/Reg] -force
  assign_bd_address -offset 0x40050000 -range 0x00010000 -target_address_space [get_bd_addr_spaces microblaze_0/Data] [get_bd_addr_segs axi_gpio_5/S_AXI/Reg] -force
  assign_bd_address -offset 0x00000000 -range 0x00080000 -target_address_space [get_bd_addr_spaces microblaze_0/Data] [get_bd_addr_segs microblaze_0_local_memory/dlmb_bram_if_cntlr/SLMB/Mem] -force
  assign_bd_address -offset 0x00000000 -range 0x00080000 -target_address_space [get_bd_addr_spaces microblaze_0/Instruction] [get_bd_addr_segs microblaze_0_local_memory/ilmb_bram_if_cntlr/SLMB/Mem] -force


  # Restore current instance
  current_bd_instance $oldCurInst

  validate_bd_design
  save_bd_design
  close_bd_design $design_name 
}
# End of cr_bd_processor4()
cr_bd_processor4 ""
set_property REGISTERED_WITH_MANAGER "1" [get_files processor4.bd ] 
set_property SYNTH_CHECKPOINT_MODE "Hierarchical" [get_files processor4.bd ] 

#call make_wrapper to create wrapper files
set wrapper_path [make_wrapper -fileset sources_1 -files [ get_files -norecurse processor4.bd] -top]
add_files -norecurse -fileset sources_1 $wrapper_path

##################################################################
# MAIN FLOW
##################################################################

# Add pre-synthesis commands

generate_target all [get_files digilent_netfpga_sume.srcs/sources_1/bd/processor4/processor4.bd]
catch { config_ip_cache -export [get_ips -all processor4_microblaze_0_0] }
catch { config_ip_cache -export [get_ips -all processor4_dlmb_v10_0] }
catch { config_ip_cache -export [get_ips -all processor4_ilmb_v10_0] }
catch { config_ip_cache -export [get_ips -all processor4_dlmb_bram_if_cntlr_0] }
catch { config_ip_cache -export [get_ips -all processor4_ilmb_bram_if_cntlr_0] }
catch { config_ip_cache -export [get_ips -all processor4_lmb_bram_0] }
catch { config_ip_cache -export [get_ips -all processor4_mdm_1_0] }
catch { config_ip_cache -export [get_ips -all processor4_rst_Clk_100M_0] }
catch { config_ip_cache -export [get_ips -all processor4_axi_gpio_0_0] }
catch { config_ip_cache -export [get_ips -all processor4_xbar_0] }
catch { config_ip_cache -export [get_ips -all processor4_auto_pc_5] }
catch { config_ip_cache -export [get_ips -all processor4_auto_pc_0] }
catch { config_ip_cache -export [get_ips -all processor4_auto_pc_1] }
catch { config_ip_cache -export [get_ips -all processor4_auto_pc_2] }
catch { config_ip_cache -export [get_ips -all processor4_auto_pc_3] }
catch { config_ip_cache -export [get_ips -all processor4_auto_pc_4] }
catch { config_ip_cache -export [get_ips -all processor4_axi_gpio_1_0] }
catch { config_ip_cache -export [get_ips -all processor4_fifo_generator_0_0] }
catch { config_ip_cache -export [get_ips -all processor4_axi_gpio_3_0] }
catch { config_ip_cache -export [get_ips -all processor4_axi_gpio_4_0] }
catch { config_ip_cache -export [get_ips -all processor4_bit_mux_2to1_0_0] }
catch { config_ip_cache -export [get_ips -all processor4_bit_mux_2to1_1_0] }
catch { config_ip_cache -export [get_ips -all processor4_mux_2to1_0_0] }
catch { config_ip_cache -export [get_ips -all processor4_bit_mux_2to1_2_0] }
catch { config_ip_cache -export [get_ips -all processor4_axi_gpio_5_0] }

export_ip_user_files -of_objects [get_files digilent_netfpga_sume.srcs/sources_1/bd/processor4/processor4.bd] -sync -force -quiet
create_ip_run [get_files -of_objects [get_fileset sources_1] digilent_netfpga_sume.srcs/sources_1/bd/processor4/processor4.bd]

launch_runs processor4_auto_pc_0_synth_1 processor4_auto_pc_1_synth_1 processor4_auto_pc_2_synth_1 processor4_auto_pc_3_synth_1 processor4_auto_pc_4_synth_1 processor4_auto_pc_5_synth_1 processor4_axi_gpio_0_0_synth_1 processor4_axi_gpio_1_0_synth_1 processor4_axi_gpio_3_0_synth_1 processor4_axi_gpio_4_0_synth_1 processor4_axi_gpio_5_0_synth_1 processor4_bit_mux_2to1_0_0_synth_1 processor4_bit_mux_2to1_1_0_synth_1 processor4_bit_mux_2to1_2_0_synth_1 processor4_dlmb_bram_if_cntlr_0_synth_1 processor4_dlmb_v10_0_synth_1 processor4_fifo_generator_0_0_synth_1 processor4_ilmb_bram_if_cntlr_0_synth_1 processor4_ilmb_v10_0_synth_1 processor4_lmb_bram_0_synth_1 processor4_mdm_1_0_synth_1 processor4_microblaze_0_0_synth_1 processor4_mux_2to1_0_0_synth_1 processor4_rst_Clk_100M_0_synth_1 processor4_xbar_0_synth_1 -jobs 4

wait_on_run processor4_auto_pc_0_synth_1 processor4_auto_pc_1_synth_1 processor4_auto_pc_2_synth_1 processor4_auto_pc_3_synth_1 processor4_auto_pc_4_synth_1 processor4_auto_pc_5_synth_1 processor4_axi_gpio_0_0_synth_1 processor4_axi_gpio_1_0_synth_1 processor4_axi_gpio_3_0_synth_1 processor4_axi_gpio_4_0_synth_1 processor4_axi_gpio_5_0_synth_1 processor4_bit_mux_2to1_0_0_synth_1 processor4_bit_mux_2to1_1_0_synth_1 processor4_bit_mux_2to1_2_0_synth_1 processor4_dlmb_bram_if_cntlr_0_synth_1 processor4_dlmb_v10_0_synth_1 processor4_fifo_generator_0_0_synth_1 processor4_ilmb_bram_if_cntlr_0_synth_1 processor4_ilmb_v10_0_synth_1 processor4_lmb_bram_0_synth_1 processor4_mdm_1_0_synth_1 processor4_microblaze_0_0_synth_1 processor4_mux_2to1_0_0_synth_1 processor4_rst_Clk_100M_0_synth_1 processor4_xbar_0_synth_1 

#export_simulation -of_objects [get_files digilent_netfpga_sume.srcs/sources_1/bd/processor4/processor4.bd] -directory digilent_netfpga_sume.ip_user_files/sim_scripts -ip_user_files_dir digilent_netfpga_sume.ip_user_files -ipstatic_source_dir digilent_netfpga_sume.ip_user_files/ipstatic -lib_map_path [list {modelsim=digilent_netfpga_sume.cache/compile_simlib/modelsim} {questa=digilent_netfpga_sume.cache/compile_simlib/questa} {xcelium=digilent_netfpga_sume.cache/compile_simlib/xcelium} {vcs=digilent_netfpga_sume.cache/compile_simlib/vcs} {riviera=digilent_netfpga_sume.cache/compile_simlib/riviera}] -use_ip_compiled_libs -force -quiet

open_bd_design digilent_netfpga_sume.srcs/sources_1/bd/processor4/processor4.bd
add_files -norecurse software/firmware/spdm_requester.elf
set_property used_in_simulation 0 [get_files -all software/firmware/spdm_requester.elf]
set_property SCOPED_TO_REF processor4 [get_files -all -of_objects [get_fileset sources_1] {software/firmware/spdm_requester.elf}]
set_property SCOPED_TO_CELLS { microblaze_0 } [get_files -all -of_objects [get_fileset sources_1] {software/firmware/spdm_requester.elf}]
close_bd_design processor4

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
