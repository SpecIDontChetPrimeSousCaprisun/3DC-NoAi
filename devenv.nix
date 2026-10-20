{ nixpkgs, config, ... }: 
let
    system = "x86_64-linux";
    pkgs = nixpkgs.legacyPackages.${system};
    mingw = pkgs.pkgsCross.mingwW64;
in {
    languages.cplusplus.enable = true;

    packages = with pkgs; [
	gcc
	glfw
	gdb
	libX11
	libXrandr
	libXi
	libXxf86vm
	libXcursor
	assimp
	mingw.buildPackages.gcc
	mingw.glfw
    ];

    enterShell = ''
	export MINGW_GLFW=${mingw.glfw}
	export LD_LIBRARY_PATH=${pkgs.libpulseaudio}/lib:${pkgs.alsa-lib}/lib:$LD_LIBRARY_PATH
    '';
}
