

const char* INSTRUCTIONS =
"***************\n"
"Game Instructions:\n"
"\n"
"Hit 'f' to toggle fullscreen.\n"
"There are two ways to drive: arrow keys or mouse. Hit 'm' to toggle mouse mode.\n"
"There are two camera views: chair or overhead. Hit 'g' to toggle.\n"
"In chair mode:\n"
"   shift + up or down arrow adjusts height.\n"
"   wsad rotates your head up/down left/right.\n"
"In overhead camera mode:\n"
"   wsad lets you pan. 'i' zooms in, 'o' zooms out.\n"
"   'h' toggles chair follow mode. Camera rotates with chair.\n\n"
"Other:\n"
"    shift + left/right arrows change ball size.\n"
"    'k' and 'l' make goal smaller or larger.\n"
"    '.' (period) stops the ball.\n"
"    'c' change ball color.\n"
"***************\n"
;

//Mac OS build: g++ maze.cpp -x c glad/glad.c -g -F/Library/Frameworks -framework SDL2 -framework OpenGL -o maze

#include "glad/glad.h"  //Include order can matter here
#if defined(__APPLE__) || defined(__linux__)
 #include <SDL2/SDL.h>
 #include <SDL2/SDL_opengl.h>
#else
 #include <SDL.h>
 #include <SDL_opengl.h>
#endif
#include <cstdio>

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "vector.h"
#include "point.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <vector>
using namespace std;
int screenWidth = 1050;
int screenHeight = 600;
float timePast = 0;
float time1 = 0, time2 = 0, dt;

float objx = 0, objy = 0, objz = .25f;
float colR = 1, colG = 1, colB = 1;
float chaircamheight = 2.;
Point lookAtpos, cam, lastcampos(0, 0, 30), poschair(3, 0, 0);
Vector up(0.0f, 0.0f, 1.0f), godcamdir(0.0, 0.0, -1.0), dir(-1, 0, 0);
bool follow = false;

float deltaX, deltaY;
float angle = -M_PI;
float headAngleUp = M_PI / 8;
float headAngleLeft = 0;

float courtaspectratio = 1.88; // 94/50
float courtw = 10;

float courtXmin = -courtw * courtaspectratio, courtXmax = courtw * courtaspectratio, courtYmin = -courtw, courtYmax = courtw;

float chairXmin = -.42, chairXmax = 2.21209, chairYmin = -.51, chairYmax = .51;

float balldiam = .65, ballradius = balldiam * .5, damp = .8, balldrag = .95;
Point ballpos(0, 0, ballradius);

Vector ballv(0, 0, 0), chairv(0, 0, 0);
float chairaspeed = 0, chairspeed = 0, drag = .95, acc = .9, dcc = 1.2, aacc = .16, adcc = .25;

Point goalpostm(courtXmin + .8, -2.5, 0), goalpostp(courtXmin + .8, 2.5 - .35, 0);
float goalpostdiam = .2, goalpostr = goalpostdiam * .5;

bool godView = false;
bool mouseMode = false;

float gravity = -2, velocity = 0;

bool DEBUG_ON = false;
GLuint InitShader(const char * vShaderFileName,
  const char * fShaderFileName);
bool fullscreen = false;
void Win2PPM(int width, int height);

//srand(time(NULL));
float rand01() {
  return rand() / (float) RAND_MAX;
}

void physics(float dt);

void drawGeometry(int shaderProgram, int model1_start, int model1_numVerts, int model2_start, int model2_numVerts, int model3_start, int model3_numVerts, int model4_start, int model4_numVerts);

void drawCourt(int shaderProgram, int model1_start, int model1_numVerts, int model2_start, int model2_numVerts);

void drawObjects(int shaderProgram, int model1_start, int model1_numVerts, int model2_start, int model2_numVerts);

