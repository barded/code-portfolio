

const char* INSTRUCTIONS =
"***************\n"
"Game Instructions:\n"
"\n"
"Up/down/left/right - Move.\n"
"Complete the level by obtaining the golden piece.\n"
"n - Go to next level.\n"
"r - Reset the current level.\n"
"j - Jump.\n"
"To unlock doors, find the corresponding key.\n"
"q - Quit.\n"
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

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <vector>
using namespace std;

int screenWidth = 800;
int screenHeight = 600;
float timePast = 0;
float time1 = 0, dt;

struct barrier{
  float x;
  float y;
  float z;
  bool isDown;
  bool inTrans;
  int texture;
  char id;
};

struct object {
  float x;
  float y;
  float z;
  bool inHand;
  bool used;
  bool pickedUp;
  int texture;
  char id;
};

//SJG: Store the object coordinates
//You should have a representation for the state of each object
float objx = 0, objy = 0, objz = .25f;
float colR = 1, colG = 1, colB = 1;
float startX, startY, startZ = .5;
float lookAtX, lookAtY, lookAtZ, posX = 3.f, posY = 0.f, posZ = .5f, dirX = -1.f, dirY = 0.f, dirZ = 0.f;
float angle = M_PI / 4;
float playerRadius = .2, pickupRadius = .22, doorRadius = .5;

bool isJumping = false;
float gravity = -2, velocity = 0;
float jumpStart = 0, jumpEnd = 0;

int lvlW, lvlH, levelcount = 0, numLevels;
vector < char > lvl;
vector < barrier > walls;
int wallcount = 0;

vector < barrier > doors;
int doorcount = 0;

vector < object > keys;
int keycount = 0;

object lvlend;

bool DEBUG_ON = false;
GLuint InitShader(const char * vShaderFileName, const char * fShaderFileName);
bool fullscreen = false;
void Win2PPM(int width, int height);

//srand(time(NULL));
float rand01() {
  return rand() / (float) RAND_MAX;
}

int g(int x, int y) {
  return lvlW * y + x;
}

void drawGeometry(int shaderProgram, int model1_start, int model1_numVerts,
  int model2_start, int model2_numVerts, int model3_start, int model3_numVerts,
  int model4_start, int model4_numVerts);

int loadLevel(int num);

void drawLevel(int shaderProgram, int model1_start, int model1_numVerts,
  int model2_start, int model2_numVerts);

void drawObjects(int shaderProgram, int model1_start, int model1_numVerts,
  int model2_start, int model2_numVerts);

bool interact(float &xp, float &yp, float dc);
bool doorDetect(float &xp, float &yp);

