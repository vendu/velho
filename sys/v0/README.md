# Velho Processor 0; V0
  ---

This directory contains an on-going design of the first version of my
new Velho Processor Architecture.

First and foremost, I'd like to extend my greetings & the best of wishes
to the whole, lively online hacker community including but not limited to

[dmr](https://www.bell-labs.com/usr/dmr/www/)

[don](https://www-cs-faculty.stanford.edu/~knuth/)

[hank](https://www.hackersdelight.org/)

[uri](https://metacpan.org/author/URI)

[raster](https://rasterman.com/)

[rms](https://stallman.org/)

[richard](http://www.kohala.com/start/)

[steve](https://www.apple.com/stevejobs/)

[bill](https://www.gatesnotes.com/)

The reasons for doing this are mostly educational, and of course having fun
doing all this.

V0 is designed to be friendly to the programmers and compiler implementors. The
instruction set is relatively RISC-like with a few additions here and there.

One of the [educational] purposes of the Velho Project is to explore and
develop new tools much like

[clash-lang](https://clash-lang.org/)
[fhdl](http://cs.ecs.baylor.edu/~maurer/fhdl.html)

Here's some software I plan to do for the virtual machine (hopefully later to
be implemented in hardware using FPGA-kits).

- an AT&T syntax assembler
  - this is quite far already, based on work I did for some earlier virtual
    architectures; the assembler needs some things added as well as more
    exhaustive testing and fixing of bugs and features
- a programming game; assembly programming resembling programming today's
  microprocessor architectures
  - possibility to compete in code size
  - relative, somewhat hypothetical speed for different architectures based
    on measurements doing things on a real processor
- C compiler? This could be either a retargeted one a'la LCC or TinyCC or a
  brand new one, in which case I would pay attention to vector-based
  intermediate program presentation to leverage utilisation of
  [hardware-specific] SIMD instructions avaiblable in many modern systems
- development tools
  - graphical debuggers for machine- and source-level debugging of programs
  - some kind of visualisation of things such as program memory footprint and
    execution flow
  - possibly profiler(s)
  - a programmable programmer's calculator; in addition to the virtual machine,
    we could experiment with arbitrary-precision mathematics
  - machine [and possibly cache] simulators in Valgrind-style
    - use V0 as the simulated architecture instead of more complicated ones
      such as IA-32 and X86-64
- perhaps support for other languages; Forth? Lisp? :)

