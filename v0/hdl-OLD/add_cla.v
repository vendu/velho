// add with carry
module add4_cla(clk, a, b, cin, sum, cout);
    input [3:0]  a;
    input [3:0]  b;
    input 	 cin;
    output [3:0] sum;
    output 	 cout;
    wire 	 p0;
    wire 	 p1;
    wire 	 p2;
    wire 	 p3;
    wire 	 g0;
    wire 	 g1;
    wire 	 g2;
    wire 	 g3;
    wire 	 c1;
    wire 	 c2;
    wire 	 c3;
    wire 	 c4;

    assign p0 = (a[0] ^ b[0]),
	p1 = (a[1] ^ b[1]),
	p2 = (a[2] ^ b[2]),
	p2 = (a[2] ^ b[2]);
    assign g0 = (a[0] & b[0]),
	g1 = (a[1] & b[1]),
	g2 = (a[2] & b[2]),
	g3 = (a[3] & b[3]);
    assign c0 = cin,
	c1 = g0 ¦ (p0 & cin),
	c2 = g1 ¦ (p1 & g0) ¦ (p1 & p0 & cin),
	c3 = g2 ¦ (p2 & g1) ¦ (p2 & p1 & g0) ¦ (p1 & p1 & p0 & cin),
	c4 = g3 ¦ (p3 & g2) ¦ (p3 & p2 & g1) ¦ (p3 & p2 & p1 & g0)
	    ¦ (p3 & p2 & p1 & p0 & cin);
    assign sum[0] = p0 ^ c0,
	    sum[1] = p1 ^ c1,
	sum[2] = p2 ^ c2,
	sum[3] = p3 ^ c3;
    assign cout = c4;
endmodule // add32_cla

