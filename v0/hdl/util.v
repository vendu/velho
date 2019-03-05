// REFERENCE: https://verilog-code.blogspot.fi/2013/08/verilog-code-for-barrel-shifter.html

module v0muxshl(input [31:0] arg;
                input [4:0]  ndx;
                output       out;
                reg          flg)

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

module v0muxshr(input [31:0] arg;
                input [4:0]  ndx;
                output       out;
                reg          flg)

    always @(ndx) begin
        if (ndx == 8'b00000000) begin
            out = arg[31];
        end
        else if (ndx == 8'b00000001) begin
            out = arg[30];
        end
        else if (ndx == 8'b00000010) begin
            out = arg[29];
        end
        else if (ndx == 8'b00000011) begin
            out = arg[28];
        end
        else if (ndx == 8'b00000100) begin
            out = arg[27];
        end
        else if (ndx == 8'b00000101) begin
            out = arg[26];
        end
        else if (ndx == 8'b00000110) begin
            out = arg[25];
        end
        else if (ndx == 8'b0000111) begin
            out = arg[24];
        end
        else if (ndx == 8'b00001000) begin
            out = arg[23];
        end
        else if (ndx == 8'b00001001) begin
            out = arg[22];
        end
        else if (ndx == 8'b00001010) begin
            out = arg[21];
        end
        else if (ndx == 8'b00001011) begin
            out = arg[20];
        end
        else if (ndx == 8'b00001100) begin
            out = arg[19];
        end
        else if (ndx == 8'b00001101) begin
            out = arg[18];
        end
        else if (ndx == 8'b00001110) begin
            out = arg[17];
        end
        else if (ndx == 8'b00001111) begin
            out = arg[16];
        end
        else if (ndx == 8'b00010000) begin
            out = arg[15];
        end
        else if (ndx == 8'b00010001) begin
            out = arg[14];
        end
        else if (ndx == 8'b00010010) begin
            out = arg[13];
        end
        else if (ndx == 8'b00010011) begin
            out = arg[12];
        end
        else if (ndx == 8'b00010100) begin
            out = arg[11];
        end
        else if (ndx == 3'b00010101) begin
            out = arg[10];
        end
        else if (ndx == 8'b00010110) begin
            out = arg[9];
        end
        else if (ndx == 8'b00010111) begin
            out = arg[8];
        end
        else if (ndx == 8'b00011000) begin
            out = arg[7];
        end
        else if (ndx == 8'b00011001) begin
            out = arg[6];
        end
        else if (ndx == 8'b00011010) begin
            out = arg[5];
        end
        else if (ndx == 8'b00011100) begin
            out = arg[4];
        end
        else if (ndx == 8'b00011101) begin
            out = arg[3];
        end
        else if (ndx == 8'b00011110) begin
            out = arg[2];
        end
        else if (ndx == 8'b00011111) begin
            out = arg[1];
        end
        else if (ndx == 8'b00100000) begin
            out = arg[0];
        end
    end
endmodule

module v0brlshl(input [31:0]  arg;
                input [4:0]   cnt;
                output [31:0] out;
                reg [31:0]    src;
                reg [31:0]    res);

    src = arg;
    res = arg;
    v0muxshl m0(src[0], res, cnt);
    v0muxshl m1({src[1:0], src[31:1]}, res[1], cnt);
    v0muxshl m2({src[2:0], src[31:2]}, res[2], cnt);
    v0muxshl m3({src[3:0], src[31:3]}, res[3], cnt);
    v0muxshl m4({src[4:0], src[31:4]}, res[4], cnt);
    v0muxshl m5({src[5:0], src[31:5]}, res[5], cnt);
    v0muxshl m6({src[6:0], src[31:6]}, res[6], cnt);
    v0muxshl m7({src[7:0], src[31:7]}, res[7], cnt);
    v0muxshl m8({src[8:0], src[31:8]}, res[8], cnt);
    v0muxshl m9({src[9:0], src[31:9]}, res[9], cnt);
    v0muxshl m10({src[10:0], src[31:10]}, res[10], cnt);
    v0muxshl m11({src[11:0], src[31:11]}, res[11], cnt);
    v0muxshl m12({src[12:0], src[31:12]}, res[12], cnt);
    v0muxshl m13({src[13:0], src[31:13]}, res[13], cnt);
    v0muxshl m14({src[14:0], src[31:14]}, res[14], cnt);
    v0muxshl m15({src[15:0], src[31:15]}, res[15], cnt);
    v0muxshl m16({src[16:0], src[31:16]}, res[16], cnt);
    v0muxshl m17({src[17:0], src[31:17]}, res[17], cnt);
    v0muxshl m18({src[18:0], src[31:18]}, res[18], cnt);
    v0muxshl m19({src[19:0], src[31:19]}, res[19], cnt);
    v0muxshl m20({src[20:0], src[31:20]}, res[20], cnt);
    v0muxshl m21({src[21:0], src[31:21]}, res[21], cnt);
    v0muxshl m22({src[22:0], src[31:22]}, res[22], cnt);
    v0muxshl m23({src[23:0], src[31:23]}, res[23], cnt);
    v0muxshl m24({src[24:0], src[31:24]}, res[24], cnt);
    v0muxshl m25({src[25:0], src[31:25]}, res[25], cnt);
    v0muxshl m26({src[26:0], src[31:26]}, res[26], cnt);
    v0muxshl m27({src[27:0], src[31:27]}, res[27], cnt);
    v0muxshl m28({src[28:0], src[31:28]}, res[28], cnt);
    v0muxshl m29({src[29:0], src[31:29]}, res[29], cnt);
    v0muxshl m30({src[30:0], src[31:30]}, res[30], cnt);
    v0muxshl m31({src[31:0], src[31:31]}, res[31], cnt);

    assign out = res;
endmodule

module v0brlshr(input [31:0]  arg;
                input [4:0]   cnt;
                output [31:0] out;
                reg [31:0]    src;
                reg [31:0]    res);

    src = cnt;
    res = arg;
    v0muxshr m0(arg[0], res, cnt);
    v0muxshr m1({arg[31:1], arg[1]}, res[1], cnt);
    v0muxshr m2({arg[31:2], arg[2]}, res[2], cnt);
    v0muxshr m3({arg[3:3], arg[3]}, res[3], cnt);
    v0muxshr m4({arg[4:4], arg[4]}, res[4], cnt);
    v0muxshr m5({arg[5:5], arg[5]}, res[5], cnt);
    v0muxshr m6({arg[6:6], arg[6]}, res[6], cnt);
    v0muxshr m7({arg[7:7], arg[7]}, res[7], cnt);
    v0muxshr m8({arg[8:8], arg[8]}, res[8], cnt);
    v0muxshr m9({arg[9:9], arg[9]}, res[9], cnt);
    v0muxshr m10({arg[10:10], arg[10]}, res[10], cnt);
    v0muxshr m11({arg[11:11], arg[11]}, res[11], cnt);
    v0muxshr m12({arg[12:12], arg[12]}, res[12], cnt);
    v0muxshr m13({arg[13:13], arg[13]}, res[13], cnt);
    v0muxshr m14({arg[14:14], arg[14]}, res[14], cnt);
    v0muxshr m15({arg[15:15], arg[15]}, res[15], cnt);
    v0muxshr m16({arg[16:16], arg[16]}, res[16], cnt);
    v0muxshr m17({arg[17:17], arg[17]}, res[17], cnt);
    v0muxshr m18({arg[18:18], arg[18]}, res[18], cnt);
    v0muxshr m19({arg[19:19], arg[19]}, res[19], cnt);
    v0muxshr m20({arg[20:20], arg[20]}, res[20], cnt);
    v0muxshr m21({arg[21:21], arg[21]}, res[21], cnt);
    v0muxshr m22({arg[22:22], arg[22]}, res[22], cnt);
    v0muxshr m23({arg[23:23], arg[23]}, res[23], cnt);
    v0muxshr m24({arg[24:24], arg[24]}, res[24], cnt);
    v0muxshr m25({arg[25:25], arg[25]}, res[25], cnt);
    v0muxshr m26({arg[26:26], arg[26]}, res[26], cnt);
    v0muxshr m27({arg[27:27], arg[27]}, res[27], cnt);
    v0muxshr m28({arg[28:28], arg[28]}, res[28], cnt);
    v0muxshr m29({arg[29:29], arg[29]}, res[29], cnt);
    v0muxshr m30({arg[30:30], arg[30]}, res[30], cnt);
    v0muxshr m31({arg[31:31], arg[31]}, res[31], cnt);

    assign out = res;
endmodule

// REFERENCE: https://verilog-code.blogspot.fi/2013/08/verilog-code-for-barrel-shifter.html

module v0muxshl(input [31:0] arg;
                input [4:0]  ndx;
                output       out;
                reg          flg)

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

module v0muxshr(input [31:0] arg;
                input [4:0]  ndx;
                output       out;
                reg          flg)

    always @(ndx) begin
        if (ndx == 8'b00000000) begin
            out = arg[31];
        end
        else if (ndx == 8'b00000001) begin
            out = arg[30];
        end
        else if (ndx == 8'b00000010) begin
            out = arg[29];
        end
        else if (ndx == 8'b00000011) begin
            out = arg[28];
        end
        else if (ndx == 8'b00000100) begin
            out = arg[27];
        end
        else if (ndx == 8'b00000101) begin
            out = arg[26];
        end
        else if (ndx == 8'b00000110) begin
            out = arg[25];
        end
        else if (ndx == 8'b0000111) begin
            out = arg[24];
        end
        else if (ndx == 8'b00001000) begin
            out = arg[23];
        end
        else if (ndx == 8'b00001001) begin
            out = arg[22];
        end
        else if (ndx == 8'b00001010) begin
            out = arg[21];
        end
        else if (ndx == 8'b00001011) begin
            out = arg[20];
        end
        else if (ndx == 8'b00001100) begin
            out = arg[19];
        end
        else if (ndx == 8'b00001101) begin
            out = arg[18];
        end
        else if (ndx == 8'b00001110) begin
            out = arg[17];
        end
        else if (ndx == 8'b00001111) begin
            out = arg[16];
        end
        else if (ndx == 8'b00010000) begin
            out = arg[15];
        end
        else if (ndx == 8'b00010001) begin
            out = arg[14];
        end
        else if (ndx == 8'b00010010) begin
            out = arg[13];
        end
        else if (ndx == 8'b00010011) begin
            out = arg[12];
        end
        else if (ndx == 8'b00010100) begin
            out = arg[11];
        end
        else if (ndx == 3'b00010101) begin
            out = arg[10];
        end
        else if (ndx == 8'b00010110) begin
            out = arg[9];
        end
        else if (ndx == 8'b00010111) begin
            out = arg[8];
        end
        else if (ndx == 8'b00011000) begin
            out = arg[7];
        end
        else if (ndx == 8'b00011001) begin
            out = arg[6];
        end
        else if (ndx == 8'b00011010) begin
            out = arg[5];
        end
        else if (ndx == 8'b00011100) begin
            out = arg[4];
        end
        else if (ndx == 8'b00011101) begin
            out = arg[3];
        end
        else if (ndx == 8'b00011110) begin
            out = arg[2];
        end
        else if (ndx == 8'b00011111) begin
            out = arg[1];
        end
        else if (ndx == 8'b00100000) begin
            out = arg[0];
        end
    end
endmodule

module v0brlshl(input [31:0]  arg;
                input [4:0]   cnt;
                output [31:0] out;
                reg [31:0]    src;
                reg [31:0]    res);

    src = arg;
    res = arg;
    v0muxshl m0(src[0], res, cnt);
    v0muxshl m1({src[1:0], src[31:1]}, res[1], cnt);
    v0muxshl m2({src[2:0], src[31:2]}, res[2], cnt);
    v0muxshl m3({src[3:0], src[31:3]}, res[3], cnt);
    v0muxshl m4({src[4:0], src[31:4]}, res[4], cnt);
    v0muxshl m5({src[5:0], src[31:5]}, res[5], cnt);
    v0muxshl m6({src[6:0], src[31:6]}, res[6], cnt);
    v0muxshl m7({src[7:0], src[31:7]}, res[7], cnt);
    v0muxshl m8({src[8:0], src[31:8]}, res[8], cnt);
    v0muxshl m9({src[9:0], src[31:9]}, res[9], cnt);
    v0muxshl m10({src[10:0], src[31:10]}, res[10], cnt);
    v0muxshl m11({src[11:0], src[31:11]}, res[11], cnt);
    v0muxshl m12({src[12:0], src[31:12]}, res[12], cnt);
    v0muxshl m13({src[13:0], src[31:13]}, res[13], cnt);
    v0muxshl m14({src[14:0], src[31:14]}, res[14], cnt);
    v0muxshl m15({src[15:0], src[31:15]}, res[15], cnt);
    v0muxshl m16({src[16:0], src[31:16]}, res[16], cnt);
    v0muxshl m17({src[17:0], src[31:17]}, res[17], cnt);
    v0muxshl m18({src[18:0], src[31:18]}, res[18], cnt);
    v0muxshl m19({src[19:0], src[31:19]}, res[19], cnt);
    v0muxshl m20({src[20:0], src[31:20]}, res[20], cnt);
    v0muxshl m21({src[21:0], src[31:21]}, res[21], cnt);
    v0muxshl m22({src[22:0], src[31:22]}, res[22], cnt);
    v0muxshl m23({src[23:0], src[31:23]}, res[23], cnt);
    v0muxshl m24({src[24:0], src[31:24]}, res[24], cnt);
    v0muxshl m25({src[25:0], src[31:25]}, res[25], cnt);
    v0muxshl m26({src[26:0], src[31:26]}, res[26], cnt);
    v0muxshl m27({src[27:0], src[31:27]}, res[27], cnt);
    v0muxshl m28({src[28:0], src[31:28]}, res[28], cnt);
    v0muxshl m29({src[29:0], src[31:29]}, res[29], cnt);
    v0muxshl m30({src[30:0], src[31:30]}, res[30], cnt);
    v0muxshl m31({src[31:0], src[31:31]}, res[31], cnt);

    assign out = res;
endmodule

module v0brlshr(input [31:0]  arg,
                input [4:0]   cnt,
                output [31:0] out,
                reg [31:0]    src,
                reg [31:0]    res);

    src = cnt;
    res = arg;
    v0muxshr m0(arg[0], res, cnt);
    v0muxshr m1({arg[31:1], arg[1]}, res[1], cnt);
    v0muxshr m2({arg[31:2], arg[2]}, res[2], cnt);
    v0muxshr m3({arg[3:3], arg[3]}, res[3], cnt);
    v0muxshr m4({arg[4:4], arg[4]}, res[4], cnt);
    v0muxshr m5({arg[5:5], arg[5]}, res[5], cnt);
    v0muxshr m6({arg[6:6], arg[6]}, res[6], cnt);
    v0muxshr m7({arg[7:7], arg[7]}, res[7], cnt);
    v0muxshr m8({arg[8:8], arg[8]}, res[8], cnt);
    v0muxshr m9({arg[9:9], arg[9]}, res[9], cnt);
    v0muxshr m10({arg[10:10], arg[10]}, res[10], cnt);
    v0muxshr m11({arg[11:11], arg[11]}, res[11], cnt);
    v0muxshr m12({arg[12:12], arg[12]}, res[12], cnt);
    v0muxshr m13({arg[13:13], arg[13]}, res[13], cnt);
    v0muxshr m14({arg[14:14], arg[14]}, res[14], cnt);
    v0muxshr m15({arg[15:15], arg[15]}, res[15], cnt);
    v0muxshr m16({arg[16:16], arg[16]}, res[16], cnt);
    v0muxshr m17({arg[17:17], arg[17]}, res[17], cnt);
    v0muxshr m18({arg[18:18], arg[18]}, res[18], cnt);
    v0muxshr m19({arg[19:19], arg[19]}, res[19], cnt);
    v0muxshr m20({arg[20:20], arg[20]}, res[20], cnt);
    v0muxshr m21({arg[21:21], arg[21]}, res[21], cnt);
    v0muxshr m22({arg[22:22], arg[22]}, res[22], cnt);
    v0muxshr m23({arg[23:23], arg[23]}, res[23], cnt);
    v0muxshr m24({arg[24:24], arg[24]}, res[24], cnt);
    v0muxshr m25({arg[25:25], arg[25]}, res[25], cnt);
    v0muxshr m26({arg[26:26], arg[26]}, res[26], cnt);
    v0muxshr m27({arg[27:27], arg[27]}, res[27], cnt);
    v0muxshr m28({arg[28:28], arg[28]}, res[28], cnt);
    v0muxshr m29({arg[29:29], arg[29]}, res[29], cnt);
    v0muxshr m30({arg[30:30], arg[30]}, res[30], cnt);
    v0muxshr m31({arg[31:31], arg[31]}, res[31], cnt);

    assign out = res;
endmodule

// REFERENCE: https://verilog-code.blogspot.fi/2013/08/verilog-code-for-barrel-shifter.html

module v0muxshl(input [31:0] arg;
                input [4:0]  ndx;
                output       out;
                reg          flg)

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

module v0muxshr(input [31:0] arg;
                input [4:0]  ndx;
                output       out;
                reg          flg)

    always @(ndx) begin
        if (ndx == 8'b00000000) begin
            out = arg[31];
        end
        else if (ndx == 8'b00000001) begin
            out = arg[30];
        end
        else if (ndx == 8'b00000010) begin
            out = arg[29];
        end
        else if (ndx == 8'b00000011) begin
            out = arg[28];
        end
        else if (ndx == 8'b00000100) begin
            out = arg[27];
        end
        else if (ndx == 8'b00000101) begin
            out = arg[26];
        end
        else if (ndx == 8'b00000110) begin
            out = arg[25];
        end
        else if (ndx == 8'b0000111) begin
            out = arg[24];
        end
        else if (ndx == 8'b00001000) begin
            out = arg[23];
        end
        else if (ndx == 8'b00001001) begin
            out = arg[22];
        end
        else if (ndx == 8'b00001010) begin
            out = arg[21];
        end
        else if (ndx == 8'b00001011) begin
            out = arg[20];
        end
        else if (ndx == 8'b00001100) begin
            out = arg[19];
        end
        else if (ndx == 8'b00001101) begin
            out = arg[18];
        end
        else if (ndx == 8'b00001110) begin
            out = arg[17];
        end
        else if (ndx == 8'b00001111) begin
            out = arg[16];
        end
        else if (ndx == 8'b00010000) begin
            out = arg[15];
        end
        else if (ndx == 8'b00010001) begin
            out = arg[14];
        end
        else if (ndx == 8'b00010010) begin
            out = arg[13];
        end
        else if (ndx == 8'b00010011) begin
            out = arg[12];
        end
        else if (ndx == 8'b00010100) begin
            out = arg[11];
        end
        else if (ndx == 3'b00010101) begin
            out = arg[10];
        end
        else if (ndx == 8'b00010110) begin
            out = arg[9];
        end
        else if (ndx == 8'b00010111) begin
            out = arg[8];
        end
        else if (ndx == 8'b00011000) begin
            out = arg[7];
        end
        else if (ndx == 8'b00011001) begin
            out = arg[6];
        end
        else if (ndx == 8'b00011010) begin
            out = arg[5];
        end
        else if (ndx == 8'b00011100) begin
            out = arg[4];
        end
        else if (ndx == 8'b00011101) begin
            out = arg[3];
        end
        else if (ndx == 8'b00011110) begin
            out = arg[2];
        end
        else if (ndx == 8'b00011111) begin
            out = arg[1];
        end
        else if (ndx == 8'b00100000) begin
            out = arg[0];
        end
    end
endmodule

module v0brlshl(input [31:0]  arg;
                input [4:0]   cnt;
                output [31:0] out;
                reg [31:0]    src;
                reg [31:0]    res);

    src = arg;
    res = arg;
    v0muxshl m0(src[0], res, cnt);
    v0muxshl m1({src[1:0], src[31:1]}, res[1], cnt);
    v0muxshl m2({src[2:0], src[31:2]}, res[2], cnt);
    v0muxshl m3({src[3:0], src[31:3]}, res[3], cnt);
    v0muxshl m4({src[4:0], src[31:4]}, res[4], cnt);
    v0muxshl m5({src[5:0], src[31:5]}, res[5], cnt);
    v0muxshl m6({src[6:0], src[31:6]}, res[6], cnt);
    v0muxshl m7({src[7:0], src[31:7]}, res[7], cnt);
    v0muxshl m8({src[8:0], src[31:8]}, res[8], cnt);
    v0muxshl m9({src[9:0], src[31:9]}, res[9], cnt);
    v0muxshl m10({src[10:0], src[31:10]}, res[10], cnt);
    v0muxshl m11({src[11:0], src[31:11]}, res[11], cnt);
    v0muxshl m12({src[12:0], src[31:12]}, res[12], cnt);
    v0muxshl m13({src[13:0], src[31:13]}, res[13], cnt);
    v0muxshl m14({src[14:0], src[31:14]}, res[14], cnt);
    v0muxshl m15({src[15:0], src[31:15]}, res[15], cnt);
    v0muxshl m16({src[16:0], src[31:16]}, res[16], cnt);
    v0muxshl m17({src[17:0], src[31:17]}, res[17], cnt);
    v0muxshl m18({src[18:0], src[31:18]}, res[18], cnt);
    v0muxshl m19({src[19:0], src[31:19]}, res[19], cnt);
    v0muxshl m20({src[20:0], src[31:20]}, res[20], cnt);
    v0muxshl m21({src[21:0], src[31:21]}, res[21], cnt);
    v0muxshl m22({src[22:0], src[31:22]}, res[22], cnt);
    v0muxshl m23({src[23:0], src[31:23]}, res[23], cnt);
    v0muxshl m24({src[24:0], src[31:24]}, res[24], cnt);
    v0muxshl m25({src[25:0], src[31:25]}, res[25], cnt);
    v0muxshl m26({src[26:0], src[31:26]}, res[26], cnt);
    v0muxshl m27({src[27:0], src[31:27]}, res[27], cnt);
    v0muxshl m28({src[28:0], src[31:28]}, res[28], cnt);
    v0muxshl m29({src[29:0], src[31:29]}, res[29], cnt);
    v0muxshl m30({src[30:0], src[31:30]}, res[30], cnt);
    v0muxshl m31({src[31:0], src[31:31]}, res[31], cnt);

    assign out = res;
endmodule

module v0brlshr(input [31:0]  arg;
                input [4:0]   cnt;
                output [31:0] out;
                reg [31:0]    src;
                reg [31:0]    res);

    src = cnt;
    res = arg;
    v0muxshr m0(arg[0], res, cnt);
    v0muxshr m1({arg[31:1], arg[1]}, res[1], cnt);
    v0muxshr m2({arg[31:2], arg[2]}, res[2], cnt);
    v0muxshr m3({arg[3:3], arg[3]}, res[3], cnt);
    v0muxshr m4({arg[4:4], arg[4]}, res[4], cnt);
    v0muxshr m5({arg[5:5], arg[5]}, res[5], cnt);
    v0muxshr m6({arg[6:6], arg[6]}, res[6], cnt);
    v0muxshr m7({arg[7:7], arg[7]}, res[7], cnt);
    v0muxshr m8({arg[8:8], arg[8]}, res[8], cnt);
    v0muxshr m9({arg[9:9], arg[9]}, res[9], cnt);
    v0muxshr m10({arg[10:10], arg[10]}, res[10], cnt);
    v0muxshr m11({arg[11:11], arg[11]}, res[11], cnt);
    v0muxshr m12({arg[12:12], arg[12]}, res[12], cnt);
    v0muxshr m13({arg[13:13], arg[13]}, res[13], cnt);
    v0muxshr m14({arg[14:14], arg[14]}, res[14], cnt);
    v0muxshr m15({arg[15:15], arg[15]}, res[15], cnt);
    v0muxshr m16({arg[16:16], arg[16]}, res[16], cnt);
    v0muxshr m17({arg[17:17], arg[17]}, res[17], cnt);
    v0muxshr m18({arg[18:18], arg[18]}, res[18], cnt);
    v0muxshr m19({arg[19:19], arg[19]}, res[19], cnt);
    v0muxshr m20({arg[20:20], arg[20]}, res[20], cnt);
    v0muxshr m21({arg[21:21], arg[21]}, res[21], cnt);
    v0muxshr m22({arg[22:22], arg[22]}, res[22], cnt);
    v0muxshr m23({arg[23:23], arg[23]}, res[23], cnt);
    v0muxshr m24({arg[24:24], arg[24]}, res[24], cnt);
    v0muxshr m25({arg[25:25], arg[25]}, res[25], cnt);
    v0muxshr m26({arg[26:26], arg[26]}, res[26], cnt);
    v0muxshr m27({arg[27:27], arg[27]}, res[27], cnt);
    v0muxshr m28({arg[28:28], arg[28]}, res[28], cnt);
    v0muxshr m29({arg[29:29], arg[29]}, res[29], cnt);
    v0muxshr m30({arg[30:30], arg[30]}, res[30], cnt);
    v0muxshr m31({arg[31:31], arg[31]}, res[31], cnt);

    assign out = res;
endmodule

// REFERENCE: https://verilog-code.blogspot.fi/2013/08/verilog-code-for-barrel-shifter.html

module v0muxshl(input [31:0] arg;
                input [4:0]  ndx;
                output       out;
                reg          flg)

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

module v0muxshr(input [31:0] arg;
                input [4:0]  ndx;
                output       out;
                reg          flg)

    always @(ndx) begin
        if (ndx == 8'b00000000) begin
            out = arg[31];
        end
        else if (ndx == 8'b00000001) begin
            out = arg[30];
        end
        else if (ndx == 8'b00000010) begin
            out = arg[29];
        end
        else if (ndx == 8'b00000011) begin
            out = arg[28];
        end
        else if (ndx == 8'b00000100) begin
            out = arg[27];
        end
        else if (ndx == 8'b00000101) begin
            out = arg[26];
        end
        else if (ndx == 8'b00000110) begin
            out = arg[25];
        end
        else if (ndx == 8'b0000111) begin
            out = arg[24];
        end
        else if (ndx == 8'b00001000) begin
            out = arg[23];
        end
        else if (ndx == 8'b00001001) begin
            out = arg[22];
        end
        else if (ndx == 8'b00001010) begin
            out = arg[21];
        end
        else if (ndx == 8'b00001011) begin
            out = arg[20];
        end
        else if (ndx == 8'b00001100) begin
            out = arg[19];
        end
        else if (ndx == 8'b00001101) begin
            out = arg[18];
        end
        else if (ndx == 8'b00001110) begin
            out = arg[17];
        end
        else if (ndx == 8'b00001111) begin
            out = arg[16];
        end
        else if (ndx == 8'b00010000) begin
            out = arg[15];
        end
        else if (ndx == 8'b00010001) begin
            out = arg[14];
        end
        else if (ndx == 8'b00010010) begin
            out = arg[13];
        end
        else if (ndx == 8'b00010011) begin
            out = arg[12];
        end
        else if (ndx == 8'b00010100) begin
            out = arg[11];
        end
        else if (ndx == 3'b00010101) begin
            out = arg[10];
        end
        else if (ndx == 8'b00010110) begin
            out = arg[9];
        end
        else if (ndx == 8'b00010111) begin
            out = arg[8];
        end
        else if (ndx == 8'b00011000) begin
            out = arg[7];
        end
        else if (ndx == 8'b00011001) begin
            out = arg[6];
        end
        else if (ndx == 8'b00011010) begin
            out = arg[5];
        end
        else if (ndx == 8'b00011100) begin
            out = arg[4];
        end
        else if (ndx == 8'b00011101) begin
            out = arg[3];
        end
        else if (ndx == 8'b00011110) begin
            out = arg[2];
        end
        else if (ndx == 8'b00011111) begin
            out = arg[1];
        end
        else if (ndx == 8'b00100000) begin
            out = arg[0];
        end
    end
endmodule

module v0brlshl(input [31:0]  arg;
                input [4:0]   cnt;
                output [31:0] out;
                reg [31:0]    src;
                reg [31:0]    res);

    src = arg;
    res = arg;
    v0muxshl m0(src[0], res, cnt);
    v0muxshl m1({src[1:0], src[31:1]}, res[1], cnt);
    v0muxshl m2({src[2:0], src[31:2]}, res[2], cnt);
    v0muxshl m3({src[3:0], src[31:3]}, res[3], cnt);
    v0muxshl m4({src[4:0], src[31:4]}, res[4], cnt);
    v0muxshl m5({src[5:0], src[31:5]}, res[5], cnt);
    v0muxshl m6({src[6:0], src[31:6]}, res[6], cnt);
    v0muxshl m7({src[7:0], src[31:7]}, res[7], cnt);
    v0muxshl m8({src[8:0], src[31:8]}, res[8], cnt);
    v0muxshl m9({src[9:0], src[31:9]}, res[9], cnt);
    v0muxshl m10({src[10:0], src[31:10]}, res[10], cnt);
    v0muxshl m11({src[11:0], src[31:11]}, res[11], cnt);
    v0muxshl m12({src[12:0], src[31:12]}, res[12], cnt);
    v0muxshl m13({src[13:0], src[31:13]}, res[13], cnt);
    v0muxshl m14({src[14:0], src[31:14]}, res[14], cnt);
    v0muxshl m15({src[15:0], src[31:15]}, res[15], cnt);
    v0muxshl m16({src[16:0], src[31:16]}, res[16], cnt);
    v0muxshl m17({src[17:0], src[31:17]}, res[17], cnt);
    v0muxshl m18({src[18:0], src[31:18]}, res[18], cnt);
    v0muxshl m19({src[19:0], src[31:19]}, res[19], cnt);
    v0muxshl m20({src[20:0], src[31:20]}, res[20], cnt);
    v0muxshl m21({src[21:0], src[31:21]}, res[21], cnt);
    v0muxshl m22({src[22:0], src[31:22]}, res[22], cnt);
    v0muxshl m23({src[23:0], src[31:23]}, res[23], cnt);
    v0muxshl m24({src[24:0], src[31:24]}, res[24], cnt);
    v0muxshl m25({src[25:0], src[31:25]}, res[25], cnt);
    v0muxshl m26({src[26:0], src[31:26]}, res[26], cnt);
    v0muxshl m27({src[27:0], src[31:27]}, res[27], cnt);
    v0muxshl m28({src[28:0], src[31:28]}, res[28], cnt);
    v0muxshl m29({src[29:0], src[31:29]}, res[29], cnt);
    v0muxshl m30({src[30:0], src[31:30]}, res[30], cnt);
    v0muxshl m31({src[31:0], src[31:31]}, res[31], cnt);

    assign out = res;
endmodule

module v0brlshr(input [31:0]  arg;
                input [4:0]   cnt;
                output [31:0] out;
                reg [31:0]    src;
                reg [31:0]    res);

    src = cnt;
    res = arg;
    v0muxshr m0(arg[0], res, cnt);
    v0muxshr m1({arg[31:1], arg[1]}, res[1], cnt);
    v0muxshr m2({arg[31:2], arg[2]}, res[2], cnt);
    v0muxshr m3({arg[3:3], arg[3]}, res[3], cnt);
    v0muxshr m4({arg[4:4], arg[4]}, res[4], cnt);
    v0muxshr m5({arg[5:5], arg[5]}, res[5], cnt);
    v0muxshr m6({arg[6:6], arg[6]}, res[6], cnt);
    v0muxshr m7({arg[7:7], arg[7]}, res[7], cnt);
    v0muxshr m8({arg[8:8], arg[8]}, res[8], cnt);
    v0muxshr m9({arg[9:9], arg[9]}, res[9], cnt);
    v0muxshr m10({arg[10:10], arg[10]}, res[10], cnt);
    v0muxshr m11({arg[11:11], arg[11]}, res[11], cnt);
    v0muxshr m12({arg[12:12], arg[12]}, res[12], cnt);
    v0muxshr m13({arg[13:13], arg[13]}, res[13], cnt);
    v0muxshr m14({arg[14:14], arg[14]}, res[14], cnt);
    v0muxshr m15({arg[15:15], arg[15]}, res[15], cnt);
    v0muxshr m16({arg[16:16], arg[16]}, res[16], cnt);
    v0muxshr m17({arg[17:17], arg[17]}, res[17], cnt);
    v0muxshr m18({arg[18:18], arg[18]}, res[18], cnt);
    v0muxshr m19({arg[19:19], arg[19]}, res[19], cnt);
    v0muxshr m20({arg[20:20], arg[20]}, res[20], cnt);
    v0muxshr m21({arg[21:21], arg[21]}, res[21], cnt);
    v0muxshr m22({arg[22:22], arg[22]}, res[22], cnt);
    v0muxshr m23({arg[23:23], arg[23]}, res[23], cnt);
    v0muxshr m24({arg[24:24], arg[24]}, res[24], cnt);
    v0muxshr m25({arg[25:25], arg[25]}, res[25], cnt);
    v0muxshr m26({arg[26:26], arg[26]}, res[26], cnt);
    v0muxshr m27({arg[27:27], arg[27]}, res[27], cnt);
    v0muxshr m28({arg[28:28], arg[28]}, res[28], cnt);
    v0muxshr m29({arg[29:29], arg[29]}, res[29], cnt);
    v0muxshr m30({arg[30:30], arg[30]}, res[30], cnt);
    v0muxshr m31({arg[31:31], arg[31]}, res[31], cnt);

    assign out = res;
endmodule

module v0gsdiv(input         clk;
	       input         rst;
	       input         start;
	       input [31:0]  arg;
	       input [31:0]  div;
	       output [31:0] out;
	       output reg    bsy;
	       output reg    rdy;
	       output [31:0] hi;
	       reg [63:0]    reg1;
	       reg [63:0]    reg2;
	       reg [2:0]     cnt;
	       wire [63:0]   tmp = ~reg2 + 1'b1;
	       wire [127:0]  prod1 = reg1 * tmp;
	       wire [127:0]  prod2 = reg2 * tmp;
	       assign        res = reg1[63:32] + |reg2[31:29];
	       assign        hi = reg2[62:31]);

    always @(posedge clk or negedge rst) begin
	if (!rst) begin
	    bsy <= 0;
	    rdy <= 0;
	end
	else begin
	    if (start) begin
		reg1 <= {1'b0, num, 31'b0};
		reg2 <= {1'b0, div, 31'b0};
		bsy <= 1;
		rdy <= 0;
		cnt <= 0;
	    end
	    else begin
		reg1 <= prod1[126:63];
		reg2 <= prod2[126:63];
		cnt <= cnt + 3'b1;
		if (count == 3'h4) begin
		    bsy <= 0;
		    rdy <= 1;
		end
	    end
	end
    end
endmodule
