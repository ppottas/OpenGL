
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <GL/glew.h>
#include <GL/glut.h>
#include <time.h> 



using std::cout;
using std::endl;

/** Number of milliseconds in a second. */
const int MILLISECONDS_PER_SECOND = 1000;

/** Character code for the escape key in GLUT. */
const int KEY_ESCAPE = 27;

/** The window's initial width. */
const int WINDOW_WIDTH = 800;
/** The window's initial height. */ 
const int WINDOW_HEIGHT = 800;
/** The target frames per second. */
const unsigned int TARGET_FPS = 60;
/** The target time interval between frames (in nanoseconds).  */ 
const long TARGET_DELTA_T = floor(
      (MILLISECONDS_PER_SECOND) *
      ((double)1 / (double)TARGET_FPS)); 

/** Last time a frame was drawn. */
int last_frame;
 
/** Last time the FPS was printed. */
int last_print;

/** Handle to the vertex shader program. */
GLuint vertexShader;
/** Handle to the fragment shader program. */
GLuint fragmentShader;
/** Handle to the shader program. */
GLuint program;

/** Handle for the quad. */
GLuint quad;

/** The amount of vertices in a triangle. */
const GLuint VERTICES_PER_TRIANGLE = 3;
/** The amount of triangles per quad. */
const GLuint TRIANGLES_PER_QUAD = 2;
/** The amount of dimensions we'll be using. We're working in 2D here. */
const GLuint DIMENSIONS = 2;
/** The size of the vertex arrays. */
const GLuint ARRAY_SIZE = DIMENSIONS * VERTICES_PER_TRIANGLE * 
   TRIANGLES_PER_QUAD;

/** Location of the "position" attribute. */
GLint positionLoc;
/** Location of the "time" attribute. */
GLint timeLoc;

/**
 * Clean up any resources we may have used.
 */
void
cleanUp();

/**
 * GLUT display function.
 */
void
display();

/**
 * Attempts to find the vertex attribute with the given name.
 *
 * Prints an error message if it can't find the  vertex attribute.
 *
 * @param name The vertex attribute's name in the shader.
 * @return The vertex attribute's location, or -1 if it wasn't found.
 */
GLint
findAttribute(const char* name);

/**
 * Attempts to find the uniform with the given value.
 *
 * Prints an error if it can't find the uniform in question.
 *
 * @param name The uniform's name in the shader.
 * @return The uniform's location, or -1 if it wasn't found.
 */
GLint
findUniform(const char* name);

/**
 * Get the program's current runtime in milliseconds.
 */
int
getTime();

/**
 * GLUT idle function. Called when GLUT has no other work to do.
 */
void
idle();

/**
 * Initialize OpenGL.
 */
void
init();

/**
 * Initialize GLEW.
 */
void
initGLEW();

/**
 * Initialize GLUT.
 *
 * @param argc The argument count (from main()).
 * @param argv The argument vector (from main()).
 */
void
initGLUT(int argc, char** argv);

/**
 * GLUT keyboard function. Called on keyboard key press.
 *
 * @param key A character indicating which key was pressed.
 * @param x The current x-position of the mouse.
 * @param y The current y-position of the mouse.
 */
void
keyboard(unsigned char key, int x, int y);

/**
 * Loads an ASCII file into a character array.
 *
 * @param fileName The name of the file to load.
 * 
 * @return The character array.
 */
char*
loadFile(const char* fileName);

/**
 * Load the geometry.
 */
void
loadGeometry();

/**
 * Load, compile and link shader programs.
 */
void
loadShaderPrograms();

/**
 * Load and compile a shader program.
 */
void
loadShaderProgram(unsigned& handle, const char* file, GLenum shaderType);

/**
 * GLUT mouse function. Called on mouse button click.
 *
 * @param button A value indicating which button was pressed.
 * @param state A value indicating which state the button was put in (pressed / 
 * released).
 * @param x The mouse's x-location.
 * @param y The mouse's y-location.
 */
void 
mouse(int button, int state, int x, int y);

/**
 * Print the instantaneous frames per second.
 */
void
printFPS();

/**
 * GLUT reshape function. In this function, the application has a chance to 
 * perform any operations that have to happen after the window's size has 
 * changed. Since the aspect ration can also change, this means that a typical 
 * application will have to initialize the projection matrix anew.
 *
 * @param newWidth The window's new width.
 * @param newHeight THe window's new height.
 */
void
reshape(int newWidth, int newHeight);