int main(int argc, char * argv[]) {

  SDL_Init(SDL_INIT_VIDEO); //Initialize Graphics (for OpenGL)

  //Ask SDL to get a recent version of OpenGL (3.2 or greater)
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

  //Create a window (offsetx, offsety, width, height, flags)
  SDL_Window * window = SDL_CreateWindow("Soccer Game", 300, 100, screenWidth, screenHeight, SDL_WINDOW_OPENGL);

  //Create a context to draw in
  SDL_GLContext context = SDL_GL_CreateContext(window);

  //Load OpenGL extentions with GLAD
  if (gladLoadGLLoader(SDL_GL_GetProcAddress)) {
    // printf("\nOpenGL loaded\n");
    // printf("Vendor:   %s\n", glGetString(GL_VENDOR));
    // printf("Renderer: %s\n", glGetString(GL_RENDERER));
    // printf("Version:  %s\n\n", glGetString(GL_VERSION));
  } else {
    printf("ERROR: Failed to initialize OpenGL context.\n");
    return -1;
  }

  //Load Model 1
  ifstream modelFile;
  modelFile.open("models/chair.txt");
  int numLines = 0;
  modelFile >> numLines;
  float * model1 = new float[numLines];
  for (int i = 0; i < numLines; i++) {
    modelFile >> model1[i];
  }
  // printf("%d\n",numLines);
  int numVertsChair = numLines / 8;
  modelFile.close();

  //Load Model 2
  modelFile.open("models/sphere.txt");
  numLines = 0;
  modelFile >> numLines;
  float * model2 = new float[numLines];
  for (int i = 0; i < numLines; i++) {
    modelFile >> model2[i];
  }
  // printf("%d\n",numLines);
  int numVertsSphere = numLines / 8;
  modelFile.close();

  //Load Model 3
  modelFile.open("models/cube.txt");
  numLines = 0;
  modelFile >> numLines;
  float * model3 = new float[numLines];
  for (int i = 0; i < numLines; i++) {
    modelFile >> model3[i];
  }
  // printf("%d\n",numLines);
  int numVertsCube = numLines / 8;
  modelFile.close();

  //Load Model 4
  modelFile.open("models/floor.txt");
  numLines = 0;
  modelFile >> numLines;
  float * model4 = new float[numLines];
  for (int i = 0; i < numLines; i++) {
    modelFile >> model4[i];
  }
  // printf("%d\n",numLines);
  int numVertsFloor = numLines / 8;
  modelFile.close();

  float * modelData = new float[(numVertsChair + numVertsSphere + numVertsCube + numVertsFloor) * 8];
  copy(model1, model1 + numVertsChair * 8, modelData);
  copy(model2, model2 + numVertsSphere * 8, modelData + numVertsChair * 8);
  copy(model3, model3 + numVertsCube * 8, modelData + numVertsChair * 8 + numVertsSphere * 8);
  copy(model4, model4 + numVertsFloor * 8, modelData + numVertsChair * 8 + numVertsSphere * 8 + numVertsCube * 8);

  int totalNumVerts = numVertsChair + numVertsSphere + numVertsCube + numVertsFloor;
  int startVertChair = 0; //The teapot is the first model in the VBO
  int startVertSphere = numVertsChair; //The knot starts right after the taepot
  int startVertCube = numVertsChair + numVertsSphere; //The cube starts right after the knot
  int startVertFloor = numVertsChair + numVertsSphere + numVertsCube; //The floor starts right after the cube

  //// Allocate Texture 0 (Wood) ///////
  SDL_Surface * surface = SDL_LoadBMP("wood.bmp");
  if (surface == NULL) { //If it failed, print the error
    printf("Error: \"%s\"\n", SDL_GetError());
    return 1;
  }
  GLuint tex0;
  glGenTextures(1, &tex0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex0);

  //What to do outside 0-1 range
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  //Load the texture into memory
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface - > w, surface - > h, 0, GL_BGR, GL_UNSIGNED_BYTE, surface - > pixels);
  glGenerateMipmap(GL_TEXTURE_2D); //Mip maps the texture

  SDL_FreeSurface(surface);
  //// End Allocate Texture ///////

  //// Allocate Texture 1 (Brick) ///////
  SDL_Surface * surface1 = SDL_LoadBMP("brick.bmp");
  if (surface1 == NULL) { //If it failed, print the error
    printf("Error: \"%s\"\n", SDL_GetError());
    return 1;
  }
  GLuint tex1;
  glGenTextures(1, &tex1);

  //Load the texture into memory
  glActiveTexture(GL_TEXTURE1);

  glBindTexture(GL_TEXTURE_2D, tex1);
  //What to do outside 0-1 range
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //How to filter
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface1 - > w, surface1 - > h, 0, GL_BGR, GL_UNSIGNED_BYTE, surface1 - > pixels);
  glGenerateMipmap(GL_TEXTURE_2D); //Mip maps the texture

  SDL_FreeSurface(surface1);
  //// End Allocate Texture ///////

  //// Allocate Texture 2 (gold) ///////
  SDL_Surface * surface2 = SDL_LoadBMP("court.bmp");
  if (surface2 == NULL) { //If it failed, print the error
    printf("Error: \"%s\"\n", SDL_GetError());
    return 1;
  }
  GLuint tex2;
  glGenTextures(1, &tex2);

  //Load the texture into memory
  glActiveTexture(GL_TEXTURE2);

  glBindTexture(GL_TEXTURE_2D, tex2);
  //What to do outside 0-1 range
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //How to filter
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface2 - > w, surface2 - > h, 0, GL_BGR, GL_UNSIGNED_BYTE, surface2 - > pixels);
  glGenerateMipmap(GL_TEXTURE_2D); //Mip maps the texture

  SDL_FreeSurface(surface2);
  //// End Allocate Texture ///////

  //Build a Vertex Array Object (VAO) to store mapping of shader attributse to VBO
  GLuint vao;
  glGenVertexArrays(1, &vao); //Create a VAO
  glBindVertexArray(vao); //Bind the above created VAO to the current context

  //Allocate memory on the graphics card to store geometry (vertex buffer object)
  GLuint vbo[1];
  glGenBuffers(1, vbo); //Create 1 buffer called vbo
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); //Set the vbo as the active array buffer (Only one buffer can be active at a time)
  glBufferData(GL_ARRAY_BUFFER, totalNumVerts * 8 * sizeof(float), modelData, GL_STATIC_DRAW); //upload vertices to vbo
  //GL_STATIC_DRAW means we won't change the geometry, GL_DYNAMIC_DRAW = geometry changes infrequently
  //GL_STREAM_DRAW = geom. changes frequently.  This effects which types of GPU memory is used

  int texturedShader = InitShader("textured-Vertex.glsl", "textured-Fragment.glsl");

  //Tell OpenGL how to set fragment shader input
  GLint posAttrib = glGetAttribLocation(texturedShader, "position");
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
  //Attribute, vals/attrib., type, isNormalized, stride, offset
  glEnableVertexAttribArray(posAttrib);

  //GLint colAttrib = glGetAttribLocation(phongShader, "inColor");
  //glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
  //glEnableVertexAttribArray(colAttrib);

  GLint normAttrib = glGetAttribLocation(texturedShader, "inNormal");
  glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void * )(5 * sizeof(float)));
  glEnableVertexAttribArray(normAttrib);

  GLint texAttrib = glGetAttribLocation(texturedShader, "inTexcoord");
  glEnableVertexAttribArray(texAttrib);
  glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void * )(3 * sizeof(float)));

  GLint uniView = glGetUniformLocation(texturedShader, "view");
  GLint uniProj = glGetUniformLocation(texturedShader, "proj");

  glBindVertexArray(0); //Unbind the VAO in case we want to create a new one

  glEnable(GL_DEPTH_TEST);

  printf("%s\n", INSTRUCTIONS);

  float dc, dr;
  bool recenterMouse = false;
  int lastx = screenWidth - 50, lasty = screenHeight - 50;

  //Event Loop (Loop forever processing each event as fast as possible)
  SDL_Event windowEvent;
  bool quit = false;
  while (!quit) {
    dt = timePast - time1;
    dc = dt * 4.0;
    dr = dt * 1.0;

    while (SDL_PollEvent( &windowEvent)) { //inspect all events in the queue
      if (windowEvent.type == SDL_QUIT) quit = true;
      //List of keycodes: https://wiki.libsdl.org/SDL_Keycode - You can catch many special keys
      //Scancode referes to a keyboard position, keycode referes to the letter (e.g., EU keyboards)
      if (windowEvent.type == SDL_KEYUP && (windowEvent.key.keysym.sym == SDLK_ESCAPE || windowEvent.key.keysym.sym == SDLK_q))
        quit = true; //Exit event loop
      if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_f) { //If "f" is pressed
        fullscreen = !fullscreen;
        SDL_SetWindowFullscreen(window, fullscreen ? SDL_WINDOW_FULLSCREEN : 0); //Toggle fullscreen
      }

      //     We can use the ".mod" flag to see if modifiers such as shift are pressed
      if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_UP) { //If "up key" is pressed
        if (windowEvent.key.keysym.mod &KMOD_SHIFT) chaircamheight += .1; //Is shift pressed?
        else {

          if (chairspeed >= 0) {
            chairspeed += acc;
          } else {
            chairspeed += dcc;
          }
        }
      }
      if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_DOWN) { //If "down key" is pressed
        if (windowEvent.key.keysym.mod &KMOD_SHIFT) chaircamheight -= .1; //Is shift pressed?
        else {

          if (chairspeed < 0) {
            chairspeed -= acc;
          } else {
            chairspeed -= dcc;
          }

        }
      }

      if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_LEFT) {
        if (windowEvent.key.keysym.mod &KMOD_SHIFT) {
          balldiam -= .05;
          ballradius = balldiam * .5;
          ballpos.z = ballradius;
        } else {
          if (chairaspeed >= 0) {
            chairaspeed += aacc;
          } else {
            chairaspeed += adcc;
          }
        }
      }
      if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_RIGHT) {
        if (windowEvent.key.keysym.mod &KMOD_SHIFT) {
          balldiam += .05;
          ballradius = balldiam * .5;
          ballpos.z = ballradius;
        } else {
          if (chairaspeed < 0) {
            chairaspeed -= aacc;
          } else {
            chairaspeed -= adcc;
          }
        }
      }

      if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_w) { //If "w" is pressed
        if (godView) {
          cam.y += dc * 2.;
        } else {
          headAngleUp -= dc;
        }
      }

      if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_s) { //If "s" is pressed
        if (godView) {
          cam.y -= dc * 2.;
        } else {
          headAngleUp += dc;
        }
      }

      if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_a) { //If "a" is pressed
        if (godView) {
          cam.x -= dc * 2.;
        } else {
          if (headAngleLeft + dc <= M_PI / 2.0) {
            headAngleLeft += dc;
          }
        }
      }

      if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_d) { //If "d" is pressed
        if (godView) {
          cam.x += dc * 2.;
        } else {
          if (headAngleLeft - dc >= -M_PI / 2.0) {
            headAngleLeft -= dc;
          }
        }
      }

      if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_c) { //If "c" is pressed
        colR = rand01();
        colG = rand01();
        colB = rand01();
      }
      if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_i)
        cam.z -= .1;
      if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_o)
        cam.z += .1;

      if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_PERIOD) {
        ballv.x = 0;
        ballv.y = 0;
      }

      if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_k) {
        if (goalpostp.y - goalpostm.y > goalpostdiam)
          goalpostm.y += .1;
        goalpostp.y -= .1;
      }

      if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_l) {
        goalpostm.y -= .1;
        goalpostp.y += .1;
      }

      if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_m) {
        mouseMode = !mouseMode;
      }

      if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_h) {
        follow = !follow;
        if (!follow) {
          up.x = 0.0;
          up.y = 1.0;
          up.z = 0.0;
          cam.x = 0;
          cam.y = 0;
        }
      }

      if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_g) {
        if (godView) {
          lastcampos = cam;
          up.x = 0.0;
          up.y = 0.0;
          up.z = 1.0;
        } else {
          cam = lastcampos;
          up.x = 0.0;
          up.y = 1.0;
          up.z = 0.0;
        }
        godView = !godView;
      }

      if (windowEvent.type == SDL_MOUSEMOTION && mouseMode) {
        deltaX = windowEvent.motion.x - lastx;
        deltaY = windowEvent.motion.y - lasty;

        if (deltaX < 0) {
          if (chairaspeed >= 0) {
            chairaspeed += aacc;
          } else {
            chairaspeed += adcc;
          }
        } else if (deltaX > 0) {
          if (chairaspeed < 0) {
            chairaspeed -= aacc;
          } else {
            chairaspeed -= adcc;
          }
        }

        if (deltaY < 0) {
          if (chairspeed >= 0) {
            chairspeed += acc;
          } else {
            chairspeed += dcc;
          }
        } else if (deltaY > 0) {

          if (chairspeed < 0) {
            chairspeed -= acc;
          } else {
            chairspeed -= dcc;
          }

        }

      }
    } //     windowEvent

    if (chairspeed >= 0) {
      chairspeed = fmin(4, chairspeed);
    } else {
      chairspeed = fmax(-4, chairspeed);
    }

    if (chairaspeed >= 0) {
      chairaspeed = fmin(M_PI, chairaspeed);
    } else {
      chairaspeed = fmax(-M_PI, chairaspeed);
    }

    if (mouseMode)
      SDL_WarpMouseInWindow(window, lastx, lasty);

    chairaspeed *= drag;
    angle += chairaspeed * dt;

    if (timePast - time2 > .2) { // every .3 sec slow ball down
      ballv = ballv * balldrag;
      time2 = timePast;
    }

    dir.x = cos(angle);
    dir.y = sin(angle);
    dir.z = 0.0;

    chairspeed *= drag;
    chairv = dir * chairspeed;
    ballpos = ballpos + ballv * dt;

    poschair = poschair + chairv * dt;
    physics(dt);

    // Clear the screen to default color
    glClearColor(.5f, 0.73f, 0.98f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(texturedShader);

    time1 = timePast;
    timePast = SDL_GetTicks() / 1000.f;

    if (godView) {
      if (follow) {
        up.x = dir.x;
        up.y = dir.y;
        up.z = 0.0;
        cam.x = poschair.x;
        cam.y = poschair.y;
      }
      lookAtpos = cam + godcamdir;

    } else { //  chair view
      cam = poschair;
      cam.z = chaircamheight;
      lookAtpos.x = poschair.x + cos(angle + headAngleLeft);
      lookAtpos.y = poschair.y + sin(angle + headAngleLeft);
      lookAtpos.z = chaircamheight - sin(headAngleUp);
    }

    glm::mat4 view = glm::lookAt(
      glm::vec3(cam.x, cam.y, cam.z), //Cam Position
      glm::vec3(lookAtpos.x, lookAtpos.y, lookAtpos.z), //Look at point
      glm::vec3(up.x, up.y, up.z)); //Up
    glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 proj = glm::perspective(3.14f / 4, screenWidth / (float) screenHeight, .501f, 40.0f); //FOV, aspect, near, far
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex0);
    glUniform1i(glGetUniformLocation(texturedShader, "tex0"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex1);
    glUniform1i(glGetUniformLocation(texturedShader, "tex1"), 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, tex2);
    glUniform1i(glGetUniformLocation(texturedShader, "tex2"), 2);

    glBindVertexArray(vao);
    drawCourt(texturedShader, startVertCube, numVertsCube, startVertFloor, numVertsFloor);

    drawObjects(texturedShader, startVertChair, numVertsChair, startVertSphere, numVertsSphere);

    // drawGeometry(texturedShader, startVertChair, numVertsChair, startVertSphere, numVertsSphere, startVertCube, numVertsCube, startVertFloor, numVertsFloor);

    SDL_GL_SwapWindow(window); //Double buffering
  }

  //Clean Up
  glDeleteProgram(texturedShader);
  glDeleteBuffers(1, vbo);
  glDeleteVertexArrays(1, &vao);

  SDL_GL_DeleteContext(context);
  SDL_Quit();
  return 0;
}

