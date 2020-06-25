# Description

This folder contains all the *.sv* files of our project with the addition of constraint file. It involves our modifications to them as well.

## Modification List


###### New Files

- LED_driver.v
- tb_top.v
- custom_module.sv
- remainder.v* 

###### Major Modifications

- ibex_decoder.sv
- top_artya7_100.sv
- ibex_ex_block.sv


###### Minor Modifications

- pins_artya7.xdc
- ibex_id_stage.sv

###### Unchanged Files
 - ram_1p.sv
 - prim_clock_gating.sv
 - ibex_register_file_ff.sv
 - ibex_prefetch_buffer.sv
 - ibex_pmp.sv
 - ibex_pkg.sv
 - ibex_multdiv_fast.sv
 - ibex_if_stage.sv
 - ibex_load_store_unit.sv
 - ibex_fetch_fifo.sv
 - ibex_cs_registers.sv
 - ibex_controller.sv
 - ibex_compressed_decoder.sv
 - ibex_alu.sv
 - clkgen_xil7series.sv
 
PS: remainder.v is taken from the paper "*TRANSLATION OF DIVISION ALGORITHM INTO VERILOG HDL* by Y. Yusmardiah, 2006"
