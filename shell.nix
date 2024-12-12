{ pkgs ? import <nixpkgs> {} }:
# let
#   pkgs = import (builtins.fetchGit {
#     url = "/home/frido/nix/nixpkgs-fork/";
#     ref = "master";
#     # ref = "nixos-24.05";
#   }) {};
# in
pkgs.mkShell {
  name = "philo";
  buildInputs = with pkgs; [
    clang-tools
    clang_12
  ];
  shellHook = ''
    echo "hi there!"
  '';
}
