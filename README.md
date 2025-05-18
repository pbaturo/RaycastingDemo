# RaycastingDemo

This repository provides a raycasting engine demo built with SDL2. It demonstrates the core rendering techniques used in classic pseudo-3D games like Wolfenstein 3D, featuring texture mapping, wall rendering, and distance-based shading.

## Features

- **Raycasting Engine**: Efficient pseudo-3D rendering technique.
- **Textured Walls**: Support for wall textures with proper perspective.
- **Distance-Based Shading**: Creates depth perception through lighting effects.
- **Cross-Platform Build Support**: Works on macOS, Linux, and Windows via Makefile.
- **SDL2 Integration**: For window management and rendering.

## Prerequisites

- SDL2 development libraries installed.
- A C compiler (clang or gcc).
- `make` build tool.

## Getting Started

1. Clone the repository:
   ```bash
   git clone https://github.com/YOUR_USERNAME/RaycastingDemo.git
   cd RaycastingDemo
   ```

2. Build the project:
   ```bash
   make build
   ```

3. Run the project:
   ```bash
   make run
   ```

4. Clean the build files:
   ```bash
   make clean
   ```

## File Structure

- `src/`: Main source code.
- `include/`: Header files.
- `bin/`: Compiled binaries.
- `Makefile`: Build configuration.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Please open issues or pull requests for improvements.

---

Happy coding!