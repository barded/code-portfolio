// code modified from http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <cstdio>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <vector>
using namespace std;

int main(int argc, char *argv[]) {
  if (argc < 4) {
    std::cout << "Need <infile.obj in triangles> <outfile.txt> and <t> if  .obj has uv coordinates, else <f>. " << '\n';
  }

  FILE * infile, * outfile;
  bool uv;

  std::vector < glm::vec3 > verts;
  std::vector < glm::vec2 > uvs;
  std::vector < glm::vec3 > normals;

  infile = fopen(argv[1], "r");
  outfile = fopen(argv[2], "w+");

  if (infile == NULL || outfile == NULL) {
    std::cout << "can't open file" << '\n';
    return 0;
  }

  verts.push_back(glm::vec3(0, 0, 0));
  normals.push_back(glm::vec3(0, 0, 0));
  uvs.push_back(glm::vec2(0, 0));

  int faces = 0;

  if (strcmp(argv[3], "t") == 0) { // parse texture coordinates?
    uv = true;
  } else uv = false;

  while (true) {
    char str[180];
    int o = fscanf(infile, "%s", str);
    if (o == EOF) {
      break;
    }

    if (strcmp(str, "v") == 0) { // vertex
      glm::vec3 vertex;
      fscanf(infile, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
      verts.push_back(vertex);
    } else if (strcmp(str, "vt") == 0) { // texture
      glm::vec2 uv;
      fscanf(infile, "%f %f\n", &uv.x, &uv.y);
      uvs.push_back(uv);
    } else if (strcmp(str, "vn") == 0) { //  normal
      glm::vec3 normal;
      fscanf(infile, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
      normals.push_back(normal);
    } else if (strcmp(str, "f") == 0 && uv) { // face with uv
      std::string vertex1, vertex2, vertex3;
      unsigned int vIndex[3], uvIndex[3], nIndex[3];
      int num = fscanf(infile, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vIndex[0], &uvIndex[0], &nIndex[0], &vIndex[1], &uvIndex[1], &nIndex[1], &vIndex[2], &uvIndex[2], &nIndex[2]);
      if (num != 9) {
        std::cout << "faces issue uv" << '\n';
        return 0;
      }

      fprintf(outfile, "%f\n", (float) verts[vIndex[0]].x);
      fprintf(outfile, "%f\n", (float) verts[vIndex[0]].y);
      fprintf(outfile, "%f\n", (float) verts[vIndex[0]].z);
      fprintf(outfile, "%f\n", (float) uvs[uvIndex[0]].x);
      fprintf(outfile, "%f\n", (float) uvs[uvIndex[0]].y);
      fprintf(outfile, "%f\n", (float) normals[nIndex[0]].x);
      fprintf(outfile, "%f\n", (float) normals[nIndex[0]].y);
      fprintf(outfile, "%f\n", (float) normals[nIndex[0]].z);

      fprintf(outfile, "%f\n", (float) verts[vIndex[1]].x);
      fprintf(outfile, "%f\n", (float) verts[vIndex[1]].y);
      fprintf(outfile, "%f\n", (float) verts[vIndex[1]].z);
      fprintf(outfile, "%f\n", (float) uvs[uvIndex[1]].x);
      fprintf(outfile, "%f\n", (float) uvs[uvIndex[1]].y);
      fprintf(outfile, "%f\n", (float) normals[nIndex[1]].x);
      fprintf(outfile, "%f\n", (float) normals[nIndex[1]].y);
      fprintf(outfile, "%f\n", (float) normals[nIndex[1]].z);

      fprintf(outfile, "%f\n", (float) verts[vIndex[2]].x);
      fprintf(outfile, "%f\n", (float) verts[vIndex[2]].y);
      fprintf(outfile, "%f\n", (float) verts[vIndex[2]].z);
      fprintf(outfile, "%f\n", (float) uvs[uvIndex[2]].x);
      fprintf(outfile, "%f\n", (float) uvs[uvIndex[2]].y);
      fprintf(outfile, "%f\n", (float) normals[nIndex[2]].x);
      fprintf(outfile, "%f\n", (float) normals[nIndex[2]].y);
      fprintf(outfile, "%f\n", (float) normals[nIndex[2]].z);

      faces++;

    } else if (strcmp(str, "f") == 0) { // face without uv
      std::string vertex1, vertex2, vertex3;
      unsigned int vIndex[3], uvIndex[3], nIndex[3];
      int num = fscanf(infile, "%d//%d %d//%d %d//%d\n", &vIndex[0], &nIndex[0], &vIndex[1], &nIndex[1], &vIndex[2], &nIndex[2]);
      if (num != 6) {
        std::cout << "faces issue" << '\n';
        return 0;
      }
      fprintf(outfile, "%f\n", (float) verts[vIndex[0]].x);
      fprintf(outfile, "%f\n", (float) verts[vIndex[0]].y);
      fprintf(outfile, "%f\n", (float) verts[vIndex[0]].z);
      fprintf(outfile, "%f\n", 0.0);
      fprintf(outfile, "%f\n", 0.0);
      fprintf(outfile, "%f\n", (float) normals[nIndex[0]].x);
      fprintf(outfile, "%f\n", (float) normals[nIndex[0]].y);
      fprintf(outfile, "%f\n", (float) normals[nIndex[0]].z);

      fprintf(outfile, "%f\n", (float) verts[vIndex[1]].x);
      fprintf(outfile, "%f\n", (float) verts[vIndex[1]].y);
      fprintf(outfile, "%f\n", (float) verts[vIndex[1]].z);
      fprintf(outfile, "%f\n", 0.0);
      fprintf(outfile, "%f\n", 0.0);
      fprintf(outfile, "%f\n", (float) normals[nIndex[1]].x);
      fprintf(outfile, "%f\n", (float) normals[nIndex[1]].y);
      fprintf(outfile, "%f\n", (float) normals[nIndex[1]].z);

      fprintf(outfile, "%f\n", (float) verts[vIndex[2]].x);
      fprintf(outfile, "%f\n", (float) verts[vIndex[2]].y);
      fprintf(outfile, "%f\n", (float) verts[vIndex[2]].z);
      fprintf(outfile, "%f\n", 0.0);
      fprintf(outfile, "%f\n", 0.0);
      fprintf(outfile, "%f\n", (float) normals[nIndex[2]].x);
      fprintf(outfile, "%f\n", (float) normals[nIndex[2]].y);
      fprintf(outfile, "%f\n", (float) normals[nIndex[2]].z);

      faces++;
    }

  }

  fprintf(outfile, "%d\n", faces * 3 * 8);

  fclose(infile);
  fclose(outfile);

  std::cout << "Remember to open .txt file and move the last number (number of lines) to the top." << '\n';

  return 0;
}
