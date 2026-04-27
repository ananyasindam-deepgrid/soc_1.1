## Run synthesis and implementation in batch mode
## Uses -jobs 1 to minimize peak memory usage on 16GB systems
open_project ./dgrid.xpr

## Force re-read of all source files
update_compile_order -fileset sources_1

## Reset previous runs
catch {reset_run impl_1}
catch {reset_run synth_1}

## Launch synthesis (1 job = less memory)
launch_runs synth_1 -jobs 1
wait_on_run synth_1

## Check synthesis status
set synth_status [get_property STATUS [get_runs synth_1]]
puts "Synthesis status: $synth_status"
if {$synth_status != "synth_design Complete!"} {
    puts "ERROR: Synthesis failed with status: $synth_status"
    exit 1
}
puts "=== SYNTHESIS COMPLETED SUCCESSFULLY ==="

## Launch implementation (1 job = less memory)
launch_runs impl_1 -jobs 1
wait_on_run impl_1

set impl_status [get_property STATUS [get_runs impl_1]]
puts "Implementation status: $impl_status"
if {$impl_status != "route_design Complete!"} {
    puts "ERROR: Implementation failed with status: $impl_status"
    exit 1
}
puts "=== IMPLEMENTATION COMPLETED SUCCESSFULLY ==="

## Generate bitstream
launch_runs impl_1 -to_step write_bitstream -jobs 1
wait_on_run impl_1

puts "=== BITSTREAM GENERATION COMPLETED ==="
puts "Bitstream location: [get_property DIRECTORY [get_runs impl_1]]/main.bit"

exit 0
