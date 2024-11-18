{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  name = "minishell";
  nativeBuildInputs = with pkgs; [
    clang-tools
    clang_12
    readline
    readline.man
  ];
}
