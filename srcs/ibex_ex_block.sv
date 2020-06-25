// Copyright lowRISC contributors.
// Copyright 2018 ETH Zurich and University of Bologna, see also CREDITS.md.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

/**
 * Execution stage
 *
 * Execution block: Hosts ALU and MUL/DIV unit
 */
module ibex_ex_block #(
    parameter bit RV32M = 1
) (
    input  logic              clk_i,
    input  logic              rst_ni,
    
    //our module
     input logic [31:0] custom_in_RS1_i ,
     input logic [31:0] custom_in_RS2_i ,
     input logic custom_en_i,
     input logic [31:0] ram_data_ex_i, //data coming from RAM
     output logic [13:0] ram_addr_out , //address send to the RAM 
     output logic custom_valid,
     output logic [31:0] custom_data,
     input [4:0] custom_op_i,
    
    // ALU
    input  ibex_pkg::alu_op_e alu_operator_i,
    input  logic [31:0]       alu_operand_a_i,
    input  logic [31:0]       alu_operand_b_i,

    // Multiplier/Divider
    input  ibex_pkg::md_op_e  multdiv_operator_i,
    input  logic              mult_en_i,
    input  logic              div_en_i,
    input  logic  [1:0]       multdiv_signed_mode_i,
    input  logic [31:0]       multdiv_operand_a_i,
    input  logic [31:0]       multdiv_operand_b_i,

    // Outputs
    output logic [31:0]       alu_adder_result_ex_o, // to LSU
    output logic [31:0]       regfile_wdata_ex_o,
    output logic [31:0]       jump_target_o,         // to IF
    output logic              branch_decision_o,     // to ID

    output logic              ex_valid_o             // EX has valid output
);


  import ibex_pkg::*;

  localparam bit MULT_TYPE = 1; // 0 -> SLOW, 1 -> FAST

  logic [31:0] alu_result, multdiv_result;
  logic [31:0] custom_result;  
  logic [31:0] ram_data_ex;  //data coming from RAM   
  logic [13:0] ram_addr_out_k;  //address send to the RAM 
  logic custom_valid_ex;
  logic custom_final;
  logic rem_valid [2:0];
  logic [31:0] custom_op_a, custom_op_b, custom_data_ex;
  logic [4:0] custom_op_ex;
  logic custom_mod;
  logic [31:0] custom_mod_result;
  logic [31:0] datareg1 [2:0] ,rem_res [2:0];
        
    assign custom_op_ex = custom_op_i ;
    assign custom_valid = custom_valid_ex;
    assign custom_data = custom_data_ex;
    assign ram_addr_out = ram_addr_out_k;
    assign ram_data_ex = ram_data_ex_i;

  logic [32:0] multdiv_alu_operand_b, multdiv_alu_operand_a; 
  logic [33:0] alu_adder_result_ext;
  logic        alu_cmp_result, alu_is_equal_result;
  logic        multdiv_valid, multdiv_en_sel;
  logic        multdiv_en;

  /*
    The multdiv_i output is never selected if RV32M=0
    At synthesis time, all the combinational and sequential logic
    from the multdiv_i module are eliminated
  */
  if (RV32M) begin : gen_multdiv_m
    assign multdiv_en_sel     = MULT_TYPE ? (div_en_i | custom_mod) : mult_en_i | (div_en_i | custom_mod);
    assign multdiv_en         = mult_en_i | (div_en_i | custom_mod);
  end else begin : gen_multdiv_no_m
    assign multdiv_en_sel     = 1'b0;
    assign multdiv_en         = 1'b0;
  end
  
  assign regfile_wdata_ex_o =  custom_en_i ? custom_result: ( multdiv_en ? multdiv_result : alu_result);


  // branch handling
  assign branch_decision_o  = alu_cmp_result;
  assign jump_target_o      = alu_adder_result_ex_o;

custom_module cust_i(
    .custom_en              (custom_en_i),
    .array1_addr            (custom_in_RS1_i),
    .array2_addr            (custom_in_RS2_i),
    .custom_result          (custom_result),
    .custom_data            (custom_data_ex),
    .clk                    (clk_i),
    .ram_data_in            (ram_data_ex),  //data coming from RAM
    .ram_addr_out           (ram_addr_out_k), //address send to RAM
    .custom_valid           (custom_valid_ex),
    .custom_final           (custom_final),
    .custom_op              (custom_op_ex),
    .custom_mod_o           (custom_mod),
    .custom_op_a_o          (datareg1),
    .custom_op_b_o          (custom_op_b),
    .custom_mod_result      (rem_res),
    .mod_valid              (rem_valid[0]&rem_valid[1]&rem_valid[2])            
);

