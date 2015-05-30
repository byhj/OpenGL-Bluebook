# OpenGL-Bluebook
Welcome to the OpenGL SuperBible Book 6th Edition Source Code by byhj!

1.Introduction
============================================================================================================================
OpenGL® SuperBible, Sixth Edition, is the definitive programmer’s guide, tutorial, and reference for the world’s leading 3D API for real-time computer graphics, OpenGL 4.3. The best all-around introduction to OpenGL for developers at all levels of experience, it clearly explains both the newest API and indispensable related concepts. You’ll find up-to-date, hands-on guidance for all facets of modern OpenGL development on both desktop and mobile platforms, including transformations, texture mapping, shaders, buffers, geometry management, and much more.

You can get more infomation by: http://www.openglsuperbible.com/

You can download the source code and media in this Website. 

The github of this book: https://github.com/openglsuperbible/sb6code

=============================================================================================================================

 
2.Requirements
=============================================================================================================================
  1. Visual Studio 2012 or 2013 (and some knowledge on how to build, run, and debug C++ projects)
  2. Microsoft Windows SDK for Windows 7 (This include the opengl)
  3. glew (The OpenGL Extension Wrangler Library)
  4. glfw3 (Use to creating render windows)
  5. glm (OpenGL Mathematics)
  
=============================================================================================================================

3.How to Buil
=============================================================================================================================
  1. Make sure you develop environment is right.
  2. You can build the environment by yourself according your target.
  glew: http://glew.sourceforge.net
  glfw: http://www.glfw.org
  glm:  http://glm.g-truc.net
  
  3.Project Organization:
		  |Root
              |-- include 
              |-- bin
              |-- lib
              |-- src
              |-- media
 Notice: I use the Visual Studio project props file to share the project setting.
         The output dir:
              |-- exe(Execute Dir) 
              |-- temp(temp file we possibly not need) 

=============================================================================================================================

4.How to Run
=============================================================================================================================
Open OpenGL-Bluebook.sln
  1. Right click on the project and set is as your startup project.
  2. Enter the exe Dir, run the exe to view the render output.
  
