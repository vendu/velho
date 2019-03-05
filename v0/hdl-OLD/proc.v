module proc32(clk, op);
    input        clk;
    input [31:0] op;
    reg [31:0] 	 code;
    reg [31:0] 	 imm32;
    // system registers; accessible in user-mode
    reg [31:0] 	 msw;	// machine status word
    reg [31:0] 	 fp;		// frame pointer
    reg [31:0] 	 sp;		// stack pointer
    reg [31:0] 	 pc;		// program counter
    // reserved system registers
    reg [31:0] 	 _res4;
    reg [31:0] 	 _res5;
    reg [31:0] 	 _res6;
    reg [31:0] 	 _res7;
    // special registers; only accessible in system-mode
    reg [31:0] 	 iht;	// interrupt handler table
    reg [31:0] 	 pdb;	// page directory base address
    reg [31:0] 	 tid;	// task ID
    reg [31:0] 	 tls;	// thread-local storage base address
    reg [31:0] 	 tsr;	// task structure base address
endmodule // proc32

