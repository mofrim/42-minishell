NL=$'\n'; line='/usr/bin/env echo $USER'; INPUT="$line$NL"; echo -n "$INPUT" | strace -f -s 999 ../../minishell 2> env.log
