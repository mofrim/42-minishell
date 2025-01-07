#!/usr/bin/env bash

# NL=$'\n'
# INPUT="/bin/echo ""$?""$NL"
# echo -n "enable -n .$NL$INPUT" | bash 2>tmp_err_bash >tmp_out_bash
# echo -n "$INPUT" | ./minishell 2>tmp_err_minishell >tmp_out_minishell

func () {
	IFS=''
	eof=0
	while [[ $eof == 0 ]] ;
	do
		read -r line
		eof=$?
		echo "line: $line"
	done
}

func $1
