#include "Main.h"

void
cleanUp()
{
}

void
display()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glDrawArrays(GL_TRIANGLES, 0, ARRAY_SIZE);

   glFlush();
   glutSwapBuffers();

   printFPS();

   /* This is now the last time we drew a frame, so we record it. */
   last_frame = getTime();
}

GLint
findAttribute(const char* name)
{
	
	
   GLint location = glGetAttribLocation(program, name);

   if (location == -1)
   {
      fprintf(stderr, "Could not find attribute named '%s'.\n", name);
   }

   return location;
}

GLint
findUniform(const char* name)
{
   GLint location = glGetUniformLocation(program, name);

   if (location == -1)
   {
      fprintf(stderr, "Could not find uniform named '%s'.", name);
   }

   return location;
}

int
getTime()
{
   return glutGet(GLUT_ELAPSED_TIME);
}
float aballpos[2] = {300,400};
float bballpos[2] = {390,400};
float cballpos[2] = {400,100};

void mouse( int x, int y) {
	float tx = (float) x;
float ty = 800-y;
aballpos[0] = tx / WINDOW_WIDTH;
aballpos[1] = ty / WINDOW_HEIGHT;
/*if (abs(tx-aballpos[0])<100 && abs(ty-aballpos[1])<100) {
aballpos[0] = tx;
aballpos[1] = ty;
} else
if (abs(tx-bballpos[0])<100 && abs(ty-bballpos[1])<100) {
bballpos[0] = tx;
bballpos[1] = ty;
} else
if (abs(tx-cballpos[0])<100 && abs(ty-cballpos[1])<100) {
cballpos[0] = tx;
cballpos[1] = ty;
}*/
//cout << button << state << endl;
}
void
idle()
{
   /* Calculate the time elapsed between frame. */
   int now = getTime();
   int deltaT = now - last_frame;

   /* Is it time to draw a new frame again? */
   if (deltaT >= TARGET_DELTA_T)
   {
      /* Update time uniform. */
      glUniform1f(3, (float)now / (float)MILLISECONDS_PER_SECOND);
//cout << now << endl;
	glUniform2f(0,aballpos[0],aballpos[1]);
	glUniform2f(1,bballpos[0],bballpos[1]);
	glUniform2f(2,cballpos[0],cballpos[1]);




      glutPostRedisplay();
   }
}

void
init()
{
   /* Initialize last_frame so that our FPS counter and idle function will work 
      right off the bat. */
   last_frame = getTime();
   /* Initialize last_print so that we start printing FPS in about one second. 
    */
   last_print = getTime();

   glClearColor(0.0, 0.0, (GLclampf)0.0, 0.0);

   loadShaderPrograms();
   glUseProgram(program);

   positionLoc = findAttribute("position");
   timeLoc = findUniform("time");

   loadGeometry();
}

void
initGLEW()
{
   glewInit();

   if (!GLEW_VERSION_3_2)
   {
      fprintf(stderr, "This program requires OpenGL 3.2");
      exit(-1);
   }
}

void
initGLUT(int argc, char** argv)
{
   /* Initialize GLUT. Pass in argc and argv, since GLUT enables some 
      command-line parameters for us. */
   glutInit(&argc, argv);
   /* Ask GLUT to initialzie OpenGL for us, using a double buffer, a depth 
      buffer and red, green and blue colour model. */
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   /* Ask GLUT to create a window for us. */
   glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
   /* Set the window's title. */
   glutCreateWindow("Metaball Framework");

   /* Set the function to be called when the window must be redraw (following a 
    * resize, or a switch to another window which obscures this one. */
   glutDisplayFunc(display);
   /* Set the function to be called on a keyboard key press. */
   glutKeyboardFunc(keyboard);
glutPassiveMotionFunc(mouse);
   /* Set the function to be called when the window's size changes. */
   glutReshapeFunc(reshape);
   /* Set the function to be called when GLUT has no other work to do. */
   glutIdleFunc(idle);
}

void
keyboard(unsigned char key, int x, int y)
{
   switch (key)
   {
      case KEY_ESCAPE:
         printf("Bye!\n");
         exit(0);
         break;
      default:
         printf("Key '%c' pressed, mouse at (%d, %d).\n", key, x, y);
         break;
   }
}

char*
loadFile(const char* fileName)
{
	FILE* file;
	   fopen_s(&file,fileName, "r");
   if (file == NULL)
   {
      fprintf(stderr, "Could not open file '%s'.\n", fileName);
   }

   unsigned bufferSize = 1;
   char* buffer = (char*)malloc(bufferSize);

   unsigned long long index = 0;
   while (true)
   {
      char c = (char)fgetc(file);
      if (ferror(file) != 0)
      {
         fprintf(stderr, "Could not read from file '%s'.\n", fileName);
      }

      /* There must be space at bufferSize - 2 for the new char, and at 
       * bufferSize - 1 for the \0 character terminating the string. So if 
       * the buffer's index has already reached bufferSize - 1 it is already 
       * out of space! We have to do this even if the file is empty, since we 
       * must have space for the NULL anyway. */
      if (index == bufferSize - 1)
      {
         bufferSize *= 2;
         buffer = (char*)realloc((void*)buffer, bufferSize);
      }

      if (feof(file))
      {
         break;
      }
      else
      {
         buffer[index] = c;
         index++;
      }
   }
   buffer[index] = '\0';

   fclose(file);

   return buffer;
}

