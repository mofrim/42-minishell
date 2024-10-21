{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  name = "minishell";
  nativeBuildInputs = with pkgs; [
    clang_12
  ];
}
