{ pkgs ? import <nixpkgs> {} }:
let
  fhs = pkgs.buildFHSEnv {
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
    clang
    ncurses
  ];

  shellHook = ''
      echo "Welcome to minisHell!"
  '';
}
