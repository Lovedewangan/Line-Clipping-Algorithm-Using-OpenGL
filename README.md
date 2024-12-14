# Line Clipping Algorithms Visualization
Project Overview
This interactive OpenGL application demonstrates two fundamental line clipping algorithms: Cohen-Sutherland and Liang-Barsky. The project provides a visual and interactive way to understand how these algorithms handle line clipping within defined viewport boundaries.
Features

Implement Cohen-Sutherland and Liang-Barsky line clipping algorithms
Interactive drawing of lines in two separate windows
Toggle clipping on/off for each algorithm
Dynamically resize and move viewport windows
Colorful line rendering with neon and darker color palettes
Real-time visualization of line clipping process

Algorithms Implemented

Cohen-Sutherland Algorithm

Efficiently determines whether a line segment is completely inside, outside, or partially inside a clipping window
Uses region codes to quickly reject or accept line segments


Liang-Barsky Algorithm

Parametric line clipping technique
Calculates line segment intersections with viewport boundaries
Determines visible portion of line segments

Controls
Cohen-Sutherland Window (Left)

Left-click: Draw lines
Right-click: Clear lines
C + 1: Toggle clipping
R + 1: Resize viewport
W/A/S/D: Move viewport

Liang-Barsky Window (Right)

Left-click: Draw lines
Right-click: Clear lines
C + 2: Toggle clipping
R + 2: Resize viewport
Arrow keys: Move viewport

Technologies Used

C++
OpenGL (FreeGLUT)
Object-Oriented Design

Build Requirements

C++ Compiler
OpenGL libraries
FreeGLUT

Author
Love Dewangan
