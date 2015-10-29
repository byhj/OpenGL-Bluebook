
/*
OpenGL ARB : Architectural Review Board

1992 OpenGL 1.0 Released
1997 OpenGL 1.1
1998 OpenGL 1.2
1998 OpenGL 1.2.1
2001 OpenGL 1.3
2002 OpenGL 1.4
2003 OpenGL 1.5
2004 OpenGL 2.0      GLSL version 110
2006 OpenGL 2.1      GLSL version 120  
2008 OpenGL 3.0 (ARB fork the OpenGL specification into two profiles(Core & compatibility)  GLSL version 130
2009 OpenGL 3.1      GLSL version 140
2009 OpenGL 3.2      GLSL version 150
2010 OpenGL 3.3      GLSL version 330
2010 OpenGL 4.0      GLSL version 400
2011 OpenGL 4.1      GLSL version 410
2011 OpenGL 4.2      GLSL version 420
2012 OpenGL 4.3      GLSL version 430
2013 OpenGL 4.4      GLSL version 440
2014 OpenGL 4.5      GLSL version 450

           (front end)   data                   vertex                                 patch
Pipeline:  Vertex Fetch --------> Vertex Shader --------> Tessellation Control Shader -------> Tessellation Engine------> 
                                                                  primitives                     pixel
		   Tessellation Evaluation Shader -----> Geometry Shader --------------> Rasterization --------->
                                                   |
										     Transform feedback
		   (back end) 
		   Fragment Shader --> Framebuffer Operations(Scissor Test, Depth Test, Stencil Test, blending, logical operator)
*/