void drawGeometry(int shaderProgram, int model1_start, int model1_numVerts, int model2_start, int model2_numVerts, int model3_start, int model3_numVerts, int model4_start, int model4_numVerts) {

  GLint uniColor = glGetUniformLocation(shaderProgram, "inColor");
  glm::vec3 colVec(colR, colG, colB);
  glUniform3fv(uniColor, 1, glm::value_ptr(colVec));

  GLint uniTexID = glGetUniformLocation(shaderProgram, "texID");

  //************
  //Draw model #1 the first time
  //This model is stored in the VBO starting a offest model1_start and with model1_numVerts num of verticies
  //*************

  //Rotate model (matrix) based on how much time has past
  glm::mat4 model;
  model = glm::translate(model, glm::vec3(2, 1, 4));
  model = glm::rotate(model, timePast * 3.14f / 2, glm::vec3(0.0f, 1.0f, 1.0f));
  model = glm::rotate(model, timePast * 3.14f / 4, glm::vec3(1.0f, 0.0f, 0.0f));

  //model = glm::scale(model,glm::vec3(.2f,.2f,.2f)); //An example of scale
  GLint uniModel = glGetUniformLocation(shaderProgram, "model");
  glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model)); //pass model matrix to shader

  //Set which texture to use (-1 = no texture)
  glUniform1i(uniTexID, -1);

  //Draw an instance of the model (at the position &orientation specified by the model matrix above)
  glDrawArrays(GL_TRIANGLES, model1_start, model1_numVerts); //(Primitive Type, Start Vertex, Num Verticies)

  //************
  //Draw model #1 the second time
  //This model is stored in the VBO starting a offest model1_start and with model1_numVerts num. of verticies
  //*************

  //Translate the model (matrix) left and back
  model = glm::mat4(); //Load intentity
  model = glm::translate(model, glm::vec3(-2, -1, -.4));
  //model = glm::scale(model,2.f*glm::vec3(1.f,1.f,0.5f)); //scale example
  glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

  //Set which texture to use (0 = wood texture ... bound to GL_TEXTURE0)
  glUniform1i(uniTexID, 0);

  //Draw an instance of the model (at the position &orientation specified by the model matrix above)
  glDrawArrays(GL_TRIANGLES, model1_start, model1_numVerts); //(Primitive Type, Start Vertex, Num Verticies)

  //************
  //Draw model #2 once
  //This model is stored in the VBO starting a offest model2_start and with model2_numVerts num of verticies
  //*************

  //Translate the model (matrix) based on where objx/y/z is
  // ... these variables are set when the user presses the arrow keys
  model = glm::mat4();
  model = glm::scale(model, glm::vec3(.8f, .8f, .8f)); //scale this model
  model = glm::translate(model, glm::vec3(1, 3, .8));

  //Set which texture to use (1 = brick texture ... bound to GL_TEXTURE1)
  glUniform1i(uniTexID, 1);
  glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

  //Draw an instance of the model (at the position &orientation specified by the model matrix above)
  glDrawArrays(GL_TRIANGLES, model2_start, model2_numVerts); //(Primitive Type, Start Vertex, Num Verticies)

  // model 3
  model = glm::mat4();
  // model = glm::scale(model,glm::vec3(.8f,.8f,.8f)); //scale this model
  model = glm::translate(model, glm::vec3(0, 0, 1.2));

  //Set which texture to use (1 = brick texture ... bound to GL_TEXTURE1)
  glUniform1i(uniTexID, 1);
  glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

  //Draw an instance of the model (at the position &orientation specified by the model matrix above)
  glDrawArrays(GL_TRIANGLES, model3_start, model3_numVerts); //(Primitive Type, Start Vertex, Num Verticies)

  // model 4
  model = glm::mat4();
  // model = glm::scale(model,glm::vec3(.8f,.8f,.8f)); //scale this model
  // model = glm::translate(model,glm::vec3(0,0,0));

  //Set which texture to use (1 = brick texture ... bound to GL_TEXTURE1)
  glUniform1i(uniTexID, 1);
  glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

  //Draw an instance of the model (at the position &orientation specified by the model matrix above)
  glDrawArrays(GL_TRIANGLES, model4_start, model4_numVerts); //(Primitive Type, Start Vertex, Num Verticies)
}

