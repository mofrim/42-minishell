#!/usr/bin/env bash

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    testmini.sh                                        :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fmaurer <fmaurer42@posteo.de>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/08 22:51:01 by fmaurer           #+#    #+#              #
#    Updated: 2024/12/08 22:51:01 by fmaurer          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
RST='\033[0m'      # Text Reset
BLK='\033[0;30m'   # Black
RED='\033[0;31m'   # Red
GRN='\033[0;32m'   # Green
YLO='\033[0;33m'   # Yellow
BLU='\033[0;34m'   # Blue
PUR='\033[0;35m'   # Purple
CYA='\033[0;36m'   # Cyan
WHT='\033[0;37m'   # White

if [ -n "$1" ]
then
	bash -c "$(cat $1)"
else
	for tst in $(ls ./tests/*.test)
	do
		echo -e "${YLO}------------------------------------------"
		echo -e "Testing ${PUR}$tst.."
		cat $tst
		echo -e "${YLO}------------------------------------------${RST}"
		bash -c "$(cat $tst)"
		echo
		echo -e "${YLO}------------------------------------------"
		echo -e "${GRN}>>>> Press <enter> for next test!${RST}"
		read
	done
fi
