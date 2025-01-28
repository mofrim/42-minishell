# { pkgs ? import <nixpkgs> {} }:
let
  pkgs = import (builtins.fetchGit {
    url = "/home/frido/nix/nixpkgs-fork/";
    # ref = "master";
    ref = "nixos-24.11";
  }) {};
  fhs = pkgs.buildFHSUserEnv {
    # pkgs = pkgs;
    name = "fhs-env";
    targetPkgs = pkgs : (with pkgs;
    [
      coreutils
    ]);
    runScript = "bash";
  };
  in
pkgs.mkShell {
  NIX_HARDENING_ENABLE = "";
  name = "minishell";
  nativeBuildInputs = with pkgs; [
    fhs
    clang-tools
    clang_12
    ncurses
  ];

  shellHook = ''
  exec fhs
  '';
}