int main(int argc, char * argv[]) {

  SDL_Init(SDL_INIT_VIDEO); //Initialize Graphics (for OpenGL)

  //Ask SDL to get a recent version of OpenGL (3.2 or greater)
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

  //Create a window (offsetx, offsety, width, height, flags)
  SDL_Window * window = SDL_CreateWindow("Dee's Maze Game", 500, 100, screenWidth, screenHeight, SDL_WINDOW_OPENGL);

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
  modelFile.open("models/teapot.txt");
  int numLines = 0;
  modelFile >> numLines;
  float * model1 = new float[numLines];
  for (int i = 0; i < numLines; i++) {
    modelFile >> model1[i];
  }
  // printf("%d\n",numLines);
  int numVertsTeapot = numLines / 8;
  modelFile.close();

  //Load Model 2
  modelFile.open("models/knot.txt");
  numLines = 0;
  modelFile >> numLines;
  float * model2 = new float[numLines];
  for (int i = 0; i < numLines; i++) {
    modelFile >> model2[i];
  }
  // printf("%d\n",numLines);
  int numVertsKnot = numLines / 8;
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

  //SJG: I load each model in a different array, then concatenate everything in one big array
  // This structure works, but there is room for improvement here. Eg., you should store the start
  // and end of each model a data structure or array somewhere.
  //Concatenate model arrays
  float * modelData = new float[(numVertsTeapot + numVertsKnot + numVertsCube + numVertsFloor) * 8];
  copy(model1, model1 + numVertsTeapot * 8, modelData);
  copy(model2, model2 + numVertsKnot * 8, modelData + numVertsTeapot * 8);
  copy(model3, model3 + numVertsCube * 8, modelData + numVertsTeapot * 8 + numVertsKnot * 8);
  copy(model4, model4 + numVertsFloor * 8, modelData + numVertsTeapot * 8 + numVertsKnot * 8 + numVertsCube * 8);

  int totalNumVerts = numVertsTeapot + numVertsKnot + numVertsCube + numVertsFloor;
  int startVertTeapot = 0; //The teapot is the first model in the VBO
  int startVertKnot = numVertsTeapot; //The knot starts right after the taepot
  int startVertCube = numVertsTeapot + numVertsKnot; //The cube starts right after the knot
  int startVertFloor = numVertsTeapot + numVertsKnot + numVertsCube; //The floor starts right after the cube

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
  SDL_Surface * surface2 = SDL_LoadBMP("gold.bmp");
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

  //// Allocate Texture 3 (blue) ///////
  SDL_Surface * surface3 = SDL_LoadBMP("blue.bmp");
  if (surface3 == NULL) { //If it failed, print the error
    printf("Error: \"%s\"\n", SDL_GetError());
    return 1;
  }
  GLuint tex3;
  glGenTextures(1, &tex3);

  //Load the texture into memory
  glActiveTexture(GL_TEXTURE3);

  glBindTexture(GL_TEXTURE_2D, tex3);
  //What to do outside 0-1 range
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //How to filter
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface3 - > w, surface3 - > h, 0, GL_BGR, GL_UNSIGNED_BYTE, surface3 - > pixels);
  glGenerateMipmap(GL_TEXTURE_2D); //Mip maps the texture

  SDL_FreeSurface(surface3);
  //// End Allocate Texture ///////

  //// Allocate Texture 4 (cracks) ///////
  SDL_Surface * surface4 = SDL_LoadBMP("cracks.bmp");
  if (surface4 == NULL) { //If it failed, print the error
    printf("Error: \"%s\"\n", SDL_GetError());
    return 1;
  }
  GLuint tex4;
  glGenTextures(1, &tex4);

  //Load the texture into memory
  glActiveTexture(GL_TEXTURE4);

  glBindTexture(GL_TEXTURE_2D, tex4);
  //What to do outside 0-1 range
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //How to filter
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface4 - > w, surface4 - > h, 0, GL_BGR, GL_UNSIGNED_BYTE, surface4 - > pixels);
  glGenerateMipmap(GL_TEXTURE_2D); //Mip maps the texture

  SDL_FreeSurface(surface4);
  //// End Allocate Texture ///////

  //// Allocate Texture 5 (fabric) ///////
  SDL_Surface * surface5 = SDL_LoadBMP("fabric.bmp");
  if (surface5 == NULL) { //If it failed, print the error
    printf("Error: \"%s\"\n", SDL_GetError());
    return 1;
  }
  GLuint tex5;
  glGenTextures(1, &tex5);

  //Load the texture into memory
  glActiveTexture(GL_TEXTURE5);

  glBindTexture(GL_TEXTURE_2D, tex5);
  //What to do outside 0-1 range
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //How to filter
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface5 - > w, surface5 - > h, 0, GL_BGR, GL_UNSIGNED_BYTE, surface5 - > pixels);
  glGenerateMipmap(GL_TEXTURE_2D); //Mip maps the texture

  SDL_FreeSurface(surface5);
  //// End Allocate Texture ///////

  //// Allocate Texture 6 (red) ///////
  SDL_Surface * surface6 = SDL_LoadBMP("red.bmp");
  if (surface6 == NULL) { //If it failed, print the error
    printf("Error: \"%s\"\n", SDL_GetError());
    return 1;
  }
  GLuint tex6;
  glGenTextures(1, &tex6);

  //Load the texture into memory
  glActiveTexture(GL_TEXTURE6);

  glBindTexture(GL_TEXTURE_2D, tex6);
  //What to do outside 0-1 range
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //How to filter
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface6 - > w, surface6 - > h, 0, GL_BGR, GL_UNSIGNED_BYTE, surface6 - > pixels);
  glGenerateMipmap(GL_TEXTURE_2D); //Mip maps the texture

  SDL_FreeSurface(surface6);
  //// End Allocate Texture ///////

  //// Allocate Texture 7 (striped) ///////
  SDL_Surface * surface7 = SDL_LoadBMP("striped.bmp");
  if (surface7 == NULL) { //If it failed, print the error
    printf("Error: \"%s\"\n", SDL_GetError());
    return 1;
  }
  GLuint tex7;
  glGenTextures(1, &tex7);

  //Load the texture into memory
  glActiveTexture(GL_TEXTURE7);

  glBindTexture(GL_TEXTURE_2D, tex7);
  //What to do outside 0-1 range
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //How to filter
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface7 - > w, surface7 - > h, 0, GL_BGR, GL_UNSIGNED_BYTE, surface7 - > pixels);
  glGenerateMipmap(GL_TEXTURE_2D); //Mip maps the texture

  SDL_FreeSurface(surface7);
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

  ifstream input("levels/numLevels.txt");

  if (input.fail()) {
    cout << "Can't open file numLevels.txt" << endl;
    return 0;
  }

  input >> numLevels >> levelcount;
  printf("Game has %d levels.\n", numLevels);
  input.close();

  int successLvl = loadLevel(levelcount);
  if (!successLvl)
    return 0;

  float dc, dr;
  //Event Loop (Loop forever processing each event as fast as possible)
  SDL_Event windowEvent;
  bool quit = false;
  while (!quit) {
    dt = timePast - time1;
    dc = dt * 1.0;
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

      //SJG: Use key input to change the state of the object
      //     We can use the ".mod" flag to see if modifiers such as shift are pressed
      if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_UP) { //If "up key" is pressed
        if (windowEvent.key.keysym.mod &KMOD_SHIFT) posZ += .1; //Is shift pressed?
        else {
          interact(posX, posY, dc);
        }
      }
      if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_DOWN) { //If "down key" is pressed
        if (windowEvent.key.keysym.mod &KMOD_SHIFT) posZ -= .1; //Is shift pressed?
        else {
          interact(posX, posY, -dc);
        }
      }
      if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_LEFT) { //If "up key" is pressed
        angle += dr;
      }
      if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_RIGHT) { //If "down key" is pressed
        angle -= dr;
      }
      if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_c) { //If "c" is pressed
        colR = rand01();
        colG = rand01();
        colB = rand01();
      }
      if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_n && lvlend.pickedUp) { //If "n" is pressed and end is reached
        levelcount++;
        successLvl = loadLevel(levelcount);
        if (successLvl == 0)
          return 0;
        else if (successLvl == 1) {
          continue;
        }
      }

      if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_r) { //If "r" is pressed
        successLvl = loadLevel(levelcount);
        if (successLvl == 0)
          return 0;
        continue;

      }
      if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_j) { //If "j" is pressed
        if (!isJumping && timePast - jumpEnd > 2 && lvl[g(floor(posX), floor(posY))] != 'A' && lvl[g(floor(posX), floor(posY))] != 'B' && lvl[g(floor(posX), floor(posY))] != 'C' && lvl[g(floor(posX), floor(posY))] != 'D' && lvl[g(floor(posX), floor(posY))] != 'E') {
          isJumping = true;
          posZ = startZ + .02;
          jumpStart = timePast;
        }
      }
    }

    if (isJumping) {
      if (posZ <= startZ) {
        posZ = startZ;
        isJumping = false;
        velocity = 0;
        jumpEnd = timePast;
      } else {
        if (timePast - jumpStart < .4) {
          float up = abs(2. * gravity);
          velocity += up * dt;
          posZ += velocity * dt;
        } else {
          velocity += gravity * dt;
          posZ += velocity * dt;
        }
      }
    }

    dirX = cos(angle);
    dirY = sin(angle);

    // Clear the screen to default color
    glClearColor(.5f, 0.73f, 0.98f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(texturedShader);

    time1 = timePast;
    timePast = SDL_GetTicks() / 1000.f;

    lookAtX = posX + dirX;
    lookAtY = posY + dirY;
    lookAtZ = posZ;

    glm::mat4 view = glm::lookAt(
      glm::vec3(posX, posY, posZ), //Cam Position
      glm::vec3(lookAtX, lookAtY, lookAtZ), //Look at point
      glm::vec3(0.0f, 0.0f, 1.0f)); //Up
    glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 proj = glm::perspective(3.14f / 4, screenWidth / (float) screenHeight, .15f, 15.0f); //FOV, aspect, near, far
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

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, tex3);
    glUniform1i(glGetUniformLocation(texturedShader, "tex3"), 3);

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, tex4);
    glUniform1i(glGetUniformLocation(texturedShader, "tex4"), 4);

    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, tex5);
    glUniform1i(glGetUniformLocation(texturedShader, "tex5"), 5);

    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, tex6);
    glUniform1i(glGetUniformLocation(texturedShader, "tex6"), 6);

    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, tex7);
    glUniform1i(glGetUniformLocation(texturedShader, "tex7"), 7);

    glBindVertexArray(vao);
    drawLevel(texturedShader, startVertCube, numVertsCube, startVertFloor, numVertsFloor);

    drawObjects(texturedShader, startVertTeapot, numVertsTeapot, startVertKnot, numVertsKnot);

    // drawGeometry(texturedShader, startVertTeapot, numVertsTeapot, startVertKnot, numVertsKnot, startVertCube, numVertsCube, startVertFloor, numVertsFloor);

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

