{

  description = "Flake of thattem-emacs-library devShell.";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils/main";
  };

  outputs =
    { nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (
      system:

      let

        pkgs = import nixpkgs { inherit system; };

        build-tools = with pkgs; [
          cmake
          pkg-config
        ];

        libraries = with pkgs; [
          libgtop
          emacs
        ];

      in

      {
        devShells.default = pkgs.mkShell {
          name = "thattem-emacs-library-shell";

          buildInputs = build-tools ++ libraries;

        };
      }
    );

}
