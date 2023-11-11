# Output file
output_file="installed_programs_linux.txt"

# Check for dpkg and use it if it's present
if command -v dpkg &> /dev/null
then
    dpkg --get-selections > "$output_file"
elif command -v rpm &> /dev/null
then
    rpm -qa > "$output_file"
elif command -v pacman &> /dev/null
then
    pacman -Q > "$output_file"
else
    echo "No known package manager found, script will not run."
    exit 1
fi

echo "The list of installed applications has been saved to $output_file"