void drawCourt(int shaderProgram, int model1_start, int model1_numVerts, int model2_start, int model2_numVerts) {
  //  1 cube     2 floor

  GLint uniColor = glGetUniformLocation(shaderProgram, "inColor");
  glm::vec3 colVec(.34, .24, 0);
  glUniform3fv(uniColor, 1, glm::value_ptr(colVec));

  GLint uniTexID = glGetUniformLocation(shaderProgram, "texID");
  GLint uniModel = glGetUniformLocation(shaderProgram, "model");

  glm::mat4 model;

  model = glm::mat4();
  model = glm::scale(model, glm::vec3(2 * courtw * courtaspectratio, 2 * courtw, 1)); //scale this model
  // model = glm::translate(model,glm::vec3((float)x+.5,(float)y+.5,0));

  //Set which texture to use (2 = court texture ... bound to GL_TEXTURE1)
  glUniform1i(uniTexID, 2);
  glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

  //Draw an instance of the model (at the position &orientation specified by the model matrix above)
  glDrawArrays(GL_TRIANGLES, model2_start, model2_numVerts); //(Primitive Type, Start Vertex, Num Verticies)

  model = glm::mat4();
  model = glm::scale(model, glm::vec3(2.5 * courtw * courtaspectratio, 2.5 * courtw, 1)); //scale this model
  model = glm::translate(model, glm::vec3(0, 0, -.01));

  glUniform1i(uniTexID, -1);
  glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

  //Draw an instance of the model (at the position &orientation specified by the model matrix above)
  glDrawArrays(GL_TRIANGLES, model2_start, model2_numVerts); //(Primitive Type, Start Vertex, Num Verticies)

  glm::vec3 colVec2(0, 0, 0);
  glUniform3fv(uniColor, 1, glm::value_ptr(colVec2));

  float z = 3;
  model = glm::mat4();
  model = glm::translate(model, glm::vec3(goalpostm.x, goalpostm.y, z * .5));
  model = glm::scale(model, glm::vec3(goalpostdiam, goalpostdiam, z)); //scale this model

  glUniform1i(uniTexID, -1);
  glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

  //Draw an instance of the model (at the position &orientation specified by the model matrix above)
  glDrawArrays(GL_TRIANGLES, model1_start, model1_numVerts); //(Primitive Type, Start Vertex, Num Verticies)

  model = glm::mat4();
  model = glm::translate(model, glm::vec3(goalpostp.x, goalpostp.y, z * .5));
  model = glm::scale(model, glm::vec3(goalpostdiam, goalpostdiam, z)); //scale this model

  glUniform1i(uniTexID, -1);
  glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

  //Draw an instance of the model (at the position &orientation specified by the model matrix above)
  glDrawArrays(GL_TRIANGLES, model1_start, model1_numVerts); //(Primitive Type, Start Vertex, Num Verticies)

}

