function chris_with_cmd_history(){
	history -15 | awk '{$1=""; print substr($0,2)}' > ~/.chris_history
	chris "$@"
}
alias chris='chris_with_cmd_history'
