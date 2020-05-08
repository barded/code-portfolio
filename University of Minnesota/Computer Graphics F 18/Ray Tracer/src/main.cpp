
#include "ray.h"
#include "point.h"
#include "vector.h"
#include "surface.h"
#include "material.h"
#include "scene.h"
#include "light.h"
#include "color.h"
#include "image.h"
#include "pixel.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION //only place once in one .cpp file
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION //only place once in one .cpp files
#include "stb_image_write.h"


using namespace std;

struct Intersection {
  bool found;
  string type;
  int num;
  Point p;
  Vector normal;
  double t;
  bool tridiff; // if triangle has different normals
};

Intersection findhit(Ray ray);
Color evalraytree(Ray ray, int depth);
Color getcolor(Intersection hit);
Color applylight(Intersection hit, Ray ray, int depth);

Color black = Color(0, 0, 0);
Scene s;
Image * img = NULL;
vector < Sphere > spheres;
vector < Triangle > tris;
vector < Plane > planes;
bool debug = false;

vector < Material > mat;
vector < Light > ambient;
vector < PL > pl;
vector < Directional > directional;
vector < Spot > spot;
vector < AL > al;

vector < Point > vert;
vector < Vector > normals;

int main(int argc, char * argv[]) {
  Image * img = NULL;
  clock_t timereq;
  srand(time(NULL));
  int materialcount = 0;
  s.eye = Point(0, 0, 0);
  s.view = Vector(0, 0, 1);
  s.up = Vector(0, 1, 0);
  s.right = Vector(1, 0, 0);
  s.ha = 45.0;
  s.w = 640;
  s.h = 480;
  s.max_depth = 5;
  int nsamp = 1;
  int sampmethod = 0;
  float avsamples = 0;
  float range = 0;
  bool errorv = true;
  bool errorn = true;
  float threshold = .05;

  string output = "raytraced.bmp";
  mat.push_back(Material(0, 0, 0, 1, 1, 1, 0, 0, 0, 5, 0, 0, 0, 1));

  string line;

  string fileName = argv[1];

  // open the file containing the scene description
  ifstream input(fileName);

  // check for errors in opening the file
  if (input.fail()) {
    cout << "Can't open file '" << fileName << "'" << endl;
    return 0;
  }

  // determine the file size (this is optional -- feel free to delete the 6 lines below)
  streampos begin, end;
  begin = input.tellg();
  input.seekg(0, ios::end);
  end = input.tellg();
  // cout << "File '" << fileName << "' is: " << (end-begin) << " bytes long.\n\n";
  input.seekg(0, ios::beg);

  //Loop through reading each line
  string command;
  while (input >> command) { //Read first word in the line (i.e., the command type)

    if (command[0] == '#') {
      getline(input, line); //skip rest of line
      cout << "Skipping comment: " << command << line << endl;
      continue;
    }

    if (command == "sphere") { //If the command is a sphere command
      float x, y, z, r;
      input >> x >> y >> z >> r;
      Point c(x, y, z);
      spheres.push_back(Sphere(c, r, materialcount));
      std::cout << "sphere at " << c << " radius " << r << " material " << materialcount << '\n';
    } else if (command == "plane") {
      float x, y, z, u, v, w;
      input >> x >> y >> z >> u >> v >> w;
      Point p(x, y, z);
      Vector n(u, v, w);
      planes.push_back(Plane(p, n.norm(), materialcount));
      std::cout << "plane added" << planes[0].pos() << planes[0].n() << '\n';
    } else if (command == "triangle") {
      if (errorv) {
        std::cout << "error import triangle" << '\n';
        return 0;
      } else {
        int v1, v2, v3;
        input >> v1 >> v2 >> v3;
        Triangle t = Triangle(vert[v1], vert[v2], vert[v3], materialcount);
        tris.push_back(t);
        std::cout << "triangle added " << vert[v1] << vert[v2] << vert[v3] << '\n';

      }
    } else if (command == "normal_triangle") {
      if (errorv || errorn) {
        std::cout << "error import triangle" << '\n';
        return 0;
      } else {
        int v1, v2, v3, n1, n2, n3;
        input >> v1 >> v2 >> v3 >> n1 >> n2 >> n3;
        tris.push_back(Triangle(vert[v1], vert[v2], vert[v3], normals[n1], normals[n2], normals[n3], materialcount));
        std::cout << "normal triangle added " << vert[v1] << vert[v2] << vert[v3] << '\n';
      }
    } else if (command == "max_vertices") {
      errorv = false;
      int n;
      input >> n;
    } else if (command == "max_normals") {
      errorn = false;
      int n;
      input >> n;
    } else if (command == "vertex") {
      if (errorv) {
        std::cout << "error import vertex" << '\n';
        return 0;
      } else {
        double v1, v2, v3;
        input >> v1 >> v2 >> v3;
        vert.push_back(Point(v1, v2, v3));
        // std::cout << "added point "<<Point(v1,v2,v3) << '\n';
      }
    } else if (command == "normal") {
      if (errorn) {
        std::cout << "error import normal" << '\n';
        return 0;
      } else {
        double n1, n2, n3;
        input >> n1 >> n2 >> n3;
        normals.push_back(Vector(n1, n2, n3).norm());
        // std::cout << "added norm "<< Vector(n1,n2,n3)<< '\n';
      }
    } else if (command == "background") { //If the command is a background command
      float r, g, b;
      input >> r >> g >> b;
      s.background = Color(r, g, b);
      printf("Background color of (%f,%f,%f)\n", s.background.r, s.background.g, s.background.b);
    } else if (command == "camera") {
      double px, py, pz, dx, dy, dz, ux, uy, uz, ha;
      input >> px >> py >> pz >> dx >> dy >> dz >> ux >> uy >> uz >> ha;
      s.eye = Point(px, py, pz);
      s.view = -Vector(dx, dy, dz).norm();
      s.up = Vector(ux, uy, uz).norm();

      if (fabs(s.view.dot(s.up)) > 1e-6) { // set up orthog basis
        s.right = (s.up.cross(s.view)).norm();
        s.up = (s.view.cross(s.right)).norm();
      } else {
        s.right = ((s.up).cross(s.view)).norm();
      }

      s.right = (s.up).cross(s.view).norm();
      s.ha = ha;
      std::cout << "camera at " << s.eye << " in dir of " << s.view << '\n';
    } else if (command == "film_resolution") {
      int w, h;
      input >> w >> h;
      s.w = w;
      s.h = h;
      printf("width: %d height: %d\n", s.w, s.h);
    } else if (command == "output_image") { //If the command is an output_image command
      string outFile;
      input >> outFile;
      output = outFile;
      printf("Render to file named: %s\n", output.c_str());
    } else if (command == "material") {
      float ar, ag, ab, dr, dg, db, sr, sg, sb, n, tr, tg, tb, i;
      input >> ar >> ag >> ab >> dr >> dg >> db >> sr >> sg >> sb >> n >> tr >> tg >> tb >> i;
      mat.push_back(Material(ar, ag, ab, dr, dg, db, sr, sg, sb, n, tr, tg, tb, i));
      materialcount++;
      std::cout << "material ambient color   " << ar << " " << ag << " " << ab << '\n';
    } else if (command == "directional_light") {
      float r, g, b;
      double x, y, z;
      input >> r >> g >> b >> x >> y >> z;
      directional.push_back(Directional(Color(r, g, b), Vector(x, y, z).norm()));
      std::cout << "directional light points in " << Vector(x, y, z) << " r " << r << " g " << g << " b " << b << '\n';
    } else if (command == "point_light") {
      float r, g, b;
      double x, y, z;
      input >> r >> g >> b >> x >> y >> z;
      pl.push_back(PL(Color(r, g, b), Point(x, y, z)));
      std::cout << "point light at point " << Point(x, y, z) << '\n';
    } else if (command == "spot_light") {
      float r, g, b, angle1, angle2;
      double x, y, z, dx, dy, dz;
      input >> r >> g >> b >> x >> y >> z >> dx >> dy >> dz >> angle1 >> angle2;
      spot.push_back(Spot(Color(r, g, b), Point(x, y, z), Vector(dx, dy, dz).norm(), angle1, angle2));
      std::cout << "spot light at point " << Point(x, y, z) << " in direction " << Vector(dx, dy, dz) << '\n';
    } else if (command == "area_light") {
      float r, g, b;
      double x, y, z, u, v, w, a, d, c;
      input >> r >> g >> b >> x >> y >> z >> u >> v >> w >> a >> d >> c;
      al.push_back(AL(Color(r, g, b), Point(x, y, z), Vector(u, v, w), Vector(a, d, c)));
      std::cout << "area light at point " << Point(x, y, z) << '\n';
    } else if (command == "ambient_light") {
      float r, g, b;
      input >> r >> g >> b;
      ambient.push_back(Light(Color(r, g, b)));
      std::cout << "ambient light color  " << r << " " << g << " " << b << '\n';
    } else if (command == "max_depth") {
      int m;
      input >> m;
      s.max_depth = m;
      printf("max depth is %d\n", m);
    } else if (command == "nsamp") {
      int n;
      input >> n;
      nsamp = n;
      std::cout << "number of samples: " << nsamp << '\n';
    } else if (command == "sampmethod") {
      int n;
      input >> n;
      sampmethod = n;
      std::cout << "sample method: " << sampmethod << '\n';
    } else if (command == "threshold") {
      float t;
      input >> t;
      threshold = t;
      std::cout << "threshold for adaptive sampling is " << threshold << '\n';
    } else {
      getline(input, line); //skip rest of line
      cout << "WARNING. Do not know command: " << command << endl;
    }
  }

  img = new Image(s.w, s.h);
  Color * colors = new Color[s.w * s.h];

  float imgW = s.w, imgH = s.h;
  float halfwidth = imgW / 2.0, halfheight = imgH / 2.0;
  float d = halfheight / tanf(s.ha * M_PI / 180.0 f);

  Vector forward = Vector(-s.view.u(), -s.view.v(), -s.view.w());
  std::cout << "up is " << s.up << " right is " << s.right << " forward is " << forward << '\n';

  for (int i = 0; i < tris.size(); i++) { // figure out triangle normals
    if (!tris[i].diff) {
      Vector tn = tris[i].n[0];
      if (forward.dot(tn) > 0) {
        tris[i].setnormals(-tn);
      }
    }
    Vector s1 = tris[i].verts[0] - tris[i].verts[1];
    Vector s2 = tris[i].verts[2] - tris[i].verts[1];
    if ((s1.cross(s2)).dot(tris[i].n[0]) >= 0) {
      tris[i].side1 = s1;
      tris[i].side2 = s2;
    } else {
      tris[i].side1 = s2;
      tris[i].side2 = s1;
      Vector temp = tris[i].n[0];
      Vector temp2 = tris[i].n[2];
      tris[i].n[0] = temp2;
      tris[i].n[2] = temp;
    }
  }

  timereq = clock();
  for (int y = 0; y < s.h; y++) {
    for (int x = 0; x < s.w; x++) {
      Color c = black;

      if (sampmethod == 0) { //  basic sampling
        for (int b = 0; b < nsamp; b++) {
          for (int a = 0; a < nsamp; a++) {
            float u = (halfwidth - (imgW) * ((x + (float) a / (float) nsamp) / imgW));
            float v = (halfheight - (imgH) * ((y + (float) b / (float) nsamp) / imgH));

            Point p = s.eye + (d * forward + u * s.right + v * s.up);
            Ray ray = Ray(s.eye, (p - s.eye).norm());

            c = c + evalraytree(ray, s.max_depth);
            avsamples += 1;
          }
        }

        colors[x + y * s.w] = c / pow(nsamp, 2.0);
      } else if (sampmethod == 1) { //    adaptive sampling
        int m = 4;
        int total = 0;
        float s1 = 0, s2 = 1.0, factor = 0.0;

        bool keepgoing = true;
        bool once = true;

        while (keepgoing) {
          int count = 0, count2 = 0;
          Color temp, temp2;
          float ltemp, ltemp2;

          for (int a = 0; a < m; a++) {
            if (once) {
              switch (a) { //  four corners
              case 0:
                s1 = factor;
                s2 = factor;
                break;
              case 1:
                s1 = 1.0 - factor;
                s2 = factor;
                break;
              case 2:
                s1 = factor;
                s2 = 1.0 - factor;
                break;
              case 3:
                s1 = 1.0 - factor;
                s2 = 1.0 - factor;
              }
            } else {
              s1 = (float) rand() / (float)(RAND_MAX);
              s2 = (float) rand() / (float)(RAND_MAX);
            }

            float u = (halfwidth - (imgW) * ((x + (float) s1) / imgW));
            float v = (halfheight - (imgH) * ((y + (float) s2) / imgH));

            Point p = s.eye + (d * forward + u * s.right + v * s.up);
            Ray ray = Ray(s.eye, (p - s.eye).norm());
            temp = temp + evalraytree(ray, s.max_depth);
            count++;
            avsamples += 1;

          }
          temp = temp / (float) count;
          ltemp = temp.lum();

          for (int a = 0; a < 1; a++) {
            float r1 = (float) rand() / (float)(RAND_MAX);
            float r2 = (float) rand() / (float)(RAND_MAX);

            float u = (halfwidth - (imgW) * ((x + (float) r1) / imgW));
            float v = (halfheight - (imgH) * ((y + (float) r2) / imgH));

            Point p = s.eye + (d * forward + u * s.right + v * s.up);
            Ray ray = Ray(s.eye, (p - s.eye).norm());
            temp2 = temp2 + evalraytree(ray, s.max_depth);
            count2++;
            avsamples += 1;
          }

          temp2 = temp2 / (float) count2;
          ltemp2 = temp2.lum();

          if (fabs(ltemp - ltemp2) * 255.0 < threshold) {
            keepgoing = false;
            if (once)
              c = (temp + temp2) / (float) 2.0;
            else
              c = (c + temp + temp2) / (float) 3.0;
          } else if (once) {
            c = (temp + temp2) / (float) 2.0;
            once = false;
          } else {
            c = (c + temp + temp2) / (float) 3.0;
          }
          total += count + count2;

          if (total > 50) {
            break;
          }
        }

        colors[x + y * s.w] = c;
      } else { //   jittered sampling
        int m = nsamp * nsamp;

        for (int a = 0; a < m; a++) {
          float r1 = (float) rand() / (float)(RAND_MAX);
          float r2 = (float) rand() / (float)(RAND_MAX);

          float u = (halfwidth - (imgW) * ((x + (float) r1) / imgW));
          float v = (halfheight - (imgH) * ((y + (float) r2) / imgH));

          Point p = s.eye + (d * forward + u * s.right + v * s.up);
          Ray ray = Ray(s.eye, (p - s.eye).norm());
          c = c + evalraytree(ray, s.max_depth);
          avsamples += 1;

        }
        colors[x + y * s.w] = c / pow(nsamp, 2.0);
      }

    }
  }

  timereq = clock() - timereq;

  for (int y = 0; y < s.h; y++) {
    for (int x = 0; x < s.w; x++) {
      img - > SetPixel(x, y, Pixel(ComponentClamp(colors[x + y * s.w].r * 255), ComponentClamp(colors[x + y * s.w].g * 255), ComponentClamp(colors[x + y * s.w].b * 255)));
    }
  }

  char * cstr = new char[output.length() + 1];
  strcpy(cstr, output.c_str());
  img - > Write(cstr);

  delete img;
  delete[] cstr;
  delete[] colors;
  printf("Done.  Sample method %d took %lf secs. Samples/pixel average %lf\n", sampmethod, (float) timereq / CLOCKS_PER_SEC, (float) avsamples / (float)(s.w * s.h));
  return EXIT_SUCCESS;
}

