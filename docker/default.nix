{ pkgs ? import <nixpkgs> {} }:

pkgs.buildEnv {
	name = "my-tools";
	paths = with pkgs; [
		clang_12
		bear
		gnumake
		ncurses
	];
}
