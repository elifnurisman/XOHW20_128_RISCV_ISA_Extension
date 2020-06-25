`timescale 1ns / 1ps


module custom_module(
    input logic custom_en,
    input logic [31:0] array1_addr,     //operand_a
    input logic [31:0] array2_addr,     //operand_b
    output logic [31:0] custom_result,  //adress of the result array
    output logic [31:0] custom_data,    
    input logic clk,
    input logic [31:0] ram_data_in, //data coming from RAM
    output logic [13:0] ram_addr_out,  //address send to RAM
    output logic custom_valid,   
    output logic custom_final,  
    input logic [4:0] custom_op,
    output logic custom_mod_o,
    output logic [31:0] custom_op_a_o [2:0],  
    output logic [31:0] custom_op_b_o,
    input logic [31:0] custom_mod_result [2:0],
    input logic mod_valid
    ); 
    
    logic [2:0] addr_check = 0,array_length = 3;
    reg [1:0] k,i=3;
    
    reg [4:0] custom_op_temp;
    reg [13:0] array1_addr_temp;
    reg [13:0] array2_addr_temp;
    
    reg [31:0] data_reg1 [2:0] ;
    reg [31:0] data_reg2 [2:0] ;
    reg [31:0] data_reg3 [2:0] ;
   
    logic c;
    logic [3:0] state=0;
    logic [31:0] mod;
    logic custom_mod;
    
    assign custom_mod_o = custom_mod;
    assign c = (i==array_length-1) ? 1 : 0;
    
    always@(posedge clk) begin

    case (state)
    
    1 :begin  
        case(custom_op_temp)
            4'b0110 : begin //mod
                if(addr_check[0]) begin
                    for(int k=0;k<array_length;k++) begin
                        if(data_reg1[k][31]) begin
                           data_reg1[k] +=  mod <<5 ;  
                        end 
                        state <= 5;
                    end
                end
                else begin
                    ram_addr_out <= ((array1_addr_temp)+i);
                    state <= state+1;
                end
                 
            end
            4'b0101 : begin //equ
                if (addr_check[1]) begin
                    ram_addr_out <= ((array1_addr_temp)+i);
                    state <= state+1;
                end
                else begin
                    addr_check <= 1;  
                    ram_addr_out <= ((array2_addr_temp)+i);
                    state <= state+1;  
                end
            end
            
            4'b0011 : begin //add
                ram_addr_out = addr_check[0] ? ((array2_addr_temp)+i) : ((array1_addr_temp)+i) ;
                state <= state+1;
            end
            
        endcase
    end
   
    
    2 :begin          
        if (!i) begin
        state <=state+1; 
        end
        else begin 
        state <= state+2;
        end
    end
     
    3 :begin  
        state <= state+1;
    end
    
        
    4 :begin  
        i = c ? 0 : i+1;
    
        case(custom_op_temp)
     
            4'b0101 : begin //equ
                if(!addr_check) begin
                    data_reg1[i] <= ram_data_in;        
                    addr_check = c ? 1 : 0;          
                    state <= 1;
                end
                else if (addr_check[0]) begin
                    data_reg3[i] <= ram_data_in;
                    addr_check = c ? 2 : 1;          
                    state <= 1;
                end
                else if (addr_check[1]) begin
                    custom_valid <= 1;
                    custom_data  <= data_reg3[i];
                    state = c ? state+2 : 1;          
                end
            end    //end of equ
         
            default: begin
                if(!addr_check) begin
                    data_reg1[i] <= ram_data_in;        
                    addr_check = c ? 1 : 0;          
                    state <= 1;
                end
                else if (addr_check[0]) begin
                    data_reg2[i] <= ram_data_in;
                    state = c ? state+1 : 1;       
                end
                else if (addr_check[1]) begin
                    custom_valid <= 1;
                    custom_data  <= data_reg3[i];
                    state = c ? state+2 : 1;          
                end //elseif end
            end //default end
        endcase //case end
    end //state end
    
    5 :begin    
        case(custom_op_temp)
       
            4'b0011: begin   //add
               for(int j=0;j<array_length;j++) begin
                   data_reg3[j] = data_reg1[j]+data_reg2[j];   
               end
               state <= 1; 
            end
            
            4'b0110 : begin  //mod
                custom_mod <= 1'b1;
                custom_op_b_o <= mod;                   
                custom_op_a_o <= data_reg1;
            
                if (mod_valid) begin
                    data_reg3 <= custom_mod_result;
                    custom_mod <= 0;
                    state <= 1;
                end
                else begin
                    state <= 5;
                end
            end
        endcase   //end of instruction cases
        
        addr_check = 2;
                       
        end //end of state5

    6  :begin  // Finalize_1     
        custom_final <= 1;
        custom_result <= array1_addr_temp<<2;
        i = 0;
        custom_valid <= 0;       
        state <= state +1;   
    end       
    
    7  :begin  //Finalize_2
        for(int i=0;i<array_length;i++) begin
        data_reg3[i] = 32'b0;
        data_reg1[i] = 32'b0;
        data_reg2[i] = 32'b0;  
        end
        state <= 0;
    end
    
    
    default: begin
        array1_addr_temp <= array1_addr>>2;
        array2_addr_temp <= array2_addr>>2;
        mod <= array2_addr;
        custom_op_temp <= custom_op;
        
        custom_op_a_o[0] <= 32'b0;
        custom_op_a_o[1] <= 32'b0;
        custom_op_a_o[2] <= 32'b0;
        custom_op_b_o <= 32'b0;
        custom_mod <= 1'b0;
        
        ram_addr_out <= 14'b0;
        custom_valid <= 1'b0;
        custom_data  <= 32'b0;
        custom_final <= 0;
        
        i = 0;
        addr_check = 0;
        custom_result = 32'b0;
        custom_data = 32'b0;
        
        if(custom_en)   state <= 1; 
    end
    endcase    
    end
endmodule