/* shifter unit */

module shl32(clk, a, b, res);
    wire         clk;
    input [31:0] a;
    input [31:0] b;
    reg [31:0] 	 res;

    always @(posedge clk)
    begin

	res <= a << b;
    end
endmodule // shl32

module shr32(clk, a, b, res);
    wire         clk;
    input [31:0] a;
    input [31:0] b;
    reg [31:0] 	 res;

    always @(posedge clk)
    begin
	res <= a >> b;
    end
endmodule // shr32

#if 0

module sar32u(clk, a, b, res);
    wire         clk;
    input [31:0] a;
    input [31:0] b;
    reg [31:0] 	 res;

    always @(posedge clk)
    begin

	res <= a >>> b;
    end
endmodule // sar32u

module sar32s(clk, a, b, res);
    wire                clk;
    input [31:0]        a;
    signed input [31:0] b;
    reg [31:0] 		res;

    always @(posedge clk)
    begin

	res <= a >>> b;
    end
endmodule // sar32s

module rol32(clk, a, b, res);
    wire         clk;
    input [31:0] a;
    input [31:0] b;
    reg [31:0] 	 res;
    reg [63:0] 	 tmp;

    always @(posedge clk)
    begin
	tmp[63:32] <= a;
	tmp <= tmp >> (b & 8'h3f);

	res <= tmp[31:0] | tmp[63:32];
    end
endmodule // rol32

module ror32(clk, a, b, res);
    wire         clk;
    input [31:0] a;
    input [31:0] b;
    reg [31:0] 	 res;
    reg [63:0] 	 tmp;

    always @(posedge clk)
    begin
	tmp <= a << (b & 8'h3f);

	res <= tmp[63:32] | tmp[31:0];
    end
endmodule // ror32

