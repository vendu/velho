// REFERENCE:

module genadder #(parameter SIZE = 4)(
                                      input logic [SIZE-1:0]  a;
                                      input logic [SIZE-1:0]  b;
                                      input logic             cin;
                                      output logic            cout;
                                      output logic [SIZE-1:0] sum)
    wire [SIZE:0] cres;
    genvar i;

    assign c[0] = cin;
    for ( i = 0 ; i < SIZE ; i = i + 1) begin : bitnum
        wire t1;
        wire t2;
        wire t3;

        xor g1 (t1, a[i], b[i]);
        xor g2 (sum[i], t1, c[i]);
        xor g3 (t2, a[i], b[i]);
        xor g4 (t3, t1, c[i]);
        xor g5 (c[i + 1], t2, t3);
    end

    assign cout = res[SIZE];
endmodule

