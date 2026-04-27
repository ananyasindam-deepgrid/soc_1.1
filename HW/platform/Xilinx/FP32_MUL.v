//----------------------------------------------------------------------------
// Copyright [2026] [Deepgrid Semi Pvt Ltd]
//
// Author: Deepgrid Semi Pvt Ltd
//----------------------------------------------------------------------------

//---
//-- Float multiplication
//---


module FP32_MUL
   #(parameter
      LATENCY=8
   )
   (
      input reset_in,
      input clock_in,
      input add_sub_in,
      input [31:0] x1_in,
      input [31:0] x2_in,
      output [31:0] y_out
   );

float_mul float_mul_inst
(
  .aclk(clock_in),
  .s_axis_a_tvalid(1),
  .s_axis_a_tdata(x1_in),
  .s_axis_b_tvalid(1),
  .s_axis_b_tdata(x2_in),
  .m_axis_result_tvalid(),
  .m_axis_result_tdata(y_out)
);

endmodule