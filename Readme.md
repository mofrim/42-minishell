# minishell

# Allowed functions

    the new:

    readline, rl_clear_history, rl_on_new_line,
    rl_replace_line, rl_redisplay, add_history,
    printf, malloc, free, write, access, open, read,
    close, fork, wait, waitpid, wait3, wait4, signal,
    sigaction, sigemptyset, sigaddset, kill, exit,
    getcwd, chdir, stat, lstat, fstat, unlink, execve,
    dup, dup2, pipe, opendir, readdir, closedir,
    strerror, perror, isatty, ttyname, ttyslot, ioctl,
    getenv, tcsetattr, tcgetattr, tgetent, tgetflag,
    tgetnum, tgetstr, tgoto, tputs

    the old:

    readline, rl_clear_history, rl_on_new_line,
    rl_replace_line, rl_redisplay, add_history,
    printf, malloc, free, write, access, open, read,
    close, fork, wait, waitpid, wait3, wait4, signal,
    sigaction, sigemptyset, sigaddset, kill, exit,
    getcwd, chdir, stat, lstat, fstat, unlink, execve,
    dup, dup2, pipe, opendir, readdir, closedir,
    strerror, perror, isatty, ttyname, ttyslot, ioctl,
    getenv, tcsetattr, tcgetattr, tgetent, tgetflag,

# Mandatory TODOs

- [x] Display a prompt when waiting for a new command.
  - [ ] make it nicer
- [x] Have a working history.
  - [ ] does it really work correctly in all cases? especially the heredoc case?
  - [ ] Search and launch the right executable (based on the PATH variable or
      using a relative or an absolute path).
- [ ] Not interpret unclosed quotes or special characters which are not required
  by the subject such as \ (backslash) or ; (semicolon).
- [ ] Handle ’ (single quote) which should prevent the shell from interpreting
  the meta- characters in the quoted sequence.
- [ ] Handle " (double quote) which should prevent the shell from interpreting
  the meta- characters in the quoted sequence except for $ (dollar sign).
- [ ] Implement redirections:
  - [ ] < should redirect input.
  - [ ] > should redirect output.
  - [ ] << should be given a delimiter, then read the input until a line
    containing the delimiter is seen. However, it doesn’t have to update the
    history!
  - [ ] >> should redirect output in append mode.
- [ ] Implement pipes (| character). The output of each command in the pipeline
  is connected to the input of the next command via a pipe.
- [ ] Handle environment variables ($ followed by a sequence of characters)
  which should expand to their values.
- [ ] Avoid using more than one global variable to indicate a received signal.
  Consider the implications: this approach ensures that your signal handler will
  not access your main data structures
- [ ] Handle $? which should expand to the exit status of the most recently
  executed foreground pipeline.
- [x] Handle ctrl-C, ctrl-D and ctrl-\ which should behave like in bash.
  + [ ] idk know. ctrl-d behaves different for bash's built with lib-readline.
    sometimes ctrl-d on an non-empty prompt is completly ignored sometimes it
    isn't. clarify
- [ ] In interactive mode:
  - [x] ctrl-C displays a new prompt on a new line.
  - [x] ctrl-D exits the shell.
    + [ ] clarify different behavior from readline man-page.
  - [x] ctrl-\ does nothing.
- [ ] Your shell must implement the following builtins:
  - [ ] echo with option -n
  - [ ] cd with only a relative or absolute path
  - [ ] pwd with no options
  - [ ] export with no options
  - [ ] unset with no options
  - [ ] env with no options or arguments
  - [ ] exit with no options
- [ ] The readline() function can cause memory leaks. You don’t have to fix
  them. But that doesn’t mean your own code, yes the code you wrote, can have
  memory leaks.

## Parsing / Lexing TODOs

- [ ] find a way to make everything obey the norm.
- Tokens to correctly tokenize:
  + [ ] command
  + [ ] arg
  + [ ] pipe
  + [ ] "<" redirect in
  + [ ] infile
  + [ ] ">" redirect out
  + [ ] outfile
  + [ ] ">>" redirect out append -> outfile stays the same
  + [ ] "<<" heredoc -> what? how?
  + [ ] heredoc-delimiter
  + [ ] "'" single quotes
  + [ ] -> single quoted text
  + [ ] """ double quotes
  + [ ] -> single quoted text (but $variables should be handled)
  + [ ] $ variable start
  + [ ] -> variable name
  + [ ] builtins
  + [ ] builtin args for echo, export ("=" + arg), unset, export, cd
  + [ ] ...

  -> -> -> **i need much more tokens!!!**

# Resources

- [https://42-cursus.gitbook.io/guide/rank-03/minishell/functions](https://42-cursus.gitbook.io/guide/rank-03/minishell/functions)
- [https://en.wikipedia.org/wiki/Lexical_analysis](https://en.wikipedia.org/wiki/Lexical_analysis)
- [http://web.mit.edu/gnu/doc/html/rlman_2.html](http://web.mit.edu/gnu/doc/html/rlman_2.html)

# Syntax Notes

Bash syntax errors:

- `cat <` or `cat >` or `cat > |` and the like

# Logbook

- **[2024-11-14 21:59]** Now i really want to start working on this. A first
  checklist:
  + [x] display the prompt
  + [ ] implement signal handling, 
    + [ ] handle Ctrl-D like bash
    + [x] that is handle Ctrl-C in a correct way

- **[2024-11-19 01:39]** Well.. the main output of todays.. "coding": there are
  version of bash (on nix bash_interactive) which are build with and linked
  against readline. With those hitting ctrl-d on a non-empty input line does
  nothing. For those bash versions which are not linked against readline ctrl-d
  behaves different. More like explained in the readline-manpage. needs some
  more: 🤔 ... but not too much. next: parsing!

- **[2024-11-20 12:00]** Some observations relevant for syntax checking /
  parsing:
  + `echo "blabla" | < bindvoutside.log wc -w` is the same as `echo "blabla" |
    wc -w < bindvoutside.log`. so '|' followed by '<' is allowed. The same holds
    for '>'
  + Something like `echo "blabal" | | cat` is not allowed -> *bash: syntax error
    near unexpected token `|'*
  
  So. That said how to parse a prompt like: `< infile cat | wc -w > out`?
  **RULE**: every word after a '<' is considered to be a input_file to cmd.

  + another special case: parsing something like `< a cat | < b wc -w > c`.
    During parsing, after the pipe symbol we would have to allocate a new
    cmdlst-member an then read in redirection first.

- **[2024-11-21 00:12]** Alright parsing of commands, <, >, and | seems to
  working so far. what about >> and << ??? at least << is a very special thing
  as it changes the whole program execution flow.
  Major TODO: somehow make the whole parsing process more elegant and shorter.
