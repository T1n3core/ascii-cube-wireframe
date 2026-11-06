# 3D ASCII Cube

A small C project that renders a rotating 3D cube directly in the terminal using ASCII characters.

Built for fun as an experiment with 3D mathematics, matrix transformations, perspective projection, and basic terminal rendering — no graphics libraries required!

## Preview

The program continuously renders a rotating wireframe cube using `#` characters in your terminal.

## Features

- **3D rotation**: Rotates the cube around the X, Y, and Z axes.
- **Matrix transformations**: Uses 3×3 rotation matrices to transform 3D vertices.
- **Perspective projection**: Projects 3D coordinates onto a 2D plane to create a sense of depth.
- **Wireframe rendering**: Draws the 12 edges of the cube using a simple line-drawing algorithm.
- **Terminal graphics**: Renders everything directly in the console using ASCII characters.
- **Cross-platform support**: Includes basic Windows and Unix-like system support for clearing the terminal and controlling frame delays.

## How It Works

The program follows a simple rendering pipeline:

1. **Vertex definition** – Defines the eight vertices of a cube in 3D space.
2. **Rotation** – Applies rotation matrices around the X, Y, and Z axes.
3. **Projection** – Converts the rotated 3D coordinates into 2D coordinates using perspective projection.
4. **Screen mapping** – Maps the projected coordinates to terminal positions.
5. **Line drawing** – Connects the vertices using a basic interpolation-based line algorithm.
6. **Rendering** – Prints the resulting frame to the terminal and repeats.

## Requirements

- A C compiler (GCC, Clang, or MSVC)
- A terminal or command prompt
- The standard C math library

## Compilation and Execution

### Linux / macOS

Compile using GCC:

```bash
gcc main.c -o cube -lm
```

Run:

```bash
./cube
```

### Windows

Compile using GCC (MinGW):

```bash
gcc main.c -o cube.exe -lm
```

Run:

```bash
.\cube.exe
```

## Configuration

You can tweak a few constants at the beginning of the source code:

| Constant          | Description                                           |
|-------------------|-------------------------------------------------------|
| `WIDTH`           | Terminal rendering width (80 characters by default).  |
| `HEIGHT`          | Terminal rendering height (40 characters by default). |
| `SCALE`           | Controls the projected size of the cube.              |
| `CAMERA_DISTANCE` | Controls the camera's distance from the cube.         |

You can also adjust the animation speed by changing:

```c
angleX += 5.0;
angleY += 5.0;
angleZ += 5.0;

sleep_ms(50);
```

## Notes

- The cube is rendered as a wireframe, meaning only its edges are drawn.
- The animation runs continuously until the program is manually terminated (e.g. Ctrl + C).
- The rendering resolution is limited by the terminal's character grid.
- This is a small educational project rather than a full 3D rendering engine.

## Motivation

Just a fun little experiment to explore how 3D graphics work under the hood, without relying on graphics APIs or external rendering libraries.
Sometimes it's fun to see what a little bit of linear algebra can do in a terminal.

*Made for fun, powered by C and a bit of math.*
