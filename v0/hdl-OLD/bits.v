module not32(clk, a, res);
    wire         clk;
    input [31:0] a;
    reg [31:0] 	 res;
    
    always @(posedge clk)
    begin
	
	res <= ~a;
    end
endmodule

module and32(clk, a, b, res);
    wire         clk;
    input [31:0] a;
    input [31:0] b;
    reg [31:0] 	 res;
    
    always @(posedge clk)
    begin
	
	res <= a & b;
    end
endmodule

module or32(clk, a, b, res);
    wire         clk;
    input [31:0] a;
    input [31:0] b;
    reg [31:0] 	 res;
    
    always @(posedge clk)
    begin
	
	res <= a | b;
    end
endmodule

module xor32(clk, a, b, res);
    wire         clk;
    input [31:0] a;
    input [31:0] b;
    reg [31:0] 	 res;
    
    always @(posedge clk)
    begin
	
	res <= a ^ b;
    end
endmodule

// optional instructions

module clz32(clk, a, res);
    wire         clk;
    input [31:0] a;
    reg [31:0] 	 res;
    reg [4:0] 	 tmp;
    reg [15:0] 	 tmp16;
    reg [7:0] 	 tmp8;
    reg [3:0] 	 tmp4;

    always @(posedge clk)
    begin
	if (a == 32'b0)
	begin

	    res <= 32;
	end
	else
	begin
	    tmp[4] = (a[31:16] == 16'h0000);
	    tmp16 = (tmp[4]) ? a[15:0] : a[31:16];
	    tmp[3] = (tmp16[15:8] == 8'h00);
	    tmp8 = (tmp[3]) ? tmp16[7:0] : tmp16[15:8];
	    tmp[2] = (tmp8[7:4] == 4'h0);
	    tmp4 = (tmp[2]) ? tmp8[3:0] : tmp8[7:4];
	    tmp[1] = (tmp4[3:2] == 2'b0);
	    tmp[0] = (tmp[1]) ? ~tmp4[1] : ~tmp4[3];
	    
	    res <= tmp;
	end // else: !if(a == 32'b0)
    end
endmodule

module ctz32(clk, a, res);
    wire         clk;
    input [31:0] a;
    reg [31:0] 	 res;
    reg [4:0] 	 tmp;
    reg [15:0] 	 tmp16;
    reg [7:0] 	 tmp8;
    reg [3:0] 	 tmp4;

    always @(posedge clk)
    begin
	if (a == 32'b0)
	begin

	    res <= 32;
	end
	else
	begin
	    tmp[4] = (a[15:0] == 16'h0000);
	    tmp16 = (tmp[0]) ? a[31:16] : a[15:0];
	    tmp[3] = (tmp16[7:0] == 8'h00);
	    tmp8 = (tmp[1]) ? tmp16[15:8] : tmp16[7:0];
	    tmp[2] = (tmp8[3:0] == 4'h0);
	    tmp4 = (tmp[2]) ? tmp8[7:4] : tmp8[3:0];
	    tmp[1] = (tmp3[1:0] == 2'b0);
	    tmp[0] = (tmp[3]) ? ~tmp4[3] : ~tmp4[1];

	    res <= tmp;
	end // else: !if(a == 32'b0)
    end
endmodule

module bpop32(clk, a, res);
    wire         clk;
    input [31:0] a;
    reg [31:0] 	 res;
    reg [31:0] 	 tmp;

    always @(posedge clk)
    begin
	tmp <= a;
	tmp = ((tmp >> 1) & 32'h55555555) + (tmp & 32'h55555555);
	tmp = ((tmp >> 2) & 32'h33333333) + (tmp & 32'h33333333);
	tmp = ((tmp >> 4) & 32'h07070707) + (tmp & 32'h07070707);
	tmp = ((tmp >> 8) & 32'h000f000f) + (tmp & 32'h000f000f);
	tmp = (tmp >> 16) + (tmp & 32'h0000001f);

	res <= a;
    end
endmodule