remainder rem_i_1(  
.clk                ( clk_i                ),           
.enable             ( custom_mod           ),
.dividend           ( datareg1[0] ),
.divisor            ( custom_op_b  ),
.result             ( rem_res[0]      ),
.valid              ( rem_valid[0]   )
);

remainder rem_i_2(  
.clk            ( clk_i                ),           
.enable         ( custom_mod           ),
.dividend       ( datareg1[1] ),
.divisor        ( custom_op_b  ),
.result         ( rem_res[1]      ),
.valid          ( rem_valid[1]   )
);

remainder rem_i_3(  
.clk            ( clk_i                ),           
.enable         ( custom_mod           ),
.dividend       (  datareg1[2] ),
.divisor        ( custom_op_b  ),
.result         ( rem_res[2]      ),
.valid          ( rem_valid[2]   )


);
  /////////
  // ALU //
  /////////

  ibex_alu alu_i (
      .operator_i          ( alu_operator_i            ),
      .operand_a_i         ( alu_operand_a_i           ),
      .operand_b_i         ( alu_operand_b_i           ),
      .multdiv_operand_a_i ( multdiv_alu_operand_a     ),
      .multdiv_operand_b_i ( multdiv_alu_operand_b     ),
      .multdiv_en_i        ( multdiv_en_sel            ),
      .adder_result_o      ( alu_adder_result_ex_o     ),
      .adder_result_ext_o  ( alu_adder_result_ext      ),
      .result_o            ( alu_result                ),
      .comparison_result_o ( alu_cmp_result            ),
      .is_equal_result_o   ( alu_is_equal_result       )
  );
  

  ////////////////
  // Multiplier //
  ////////////////

  if (!MULT_TYPE) begin : gen_multdiv_slow
    ibex_multdiv_slow multdiv_i (
        .clk_i              ( clk_i                 ),
        .rst_ni             ( rst_ni                ),
        .mult_en_i          ( mult_en_i             ),
        .div_en_i           (div_en_i ),
        .operator_i         (multdiv_operator_i),
        .signed_mode_i      ( multdiv_signed_mode_i ),
        .op_a_i             ( multdiv_operand_a_i   ),
        .op_b_i             ( multdiv_operand_b_i   ),
        .alu_adder_ext_i    ( alu_adder_result_ext  ),
        .alu_adder_i        ( alu_adder_result_ex_o ),
        .equal_to_zero      ( alu_is_equal_result   ),
        .valid_o            ( multdiv_valid         ),
        .alu_operand_a_o    ( multdiv_alu_operand_a ),
        .alu_operand_b_o    ( multdiv_alu_operand_b ),
        .multdiv_result_o   ( multdiv_result        )
    );
  end else begin : gen_multdiv_fast
    ibex_multdiv_fast multdiv_i (
        .clk_i              ( clk_i                 ),
        .rst_ni             ( rst_ni                ),
        .mult_en_i          ( mult_en_i             ),
        .div_en_i           (div_en_i ),
        .operator_i         (  multdiv_operator_i   ),
        .signed_mode_i      ( multdiv_signed_mode_i     ),
        .op_a_i             ( multdiv_operand_a_i   ),
        .op_b_i             (  multdiv_operand_b_i   ),
        .alu_operand_a_o    ( multdiv_alu_operand_a ),
        .alu_operand_b_o    ( multdiv_alu_operand_b ),
        .alu_adder_ext_i    ( alu_adder_result_ext  ),
        .alu_adder_i        ( alu_adder_result_ex_o ),
        .equal_to_zero      ( alu_is_equal_result   ),
        .valid_o            ( multdiv_valid         ),
        .multdiv_result_o   ( multdiv_result        )
            );
  end  // ALU output valid in same cycle, multiplier/divider may require multiple cycles
 
 //added for multiclock cycle processes
  assign ex_valid_o = custom_en_i ? custom_final : (multdiv_en ? multdiv_valid : 1'b1);

endmodule