void drawObjects(int shaderProgram, int model1_start, int model1_numVerts, int model2_start, int model2_numVerts) {
  //  1 chair     2 ball

  GLint uniColor = glGetUniformLocation(shaderProgram, "inColor");
  glm::vec3 colVec(colR, colG, colB);
  glUniform3fv(uniColor, 1, glm::value_ptr(colVec));

  GLint uniTexID = glGetUniformLocation(shaderProgram, "texID");
  GLint uniModel = glGetUniformLocation(shaderProgram, "model");

  glm::mat4 model;

  model = glm::mat4();
  model = glm::translate(model, glm::vec3(ballpos.x, ballpos.y, ballpos.z));
  model = glm::scale(model, glm::vec3(balldiam, balldiam, balldiam)); //scale this model

  glUniform1i(uniTexID, -1);
  glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

  //Draw an instance of the model (at the position &orientation specified by the model matrix above)
  glDrawArrays(GL_TRIANGLES, model2_start, model2_numVerts); //(Primitive Type, Start Vertex, Num Verticies)

  glm::vec3 colVec2(.5, .5, .5);
  glUniform3fv(uniColor, 1, glm::value_ptr(colVec2));

  float rt90 = M_PI * .5;
  // float pi=M_PI;

  model = glm::mat4();
  // model = glm::translate(model,glm::vec3(3,0,0.4));
  // model = glm::translate(model,glm::vec3(poschair.x,poschair.y,0.4));
  // model = glm::rotate(model,angle,glm::vec3(0.0f, 0.0f, 1.0f));
  // model = glm::scale(model,glm::vec3(1,1.2,1)); //scale this model
  // model = glm::rotate(model,rt90,glm::vec3(1.0f, 0.0f, 0.0f));

  glUniform1i(uniTexID, -1);
  glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

  //Draw an instance of the model (at the position &orientation specified by the model matrix above)
  glDrawArrays(GL_TRIANGLES, model1_start, model1_numVerts); //(Primitive Type, Start Vertex, Num Verticies)

}

