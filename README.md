# Rotating Cubes with Multi-Texturing and Camera Controls

![OpenGL Cube](https://img.shields.io/badge/OpenGL-3.3-blue) ![C++](https://img.shields.io/badge/C++-Modern-green) ![GLFW](https://img.shields.io/badge/GLFW-Windowing-yellow) ![GLEW](https://img.shields.io/badge/GLEW-Extension%20Loader-orange)

## Project Overview

This project demonstrates a **3D rendering engine** using **Modern OpenGL (3.3 Core)** with advanced features such as **multi-texturing**, **independent cube rotations**, and **keyboard-controlled camera movement**.
---

## Features

- **Multiple Rotating Cubes**  
  - Three independent cubes, each with its own rotation axis and speed.  
  - Each cube can display a single texture or a **blend of two textures** dynamically.  

- **Multi-Texturing with Dynamic Blend Ratio**  
  - Supports single texture, alternate texture, or a custom blend ratio between two textures.  
  - Uniform-controlled `mode` and `mixRatio` allow per-object texture flexibility.  

- **Keyboard Camera Controls**  
  - WASD movement for free navigation in the 3D scene.  
  - View matrix updates in real-time based on camera position.  

- **Modern OpenGL Practices**  
  - Core-profile context (3.3+), VAO/VBO, shaders for vertex and fragment stages.  
  - Shader uniforms for dynamic transformations and texture blending.  

- **Texture Management**  
  - Supports JPEG and PNG textures using **stb_image**.  

---

## Demo

![Rotating Cubes Demo](Outputgif.gif)

---

## Getting Started

### Prerequisites

- **C++17** or later  
- **GLFW**, **GLEW**, **GLM**, **stb_image**  
- OpenGL 3.3 compatible GPU  

### Compilation Example

```bash
g++ Cubes.cpp -lGLEW -lGL -lglfw -ldl -o cubes
./cubes
