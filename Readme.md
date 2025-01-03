# minishell

# Allowed functions

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

# Roadmap

1) **DONE** Finish Tokenization / Lexing
2) Implement Environment variables
3) Implement HEREDOC functionality
4) Implement Builtins dispatching
4) Implement Parsing of Commands into Command-struct (respecting builtins,
   heredoc and envvars)
5) Implement access-/executability checking
6) Implement execution & error handling **and** return value management in order
   to implement `$?`.

# The Found-An-Error/FIXME-list

- open pipes and redir handling:

      bash-5.2$ asdf asdkjf asdlkj | adsf sd d d d d d|    
      > ^C

      bash-5.2$ asdf asdkjf asdlkj | adsf sd d d d d d >   
      bash: syntax error near unexpected token `newline'
      bash-5.2$ asdf asdkjf asdlkj | adsf sd d d d d d <   
      bash: syntax error near unexpected token `newline'

  so, after a pipe there should be a prompt waiting for more cmdline input, but
  a redir without words afterwards is an error.

- reproduce this ?!

      =8-) cat
      << DEBUG >> toklst after lvl1:
      token: TOK_WORD, value: `cat`
      << DEBUG >> toklst after lvl2:
      token: TOK_CMD, value: `cat`
      << DEBUG >> maxargs = 0
      << DEBUG >> toklst in parse_tokenlist:
      token: TOK_CMD, value: `cat`
      << DEBUG >> cmdlst:
      -- cmd start --
      Command: cat
      Arguments: 
      -- cmd end --
      ^C
      =8-) =8-) 
      =8-) 
      =8-) 
      =8-) 
      =8-) cat
      << DEBUG >> toklst after lvl1:
      token: TOK_WORD, value: `cat`
      << DEBUG >> toklst after lvl2:
      token: TOK_CMD, value: `cat`
      << DEBUG >> maxargs = 0
      << DEBUG >> toklst in parse_tokenlist:
      token: TOK_CMD, value: `cat`
      << DEBUG >> cmdlst:
      -- cmd start --
      Command: cat
      Arguments: 
      -- cmd end --
      ^C
      =8-) =8-) 
      =8-) cat
      << DEBUG >> toklst after lvl1:
      token: TOK_WORD, value: `cat`
      << DEBUG >> toklst after lvl2:
      token: TOK_CMD, value: `cat`
      << DEBUG >> maxargs = 0
      << DEBUG >> toklst in parse_tokenlist:
      token: TOK_CMD, value: `cat`
      << DEBUG >> cmdlst:
      -- cmd start --
      Command: cat
      Arguments: 
      -- cmd end --
      ^C
      =8-) =8-) 
      =8-) asdf asdkjf asdlkj | adsf sd d d d d d| asd
      << DEBUG >> toklst after lvl1:
      token: TOK_WORD, value: `asdf`
      ...
      token: TOK_PIP, value: `|`
      token: TOK_WORD, value: `asd`
      << DEBUG >> toklst after lvl2:
      token: TOK_CMD, value: `asdf`
      ...
      token: TOK_CMD, value: `asd`
      << DEBUG >> maxargs = 6
      << DEBUG >> toklst in parse_tokenlist:
      token: TOK_CMD, value: `asdf`
      ...
      token: TOK_CMD, value: `asd`
      << DEBUG >> cmdlst:
      -- cmd start --
      Command: asdf
      Arguments: asdkjf asdlkj 
      Piped to: adsf
      -- cmd end --
      -- cmd start --
      Command: adsf
      Arguments: sd d d d d d 
      Piped to: asd
      -- cmd end --
      -- cmd start --
      Command: asd
      Arguments: 
      -- cmd end --
      corrupted size vs. prev_size
      zsh: IOT instruction (core dumped)  ./minishell
      k-compiler-rt-libc-12.0.1/bin:/nix/store/k48bha2fjqz

# Mandatory TODOs

- [x] Display a prompt when waiting for a new command.
  - [ ] make it nicer
