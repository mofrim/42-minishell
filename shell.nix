# { pkgs ? import <nixpkgs> {} }:
let
  pkgs = import (builtins.fetchGit {
    url = "/home/frido/nix/nixpkgs-fork/";
    # ref = "master";
    ref = "nixos-24.05";
  }) {};
in

pkgs.mkShell {
  name = "minishell";
  nativeBuildInputs = with pkgs; [
    clang-tools
    clang_12
    readline
    readline.man
  ];

  shellHook = ''
    export MANPATH=$MANPATH:${pkgs.readline.man.outPath}/share/man
  '';
}
