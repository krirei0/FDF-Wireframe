# FDF Wireframe

A C++ program that reads a heightmap file and renders it as a 3D wireframe landscape, where every surface is represented purely by connecting lines between elevation points. Built with SDL2.


## Overview

<img width="1594" height="927" alt="42_map_scrshot" src="https://github.com/user-attachments/assets/eb93fb54-a128-49dc-8b52-99676fb39409" />

This project takes a .fdf map file - a grid of numbers representing elevation at each point — and turns it into an isometric 3D wireframe. Rather than filling in solid surfaces, the landscape is represented entirely by outlining connections between neighboring points, similar to old-school vector graphics or topographic wireframe models.


## Features
- Custom .fdf file parser - reads a grid of elevation values from a text file into a 2D point structure.
- 3D rotation - rotates the point grid before projection, allowing the landscape to be viewed from different angles.
- Isometric projection - converts 3D world coordinates into 2D screen coordinates using an isometric transform.
- Automatic scaling and centering - calculates the bounding box of the projected points, scales the model to fit the window, and centers it regardless of map size.
- Wireframe rendering - draws lines between connected grid points using SDL2.
- Height-based coloring - elevation is visually distinguished via DrawZ, so higher/lower points read differently on screen.


## How it works
The rendering pipeline in main.cpp runs in stages:

Parse - ParseFdf() reads the map file into a grid of Point structs (x, y, z).
Rotate - RotatePoint() applies a 3D rotation to the grid.
Project - IsometricProjection() converts the rotated 3D points into 2D screen-space coordinates.
Fit to window -
FindBoundaries() finds the bounding box of the projected points.
FindScaleFactor() computes how much to scale the model so it fits the window.
ApplyScale() scales all points accordingly.
FindCenter() and CalculateTranslation() work out how to center the scaled model in the window.
ApplyTranslation() shifts all points to their final screen position.
Render - DrawConnection() draws the wireframe lines between neighboring points, and DrawZ() adds elevation-based coloring, both via SDL2.


## Requirements
C++ compiler (C++11 or later)
SDL2 development libraries

