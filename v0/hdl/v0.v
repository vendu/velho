`include "v0macro.v"
// TODO: handling of OF for v0arith and v0multi
// flg: 0x01 - zero-flag
//      0x02 - carry-bit
//      0x04 - overflow - TODO
module v0arith(op, a, b, fin, out, flg)
    input [2:0]  op;
    input [31:0] a;
    input [31:0] b;
    input [2:0]  fin;
    output [31:0]  out;
    wire [2:0]   flg;
    reg [31:0]   src;
    reg [31:0]   inc;
    reg [31:0]   tmp;
    reg [31:0]   val;
    reg [31:0]   res;
    reg [31:0]   sign;

    always @(op and a and b and fin) begin
        sign = 32'h80000000;
        bits = fin;
        src = a;
        // DEC is done by adding 0xffffffff (i.e. -1) to src
        tmp = op[2:0] == 3'h002 ? 32'hffffffff : 32'h00000001;
        val = ~src;
        if (op[2:0] < 3'b011) begin
            // INC; DEC
            res = src + tmp;
        end
        inc = tmp + val;        // inc = ~src + 1, i.e. neg(src)
        if (op[2:0] == 3'011) begin
            // 0x03 - CMP
            res = src + inc;
            /* set flag-bits */
            if (!res) begin
                // set zero-flag
                assign flg[0] = 1'b1;
            end
            else if (res < b) begin
                // set carry-bit
                assign flg[1] = 1'b1;
            end
            if (res & sign) begin
                // set sign-bit
                assign flg[2] = 1'b1;
            end
        end
        else if (op[2:0] > 3'b011 && !op[0]) begin
            // ignore carry-bit
            if (op[2:1] == 2'b10) begin
                // 0x04 - ADD
                res = src + b;
            end
            if (op[2:1] = 2'b11) begin
                // 0x06 - SUB
                res = inc + b;
            end
        end
        else if (op[2:0] > 3'b011 && op[0]) begin
            // set carry-bit
            if (op[2:1] == 2'b10) begin
                // 0x05 ADC
                res = src + b;
                if (res < b) begin
                    // set carry-bit
                    assign flg[1] = 1'h1;
                end
            end
            if (op[2:1] == 2'b11) begin
                // SBC
                res = inc + b;
                if (res < b) begin
                    // set carry-bit
                    assign flg[1] = 1'h1;
                end
            end
        end
        assign out = res;
    end
endmodule

module v0shift(op, a, b, out)
    input [2:0]  op;
    input [4:0]  a;
    input [31:0] b;
    output [31:0]  out;
    reg [31:0]   val;
    reg [31:0]   res;
    reg [31:0]   mask;
    reg [31:0]   sign;
    reg [31:0]   tmp;
    reg [31:0]   cnt;
    reg [31:0]   bits;

    always @(op and a and b) begin
        val = b;
        if (!op[1]) begin
            // shift left
            // logical SHL
            res = val << a;
        end
        else begin
            // shift right
            if (!op[0]) begin
                // logical SHR
                res = val >> a;
            end
            else begin
                // arithmetic SAR
                sign = 8'h80000000;
                bits = 8'hffffffff;
                tmp = b & sign;
                cnt = 32 - a;
                if (tmp) begin
                    mask = bits << cnt;
                    bits = val >> a;
                    res = bits | mask;
                end
            end
        end
        else begin
            res = val >> a;
            //                wres = val >>> a;
        end
        assign out = res;
    end
endmodule

module v0logic(op, a, b, out)
    input [3:0]  op;
    input [31:0] a;
    input [31:0] b;
    output [31:0]  out;
    reg [31:0]   val;
    reg [31:0]   dest;

    always (op and a and b) begin
        val = a;
        if (op[3:0] == 4'h0c) begin
            // single-argument NOT
            dest = ~val;
        end
        else if (op[3:0] > 4'h0c) begin
            dest = b;
            case (op[3:0])
                4'h0d: dest = val & b;
                4'h0e: dest = val ^ b;
                4'h0f: dest = val | b;
            endcase // case (op[3:0])
        end
        assign out = dest;
    end
endmodule

module v0multi(op, a, b, fin, out, flg)
    input [3:0]  op;
    input [31:0] a;
    input [31:0] b;
    input [2:0]  fin;
    output [31:0]  out;
    wire [2:0]   flg;
    reg [31:0]   src;
    reg [31:0]   dest;
    reg [63:0]   res;
    reg [31:0]   sign;
    reg [2:0]    bits;

    always $(op and a and b and fin) begin
        sign = 32'h80000000;
        res = a * b;
        if (op[0]) begin
            // MUL; return low-word of result
            dest = res[31:0];
            if (dest < a) begin
                bits[2] = 1'h1;
            end
            else if (!dest) begin
                bits[0] = 1'h1;
            end
        end
        else begin
            dest = res[63:32];
        end
        assign flg = bits;
        assign out = dest;
    end
endmodule

module v0clz(a; out)
    input [31:0]  a;
    output [31:0] b;
    reg [31:0]    res;
    reg [31:0]    tmp;
    reg [31:0]    mask;

    always @(a) begin
        if (!u32) begin
            res = 32;
        end
        else begin
            res = 0;
            tmp = u32;
            mask = 32'h80000000;
            if (!(tmp & mask)) begin
                mask = 0xffff0000;
                if (!(tmp & mask)) begin
                    tmp = tmp << 16;
                    res += 16;
                end
                mask <<= 8;
                if (!(tmp & mask)) begin
                    tmp = tmp << 8;
                    res += 8;
                end
                mask <<= 4;
                if (!(tmp & mask)) begin
                    tmp = tmp << 4;
                    res += 4;
                end
                mask <<= 2;
                if (!(tmp & mask)) begin
                    tmp = tmp << 2;
                    res += 2;
                end
                mask <<= 1;
                if (!(tmp & mask)) begin
                    res++;
                end
            end
        end
        assign out = res;
    end
endmodule

module v0ham(a, out)
    input [31:0]  a;
    output [31:0] out;
    reg [31:0]    res;
    reg [31:0]    tmp1;
    reg [31:0]    tmp2;
    reg [31:0]    tmp3;
    reg [31:0]    tmp4;
    reg [31:0]    val1;
    reg [31:0]    val2;
    reg [31:0]    val3;
    reg [31:0]    val4;
    reg [31:0]    val5;

    always @(a) begin
        val1 = 32'h55555555;
        val2 = 32'h33333333;
        val3 = 32'h0f0f0f0f;
        val4 = 32'h00ff00ff;
        val5 = 32'h0000ffff;
        tmp1 = a >> 1;
        tmp2 = a & val1;
        tmp3 = tmp1 & val1;
        tmp4 = tmp2 + tmp3;
        tmp1 = tmp4;
        tmp2 = tmp4 >> 2;
        tmp3 = tmp4 & val2;
        tmp1 = tmp2 & val2;
        tmp4 = tmp1 + tmp3;
        tmp2 = tmp4;
        tmp1 = tmp4 >> 4;
        tmp3 = tmp2 & val3;
        tmp4 = tmp1 & val3;
        tmp3 = tmp3 + tmp4;
        tmp2 = tmp3;
        tmp1 = tmp3 >> 8;
        tmp4 = tmp3 & val4;
        tmp2 = tmp1 & val4;
        tmp3 = tmp2 + tmp4;
        tmp1 = tmp3;
        tmp2 = tmp3 >> 16;
        tmp4 = tmp1 & val5;
        tmp3 = tmp2 & val5;
        res = tmp3 + tmp4;
        assign out = res;
    end
endmodule

module v0bitop1(op, a, out)
    input [4:0]  op;
    input [31:0] a;
    output  [31:0] out;
    reg [31:0]   res;

    always @(op and a and out) begin
        if (op[4:0] == 5'h13) begin
            v0clz(a, res);
        end
        else if (op[4:0] == 5'h14) begin
            v0ham(a, res);
        end
        assign out = res;
    end
endmodule

// str == 1 -> store; determine destination address
// exc: 1 -> INV-error
//      2 -> ALN-error
//      4 -> BUS-error
//      8 -> memory address written to out
module v0ldstr(op, a, parm, aflg, out, flg)
    input [31:0]  op;
    input [31:0]  a;
    input [3:0]   parm;
    input [1:0]   aflg;
    output [31:0] out;
    wire [3:0]    flg;
    output        mem;
    reg [31:0]    val;
    reg [31:0]    adr;
    reg [31:0]    ptr;
    reg [31:0]    ofs;
    reg [3:0]     reg1;
    reg [3:0]     reg2;
    reg [3:0]     cnt;
    reg [3:0]     bits;
    reg [2:0]     err;
    reg           mbit;

    // TODO: fetch instructions
    always @(op and a and parm and aflg and out and flg) begin
        ofs = 4;
        reg1 = op[11:8];
        reg2 = op[15:12];
        ptr = op + ofs;
        cnt = op[21:18];        // parm-field
        err = 0;
        // LDR, LDX - memory to register, register to register
        if (aflg == 2'b00) begin
            // register arguments
            val = a;
        end
        else if (aflg == 2'b01) begin
            // immediate argument
            val = op[31:22];
        end
        else if (aflg == 2'b10) begin
            // direct-address argument follows operation
            mbit = 1;
            v0fetch(ptr, adr);
        end
        else if (aflg == 2'b11) begin
            // indexed addressing mode
            mbit = 1'b1;
            v0fetch(ptr, ofs);
            val = ofs << cnt;
            adr = a + val;
            v0fetch(adr, val);
        end
        if (mbit) begin
            v0fetch(ptr, val);
        end
        if (op[0]) begin
            v0sex(val, parm);
        end
        if (mbit) begin
            // have memory-argument
            if (op[7:0] <= 0x21) begin
                // LDR, LDX
                v0load(val, reg2, parm, exc);
            end
            else begin
                // STR, STX
                v0store(val, adr, parm, exc);
            end
            assign out = adr;
        end
        else begin
            // register-to-register
            v0copy(val, reg2, parm, exc);
            assign out = val;
        end
        assign flg = bits;
    end
endmodule

// op: stack operation ID; see <v0/vm.h>
// map: register ID (PSH, POP) or bitmap (PSM, POM)
// sp: stack pointer before operation
// out: stack pointer after operation
module v0stkop(op, a, b, sp, spout)
    input [2:0]  op;
    input [31:0] a;
    input [31:0] b;
    input [31:0] sp;
    wire [31:0]  spout;

    always $(op and a and b and sp and spout) begin
        if (!op[2]) begin
            // PSH, PSM
            if (!op[0]) begin
                v0psh(a, sp, spout);
            end
            if (op[0]) begin
                v0pop(a, sp, spout);
            end
        end
        else begin
            // POP, POM
            if (!op[0]) begin
                v0psm(a, b, sp, spout);
            end
            if (op[2]) begin
                v0pom(a, b, sp, spout);
            end
        end
    end
endmodule

module v0proc(rst, start, stk, intr)
    input        rst; // reset signal
    input [31:0] start; // code segment base address
    input [31:0] stk; // stack base address (top)
    wire [7:0]   intr; // interrupt lines
    reg [31:0]   iregs[15:0]; // general-purpose register
    reg [31:0]   pc; // program counter ('instruction pointer')
    reg [31:0]   fp; // frame pointer
    reg [31:0]   sp; // stack pointer
    reg [31:0]   msw; // machine status-word
    reg [31:0]   op; // current operation parcel
    reg [31:0]   arg1;
    reg [31:0]   arg2;
    reg [31:0]   adr;
    reg [31:0]   tmp;
    reg [3:0]    reg1;
    reg [3:0]    reg2;
    reg [9:0]    imm;
    reg [3:0]    flg; // [memory] fault, adjust to bigger size later

    pc = start;
    sp = stk;
    always @(posedge clk) begin
        v0getop(pc, op);
        reg1 = op[11:8];    // register #1 ID
        reg2 = op[15:12];   // register #2 ID
        arg1 = iregs[reg1]; // prefetch register #1
        arg2 = iregs[reg2]; // prefetch register #2
        adr = op[17:16];
        parm = op[21:18];        // parm-field
        if (op[7:0] <= 8'h07) begin
            v0arith(op[2:0], arg1, arg2, msw[2:0], iregs[reg2], msw[2:0]);
        end
        else if (op[7:0] <= 8'h0b) begin
            v0shift(op[3:0], arg1, arg2, iregs[reg2]);
        end
        else if (op[7:0] <= 8'h0f) begin
            v0logic(op[3:0], arg1, arg2, iregs[reg2]);
        end
        else if (op[7:0] == 8'h10) begin
            v0recip(arg1, arg2, iregs[reg2]);
        end
        else if (op[7:0] == 8'h11 || op[7:0] <= 8'h12) begin
            v0multi(op, arg1, arg2, msw[2:0], iregs[reg2], msw[2:0]);
        end
        else if (op[7:0] <= 8'h18) begin
            v0bitop1(op[4:0], arg1, iregs[reg2]);
        end
        else if (op[7:0] >= 8'h20 && op[7:0] <= 8'h23) begin
            v0ldstr(op, arg1, parm, val, flg);
        end
        else if (op[7:0] <= 8'h27) begin
            arg2 = sp;
            v0stkop(op[2:0], arg1, parm, arg2, sp);
        end
    end
endmodule

