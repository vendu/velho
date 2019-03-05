module alu32(clk, unit, inst, arg1, arg2, arg1t, arg2t, argsz, imm32);
    input        clk;
    input [3:0]  unit;
    input [3:0]  inst;
    input [31:0] arg1;
    input [31:0] arg2;
    input [1:0]  arg1t;
    input [1:0]  arg2t;
    input [2:0]  argsz;
    input [31:0] imm32;
    // general purpose registers
    reg [31:0] 	 r0;
    reg [31:0] 	 r1;
    reg [31:0] 	 r2;
    reg [31:0] 	 r3;
    reg [31:0] 	 r4;
    reg [31:0] 	 r5;
    reg [31:0] 	 r6;
    reg [31:0] 	 r7;
    reg [31:0] 	 r8;
    reg [31:0] 	 r9;
    reg [31:0] 	 r10;
    reg [31:0] 	 r11;
    reg [31:0] 	 r12;
    reg [31:0] 	 r13;
    reg [31:0] 	 r14;
    reg [31:0] 	 r15;

    always @(posedge clk)
    begin
	case (op[3:0])
	    4'h0: v0bit(clk, inst, a, b);
	    4'h1: v0shift(clk, inst, a, b);
	    4'h2: v0arith(clk, inst, a, b);
	    4'h3: v0flow(clk, inst, a, b);
	    4'h4: v0mul(clk, inst, a, b);
	    4'h5: v0div(clk, inst, a, b);
	    4'h6: v0xfer(clk, inst, a, b);
	    4'h8: v0io(clk, inst, a, b);
	    4'h9: v0sys(clk, inst, a, b);
	    // a..e are optional units, f is coprocessor
	    4'ha: v0bitf(clk, inst, a, b);
	    4'hc: v0task(clk, inst, a, b);
	    // units that access memory
	    4'h7: v0stk(clk, inst, a, b);
	    4'hb: v0mem(clk, inst, a, b);
	    4'hd: v0atom(clk, inst, a, b);
	    default: // unknown unit
	endcase
    end
endmodule // alu32

