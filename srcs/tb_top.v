

module tb_top;  

    reg               IO_CLK;
    reg               IO_RST_N;
   
    top_artya7_100 UUT (
        .IO_CLK(IO_CLK),
        .IO_RST_N(IO_RST_N)
    );
    
    initial begin 
    IO_CLK = 0;
        IO_RST_N = 0;
        #20 IO_RST_N = 1;
    end
      
    always #10 IO_CLK = ~IO_CLK;
      
endmodule