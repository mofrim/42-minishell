# { pkgs ? import <nixpkgs> {} }:
let
  pkgs = import (builtins.fetchGit {
    url = "/home/frido/nix/nixpkgs-fork/";
    # ref = "master";
    ref = "nixos-24.11";
  }) {};
in

pkgs.mkShell {
  NIX_HARDENING_ENABLE = "";
  name = "minishell";
  nativeBuildInputs = with pkgs; [
    clang-tools
    clang_12
    ncurses
    # readline
    # readline.man
  ];

  shellHook = ''
    export MANPATH=$MANPATH:${pkgs.readline.man.outPath}/share/man
  '';
}