/*
Functions
*/

Color evalraytree(Ray ray, int depth) {
  if (depth <= 0)
    return black;

  Intersection hit = findhit(ray);

  if (hit.found)
    return applylight(hit, ray, depth);
  else return s.background;
}

Intersection findhit(Ray ray) {
  double tmin = 9999999;
  int closest;
  Vector ec;
  Intersection hit;
  Vector raydir = ray.dir();
  hit.found = false;
  double trmax = ray.tmax();
  double trmin = ray.tmin();
  int numsph = spheres.size();
  int numtri = tris.size();
  int numplanes = planes.size();

  for (int i = 0; i < numsph; i++) { // spheres
    double t = 999999;
    ec = ray.org() - spheres[i].get_c();

    double a = 1;
    double b = 2 * raydir.dot(ec);
    double c = pow(ec.len(), 2.0) - pow(spheres[i].get_r(), 2.0);
    double disc = b * b - 4 * a * c;
    if (disc >= 0) {

      double dsqrt = sqrt(disc);
      double tp = (-b + dsqrt) / (2.0 * a), tm = (-b - dsqrt) / (2.0 * a);
      if (tp >= trmax) {
        tp = trmin - 1;
      }

      if (tm >= trmax) {
        tm = trmin - 1;
      }

      double m1 = fmax(trmin, tp);
      double m2 = fmax(trmin, tm);

      if (m1 > trmin && m2 > trmin) {
        t = fmin(m1, m2);
      } else if (m1 > trmin && m2 <= trmin) {
        t = m1;
      } else if (m2 > trmin && m1 <= trmin) {
        t = m2;
      } else {
        continue; //  both trmin or less
      }
      if (t < tmin) {
        tmin = t;
        hit.found = true;
        hit.type = "sphere";
        hit.p = ray.eval(t);
        hit.num = i;
        if (ray.inside) { //  inside sphere, flip sign of normal
          hit.normal = (spheres[i].get_c() - hit.p).norm();
        } else {
          hit.normal = (hit.p - spheres[i].get_c()).norm();
        }
        hit.t = tmin;

      }
    }
  }

  for (int i = 0; i < numtri; i++) { // triangles with bary coords
    double t = 999999;
    Vector a = tris[i].side1; // 0-1
    Vector b = tris[i].side2; // 2-1
    Vector e = ray.org() - tris[i].verts[1];
    double bottom = ray.dir().cross(b).dot(a);
    double tm = ((e.cross(a)).dot(b)) / (bottom);
    double alpha = (ray.dir().cross(b).dot(e)) / (bottom);
    double beta = ((e.cross(a)).dot(ray.dir())) / (bottom);
    double gamma = 1.0 - alpha - beta;
    // std::cout << "a "<<a<<" b "<<b<<" e"<<e<<" bottom "<<bottom << '\n';

    if (alpha >= 0 && beta >= 0 && alpha + beta <= 1 &&
      tm > trmin && tm < trmax && tm < tmin) {
      tmin = tm;
      hit.found = true;
      hit.type = "triangle";
      hit.p = ray.eval(tmin);
      hit.num = i;
      if (tris[i].diff) { // normal triangle
        hit.tridiff = true;
        hit.normal = (tris[i].n[0] * alpha + tris[i].n[2] * beta + tris[i].n[1] * gamma).norm();
        // std::cout << "tri norm "<<hit.normal<< "t="<<tm<<" alpha="<<alpha<<" beta="<<beta<<" gamma "<<gamma<<tris[i].n[0]<<tris[i].n[1]<<tris[i].n[2]<< '\n';

      } else {
        hit.normal = tris[i].n[0];
        hit.tridiff = false;
      }
      hit.t = tmin;
    }

  }

  for (int i = 0; i < numplanes; i++) { // planes
    double t = 999999;
    Vector n = planes[i].n();
    ec = planes[i].pos() - ray.org();
    t = ec.dot(n) / (ray.dir()).dot(n);

    if (t < tmin && t < trmax && t > trmin) {
      tmin = t;
      hit.found = true;
      hit.type = "plane";
      hit.p = ray.eval(t);
      hit.num = i;
      hit.normal = n;
      hit.t = tmin;
    }
  }

  return hit;
}

