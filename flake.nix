{
  description = "A clean C development environment";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-25.11";
  };

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};
    in {
      devShells.${system}.default = pkgs.mkShell {
        nativeBuildInputs = with pkgs; [
          gcc
          gnumake
          pkg-config
          xclip
        ];

        buildInputs = with pkgs; [
          raylib
          xorg.libX11
          mesa
        ];

        shellHook = ''
          echo "C environment loaded!"
        '';
      };
    };
}
