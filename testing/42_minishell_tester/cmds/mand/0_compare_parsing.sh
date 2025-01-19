# **************************************************************************** #
#                                COMPARE PARSING                               #
# **************************************************************************** #

### comparing your input parsing to bash ###
/usr/bin/env echo ""$?""

/usr/bin/env echo $?"42"

/usr/bin/env echo ''$?''"42"

/usr/bin/env echo '"$USER"'

/usr/bin/env echo "'$USER'"

/usr/bin/env echo ""'$USER'""

/usr/bin/env echo '"'$USER'"'

/usr/bin/env echo "''$USER''"

/usr/bin/env echo "'"'$USER'"'"

/usr/bin/env echo '"'"$USER"'"'

/usr/bin/env echo $"HOME"$USER

/usr/bin/env echo $"HOM"E$USER

/usr/bin/env echo "exit_code ->$? user ->$USER home -> $HOME"

/usr/bin/env echo $"HOME"

/usr/bin/env echo $"42$"

/usr/bin/env echo $42$

/usr/bin/env echo "$ "

/usr/bin/env echo hi >./outfiles/outfile01 | /usr/bin/env echo bye

/usr/bin/env echo <123 <456 hi | /usr/bin/env echo 42

/usr/bin/env echo '$='

/usr/bin/env echo '$ '

/usr/bin/env echo "$?"

/usr/bin/env echo '$?'

/usr/bin/env echo "'$?'"

# /usr/bin/env echo \$USER

# /usr/bin/env echo \\$USER

# /usr/bin/env echo \\\$USER

# /usr/bin/env echo \\\\$USER

# /usr/bin/env echo \\\\\$USER

# /usr/bin/env echo \\\\\\\\\$USER

# /usr/bin/env echo \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\$USER \$PATH \\$PWD

/usr/bin/env echo "cat lol.c | cat > lol.c"

/usr/bin/env echo "cat lol.c '|' cat > lol.c"

/usr/bin/env echo '$USER' "$USER" "text  ' text"

/usr/bin/env echo $USER =intergalaktikus miaf*szomez

/usr/bin/env echo -n"-n" bonjour

/usr/bin/env echo "'$USER'"

/usr/bin/env echo " '$USER' "

/usr/bin/env echo text"$USER"

/usr/bin/env echo text"'$USER'" ' $USER '

/usr/bin/env echo "text"   "$USER"    "$USER"

/usr/bin/env echo '              $USER          '

/usr/bin/env echo ''''''''''$USER''''''''''

/usr/bin/env echo """"""""$USER""""""""

/usr/bin/env echo '""""""""$USER""""""""'

/usr/bin/env echo ""'""""""$USER""""""'""

/usr/bin/env echo """"""""'$USER'""""""""

/usr/bin/env echo """""""'"$USER"'"""""""

/usr/bin/env echo """"""'""$USER""'""""""

/usr/bin/env echo ""'""'""""$USER""""'""'""

/usr/bin/env echo '""'""'""""$USER""""'""'""'

/usr/bin/env echo $USER'$USER'text oui oui     oui  oui $USER oui      $USER ''

/usr/bin/env echo "text" "text$USER" ... "$USER"

/usr/bin/env echo

/usr/bin/env echo ''

/usr/bin/env echo ""

/usr/bin/env echo '' ""

/usr/bin/env echo /bin/echo

/usr/bin/env echo test1

/usr/bin/env echo 'test1'

/usr/bin/env echo "/bin/echo hallo"

/usr/bin/env echo /bin/echo"1"

/usr/bin/env echo "test1 test2"

/usr/bin/env echo "test1  test2" test3

/usr/bin/env echo " test1 test2 " 'test3 '

/usr/bin/env echo test1		test2

/usr/bin/env echo $USER$TESTNOTFOUND$HOME$

/usr/bin/env echo $USER$TESTNOTFOUND$HOME$WTF$PWD

/usr/bin/env echo -n test1

/usr/bin/env echo -n "test1 test2"

/usr/bin/env echo -n "test1 test2" test3

/usr/bin/env echo -n " test1 test2 " 'test3 '

/usr/bin/env echo -n test1		test2

/usr/bin/env echo test1 -n

/usr/bin/env echo "test1 -n"

/usr/bin/env echo -n -n test1

/usr/bin/env echo -n -n -n -n -n test1

/usr/bin/env echo "  -nn "

/usr/bin/env echo "-n test1 -n test2"

/usr/bin/env echo "test1 -n test2"

/usr/bin/env echo ~42

/usr/bin/env echo -n -n -nasd

/usr/bin/env echo -n -n -n-nnnnn

/usr/bin/env echo -n -nnnnnnn -n -nnn -nnnnn -n-n

/usr/bin/env echo -n -nnnnnnn -n -nnn -nnnnn -n feel my pain

/usr/bin/env echo -n -n -n-n

/usr/bin/env echo "'totally logical'"

/usr/bin/env echo 'totally logical'

/usr/bin/env echo ''totally logical''

/usr/bin/env echo ""'totally logical'""
