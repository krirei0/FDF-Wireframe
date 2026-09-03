# FDF Wireframe

A C++ program that reads a heightmap file and renders it as a 3D wireframe landscape, where every surface is represented purely by connecting lines between elevation points. Built with SDL2.


## Overview

<img width="1594" height="888" alt="42_map_scrshot" src="https://github.com/user-attachments/assets/734bd3a3-071d-4e9d-ba8e-b3e23cd2d0b8" />

<img width="1556" height="838" alt="Screenshot 2026-08-31 130545" src="https://github.com/user-attachments/assets/7c3d8f2f-c0bb-48cd-bc61-8b8ecadc4bac" />



This project takes a .fdf map file - a grid of numbers representing elevation at each point - and turns it into an isometric 3D wireframe. Rather than filling in solid surfaces, the landscape is represented entirely by outlining connections between neighboring points, similar to old-school vector graphics or topographic wireframe models.


## Features
- Custom .fdf file parser - reads a grid of elevation values from a text file into a 2D point structure.
- 3D rotation - rotates the point grid before projection, allowing the landscape to be viewed from different angles.
- Isometric projection - converts 3D world coordinates into 2D screen coordinates using an isometric transform.
- Automatic scaling and centering - calculates the bounding box of the projected points, scales the model to fit the window, and centers it regardless of map size.
- Wireframe rendering - draws lines between connected grid points using SDL2.
- Height-based coloring - elevation is visually distinguished via DrawZ, so higher/lower points read differently on screen.


## How it works
The rendering pipeline in `main.cpp` runs in several stages:

1. **Parse** - `ParseFdf()` reads the map file into a grid of `Point` structs (`x`, `y`, `z`).
2. **Rotate** - `RotatePoint()` applies 3D rotation to the point grid.
3. **Project** - `IsometricProjection()` converts the 3D coordinates into 2D screen coordinates.
4. **Fit to window** - `FindBoundaries()` determines the model's bounds and `FindScaleFactor()` calculates the appropriate scale.
5. **Center** - `FindCenter()` and `CalculateTranslation()` calculate the translation needed to center the model.
6. **Translate** - `ApplyTranslation()` moves the points to their final screen positions.
7. **Render** - `DrawConnection()` draws the wireframe and `DrawZ()` adds elevation-based coloring using SDL2.


## Requirements
C++ compiler (C++11 or later)
SDL2 development libraries

