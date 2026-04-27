## Generate bitstream only - implementation already complete
open_project ./dgrid.xpr

## Generate bitstream
launch_runs impl_1 -to_step write_bitstream -jobs 1
wait_on_run impl_1

puts "=== BITSTREAM GENERATION COMPLETED ==="
set bit_dir [get_property DIRECTORY [get_runs impl_1]]
puts "Bitstream location: ${bit_dir}/main.bit"

exit 0
