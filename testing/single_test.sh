#!/usr/bin/env bash

## initial setup
NL=$'\n'
INPUT=""

## enter line from test file here
# line='/run/current-system/sw/bin/echo """"""""$USER""""""""'
# line='/usr/bin/env echo """"""""$USER""""""""'
# line='echo """"""""$USER""""""""'
#
# line='/run/current-system/sw/bin/echo $"HOME"'
# line='/usr/bin/env echo $"HOME"'
# line='echo $"HOME"'
#
# line='/run/current-system/sw/bin/echo $USER'
# line='/usr/bin/env echo $USER'
# line='echo $USER'
#
# line='/run/current-system/sw/bin/echo $HOME'
# line='/usr/bin/env echo $HOME'
# line='echo $HOME'
#
# line='/run/current-system/sw/bin/echo $bla'
# line='/usr/bin/env echo $bla'
# line='echo $bla'
#
line='/run/current-system/sw/bin/echo $"USER"'
# line='/usr/bin/env echo $"USER"'
#

## no need to touch this
INPUT+="$line$NL"

## comment out if you just want to run one of them
#
# echo -n "enable -n .$NL$INPUT" | bash 2>tmp_err_bash >tmp_out_bash
# echo -n "enable -n .$NL$INPUT" | bash
exit_code_bash=$?
# echo -n "$INPUT" | ../minishell 2>tmp_err_minishell >tmp_out_minishell
echo -n "$INPUT" | ../minishell
exit_code_minish=$?
echo
echo "exit_code_bash: $exit_code_bash, exit_code_minish: $exit_code_minish"
