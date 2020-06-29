# Extending the Instruction Set of RISC-V Processor for NTRU Algorithm

<p align="center">
<img src="https://www.pikpng.com/pngl/b/510-5109202_cryptography-png-secure-network-icon-clipart.png" width="200" height="250" />
</p>


 - **Team number:** 128

 - **Project name:** Extending the Instruction Set of RISC-V Processor for NTRU Algorithm

 - **University name:** Istanbul Technical University  

 - **Supervisor name:** Sıddıka Berna Örs Yalçın

 - **Supervisor e-mail:** orssi@itu.edu.tr 

 - **Participants:** Elif Nur İşman, Canberk Topal

   **Emails:** 
  
   - isman15@itu.edu.tr

   - topalc16@itu.edu.tr
 
## Specifications

 - **Board used:** Digilent Nexys 4 DDR Artix-7  

 - **Software Version:** Xilinx Vivado 2018.1 
 
 - **Hardware Description Language:** SystemVerilog

## Description of the Project

With the developing technology, interest and investments in developing
quantum computers are increasing rapidly[1, 2]. However, this poses a threat to
cryptography algorithms used in every system where information security is needed
today. The quantum era requires fundamental changes in information security. New
cryptography algorithms that can resist post-quantum computers are being developed
in order to maintain information security in banking, military and many other areas. In
order to be usable and practical in daily life, low area usage and low performance are
prioritized in the algorithms created.

Based on the Reduced Instruction Set Computer (RISC) architecture[3], RISC-
V[4] is an open source alternative to a world of proprietary instruction set
architectures. Our project aims to increase the performance of a NTRU cryptosystem
application on an open source, low-power RISC-V processor. The plan is to increase
the performance by extending the instruction set with most commonly used operations
in the application.



<p align="center">
<img src="https://i.imgur.com/4DKMCg7.png"/>
</p>
 

## Description of Folders

 - /srcs Contains source files for RV32IMC Ibex Core with our modifications.
 - /sw  Contains our 48-bit message length NTRU C implementation with parameters **N = 53**, **p = 3**,   **q = 101** with added custom instructions.
 - /bit Contains the project bitstream compatible with our FPGA card specified above. 

## Instructions to Test the Project

 - Generated bitstream is in the /bit folder. Anyone can program their Nexys4DDR FPGA with that bitstream and can verify our results on the hardware manager window of Vivado 2018.1
 
 - In order to reach to the project details, unzipping the compressed file in */prj* and opening the .xpr file in Vivado 2018.1 would be sufficient. 
 

## Link to YouTube Video: 
https://www.youtube.com/watch?v=2DyESQCIsOo

## References:

 1. N. S. Yanofsky and M. A. Mannucci, Quantum Computing for Computer Scientists , 1st ed. Cambridge University Press, 11 Aug 2008.
 2. M. Mohseni, P. Read, H. Neven, S. Boixo, V. Denchev, R. Babbush, A. Fowler, V. Smelyanskiy, and J. Martinis, “Commercialize quantum technologies in five years,” Nature, vol. 543, no. 7644, pp. 171–174, 2017.
 3. S. B. Furber, VLSI RISC Architecture and Organization . Routledge, 19 Sep 2017
 4. D. A. Patterson and J. L. Hennessy, Computer Organization and Design RISC-V Edition , 1st ed. Morgan Kaufmann, 12 May 2017.