void
loadGeometry()
{
   GLfloat* vertices = new GLfloat[ARRAY_SIZE];
   GLuint i = 0;
   vertices[i++] = -1.0f;
   vertices[i++] = -1.0f;

   vertices[i++] = 1.0f;
   vertices[i++] = -1.0f;

   vertices[i++] = -1.0f;
   vertices[i++] = 1.0f;

   vertices[i++] = -1.0f;
   vertices[i++] = 1.0f;

   vertices[i++] = 1.0f;
   vertices[i++] = -1.0f;

   vertices[i++] = 1.0f;
   vertices[i++] = 1.0f;

   glGenVertexArrays(1, &quad);
   glBindVertexArray(quad);

   /* Where we will store the handle of the buffer storing the red vertices. */
   GLuint buffer;

   glGenBuffers(1, &buffer);
   glBindBuffer(GL_ARRAY_BUFFER, buffer);

   glBufferData(GL_ARRAY_BUFFER, ARRAY_SIZE * (int)sizeof(GLfloat),
         vertices, GL_STATIC_DRAW);

   glEnableVertexAttribArray((GLuint)positionLoc);
   glVertexAttribPointer((GLuint)positionLoc, DIMENSIONS, GL_FLOAT, GL_FALSE, 
         0, 0);

   /* We can get rid of our array, since its data is now stored on the graphics 
      card. */
   delete[] vertices;
}

void
loadShaderPrograms()
{
   loadShaderProgram(vertexShader, "vertex.glsl", GL_VERTEX_SHADER);
   loadShaderProgram(fragmentShader, "fragment.glsl", GL_FRAGMENT_SHADER);

   /* Start by defining a shader program which acts as a container. */
   program = glCreateProgram();

   /* Next, shaders are added to the shader program. */
   glAttachShader(program, vertexShader);
   glAttachShader(program, fragmentShader);

   /* Finally, the program must be linked. */
   glLinkProgram(program);

   /* Check if it linked  properly. */
   int status;
   glGetProgramiv(program, GL_LINK_STATUS, &status);

   if (status == GL_FALSE)
   {
      /* Get the length of the info log. */
      int len;
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

      /* Get the info log. */
      char* log = new char[len];
      glGetProgramInfoLog(program, len, &len, log);

      /* Throw an exception. */
      fprintf(stderr, "Link error: %s.\n", log);

      /* Finally, free the memory allocated. */
      delete log;

      /* Exit the program. */
	  system("PAUSE");
      exit(-1);
   }
}

void
loadShaderProgram(unsigned& handle, const char* file, GLenum shaderType)
{
   /* First, a handle to a shader object of the appropriate type must be 
    * obtained. */
   handle = glCreateShader(shaderType);

   /* Second, the shader source must be loaded... */
   char* source = loadFile(file);

   /* ...and set to be the current source in the OpenGL state machine. The first 
    * parameter here is a handle to a shader object, the second is the number of 
    * strings in the array which we provide next. The last parameter is an 
    * integer array indicating the length of these strings. The third parameter
    * may also contain the whole source code as one long string, while the 
    * fourth parameter may be NULL, to indicate that the string(s) is (are) NULL 
    * terminated. Since that is the case in our implementation, the arguments 
    * passed here correspond. */
   glShaderSource(handle, 1, (const GLchar**)&source, NULL);

   /* Third, we compile the shader program. */
   glCompileShader(handle);

   /* After the shader source has been set inside OpenGL, we can free the memory 
    * we allocated. */
   free(source);

   /* Check if it compiled properly, if not, print any log information. */
   int status;
   glGetShaderiv(handle, GL_COMPILE_STATUS, &status);

   if (status == GL_FALSE)
   {
      /* Get the length of the info log. */
      int len;
      glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &len);

      /* Get the info log. */
      char* log = new char[len];
      glGetShaderInfoLog(handle, len, &len, log);

      /* Throw an exception. */
      fprintf(stderr, "Compilation error: %s.\n", log);

      /* Finally, free the memory allocated. */
      delete log;
	  system("PAUSE");
      /* Exit the program. */
      exit(-1);
   }
}

void
printFPS()
{
   int now = glutGet(GLUT_ELAPSED_TIME);

   /* Has it been a second since the last time we printed the FPS? */
   if (now - last_print > MILLISECONDS_PER_SECOND)
   {
      /* Get the time elapsed between frames. */
      int deltaT = now - last_frame;
      /* Convert from milliseconds to seconds. */
      double seconds = (double)deltaT / MILLISECONDS_PER_SECOND;
      /* We have the elapsed time, so in order to get the frequency, we must 
         invert the value of "seconds". */
      double fps = 1.0 / seconds;

      printf("Instantaneous FPS: %.2f\n", fps);
      /* This is now the last time we printed the FPS, so we record it. */
      last_print = now;
   }
}

void
reshape(int newWidth, int newHeight)
{
   /* Note the new width and height of the window. */
   printf("Resized. New width = %d and new height = %d.\n", newWidth, newHeight);

   /* Correct the viewport. */
   glViewport(0, 0, newWidth, newHeight);

   /* Redraw the scene if the window changed. */
   glutPostRedisplay();
}

int
main(int argc, char** argv)
{
   initGLUT(argc, argv);
   initGLEW();
   init();
   glutSetCursor(GLUT_CURSOR_NONE);
   reshape(WINDOW_WIDTH, WINDOW_HEIGHT);
  // glutFullScreen();
   glutMainLoop();

   cleanUp();
  
   return 0;
}
