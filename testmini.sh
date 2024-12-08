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

for tst in $(ls ./tests/*.test)
do
	echo "------------------------------------------"
	echo "Testing $tst.."
	echo "------------------------------------------"
	bash -c "$(cat $tst)"
	echo
	echo "------------------------------------------"
done
