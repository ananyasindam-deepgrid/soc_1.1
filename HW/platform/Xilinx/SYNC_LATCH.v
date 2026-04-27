//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//----------------------------------------------------------------------------

// Implement latch
// This latch is used in a synchronous manner so it is safe
// Input signal is guaranteed to be stable during the level transision
// The latch is mainly used to overclock the memory block to improve memory utiization
// This code should be portable among different synthesis. It is part of the
// platform package because GHDL has trouble with converting latch construct from
// VHDL to verilog
//
module SYNC_LATCH #(parameter DATA_WIDTH=32)
   (
      input enable_in,
      input [DATA_WIDTH-1:0] data_in,
      output [DATA_WIDTH-1:0] data_out
   );

reg [DATA_WIDTH-1:0] data_r;

assign data_out = data_r;

always @(enable_in or data_in) begin
   if(enable_in)
      data_r <= data_in;
end

endmodule
