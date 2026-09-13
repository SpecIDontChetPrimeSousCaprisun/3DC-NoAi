{
    description = "3d gem dev";
    inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    outputs = { self, nixpkgs }:
	let
	    system = "x86_64-linux";
	    pkgs = nixpkgs.legacyPackages.${system};
	    mingw = pkgs.pkgsCross.mingwW64;
	in {
	    devShells.${system}.default = pkgs.mkShell {
		buildInputs = with pkgs; [
		    gcc
		    glfw
		    gdb
		    libX11
		    libXrandr
		    libXi
		    libXxf86vm
		    libXcursor
		];

		shellHook = ''
		'';
	    };
	};
}
