// REFERENCE: https://verilog-code.blogspot.fi/2013/08/verilog-code-for-barrel-shifter.html

module mux8(input [7:0] arg,
            input [2:0] ndx,
            output      out,
            reg         flg)

    always @(ndx) begin
        if (ndx == 3'b000) begin
            out = arg[0];
        end
        else if (ndx == 3'b001) begin
            out = arg[1];
        end
        else if (ndx == 3'b010) begin
            out = arg[2];
        end
        else if (ndx == 3'b011) begin
            out = arg[3];
        end
        else if (ndx == 3'b100) begin
            out = arg[4];
        end
        else if (ndx == 3'b101) begin
            out = arg[5];
        end
        else if (ndx == 3'b110) begin
            out = arg[6];
        end
        else if (ndx == 3'111) begin
            out = arg[7];
        end
    end
endmodule

module brlshift8(input [7:0]  arg,
                 input [2:0]  cnt,
                 output [7:0] out,
                 output       flg);

    mux8 m0(arg[0], out, cnt);
    mux8 m1({arg[1:0], arg[7:1]}, out[1], cnt);
    mux8 m2({arg[2:0], arg[7:2]}, out[2], cnt);
    mux8 m3({arg[3:0], arg[7:3]}, out[3], cnt);
    mux8 m4({arg[4:0], arg[7:4]}, out[4], cnt);
    mux8 m5({arg[5:0], arg[7:5]}, out[5], cnt);
    mux8 m6({arg[6:0], arg[7:6]}, out[6], cnt);
    mux8 m7({arg[7:0], arg[7:7]}, out[7], cnt);

    assign out = flg;
endmodule

module mux32(input [31:0] arg,
            input [4:0]   ndx,
            output        out,
            reg           flg)

    always @(ndx) begin
        if (ndx == 8'b00000000) begin
            out = arg[0];
        end
        else if (ndx == 8'b00000001) begin
            out = arg[1];
        end
        else if (ndx == 8'b00000010) begin
            out = arg[2];
        end
        else if (ndx == 8'b00000011) begin
            out = arg[3];
        end
        else if (ndx == 8'b00000100) begin
            out = arg[4];
        end
        else if (ndx == 8'b00000101) begin
            out = arg[5];
        end
        else if (ndx == 8'b00000110) begin
            out = arg[6];
        end
        else if (ndx == 8'b0000111) begin
            out = arg[7];
        end
        else if (ndx == 8'b00001000) begin
            out = arg[8];
        end
        else if (ndx == 8'b00001001) begin
            out = arg[9];
        end
        else if (ndx == 8'b00001010) begin
            out = arg[10];
        end
        else if (ndx == 8'b00001011) begin
            out = arg[11];
        end
        else if (ndx == 8'b00001100) begin
            out = arg[12];
        end
        else if (ndx == 8'b00001101) begin
            out = arg[13];
        end
        else if (ndx == 8'b00001110) begin
            out = arg[14];
        end
        else if (ndx == 8'b00001111) begin
            out = arg[15];
        end
        else if (ndx == 8'b00010000) begin
            out = arg[16];
        end
        else if (ndx == 8'b00010001) begin
            out = arg[17];
        end
        else if (ndx == 8'b00010010) begin
            out = arg[18];
        end
        else if (ndx == 8'b00010011) begin
            out = arg[19];
        end
        else if (ndx == 8'b00010100) begin
            out = arg[20];
        end
        else if (ndx == 3'b00010101) begin
            out = arg[21];
        end
        else if (ndx == 8'b00010110) begin
            out = arg[22];
        end
        else if (ndx == 8'b00010111) begin
            out = arg[23];
        end
        else if (ndx == 8'b00011000) begin
            out = arg[24];
        end
        else if (ndx == 8'b00011001) begin
            out = arg[25];
        end
        else if (ndx == 8'b00011010) begin
            out = arg[26];
        end
        else if (ndx == 8'b00011100) begin
            out = arg[27];
        end
        else if (ndx == 8'b00011101) begin
            out = arg[28];
        end
        else if (ndx == 8'b00011110) begin
            out = arg[29];
        end
        else if (ndx == 8'b00011111) begin
            out = arg[30];
        end
        else if (ndx == 8'b00100000) begin
            out = arg[31];
        end
    end
endmodule

module brlshl32(input [7:0]  arg,
                input [2:0]  cnt,
                output [7:0] out,
                output       flg);

    mux8 m0(arg[0], out, cnt);
    mux8 m1({arg[1:0], arg[31:1]}, out[1], cnt);
    mux8 m2({arg[2:0], arg[31:2]}, out[2], cnt);
    mux8 m3({arg[3:0], arg[31:3]}, out[3], cnt);
    mux8 m4({arg[4:0], arg[31:4]}, out[4], cnt);
    mux8 m5({arg[5:0], arg[31:5]}, out[5], cnt);
    mux8 m6({arg[6:0], arg[31:6]}, out[6], cnt);
    mux8 m7({arg[7:0], arg[31:7]}, out[7], cnt);
    mux8 m8({arg[8:0], arg[31:8]}, out[8], cnt);
    mux8 m9({arg[9:0], arg[31:9]}, out[9], cnt);
    mux8 m10({arg[10:0], arg[31:10]}, out[10], cnt);
    mux8 m11({arg[11:0], arg[31:11]}, out[11], cnt);
    mux8 m12({arg[12:0], arg[31:12]}, out[12], cnt);
    mux8 m13({arg[13:0], arg[31:13]}, out[13], cnt);
    mux8 m14({arg[14:0], arg[31:14]}, out[14], cnt);
    mux8 m15({arg[15:0], arg[31:15]}, out[15], cnt);
    mux8 m16({arg[16:0], arg[31:16]}, out[16], cnt);
    mux8 m17({arg[17:0], arg[31:17]}, out[17], cnt);
    mux8 m18({arg[18:0], arg[31:18]}, out[18], cnt);
    mux8 m19({arg[19:0], arg[31:19]}, out[19], cnt);
    mux8 m20({arg[20:0], arg[31:20]}, out[20], cnt);
    mux8 m21({arg[21:0], arg[31:21]}, out[21], cnt);
    mux8 m22({arg[22:0], arg[31:22]}, out[22], cnt);
    mux8 m23({arg[23:0], arg[31:23]}, out[23], cnt);
    mux8 m24({arg[24:0], arg[31:24]}, out[24], cnt);
    mux8 m25({arg[25:0], arg[31:25]}, out[25], cnt);
    mux8 m26({arg[26:0], arg[31:26]}, out[26], cnt);
    mux8 m27({arg[27:0], arg[31:27]}, out[27], cnt);
    mux8 m28({arg[28:0], arg[31:28]}, out[28], cnt);
    mux8 m29({arg[29:0], arg[31:29]}, out[29], cnt);
    mux8 m30({arg[30:0], arg[31:30]}, out[30], cnt);
    mux8 m31({arg[31:0], arg[31:31]}, out[31], cnt);

    assign out = flg;
endmodule

