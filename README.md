# SDL Game Template

This repository provides a ready-to-use cross-platform SDL game template. It includes a basic game loop, input handling, and rendering setup – perfect for getting started with 2D game development.

## Features

- **Basic Game Loop**: Initialization, input processing, update, render, and cleanup.
- **Cross-Platform Build Support**: Works on macOS, Linux, and Windows via Makefile.
- **SDL2 Integration**: For window management and rendering.
- **Delta Time Management**: Smooth frame updates across different machines.

## Prerequisites

- SDL2 development libraries installed.
- A C compiler (clang or gcc).
- `make` build tool.

## Getting Started

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/sdl-game-template.git
   cd sdl-game-template
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