# Project: generator_bryl

## Project Overview

This is a C++ project that uses the Qt 6 framework. It appears to be a desktop application in its early stages of development. The project is set up to use `CMake` for building and includes Qt's `Core`, `Widgets`, `LinguistTools`, and `OpenGL` modules, suggesting that it may be intended to be a graphical application with 2D or 3D rendering capabilities. The application has a main window but no specific UI elements or functionality have been implemented yet.

## Building and Running

This project uses `CMake` to generate build files.

### Prerequisites

*   C++ compiler
*   CMake
*   Qt 6 (including Core, Widgets, LinguistTools, OpenGL, and OpenGLWidgets modules)

### Building

1.  Create a build directory:
    ```bash
    mkdir build
    cd build
    ```

2.  Run CMake to generate the build files. On Windows with MinGW, you might run:
    ```bash
    cmake .. -G "MinGW Makefiles"
    ```
    On other platforms, you can often just run:
    ```bash
    cmake ..
    ```

3.  Compile the project:
    ```bash
    cmake --build .
    ```

### Running

The executable will be located in the build directory. You can run it from there:

```bash
./generator_bryl
```

## Development Conventions

*   The code follows standard C++/Qt conventions.
*   The UI is defined in `.ui` files (XML format) which are then compiled into C++ code.
*   Internationalization is supported through `.ts` files.

## Project Assumptions

*   **Object-Oriented Design for Shapes:** The application will utilize a robust object-oriented approach for handling 3D shapes.
*   **Base Shape Class:** A common base class, `Bryla` (Shape), will be introduced, providing an interface for all drawable 3D objects.
*   **Specific Shape Classes:** Each distinct 3D shape (e.g., `Walec` for Cylinder, `Kula` for Sphere) will have its own dedicated class, inheriting from `Bryla`.
*   **File Organization:** Each class will reside in its own `.h` and `.cpp` files to promote modularity and maintainability.
*   **Full OOP Utilization:** The design will leverage inheritance, polymorphism, and other OOP principles to manage shape properties, rendering logic, and interactions effectively.