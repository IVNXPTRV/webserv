{
  description = "A development shell for C++ webserver";

  inputs = {
    # Specify the version of the Nixpkgs repository to use for dependencies
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-23.11"; # Use a stable branch
    nixpkgs-unstable.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs = { self, nixpkgs, nixpkgs-unstable, ... }:
  let
    # Define the system architecture (e.g., "x86_64-linux")
    system = "x86_64-linux";

    # Import the nixpkgs library for the specified system
    pkgs = import nixpkgs {
      inherit system;
    };
    unstable-pkgs = import nixpkgs-unstable { inherit system; };
  in {
    # This is the crucial part: defining the development shell
    devShells.${system}.default = pkgs.mkShell {
      # Define the dependencies available in the shell's PATH
      # Add any C++-related packages you need here
      packages = with pkgs; [
        # The C/C++ compiler suite (GCC or clang)
        gcc
        valgrind

        # The build system generator (essential for most C++ projects)
        cmake

        # Debugger
        gdb

        # Formatters
        clang-tools
        unstable-pkgs.mbake

        # Version control
        git

        # compile help
        bear
        tree
        
        # LLM
        unstable-pkgs.gemini-cli
      ];

      # Set environment variables if needed (e.g., for specific library paths)
      # shellHook = ''
      #   echo "Entering C++ development environment..."
      # '';

    };
  };
}
