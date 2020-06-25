`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/10/2020 10:36:12 PM
// Design Name: 
// Module Name: LED_driver
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module LED_driver(
    input rst_sys_n,
    input [31:0] LEDS,
    output reg [15:0] IO_OUT 
    );
    
    always@(rst_sys_n, LEDS) begin
        if(!rst_sys_n) begin
            IO_OUT = 16'b0;
        end
        else begin
            if(LEDS == 32'HAAAAAAA0) begin
                IO_OUT = 16'h000F;
            end
            else if(LEDS == 32'HBBBBBBB0) begin
                IO_OUT = 16'h00FF;
            end
            else if(LEDS == 32'HCCCCCCC0) begin
                IO_OUT = 16'h0FFF;
            end
            else if(LEDS == 32'HDEDEDEDE) begin
                IO_OUT = 16'hFFFF;
            end
            else IO_OUT = IO_OUT;
        end
    end
    
endmodule
