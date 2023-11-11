# Output file
output_file="installed_programs_macos.txt"

# Use system_profiler to get a list of installed applications and write to file
system_profiler SPApplicationsDataType > "$output_file"

echo "The list of installed applications has been saved to $output_file"
