# Output file
output_file="$1/network_info.txt"

# Clear the file if it already exists
> "$output_file"

# Use system_profiler to get a list of installed applications and write to file
netstate >> "$output_file"

route -n get default >> "$output_file"

lsof -i >> "$output_file"

echo "The list of installed applications has been saved to $output_file"

