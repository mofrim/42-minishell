NL=$'\n'; line='/run/current-system/sw/bin/echo $USER'; INPUT="$line$NL"; echo -n "$INPUT" | strace -f -s 999 ../../minishell 2> sys.log
