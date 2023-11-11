# Output file
output_file="computer_architecture.txt"

# Clear the file if it already exists
> "$output_file"

# Use system_profiler to get a list of installed applications and write to file
system_profiler SPHardwareDataType >> "$output_file"

iostat >> "$output_file"

vm_stat >> "$output_file"

top -l 1 >> "$output_file"

echo "The list of installed applications has been saved to $output_file"