- [x] Have a working history.
  - [ ] does it really work correctly in all cases? especially the heredoc case?
  - [ ] Search and launch the right executable (based on the PATH variable or
      using a relative or an absolute path).
- [x] Not interpret unclosed quotes or special characters which are not required
  by the subject such as \ (backslash) or ; (semicolon).
- [x] Handle ’ (single quote) which should prevent the shell from interpreting
  the meta- characters in the quoted sequence.
- [x] Handle " (double quote) which should prevent the shell from interpreting
  the meta- characters in the quoted sequence except for $ (dollar sign).
- [x] Implement redirections:
  - [x] < should redirect input.
  - [x] > should redirect output.
  - [x] 2> should redirect stderr output.
  - [ ] << should be given a delimiter, then read the input until a line
    containing the delimiter is seen. However, it doesn’t have to update the
    history!
  - [x] >> should redirect output in append mode.
  - [x] 2>> should redirect stdout output in append mode.
- [x] Implement pipes (| character). The output of each command in the pipeline
  is connected to the input of the next command via a pipe.
- [x] Handle environment variables ($ followed by a sequence of characters)
  which should expand to their values.
- [x] Avoid using more than one global variable to indicate a received signal.
  Consider the implications: this approach ensures that your signal handler will
  not access your main data structures
- [x] Handle $? which should expand to the exit status of the most recently
  executed foreground pipeline.
- [x] Handle ctrl-C, ctrl-D and ctrl-\ which should behave like in bash.
  + [ ] idk know. ctrl-d behaves different for bash's built with lib-readline.
    sometimes ctrl-d on an non-empty prompt is completly ignored sometimes it
    isn't. clarify
- [x] In interactive mode:
  - [x] ctrl-C displays a new prompt on a new line.
  - [x] ctrl-D exits the shell.
    + [ ] clarify different behavior from readline man-page.
  - [x] ctrl-\ does nothing.
- [x] Your shell must implement the following builtins:
  - [ ] echo with option -n
  - [ ] cd with only a relative or absolute path
  - [ ] pwd with no options
  - [x] export with no options
  - [x] unset with no options
  - [x] env with no options or arguments
  - [ ] exit with no options
- [x] The readline() function can cause memory leaks. You don’t have to fix
  them. But that doesn’t mean your own code, yes the code you wrote, can have
  memory leaks.

## Parsing / Lexing TODOs

