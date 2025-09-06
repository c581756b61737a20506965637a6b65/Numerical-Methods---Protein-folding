{
  description = "Projekt: modelowanie zwijania białek (HP model + Metropolis)";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable"; # lub stable, jeśli wolisz
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs {
          inherit system;
        };

        # Lista narzędzi potrzebnych do pracy nad projektem
        buildInputs = with pkgs; [
          gcc
          cmake
          gdb
          python3
          python3Packages.numpy
          python3Packages.matplotlib
          imagemagick
          ffmpeg
        ];

      in
      {
        devShells.default = pkgs.mkShell {
          packages = buildInputs;
          shellHook = ''
            echo "Środowisko programistyczne do symulacji zwijania białek gotowe!"
            echo "Dostępne: gcc, cmake, gdb"
          '';
        };
      }
    );
}