void physics(float dt) {

  // check ball hit goal

  if (ballpos.x <= goalpostm.x + ballradius && ballpos.y + ballradius < goalpostp.y - goalpostr && ballpos.y - ballradius > goalpostm.y + goalpostr) {
    ballv.x = 0;
    ballv.y = 0;
    ballpos.x = 0;
    ballpos.y = 0;
  }

  // check ball hit walls.

  if (ballpos.x >= courtXmax - ballradius) {
    Vector n(-1, 0, 0);
    ballpos.x = courtXmax - ballradius;
    Vector r = ballv.reflect(ballv, n);
    r = r * damp;
    ballv = r;
  }

  if (ballpos.x <= courtXmin + ballradius) {
    Vector n(1, 0, 0);
    ballpos.x = courtXmin + ballradius;
    Vector r = ballv.reflect(ballv, n);
    r = r * damp;
    ballv = r;
  }

  if (ballpos.y >= courtYmax - ballradius) {
    Vector n(0, -1, 0);
    ballpos.y = courtYmax - ballradius;
    Vector r = ballv.reflect(ballv, n);
    r = r * damp;
    ballv = r;
  }

  if (ballpos.y <= courtYmin + ballradius) {
    Vector n(0, 1, 0);
    ballpos.y = courtYmin + ballradius;
    Vector r = ballv.reflect(ballv, n);
    r = r * damp;
    ballv = r;
  }

  //  rotate ball pos &velocity to chair axis x&y

  bool convert = false;

  Point tempball;
  float bx = ballpos.x - poschair.x;
  float by = ballpos.y - poschair.y;

  Vector tempballv, tempchairv;

  tempball.x = cos(-angle) * bx - sin(-angle) * by;
  tempball.y = sin(-angle) * bx + cos(-angle) * by;

  tempballv.x = cos(-angle) * ballv.x - sin(-angle) * ballv.y;
  tempballv.y = sin(-angle) * ballv.x + cos(-angle) * ballv.y;

  tempchairv.x = cos(-angle) * chairv.x - sin(-angle) * chairv.y;
  tempchairv.y = sin(-angle) * chairv.x + cos(-angle) * chairv.y;

  if (tempball.x - ballradius < chairXmax && tempball.x > chairXmax - .5 && tempball.y <= chairYmax && tempball.y >= chairYmin) { //   chair front
    Vector n(1, 0, 0);
    tempball.x = ballradius + chairXmax;
    Vector relative = tempballv - tempchairv;
    Vector r = relative.reflect(relative, n);
    tempballv = r + tempchairv;
    convert = true;
  }

  if (tempball.x + ballradius > chairXmin && tempball.x < chairXmin + .5 && tempball.y <= chairYmax && tempball.y >= chairYmin) { //  chair back
    Vector n(-1, 0, 0);
    tempball.x = chairXmin - ballradius;
    Vector relative = tempballv - tempchairv;
    Vector r = relative.reflect(relative, n);
    tempballv = r + tempchairv;
    convert = true;
  }

  /*  tangent velocity=(arc len)/(dt)
      arc len=r*(change in angle)=r*(chairaspeed*dt)
      tangent velocity=r*chairaspeed
  */
  if (tempball.y < chairYmax + ballradius && tempball.y > chairYmax - .5 && tempball.x >= chairXmin && tempball.x <= chairXmax) { //   chair left
    Vector n(0, 1, 0);

    tempball.y = chairYmax + ballradius;

    float r = sqrt(tempball.x * tempball.x + tempball.y * tempball.y);
    Vector tanvelocity = n * r * fabs(chairaspeed);

    Vector relative = tempballv - tanvelocity;
    Vector reflect = relative.reflect(relative, n);
    tempballv = reflect + tanvelocity;

    convert = true;
  }

  if (tempball.y > chairYmin - ballradius && tempball.y < chairYmin + .5 && tempball.x >= chairXmin && tempball.x <= chairXmax) { //   chair right
    Vector n(0, -1, 0);

    tempball.y = chairYmin - ballradius;

    float r = sqrt(tempball.x * tempball.x + tempball.y * tempball.y);
    Vector tanvelocity = n * r * fabs(chairaspeed);

    Vector relative = tempballv - tanvelocity;
    Vector reflect = relative.reflect(relative, n);
    tempballv = reflect + tanvelocity;

    convert = true;
  }

  //  put ball pos &v back to court coordinates

  if (convert) {
    bx = tempball.x;
    by = tempball.y;

    tempball.x = cos(angle) * bx - sin(angle) * by;
    tempball.y = sin(angle) * bx + cos(angle) * by;

    ballv.x = cos(angle) * tempballv.x - sin(angle) * tempballv.y;
    ballv.y = sin(angle) * tempballv.x + cos(angle) * tempballv.y;

    ballpos.x = poschair.x + tempball.x;
    ballpos.y = poschair.y + tempball.y;

  }

}

