module inc32(clk, a, res);
    wire         clk;
    input [31:0] a;
    reg [31:0] 	 res;

    always @(posedge clk)
    begin

	res <= a + 1;
    end
endmodule

module dec32(clk, a, res);
    wire         clk;
    input [31:0] a;
    reg [31:0] 	 res;

    always @(posedge clk)
    begin

	res <= a - 1;
    end
endmodule

module add32(clk, a, b, res);
    wire         clk;
    input [31:0] a;
    input [31:0] b;
    reg [31:0] 	 res;

    always @(posedge clk)
    begin

	res <= a + b;
    end
endmodule

module adf32(clk, a, b, res, msw);
    wire         clk;
    input [31:0] a;
    input [31:0] b;
    reg [31:0] 	 res;
    reg [31:0] 	 msw;
    reg [32:0] 	 tmp;
    reg [1:0] 	 flg;

    always @(posedge clk)
    begin
	tmp <= a;
	tmp = tmp + b;
	if (tmp < a)
	begin
	    flg[0] = 1; // overflow
	end
	if (tmp[32])
	begin
	    flg[1] = 1; // carry
	end
	msw[2:1] = flg;

	res = tmp[31:0];
    end
endmodule

module adi32(clk, a, b, res);
    wire         clk;
    input [31:0] a;
    input [31:0] b;
    reg [31:0] 	 res;
    reg [31:0] 	 tmp;

    always @(posedge clk)
    begin
	tmp = ~a;

	res <=  tmp + b;
    end
endmodule

module sub32(clk, a, b, res);
    wire         clk;
    input [31:0] a;
    input [31:0] b;
    reg [31:0] 	 res;
    reg [31:0] 	 tmp;

    always @(posedge clk)
    begin
	tmp = ~a;
	tmp = tmp + b;

	res <= tmp + 1;
    end
endmodule

module cmp32u(clk, a, b, res);
    wire         clk;
    input [31:0] a;
    input [31:0] b;
    reg [31:0] 	 res;
    reg [31:0] 	 tmp;
    reg [2:0] 	 flg;

    always @(posedge clk)
    begin
	if (a == b)
	begin
	    flg[0] = 1; // zero
	end
	else
	begin
	    if (a < b)
	    begin
		flg[2] = 1; // carry
	    end
	    else if (a > b)
	    begin
		flg[1] = 1; // overflow
	    end
	end
    end // always @ (posedge clk)
endmodule

// TODO: cmp32s

