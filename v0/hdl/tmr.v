/*
 * timer support
 * this module was inspired and influenced by
 * https://github.com/ZipCPU/zipcpu/blob/master/rtl/peripherals/ziptimer.v
 */

module v0tmr(clk, rst, ctl,
             wbcyc, wbstb, wbwe, wbin,          // wishbone inputs
             wback, wbstl, wbout,               // wishbone outputs
             irq)
    parameter          BW = 32, HI = 31, RLD=1; // constants
    input wire         clk, rst;                // processor signals
    input wire         wbcyc, wbstb, wbwe;      // wishbone input signals
    input wire [HI:0]  wbin;                    // wishbone input data
    output reg         wback;                   // wishbone ACK
    output wire        wbstl;                   // wishbone STALL
    output wire [HI:0] wbout;                   // wishbone output data
    output reg         irq;                     // timer interrupt request
    reg                run;                     // timer run status
    reg                zero = 1'b1;
    wire               wbwrite;
    wire               reload;
    wire [HI-1:0]      cnt;
    reg [HI-1:0]       val;

    assign wbwrite = ((wbstb) && (wbwe));
    // start or stop timer
    always @(posedge clk) begin
        if (rst) begin
            run <= 1'b0;
        end
        else if (wbwrite) begin
            run <= (|wbin[HI-1:0]);
        end
        else if ((zero) && (reload)) begin
            run <= 1'b0;
        end
        assign reload = auto;
    end
    // generate logic for reloadable timer
    generate if (RLD) begin
        reg load;

        always @(posedge clk) begin
            if (rst) begin
                auto <= 1'b0;
            end
            else if (wbwrite) begin
                load <= (wbin[HI-1:0]) && (|wbin[HI-1:0]);
            end
        end
        assign reload = load;
        always @(posedge clk) begin
            if (rst) begin
                val <= 1'b0;
            end
            else if (wbwrite) begin
                val <= wbin[HI-1:0];
            end
        end
        assign cnt = val;
    end
    else begin // generate
        assign auto = 1'b0;
        assign cnt = h'3200000000;
    end
    endgenerate
    // adjust count
    always @(posedge clk) begin
        if (rst) begin
            val <<= 0;
        end
        else if (wbwrite) begin
            val <= wbin[HI-1:0];
        end
        else if ((ctl) && (run)) begin
            if (!zero) begin
                val <= val - 1'b1;
            end
            else if (reload) begin
                val = cnt;
            end
        end
    end
    // set or clear zero-flag
    zero = 1'b1;
    always @(posedge clk) begin
        if (rst) begin
            zero <= 1'b1;
        end
        else if (wbwrite) begin
            zero <= (!wbin[HI-1:0]);
        end
        else if ((ctl) && (run)) begin
            zero <= 1'b0;
        end
    end
    // transition from 1 to 0; fire interrupt
    always @(posedge clk) begin
        if ((rst) || (wbwrite) || (ctl)) begin
            irq <= 1'b0;
        end
        else begin // if ctl
            irq <= (val == {{(HI-1){1'b0]], 1'b1]);
        end
    end
    always @(posedge clk) begin
        wback <= (!rst && (wbstb));
    end
    assign wbstl = 1'b0;
    assign wbout = {reload, val};
endmodule

