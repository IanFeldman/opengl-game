with import <nixpkgs> {};

mkShell {
  nativeBuildInputs = [ pkg-config cmake libepoxy ];
  buildInputs = [ glfw glew wayland ];
}
