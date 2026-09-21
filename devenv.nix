{ pkgs, config, ... }: {
    languages.cplusplus.enable = true;

    packages = with pkgs; [
	gcc
	glfw
	gdb
	gdb
	libX11
	libXrandr
	libXi
	libXxf86vm
	libXcursor
	assimp
    ];
}
