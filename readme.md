# 42's minishell

This is a implementation of 42's minishell proj. This minishell supports basic
piping, quoting, redirects, heredoc, envvars and **no backslash** (!). It is also
possible to use in some kind of scripting-mode by piping stuff to it on the
cmdline.

The included submodules are my libft, the readline lib, and my modified version
of the 42_minishell_tester by [zstenger93's](https://github.com/zstenger93).

## Setup

- `git clone https://github.com/mofrim/42-minishell.git`
- `cd 42-minishell && make`
- `./minishell` -> `=8-)`

The submodules will be cloned and initialized automatically.
