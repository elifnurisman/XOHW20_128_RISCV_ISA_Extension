`timescale 1ns / 1ps


module remainder(clk,enable,dividend,divisor,result,valid);

	input clk;
	input enable;
	input [31:0] dividend,divisor;
	output [31:0] result;
	output valid;
	
	logic [63:0] dividend_copy,divisor_copy,diff;
	
	logic [31:0] remainder = dividend_copy [31:0];
	logic [6:0]  bits = 64;  
	
	logic valid=0;
	
	
	always@(posedge clk)	begin
        if (enable) begin
            if(bits[6])  begin
                bits = 32;
                dividend_copy = {32'd0,dividend};
                divisor_copy = {1'd0,divisor,31'd0};
            end
            else  begin
                valid = (~bits[6] && ~bits[5] && ~bits[4] && ~bits[3] && ~bits[2] && ~bits[1] && bits[0]) && enable;
                diff = dividend_copy - divisor_copy;
                
                if( !diff[63] )    begin
                    dividend_copy = diff;
                end
            
            divisor_copy = divisor_copy >> 1;
            bits = (!bits) ? 64 :  bits - 1; 
            end
        end
	 end
	
	assign result = remainder;
	endmodule