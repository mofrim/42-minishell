#!/usr/bin/env bash

## initial setup
NL=$'\n'
INPUT=""

## enter line from test file here
line="/bin/echo "'"'$USER'"'""

## no need to touch this
INPUT+="$line$NL"

## comment out if you just want to run one of them
echo -n "enable -n .$NL$INPUT" | bash 2>tmp_err_bash >tmp_out_bash
echo -n "$INPUT" | ./minishell 2>tmp_err_minishell >tmp_out_minishell
