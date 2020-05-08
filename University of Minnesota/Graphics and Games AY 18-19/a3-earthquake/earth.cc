/** CSci-4611 Assignment 3:  Earthquake
 */

#include "earth.h"
#include "earthquake_database.h"
#include "earthquake.h"

#include "config.h"

#include <vector>

// for M_PI constant
#define _USE_MATH_DEFINES
#include <math.h>


Earth::Earth() {
  rotate = 0;
}

Earth::~Earth() {
}
void Earth::Init(const std::vector < std::string > &search_path) {
  // init shader program
  shader_.Init();

  // init texture: you can change to a lower-res texture here if needed
  earth_tex_.InitFromFile(Platform::FindFile("earth-256.png", search_path));

  // init geometry
  const int nslices = 20;
  const int nstacks = 20;

  std::vector < unsigned int > indices;
  std::vector < Point2 > texcoords;
  std::vector < Point3 > vertices;
  std::vector < Vector3 > normals;

  float dx = 2 * M_PI / (nslices);
  float dy = M_PI / (nstacks);
  Point3 o = Point3(0, 0, 0);

  for (float y = M_PI / 2.0; y >= -M_PI / 2.0 - dy * .9; y -= dy) {
    for (float x = -M_PI; x <= M_PI + dx * .9; x += dx) {
      fvertices.push_back(Point3(x, y, 0));
      fnormals.push_back(Vector3(0, 0, 1));
      tvertices.push_back(Point3(x, y, 0));
      tnormals.push_back(Vector3(0, 0, 1));

      Point3 p = Point3(cos(y) * sin(x), sin(y), cos(y) * cos(x));
      rvertices.push_back(p);
      rnormals.push_back(p - o);
      texcoords.push_back(Point2((x + M_PI) / (2.0 * M_PI), fabs((y - M_PI / 2.0)) / M_PI));

    }
  }

  for (int y = 0; y < nstacks; y++) {
    for (int x = 0; x < nslices; x++) {
      indices.push_back(x + y * (nslices + 1));
      indices.push_back(x + (y + 1) * (nslices + 1));
      indices.push_back(x + 1 + (y + 1) * (nslices + 1));

      indices.push_back(x + y * (nslices + 1));
      indices.push_back(x + 1 + (y + 1) * (nslices + 1));
      indices.push_back(x + 1 + y * (nslices + 1));

    }
  }

  earth_mesh_.SetNormals(fnormals);
  earth_mesh_.SetVertices(fvertices);
  earth_mesh_.SetIndices(indices);
  earth_mesh_.SetTexCoords(0, texcoords);

  earth_mesh_.UpdateGPUMemory();
}

void Earth::Draw(const Matrix4 &model_matrix,
  const Matrix4 &view_matrix,
    const Matrix4 &proj_matrix) {
  // Define a really bright white light.  Lighting is a property of the "shader"
  DefaultShader::LightProperties light;
  light.position = Point3(10, 10, 10);
  light.ambient_intensity = Color(1, 1, 1);
  light.diffuse_intensity = Color(1, 1, 1);
  light.specular_intensity = Color(1, 1, 1);
  shader_.SetLight(0, light);

  // Adust the material properties, material is a property of the thing
  // (e.g., a mesh) that we draw with the shader.  The reflectance properties
  // affect the lighting.  The surface texture is the key for getting the
  // image of the earth to show up.
  DefaultShader::MaterialProperties mat;
  mat.ambient_reflectance = Color(0.5, 0.5, 0.5);
  mat.diffuse_reflectance = Color(0.75, 0.75, 0.75);
  mat.specular_reflectance = Color(0.75, 0.75, 0.75);
  mat.surface_texture = earth_tex_;

  // Draw the earth mesh using these settings
  if (earth_mesh_.num_triangles() > 0) {
    Matrix4 rt = Matrix4::RotationY(rotate);

    shader_.Draw(model_matrix * rt, view_matrix, proj_matrix, &earth_mesh_, mat);
  }

}

Point3 Earth::LatLongToSphere(double latitude, double longitude) const {
  return Point3(cos(latitude * M_PI / 180.0f) * sin(longitude * M_PI / 180.0f), sin(latitude * M_PI / 180.0f), cos(latitude * M_PI / 180.0f) * cos(longitude * M_PI / 180.0f));
}

Point3 Earth::LatLongToPlane(double latitude, double longitude) const {
  return Point3(longitude * M_PI / 180.0f, latitude * M_PI / 180.0f, 0.05);
}

void Earth::DrawDebugInfo(const Matrix4 &model_matrix,
  const Matrix4 &view_matrix,
    const Matrix4 &proj_matrix) {
  // This draws a cylinder for each line segment on each edge of each triangle in your mesh.
  // So it will be very slow if you have a large mesh, but it's quite useful when you are
  // debugging your mesh code, especially if you start with a small mesh.

  for (int t = 0; t < earth_mesh_.num_triangles(); t++) {
    std::vector < unsigned int > indices = earth_mesh_.triangle_vertices(t);
    std::vector < Point3 > loop;
    loop.push_back(earth_mesh_.vertex(indices[0]));
    loop.push_back(earth_mesh_.vertex(indices[1]));
    loop.push_back(earth_mesh_.vertex(indices[2]));

    quick_shapes_.DrawLines(model_matrix, view_matrix, proj_matrix,
      Color(1, 1, 0), loop, QuickShapes::LinesType::LINE_LOOP, 0.005);

  }

}
