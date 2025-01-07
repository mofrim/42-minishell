{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  name = "shell-dummy";
  buildInputs = [ (import ./default.nix { inherit pkgs; }) ];
}