Color applylight(Intersection hit, Ray ray, int depth) {
  Color color;
  Point p = hit.p;
  Vector n = hit.normal;
  Material material;

  if (hit.type == "sphere") {
    material = mat[spheres[hit.num].material()];
  } else if (hit.type == "triangle") {
    material = mat[tris[hit.num].material()];
  } else if (hit.type == "plane") {
    material = mat[planes[hit.num].material()];
  } else return s.background;

  Color d = material.d;
  Color sp = material.s;
  Color trans = material.t;
  float ior = material.ior;

  for (int i = 0; i < pl.size(); i++) { // point lights
    PL light = pl[i];
    Point lp = light.pos();
    Vector l = (lp - p).norm();

    Vector v = (s.eye - p).norm();
    double tolight = (lp - p).len();
    Ray shadow(p, l, .005, tolight);
    Intersection shadhit = findhit(shadow);

    if (shadhit.found)
      continue;

    Vector h = (v + l).norm();
    Color I = light.intensity(p);
    color = color + d * I * fmax(0, n.dot(l)) + sp * I * pow(fmax(0, n.dot(h)), material.ns);
  }

  for (int i = 0; i < directional.size(); i++) { // directional light
    Directional light = directional[i];
    Point lp = p + (light.dir() * -50); // make up a position
    Vector l = -light.dir();
    Vector v = (s.eye - p).norm();
    double tolight = (lp - p).len();
    Ray shadow(p, l, .005, tolight);
    Intersection shadhit = findhit(shadow);

    if (shadhit.found && shadhit.t < tolight)
      continue;

    Vector h = (v + l).norm();
    Color I = light.intensity();
    color = color + d * I * fmax(0, n.dot(l)) + sp * I * pow(fmax(0, n.dot(h)), material.ns);
  }

  for (int i = 0; i < spot.size(); i++) { // spot lights
    Spot light = spot[i];
    Point lp = light.pos();
    Vector l = (lp - p).norm();
    Vector v = (s.eye - p).norm();
    double tolight = (lp - p).len();
    Ray shadow(p, l, .005, tolight);
    Intersection shadhit = findhit(shadow);

    if (shadhit.found && shadhit.t < tolight)
      continue;

    Vector h = (v + l).norm();
    Color I = light.intensity(p, l);
    color = color + d * I * fmax(0, n.dot(l)) + sp * I * pow(fmax(0, n.dot(h)), material.ns);
  }

  for (int i = 0; i < al.size(); i++) { // area light
    float shadowamnt = 0;
    AL light = al[i];
    int numsrays = 10;
    int count = 0;
    Vector v = (s.eye - p).norm();
    Point lp;
    Vector l;
    for (int j = 0; j < numsrays; j++) {
      lp = light.randpos();
      l = (lp - p).norm();
      double tolight = (lp - p).len();
      Ray shadow(p, l, .005, tolight);
      Intersection shadhit = findhit(shadow);

      if (shadhit.found) {
        count++;
      }
    }
    shadowamnt = (float)(numsrays - count) / (float) numsrays;
    lp = light.center();

    l = (lp - p).norm();
    Vector h = (v + l).norm();
    Color I = light.intensity(p);

		// if more shadow rays blocked, diffuse & spec amount are less
    color = color + (d * I * fmax(0, n.dot(l)) + sp * I * pow(fmax(0, n.dot(h)), material.ns)) * shadowamnt;
  }

  Ray mirror = Ray(p, ray.reflect(n), .005, 9999, ray.inside, ray.shapetype, ray.shapenum);
  color = color + sp * evalraytree(mirror, depth - 1);

  Ray glass;
  float rayior;
  if (ray.shapetype == "sphere") {
    rayior = mat[spheres[ray.shapenum].material()].ior;
  } else rayior = 1.0; // air

  if (hit.type == "triangle") { // triangles are weird
    if (hit.tridiff) {
      if ((-s.view).dot(hit.normal) <= 0) { //  hit front side
        Vector rdir = ray.refract(n, rayior, ior);
        glass = Ray(p, rdir, .005, 9999, true, hit.type, hit.num);
      } else { // hit back side
        Vector rdir = ray.refract(n, ior, 1.0);
        glass = Ray(p, rdir, .005, 9999, ray.inside);
      }
    } else
      glass = Ray(p, ray.dir(), .005, 9999);
  } else if (!ray.inside) { // ray in air
    Vector rdir = ray.refract(n, rayior, ior);
    glass = Ray(p, rdir, .005, 9999, true, hit.type, hit.num);
  } else if (ray.inside && ray.shapetype == hit.type &&
    ray.shapenum == hit.num) { // hit other side of shape
    Vector rdir = ray.refract(n, rayior, 1.0);
    glass = Ray(p, rdir, .005, 9999, false);
  } else { //     inside & hit another shape
    Vector rdir = ray.refract(n, rayior, ior);
    glass = Ray(p, rdir, .005, 9999, true, hit.type, hit.num);
  }

  color = color + material.t * evalraytree(glass, depth - 1);

  for (int i = 0; i < ambient.size(); i++) { // ambient lights
    Light light = ambient[i];
    Color I = light.intensity();
    color = color + material.a * I;
  }

  return color;

}
