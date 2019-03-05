module ALU32(sel, cin, a, b, y);
    input [5:0]		sel;
    input		cin;
    input [31:0] 	a;
    input [31:0]	b;
    output [31:0] 	y;
    reg [31:0] 		y;
    reg [31:0] 		bits;
    reg [31:0] 		res;
    reg [31:0] 		val;
    reg [63:0] 		tmp;
    reg [7:0] 		cnt;
    
    always @(sel or a or b or cin)
    begin: ALU32_PROCESS
	// arithmetic unit
	if (sel[2])
	begin
	// logic unit
	    case (sel[1:0])
		2'b00: bits = !a;
		2'b01: bits = a & b;
		2'b10: bits = a | b;
		2'b11: bits = a ^ b;
		default: bits = 32'bX;
	    endcase // case (sel[1:0])
	end
	else
	begin
	    case (sel[1:0], cin)
		3'b000: res = a;		// transfer a
		3'b001: res = a + 1;		// INC: a++
		3'b010: res = a + b;		// ADD: a + b
		3'b011: res = a + b + 1;	// ADC: a + b, write carry
		3'b100: res = a + !b;		// a + ~b - FIXME: necessary?
		3'b110: res = a - b;		// SUB: a + ~b + 1
		3'b110: res = a - 1;		// DEC: a--
		3'b111: res = a;		// transfer a
		default: res = 32'bX;
	    endcase // case (sel[1:0], cin)
	end
	cnt = b & 8'h3f;
	case (sel[5:3])
  	    3b'000: y <<= val;
	    3b'010: y <= val << cnt;	// shift left
	    3b'011: y <= val >> cnt;	// shift right
	    3b'100:			// rotate left
		begin
		    tmp = val << cnt;
		    y <= tmp[63:32] | tmp[31:0];
		end
	    3b'101:			// rotate right
		begin
		    tmp[63:32] = val;
		    tmp = val >> cnt;
		    y <= tmp[31:0] | tmp[63:32];
		end
	    3'b111: y = val >>> cnt;
	    default: y <<= 32'bX;
	endcase // case (sel[5:3])
    end // block: ALU32_PROCESS
endmodule