- [x] find a way to make everything obey the norm.
- Tokens to correctly tokenize:
  + [x] command
  + [x] arg
  + [x] pipe
  + [x] "<" redirect in
  + [x] infile
  + [x] ">" redirect out
  + [x] outfile
  + [x] ">>" redirect out append -> outfile stays the same
  + [x] "<<" heredoc -> what? how?
  + [x] heredoc-delimiter
  + [x] "'" single quotes
  + [x] -> single quoted text
  + [x] """ double quotes
  + [x] -> single quoted text (but $variables should be handled)
  + [x] $ variable start
  + [x] -> variable name
  + [x] builtins
  + [x] builtin args for echo, export ("=" + arg), unset, export, cd

# Resources

- [https://42-cursus.gitbook.io/guide/rank-03/minishell/functions](https://42-cursus.gitbook.io/guide/rank-03/minishell/functions)
- [https://en.wikipedia.org/wiki/Lexical_analysis](https://en.wikipedia.org/wiki/Lexical_analysis)
- [http://web.mit.edu/gnu/doc/html/rlman_2.html](http://web.mit.edu/gnu/doc/html/rlman_2.html)

# Syntax Notes

Bash syntax errors:

- `cat <` or `cat >` or `cat > |` and the like
- `cat << "<<"` is legit!

# Logbook

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

- **[2024-11-23 11:24]** I wrote this as a comment to my `get_next_token()`
  yesterday:

  ```c
  /* Get next token from input...
  *
  * This function should return (and increment cl->position accordingly):
  * - the VALUE of the token it found (f.ex. '<', for a word the whole word ->
  *   read until there is some new token)
  * - the position of the token in cmdline
  * 
  * This means the whole classification thing happens in the tokenizing function.
  * Maybe i need to add some flags to the cmdline struct:
  * - for squot / dquot
  * - for var name
  * - for if / of
  * - for heredoc
  * - for pipe
  * - forgot somethin ?!?!
  *
  * then i can really read token-symbol by token-symbol, word by word from the
  * input string using get_next_token(), if it ecounters f.ex. '<' it sets the
  * if_flag inside cmdline-struct, returns to tokenize(), which handles the
  * addition to toklst (and maybe stuff like finding out wether we have a
  * builtin * or not?!?!)
  *
  * Example tokenization-flow: find '<' -> return just "<" -> in tokenize() there
  * is handle_redirect_token(): checks if quot-flags are set, checks if already
  * inside a redirection (actually waiting for the file name), if yes, throw
  * error.... AAAHHHH, maybe this is too much for a tokenizer. This would be the
  * job of my parser!
  *
  * Maybe words should just be TOK_WORD and only during parsing the decision is
  * made that they should be TOK_IF, TOK_CMD, TOK_ARG, ...
  *
  */
  ```
  Today my view on this is as follows. I will have several tokenization and
  parsing steps:

  1) Tokenize very low-level with only as little as possible grammar. I think
     that "as-little" is interpreting quots because in this case all text
     between the quots **will need to be tokenized as-is**, that is with all
     whitespaces and stuff.
  2) My hypothesis: for everything else 1st level of tokenization / parsing can
     happen by simply assigning f.ex. `TOK_RIN` to '<' and `TOK_WORD` to stuff
     like "cat" and "-w".
  3) 2nd level: Iterate over tokenlist. For something like TOK_RIN followed by
     TOK_WORD set token->type of TOK_WORD token to TOK_IF. One very special case
     is the heredoc. If i find this and next toke is TOK_WORD, make it
     TOK_HERE_DLIM (*Error handling ?!?!?!* perhaps in this very step).
  4) 3rd Level: the real parsing starts. Until here couple of errors have been
     ruled out. Time to fill the Command-struct.
  5) maybe 4th Level: Check filenames, permissions etc.
  6) Execution: forks(), run builtins, run heredoc (*TODO: how to implement
     this?!*).
    ```

- **[2024-12-05 10:20]** How to streamline the execution process with bltins?
  and possible multi-pipe cmds? How to handle the output redirection / piping of
  a bltin function. Definitely the env-var substitution would have to happen
  beforehand.
  
  Notes on $env-vars:

  - something like `prompt> export LS=ls` and then `prompt> $LS` should execute
    the `ls` cmd.
  - inside quotes text should be substituted
  - BUT if env-vars should also be possibly expanded to cmds, this means the
    expansion *must* happen before cmd parsing!!! F.ex. stuff like 
    `echo "dir" | $LS` should also be possible!

- **[2024-12-06 13:13]** Notes on HEREDOC:
  
  * envvar expansion must happen in HEREDOCs.
  * but envvars should not be intrepreted further!
  * uffz! but there are also multiple HEREDOCs enchained by pipes possible:

        bash-5.2$ cat << eof | cat << miau 
        > asdjf
        > kasdjf
        > eof
        > at
        > miau
        at
        bash-5.2$ 

    it is even worse. this several heredocs problem even exists with one cmd:

        bash-5.2$ cat << eof << moep
        > asdfsdf
        > eof
        > moep
        bash-5.2$ 

    in both cases only the last content is kept. but the heredoc-input is not
    ended before both delimiters have been entered. 
    <center> **HOW to deal with that???** </center>
  * another tricky thing:

        bash-5.2$ cat << eof < moep
        > asdl
        > lsakd
        > eof
        bash: moep: No such file or directory
        bash-5.2$ cat < Makefile << eof
        > alskdf
        > akd
        > eof
        alskdf
        akd
        bash-5.2$

      soooo... the general rule seems to be: the last redirect / heredoc wins.
      So we somehow need a mechanism on deciding which redirect we want to take.
      Furthermore, the heredoc really goes to **STDIN**!!! It is not an
      argument! So, wow.
  * aaaaaaand another one:

        bash-5.2$ cat << eof > bla | cat << miau > blub
        > skdjfasdj
        > akdsjf
        > miau
        > kadsf
        > eof
        > adskfm
        > akjsd
        > miau
        bash-5.2$ cat bla
        skdjfasdj
        akdsjf
        miau
        kadsf
        bash-5.2$ cat blub
        adskfm
        akjsd
        bash-5.2$

      so. yes. all the heredocs need to be associated to their commands, of
      course!! This means, we need to know the cmd structure so that we can
      assign the heredocs to the cmd they belong to.

      the ultimate benchmark:

        bash-5.2$ cat << eof | cat 1>&2 | cat <<miau | cat 1>&2 | cat << moep
        > at
        > eof
        > atat
        > miau
        > atatat
        > moep
        atat
        atatat
        at
        bash-5.2$

      **ARGGGGL!** even this is possible:

        bash-5.2$ cat << eof << miau
        > miau
        > eof
        > miau
        bash-5.2$ cat << eof << miau
        > bla eof
        > eof
        > blau
        > miau
        blau
        bash-5.2$

      -> RULE: even multiple heredocs per cmd are possible AND they are both
      needed to terminate the heredoc-reading BUT only the last reading is
      kept!! this means i really need an array of heredoc-delim strings per cmd
      but only one string for the actual content.


- **[2024-12-07 16:47]** Got the idea for evaluating the heredocs:
  + the heredoc gathering is done for all heredocs at once
  + there can be multiple heredocs per cmd, they also need to be entered at the
    heredoc prompts in FIFO, BUT only the last entry is passed on.
    **SIDENOTE: there can also be multiple redirects, but only the last one is
    used. NOOOOO! See next note... **
  + the execution order seems to be seems to be semi LIFO

- **[2024-12-07 17:19]**
  with a cdmline like this `cat shell.nix > bla > blub` the file `bla` is
  created but empty and the file `blub` has the content of `shell.nix`!
  How should i implement this!? At least this also means, `cmd.output_file` has
  to be a `char **`. And inside `exec_cmd` we need to `open(infile[i], O_CREAT
  ...)` in a loop.

- **[2024-12-07 17:25]**
  What i will implement next step by step:

  - [x] make INT_MAX cmd_args possible, in principle. Therefore count number of
    args in each pipe-seperated tokenlst and take the max of it.
  - [x] make multiple input redirects possible. This only requires being aware
    of that `cmdlst.input_file` might be overridden and therefore should be
    checked `if NULL` otherwise freed before. So this is easy!
  - [ ] rewrite `exec_cmd` in a final way: to possible excution paths - 1) with
    pipes, 2) pipeless
  - [x] make multiple output redirects possible as described above.   
  - [ ] heredoc
  - [ ] continue working on the exec problem
  - [x] how to connect the stdout from builtins to the stdin? Use `dup()`!
  - [ ] Implement `cmd 2>1` or `cmd 2>file` stuff. Certainly needs new tokens.
    Possibilities for implementing advanced redir stuff:
    - `[n]<word`: expansion of word will be opened for reading from filedes `n`.
      if `n` not specified -> n=1.
    - `[n]>word`: same but for writing
    - `[n]>>word`: same for appending
    - `&>word`: stdin and stderr will go to word. == `>word 2>&1`
    - `&>>word`: same same for appending

- **[2024-12-11 06:01]** Notes for advanced redirs:
  - `echo "abcd" 2>1` really only redirects stderr to a file called `1`.
  - `echo "abcd" 2>&1` outputs to stdout AND redirs stderr to stdout.
  - `ls dirlist > myfile 2>&1` redirects both stderr and stdout to `myfile`,
    bt `ls dirlist 2>&1 > myfile` redirects only stdout to `myfile` => the order
    of redirs matters -> Implement redirs as a... **linked list** which can be
    processed later.
  - input redir only supports the format `[n]<word`.
  - `>& echo "assddd"` will create a file `echo` with "assddd: command not
    found" in it.

- **[2024-12-18 21:26]** from my own bash cheatsheet:

      cmd1|cmd2  # pipe; takes standard output of cmd1 as standard input to cmd2
      > file     # directs standard output to file
      < file     # takes standard input from file
      >> file    # directs standard output to file; append to file if it already exists
      >|file     # forces standard output to file even if noclobber is set
      n>|file    # forces output to file from file descriptor n even if noclobber is set
      <> file    # uses file as both standard input and standard output
      n<>file    # uses file as both input and output for file descriptor n
      <<label    # here-document
      n>file     # directs file descriptor n to file
      n<file     # takes file descriptor n from file
      n>>file    # directs file description n to file; append to file if it already exists
      n>&        # duplicates standard output to file descriptor n
      n<&        # duplicates standard input from file descriptor n
      n>&m       # file descriptor n is made to be a copy of the output file descriptor
      n<&m       # file descriptor n is made to be a copy of the input file descriptor
      &>file     # directs standard output and standard error to file
      <&-        # closes the standard input
      >&-        # closes the standard output
      n>&-       # closes the ouput from file descriptor n
      n<&-       # closes the input from file descripor n

- **[2024-12-26 20:14]** time for some todos again:

  + implement `[n]<file` as opening `file` for reading as fildes `n`.
    TOK_RIN1. `cat 42<` is TOKERR_NL!!
  + maybe implement `<> file` for opening stdin and stdout to `file`
  + that should be enough redir stuff!
  + implement pipes
  + implement the heredoc

- **[2024-12-29 10:37]** Some notes on signal processing
  claude told me: 

      *the only signal-related settings that are inherited across execve() are:*

      + the signal mask
      + the set of pending signals
      + signal dispositions that were set to SIG_IGN

  => this is funny! bc atm i set SIGINT to SIG_IGN before fork the child and if
  i do not reset SIGINT to something else SIGINT will still be ignored inside
  any execve-ed program. **BUT** if i set SIGINT to call my normal
  signal_handler this will also be replaced by the default handler in the
  subprocess 🤯

  + `wait(NULL)` returns -1 and sets `errno = ECHILD` if there is no more child
    to wait for... so the correct way to wait for childs to finish would be
    ```c
    while (!(wait(NULL) == -1 && errno == ECHILD))
        if (errno && errno != ECHILD)
          printf("something bad has happened.");
    ```

  Why does hitting `Ctrl-C` with a cmdline like `cat < shell.nix | sleep 2 |
  echo "bla"` work in our minishell? I mean, form how i understand our current
  signal handling the `echo` should still be executed ?!?!
  **-> clarify that!**

  + a **TODO**...  adjust errormsging like this:
    std bash:

        bash-5.2$ cat | ls /root/sfdasd > moep | cat moep
        ls: cannot access '/root/sfdasd': Permission denied
        ^C
        bash-5.2$ 

    our minish:

        /nix/store/k48bha2fjqzarg52picsdfwlqx75aqbb-coreutils-9.5/bin/ls: cannot access '/root/sfdasd': Permission denied
        ^C
        =8-) 

    => needs a `truncate-to-last-slash` function!

    + from the `bash` man:

          3.7.5 Exit Status

          The exit status of an executed command is the value returned by the
          waitpid system call or equivalent function. Exit statuses fall between
          0 and 255, though, as explained below, the shell may use values above
          125 specially. Exit statuses from shell builtins and compound commands
          are also limited to this range. Under certain circumstances, the shell
          will use special values to indicate specific failure modes.

          For the shell’s purposes, a command which exits with a zero exit
          status has succeeded. A non-zero exit status indicates failure. This
          seemingly counter-intuitive scheme is used so there is one
          well-defined way to indicate success and a variety of ways to indicate
          various failure modes. When a command terminates on a fatal signal
          whose number is N, Bash uses the value 128+N as the exit status.

          If a command is not found, the child process created to execute it
          returns a status of 127. If a command is found but is not executable,
          the return status is 126.

          If a command fails because of an error during expansion or
          redirection, the exit status is greater than zero.

          The exit status is used by the Bash conditional commands (see
          Conditional Constructs) and some of the list constructs (see Lists of
          Commands).

          All of the Bash builtins return an exit status of zero if they succeed
          and a non-zero status on failure, so they may be used by the
          conditional and list constructs. All builtins return an exit status of
          2 to indicate incorrect usage, generally invalid options or missing
          arguments.

          The exit status of the last command is available in the special
          parameter $? (see Special Parameters). 

- **[2025-01-03 01:06]** Chat with claude about the pipe problem:
  i have written the following C code in order to implement execution of chains
  of commands piping the standard output of the previous command to the stdin of
  the next command like in f.ex. bash's pipeline:

  ```c
  int    exec_pipeline(t_cmdlst cl, char *env, t_envlst **el)
  {
      int prev_read;
      int    status;
      prev_read = dup(0);
      status = 0;
      signal(SIGINT, SIG_IGN);
      while (cl->next)
      {
          if (cl->is_builtin)
              exec_pipe_bltin(cl, el, &prev_read);
          else
              exec_pipe_cmd(cl, env, &prev_read);
          cl = cl->next;
      }
      if (cl->is_builtin)
          exec_pipe_bltin_last(cl, el, &prev_read);
      else
          exec_pipe_cmd_last(cl, env, prev_read);
      while (!(wait(&status) == -1 && errno == ECHILD))
          ft_printf(RED "<< DEBUG >> status = %d\n" RST, status);
      signal(SIGINT, sigint_handler);
      ft_printf(RED "<< DEBUG >> final status = %d\n" RST, status);
      return (status);
  }

  static void    run_child(t_cmdlst cl, int pipefd[2], int prev_read, char *env)
  {
      close(pipefd[0]);
      dup2(pipefd[1], STDOUT_FILENO);
      close(pipefd[1]);
      dup2(prev_read, STDIN_FILENO);
      close(prev_read);
      if (open_redir_files(cl->redirs) != 0)
          exit(errno);
      signal(SIGINT, sigint_handler);
      execve(cl->args[0], cl->args, env);
  }

  int    exec_pipe_cmd(t_cmdlst cl, char *env, int *prev_read)
  {
      int        pipefd[2];
      pid_t    cpid;
      int        err_exec_path;
      if (pipe(pipefd) == -1)
          return (minish_errormsg("exec_pipe_cmd", "create pipe failed", errno));
      err_exec_path = set_exec_path(cl, env);
      if (err_exec_path)
          return (err_exec_path);
      cpid = fork();
      if (cpid < 0)
          return (minish_errormsg("exec_pipe_cmd", "fork failed", errno));
      if (cpid == 0)
          run_child(cl, pipefd, *prev_read, env);
      else
      {
          close (pipefd[1]);
          close (*prev_read);
          *prev_read = pipefd[0];
          ft_printf(RED "<< DEBUG >> prev_read = %d\n" RST, prev_read);
      }
      return (0);
  }

  int    exec_pipe_cmd_last(t_cmdlst cl, char *env, int prev_read)
  {
      pid_t    cpid;
      int        err_exec_path;
      err_exec_path = set_exec_path(cl, env);
      if (err_exec_path)
          return (err_exec_path);
      cpid = fork();
      if (cpid < 0)
          return (minish_errormsg("exec_pipe_cmd_last", "fork failed!", errno))
      if(cpid == 0)
      {
          dup2(prev_read, STDIN_FILENO);
          close(prev_read);
          if (open_redir_files(cl->redirs))
              exit(errno);
          signal(SIGINT, sigint_handler);
          execve(cl->args[0], cl->args, env);
      }
      else
          close (prev_read);
      return (0);
  }
  ```

  if i call this with a long pipline like
  ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls|ls some calls to ls exit with exit code
  13 BROKEN_PIPE. Why is that?

  Claude's Answer:

  This is happening because of a race condition in how the processes are being
  created and how their pipes are being handled. Let me explain:

  1. When you have a pipeline like `ls|ls|ls|ls...`, each `ls` command tries to
     write its output to a pipe that's being read by the next command in the
     chain.

  2. The issue occurs because there's no guarantee about the order in which
     these processes will actually execute. Some later `ls` commands might
     finish and exit before earlier ones in the pipeline.

  3. When a process writing to a pipe exits while the reading end is still open,
     everything works fine. However, when a process tries to write to a pipe
     whose reading end has been closed (because the next process already
     finished), it receives a SIGPIPE signal, which by default terminates the
     process with exit code 13 (EPIPE - Broken pipe).

  To fix this, you have two main options:

  1. Either ignore SIGPIPE in the child processes before executing the command:

  ```c 
  static void run_child(t_cmdlst cl, int pipefd[2], int prev_read, char
      *env) 
  { 
    signal(SIGPIPE, SIG_IGN);  // Add this line 
    close(pipefd[0]);
    dup2(pipefd[1], STDOUT_FILENO); 
    // ... rest of the function 
  } 
  ```

  2. Or wait for each process in the pipeline in order, which ensures that
     processes don't exit before their output can be consumed:

  ```c 
  int exec_pipeline(t_cmdlst cl, char *env, t_envlst **el) 
  { 
    int prev_read;
    int status; 
    pid_t *pids;  // Array to store process IDs int cmd_count = 0;
    t_cmdlst current = cl;

    // Count commands 
    while (current) 
    { 
      cmd_count++; 
      current = current->next;
    }

    pids = malloc(sizeof(pid_t) * cmd_count); 
    int pid_index = 0;
    prev_read = dup(0); 
    status = 0; 
    signal(SIGINT, SIG_IGN);

    // Execute pipeline storing PIDs 
    while (cl->next) 
    { 
      if (cl->is_builtin)
        exec_pipe_bltin(cl, el, &prev_read); 
      else 
      { 
        pids[pid_index++] = exec_pipe_cmd(cl, env, &prev_read); 
      } 
      cl = cl->next; 
    }
    if (cl->is_builtin) 
      exec_pipe_bltin_last(cl, el, &prev_read); 
    else
      pids[pid_index++] = exec_pipe_cmd_last(cl, env, prev_read);
    // Wait for processes in order 
    for (int i = 0; i < pid_index; i++)
      waitpid(pids[i], &status, 0);
    // Clean up remaining processes if any
    while (!(wait(&status) == -1 && errno == ECHILD));
    free(pids);
    signal(SIGINT, sigint_handler);
      return (status);
    }
    free(pids); 
    signal(SIGINT, sigint_handler); 
    return (status); 
  }
  ```

  This second approach requires modifying your `exec_pipe_cmd` and
  `exec_pipe_cmd_last` functions to return the child process ID instead of 0.

  The first approach (ignoring SIGPIPE) is simpler but might mask other
  legitimate pipe errors. The second approach (ordered waiting) is more robust
  but requires more code changes. Both will solve the EPIPE errors you're
  seeing.

  And me again:

  Yeah... i have tried with removing the `close(pipefd[0]);` inside the
  run_child function. This also made the BROKE_PIPE errors go away. What do you
  think about that solution?`

  That solution would technically work but it's not ideal from a resource
  management perspective. Here's why:

  By keeping the read end of the pipe open in the child process, you're
  preventing SIGPIPE because the pipe technically still has an open reader.
  However, this means you're leaving unused file descriptors open in each
  process. While this might not be a huge issue for a small pipeline, it's not a
  clean solution because:

      Each process will have all the read ends of previous pipes open
      unnecessarily These file descriptors remain open until the process exits
      It could potentially hit file descriptor limits in very long pipelines It
      masks the underlying synchronization issue rather than addressing it

  The SIGPIPE signal exists specifically to handle the case where a reader exits
  before a writer is done - it's part of the pipe mechanism's design. Either
  handling SIGPIPE explicitly or ensuring proper process synchronization (as in
  the ordered wait solution) would be more "correct" approaches.

  But I understand the appeal of your solution - it's simple and it works. If
  you're building this for learning purposes and don't need production-level
  robustness, it's a creative way to solve the immediate problem. Just be aware
  of the trade-offs.