// Create a NULL-terminated string by reading the provided file
static char * readShaderSource(const char * shaderFile) {
  FILE * fp;
  long length;
  char * buffer;

  // open the file containing the text of the shader code
  fp = fopen(shaderFile, "r");

  // check for errors in opening the file
  if (fp == NULL) {
    printf("can't open shader source file %s\n", shaderFile);
    return NULL;
  }

  // determine the file size
  fseek(fp, 0, SEEK_END); // move position indicator to the end of the file;
  length = ftell(fp); // return the value of the current position

  // allocate a buffer with the indicated number of bytes, plus one
  buffer = new char[length + 1];

  // read the appropriate number of bytes from the file
  fseek(fp, 0, SEEK_SET); // move position indicator to the start of the file
  fread(buffer, 1, length, fp); // read all of the bytes

  // append a NULL character to indicate the end of the string
  buffer[length] = '\0';

  // close the file
  fclose(fp);

  // return the string
  return buffer;
}

// Create a GLSL program object from vertex and fragment shader files
GLuint InitShader(const char * vShaderFileName,
  const char * fShaderFileName) {
  GLuint vertex_shader, fragment_shader;
  GLchar * vs_text, * fs_text;
  GLuint program;

  // check GLSL version
  printf("GLSL version: %s\n\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

  // Create shader handlers
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

  // Read source code from shader files
  vs_text = readShaderSource(vShaderFileName);
  fs_text = readShaderSource(fShaderFileName);

  // error check
  if (vs_text == NULL) {
    printf("Failed to read from vertex shader file %s\n", vShaderFileName);
    exit(1);
  } else if (DEBUG_ON) {
    printf("Vertex Shader:\n=====================\n");
    printf("%s\n", vs_text);
    printf("=====================\n\n");
  }
  if (fs_text == NULL) {
    printf("Failed to read from fragent shader file %s\n", fShaderFileName);
    exit(1);
  } else if (DEBUG_ON) {
    printf("\nFragment Shader:\n=====================\n");
    printf("%s\n", fs_text);
    printf("=====================\n\n");
  }

  // Load Vertex Shader
  const char * vv = vs_text;
  glShaderSource(vertex_shader, 1, &vv, NULL); //Read source
  glCompileShader(vertex_shader); // Compile shaders

  // Check for errors
  GLint compiled;
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compiled);
  if (!compiled) {
    printf("Vertex shader failed to compile:\n");
    if (DEBUG_ON) {
      GLint logMaxSize, logLength;
      glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &logMaxSize);
      printf("printing error message of %d bytes\n", logMaxSize);
      char * logMsg = new char[logMaxSize];
      glGetShaderInfoLog(vertex_shader, logMaxSize, &logLength, logMsg);
      printf("%d bytes retrieved\n", logLength);
      printf("error message: %s\n", logMsg);
      delete[] logMsg;
    }
    exit(1);
  }

  // Load Fragment Shader
  const char * ff = fs_text;
  glShaderSource(fragment_shader, 1, &ff, NULL);
  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compiled);

  //Check for Errors
  if (!compiled) {
    printf("Fragment shader failed to compile\n");
    if (DEBUG_ON) {
      GLint logMaxSize, logLength;
      glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &logMaxSize);
      printf("printing error message of %d bytes\n", logMaxSize);
      char * logMsg = new char[logMaxSize];
      glGetShaderInfoLog(fragment_shader, logMaxSize, &logLength, logMsg);
      printf("%d bytes retrieved\n", logLength);
      printf("error message: %s\n", logMsg);
      delete[] logMsg;
    }
    exit(1);
  }

  // Create the program
  program = glCreateProgram();

  // Attach shaders to program
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);

  // Link and set program to use
  glLinkProgram(program);

  return program;
}
