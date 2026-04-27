# Support for different RISCV implementation 

dgrid uses RISCV implementation from DgsRiscv

The subfolders below provides different riscv implementation

* [HW/riscv/xilinx_jtag](./xilinx_jtag): RISCV program is loaded using xilinx built in JTAG

* [HW/riscv/sim](./sim): For simulation, memory content is set by test bench before boot

You may replace this default riscv with other implementations as long as it has the same interface below 

dgrid expects riscv with the following interface

* IBUS: 32 bit AXI master

* DBUS: 32 bit AXI master