void drawGeometry(int shaderProgram, int model1_start, int model1_numVerts,
  int model2_start, int model2_numVerts, int model3_start, int model3_numVerts,
  int model4_start, int model4_numVerts) {
  // 1 teapot     2 knot       3 cube    4 floor

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
  model = glm::translate(model, glm::vec3(1, 3, 0));

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
  glUniform1i(uniTexID, 4);
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

int loadLevel(int num) {

  if (num > numLevels) {
    cout << "No more levels." << '\n';
    return 2;
  }

  string file = "levels/level" + to_string(num) + ".txt";
  string line;

  // open the file containing the scene description
  ifstream input(file);

  // check for errors in opening the file
  if (input.fail()) {
    cout << "Can't open file '" << file << "'" << endl;
    return 0;
  }

  cout << "Level " << num << '\n';

  input >> lvlW >> lvlH;
  lvl.clear();
  lvl.reserve(lvlW * lvlH);
  walls.clear();
  doors.clear();
  keys.clear();
  wallcount = 0;
  doorcount = 0;
  keycount = 0;

  int sx, sy;

  barrier temp;
  temp.isDown = false;
  temp.inTrans = false;

  object tempobj;
  tempobj.used = false;
  tempobj.inHand = false;
  tempobj.pickedUp = false;

  for (int y = lvlH - 1; y >= 0; y--) {
    input >> line;
    for (int x = 0; x < lvlW; x++) {
      char c = line.at(x);
      lvl[g(x, y)] = c;

      if (c == 'W') {
        wallcount++;
        temp.x = x;
        temp.y = y;
        temp.z = 0.5;
        temp.id = c;
        temp.texture = 1; // brick
        walls.push_back(temp);
      } else if (c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e') {
        keycount++;
        tempobj.x = x;
        tempobj.y = y;
        tempobj.z = objz;
        tempobj.texture = (int) c - 97 + 3; // 3-7
        tempobj.id = c;
        keys.push_back(tempobj);
      } else if (c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E') {
        doorcount++;
        temp.x = x;
        temp.y = y;
        temp.z = 0.5;
        temp.id = c;
        temp.texture = c - 65 + 3; //  3-7
        doors.push_back(temp);
      } else if (c == 'G') {
        lvlend.x = x;
        lvlend.y = y;
        lvlend.z = .5;
        lvlend.texture = 2; // gold
        lvlend.inHand = false;
        lvlend.used = false;
        lvlend.pickedUp = false;
      } else if (c == 'S') {
        startX = x + .5;
        startY = y + .5;
        posX = startX;
        posY = startY;
        posZ = startZ;

        sx = x;
        sy = y;
      }
    }
  }

  if (sx - 1 >= 0 && sx - 1 < lvlW && lvl[g(sx - 1, sy)] == '0') {
    angle = M_PI;
  } else if (sx + 1 >= 0 && sx + 1 < lvlW && lvl[g(sx + 1, sy)] == '0') {
    angle = 0;
  } else if (sy - 1 >= 0 && sy - 1 < lvlH && lvl[g(sx, sy - 1)] == '0') {
    angle = -M_PI / 2;
  } else {
    angle = M_PI / 2;
  }

  input.close();
  return 1;
}

void drawLevel(int shaderProgram, int model1_start, int model1_numVerts,
  int model2_start, int model2_numVerts) {
  //  1 cube     2 floor

  GLint uniColor = glGetUniformLocation(shaderProgram, "inColor");
  glm::vec3 colVec(colR, colG, colB);
  glUniform3fv(uniColor, 1, glm::value_ptr(colVec));

  GLint uniTexID = glGetUniformLocation(shaderProgram, "texID");
  GLint uniModel = glGetUniformLocation(shaderProgram, "model");

  glm::mat4 model;
  barrier wall;
  barrier door;

  for (int i = 0; i < walls.size(); i++) {
    wall = walls[i];
    model = glm::mat4();
    // model = glm::scale(model,glm::vec3(.8f,.8f,.8f)); //scale this model
    model = glm::translate(model, glm::vec3(wall.x + .5, wall.y + .5, wall.z));

    //Set which texture to use (1 = brick texture ... bound to GL_TEXTURE1)
    glUniform1i(uniTexID, 1);
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

    //Draw an instance of the model (at the position &orientation specified by the model matrix above)
    glDrawArrays(GL_TRIANGLES, model1_start, model1_numVerts); //(Primitive Type, Start Vertex, Num Verticies)
  }

  for (int i = 0; i < doorcount; i++) {
    door = doors[i];
    if (!door.isDown && !door.inTrans) {
      model = glm::mat4();
      model = glm::translate(model, glm::vec3(door.x + .5, door.y + .5, door.z));

      glUniform1i(uniTexID, door.texture);
      glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

      glDrawArrays(GL_TRIANGLES, model1_start, model1_numVerts);
    } else if (!door.isDown && door.inTrans && door.z < 1.6) {
      door.z += dt / 3;
      if (door.z >= 1.6) {
        door.isDown = true;
        door.inTrans = false;
      }

      doors[i] = door;

      model = glm::mat4();
      model = glm::translate(model, glm::vec3(door.x + .5, door.y + .5, door.z));

      glUniform1i(uniTexID, door.texture);
      glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

      glDrawArrays(GL_TRIANGLES, model1_start, model1_numVerts);
    } else {
      model = glm::mat4();
      model = glm::translate(model, glm::vec3(door.x + .5, door.y + .5, door.z));

      glUniform1i(uniTexID, door.texture);
      glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

      glDrawArrays(GL_TRIANGLES, model1_start, model1_numVerts);
    }

  }

  for (int y = 0; y < lvlH; y++) {
    for (int x = 0; x < lvlW; x++) {
      model = glm::mat4();
      // model = glm::scale(model,glm::vec3(lvlW,lvlH,1)); //scale this model
      model = glm::translate(model, glm::vec3((float) x + .5, (float) y + .5, 0));

      //Set which texture to use (0 = wood texture ... bound to GL_TEXTURE1)
      glUniform1i(uniTexID, 0);
      glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

      //Draw an instance of the model (at the position &orientation specified by the model matrix above)
      glDrawArrays(GL_TRIANGLES, model2_start, model2_numVerts); //(Primitive Type, Start Vertex, Num Verticies)
    }
  }
}

void drawObjects(int shaderProgram, int model1_start, int model1_numVerts, int model2_start, int model2_numVerts) {
  //  1 teapot     2 knot

  GLint uniColor = glGetUniformLocation(shaderProgram, "inColor");
  glm::vec3 colVec(colR, colG, colB);
  glUniform3fv(uniColor, 1, glm::value_ptr(colVec));

  GLint uniTexID = glGetUniformLocation(shaderProgram, "texID");
  GLint uniModel = glGetUniformLocation(shaderProgram, "model");

  glm::mat4 model;
  object key;

  for (int i = 0; i < keycount; i++) {
    key = keys[i];

    if (!key.inHand && !key.pickedUp && !key.used) {
      model = glm::mat4();
      model = glm::translate(model, glm::vec3(key.x + .5, key.y + .5, key.z));
      model = glm::rotate(model, timePast * 3.14f / 8, glm::vec3(0.0f, 0.0f, 1.0f));
      model = glm::scale(model, glm::vec3(.5, .5, .5)); //scale this model

      glUniform1i(uniTexID, key.texture);
      glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

      //Draw an instance of the model (at the position &orientation specified by the model matrix above)
      glDrawArrays(GL_TRIANGLES, model1_start, model1_numVerts); //(Primitive Type, Start Vertex, Num Verticies)
    } else if (key.pickedUp && key.inHand && !key.used && !lvlend.inHand) {
      model = glm::mat4();
      model = glm::translate(model, glm::vec3(posX + dirX / 2, posY + dirY / 2, posZ - .15));
      model = glm::rotate(model, 3.14f / 2 + angle, glm::vec3(0.0f, 0.0f, 1.0f));
      model = glm::scale(model, glm::vec3(.2, .2, .2));

      glUniform1i(uniTexID, key.texture);
      glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

      glDrawArrays(GL_TRIANGLES, model1_start, model1_numVerts);
    } else if (key.pickedUp && !key.inHand && !key.used && !lvlend.inHand) {
      glm::vec3 left = glm::cross(glm::vec3(0, 0, 1), glm::vec3(dirX, dirY, 0));
      model = glm::mat4();
      if (i < 4)
        model = glm::translate(model, glm::vec3(posX + cos(angle + M_PI / 30 + M_PI / 36 * (i + 1)) / 2, posY + sin(angle + M_PI / 30 + M_PI / 36 * (i + 1)) / 2, posZ - .15));
      else
        model = glm::translate(model, glm::vec3(posX + cos(angle + M_PI / 30 * 1.5 + M_PI / 36 * (i + 1 - 4)) / 2, posY + sin(angle + M_PI / 30 * 1.5 + M_PI / 36 * (i + 1 - 4)) / 2, posZ - .18));

      model = glm::rotate(model, 3.14f / 2 + angle, glm::vec3(0.0f, 0.0f, 1.0f));
      model = glm::scale(model, glm::vec3(.05, .05, .05));

      glUniform1i(uniTexID, key.texture);
      glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

      glDrawArrays(GL_TRIANGLES, model1_start, model1_numVerts);
    }

  }

  // end   of maze knot
  if (!lvlend.inHand && !lvlend.pickedUp) {
    model = glm::mat4();
    model = glm::translate(model, glm::vec3(lvlend.x + .5, lvlend.y + .5, lvlend.z));
    model = glm::rotate(model, timePast * 3.14f / 2, glm::vec3(0.0f, 1.0f, 1.0f));
    model = glm::rotate(model, timePast * 3.14f / 4, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(.6, .6, .6)); //scale this model

    glUniform1i(uniTexID, lvlend.texture);
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

    //Draw an instance of the model (at the position &orientation specified by the model matrix above)
    glDrawArrays(GL_TRIANGLES, model2_start, model2_numVerts); //(Primitive Type, Start Vertex, Num Verticies)
  } else if (lvlend.inHand && lvlend.pickedUp) {
    model = glm::mat4();
    model = glm::translate(model, glm::vec3(posX + dirX / 2, posY + dirY / 2, posZ - .15));
    model = glm::rotate(model, timePast * 3.14f / 2, glm::vec3(0.0f, 1.0f, 1.0f));
    model = glm::rotate(model, timePast * 3.14f / 4, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(.2, .2, .2));

    glUniform1i(uniTexID, lvlend.texture);
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

    glDrawArrays(GL_TRIANGLES, model2_start, model2_numVerts);
  }
}

bool interact(float &xp, float &yp, float dc) {
  float newX = xp + dirX * dc;
  float newY = yp + dirY * dc;
  int i, j, k, l, doorX, doorY;
  char c;

  for (int y = -1; y <= 1; y++) {
    for (int x = -1; x <= 1; x++) {
      i = floor(newX + playerRadius * x);
      j = floor(newY + playerRadius * y);
      k = floor(newX + pickupRadius * x);
      l = floor(newY + pickupRadius * y);
      doorX = floor(newX + doorRadius * x);
      doorY = floor(newY + doorRadius * y);

      if (doorX >= 0 && doorY >= 0 && doorX < lvlW && doorY < lvlH) {
        c = lvl[g(doorX, doorY)];

        if (c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E') {
          for (int d = 0; d < doorcount; d++) {
            if (c == doors[d].id && !doors[d].isDown) {
              for (int K = 0; K < keycount; K++) {
                if (!keys[K].used && keys[K].pickedUp && doors[d].texture == keys[K].texture) {
                  doors[d].inTrans = true;
                  keys[K].used = true;
                  break;
                }
              }
            }
          }
        }
      }

      if ((i < 0 || j < 0 || i > lvlW - 1 || j > lvlH - 1) && !DEBUG_ON)
        return false;

      c = lvl[g(i, j)];
      if (c == 'W') {
        return false;
      } else if (c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E') {
        for (int d = 0; d < doorcount; d++) {
          if (c == doors[d].id && !doors[d].isDown)
            return false;
        }
      }

      if (k < 0 || j < 0 || i > lvlW - 1 || j > lvlH - 1) {} else {
        c = lvl[g(k, l)];

        if (c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e') {
          for (int K = 0; K < keycount; K++) {
            if (c == keys[K].id && !keys[K].used && !keys[K].inHand && !keys[K].pickedUp) {
              for (int z = 0; z < keycount; z++) {
                keys[z].inHand = false;
              }
              keys[K].pickedUp = true;
              keys[K].inHand = true;
            }
          }

        }

        if (c == 'G') {
          lvlend.inHand = true;
          lvlend.pickedUp = true;
        }
      }

    }
  }

  xp = newX;
  yp = newY;
  return true;

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
