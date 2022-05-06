#include "rand_ops.h"
#include "rand.h"

#include <iostream>

// outputs to colour array, which should have length channels
void rand_colour(const int channels, unsigned char *colour) {
  for (int i = 0; i < channels; ++i) {
    colour[i] = randInt(0, 255);
  }
}

// vertices should be array of length 2*num_vertices
// vertices[2*n] = {x1, y1, x2, y2... xn, yn}
// outputs to colour array
void centre_colour(const int num_vertices, const int *vertices, const Image *im,
                   unsigned char *colour) {
  // calculate centre of all vertices
  int avg_x(0), avg_y(0);
  for (int i = 0; i < num_vertices * 2; i += 2) {
    avg_x += vertices[i];
    avg_y += vertices[i+1];
  }
  avg_x /= num_vertices;
  avg_y /= num_vertices;
  
  // retrieve colour from this location
  int channels = im->spectrum();
  for (int i = 0; i < channels; ++i) {
    colour[i] = (*im)(avg_x, avg_y, 0, i);
  }

  // if (colour[0] == 255 && colour[1] == 255 && colour[2] == 255) {
  //   {
  //     using namespace std;
  //     cout << avg_x << " " << avg_y << endl;
  //     for (int i = 0; i < 6; i += 2) {
  //       cout << "(" << vertices[i] << "," << vertices[i+1] << ")";
  //     }
  //     cout << endl;
  //   }
  // }
}

void get_triangle_vertices(const int w, const int h, const float feature_size, int *vertices) {
  const int feature_w = feature_size*w;
  const int feature_h = feature_size*h;
  const int max_feature_offset_x = w - feature_w;
  const int max_feature_offset_y = h - feature_h;
  for (int i = 0; i < 6; i += 2) {
    vertices[i] = randInt(0, feature_w - 1) + randInt(0, max_feature_offset_x);
    vertices[i + 1] = randInt(0, feature_h - 1) + randInt(0, max_feature_offset_y);
  }
}

// if vertices is not null, the vertices of the triangle will be saved
// vertices should be an array of length 6
// output image is allocated on heap; make sure to delete when done
Image *rand_triangle(const unsigned char *colour, const Image *canvas, const float feature_size, int *vertices) {
  Image *output = new Image(*canvas);

  const int w = canvas->width();
  const int h = canvas->height();

  int v[6];
  get_triangle_vertices(w, h, feature_size, v);

  output->draw_triangle(v[0], v[1],
                        v[2], v[3],
                        v[4], v[5],
                        colour);

  if (vertices) {
    std::copy(v, v + 6, vertices);
  }
  return output;
}

Image *rand_triangle_centre_colour(const Image *canvas, const Image *target, const float feature_size, int *vertices) {
  Image *output = new Image(*canvas);

  const int w = canvas->width();
  const int h = canvas->height();

  int v[6];
  get_triangle_vertices(w, h, feature_size, v);

  unsigned char colour[target->spectrum()];
  centre_colour(3, v, target, colour);

  output->draw_triangle(v[0], v[1],
                        v[2], v[3],
                        v[4], v[5],
                        colour);

  if (vertices) {
    std::copy(v, v + 6, vertices);
  }
  return output;
}
