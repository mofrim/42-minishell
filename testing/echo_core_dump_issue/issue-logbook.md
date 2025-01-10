# the `/usr/bin/env echo """"""""$USER""""""""` core dump issue

## the commands for entering at the prompt of bash / zsh

- the command for entering on command line, with builtin echo:
`NL=$'\n'; line='echo """"""""$USER""""""""'; INPUT="$line$NL"; echo -n "$INPUT" | ..hell`

- using system echo:
`NL=$'\n'; line='/run/current-system/sw/bin/echo """"""""$USER""""""""'; INPUT="$line$NL"; echo -n "$INPUT" | ../minishell`

- using env's echo:
`NL=$'\n'; line='/usr/bin/env echo """"""""$USER""""""""'; INPUT="$line$NL"; echo -n "$INPUT" | ../minishell`

- or with strace:
`NL=$'\n'; line='echo """"""""$USER""""""""'; INPUT="$line$NL"; echo -n "$INPUT" | strace ../minishell > shell.log`

- with strace simple version
  `NL=$'\n'; line='/usr/bin/env echo $USER'; INPUT="$line$NL"; echo -n "$INPUT" | strace ../minishell 2> env.log`

  `NL=$'\n'; line='/run/current-system/sw/bin/echo $USER'; INPUT="$line$NL"; echo -n "$INPUT" | strace ../minishell 2> env.log`


## results

- the problems do not exist if running echo like
  `/run/current-system/sw/bin/echo`. if i run echo with `/usr/bin/env echo` i
  run into memory corruption issues.

## riddles

okay. test input

    """"""""$USER""""""""

- fails on
  + `/usr/bin/env echo`
    -> bash: "invalid pointer", zsh: "double free"
- exit finely on
  + `/run/current-system/sw/bin/echo`
  + `echo`
  
the test input

    $"HOME" 

- fails on:
  + `/run/current-system/sw/bin/echo`
- exits finely on
  + `/usr/bin/env echo`
  + `echo`

Of course this is all only happening in script-mode not in interactive mode!!!

**WTF is going on here ?!?!?!?!?!?**

-> Observations:
  - If envvar exists, `/usr/bin/env echo` fails
  - If it does not exist none fails.
  - but what about $"HOME" ??? 
    + if input is `$"EXISTING_VAR"`, `/run/current-system/sw/bin/echo` fails
    + if input is `$"NOT_EXISTING_VAR"`, none fails.

# the solution 🥳!

i had a little buffer overflow / stack smash / what ever going on in
`ft_strtrim` where i wrote a `0` byte one byte after the allowed allocated
memory for the trimmed string. this somehow corrupted the `char **env_arr` array
at some point. or the `char *line` thingy in the `read_prompt` or the `char
*input` in the main function. seemingly random corruption errors hinting to this
little error. and francinette didn't find it too, back then.

So the takeway is:

    If you run into seemingly random mem corruption errors with "invalid
    pointer" or "double free" errors where they can not be, better run valgrind
    sooner than later as it will point you to error locations more accurately
    than gdb or strace.
