
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define WIDTH 80
#define HEIGHT 40
#define SCALE 20.0
#define CAMERA_DISTANCE 3.0
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
  double x, y;
} Vec2;
typedef struct {
  double x, y, z;
} Vec3;
typedef struct {
  Vec3 center, edgeU, edgeV, normal;
} Face;
typedef struct {
  int x, y;
} ScreenCoords;
typedef struct {
  Vec3 basisX, basisY, basisZ;
} Matrix3x3;

// function prototypes
Vec3 add(Vec3 a, Vec3 b);
Vec3 scale(Vec3 v, double s);
double dot(Vec3 a, Vec3 b);
double magnitude(Vec3 v);
Vec3 normalize(Vec3 v);
Vec2 project(Vec3 v);
ScreenCoords ResolveScreenCoords(Vec2 v);
void draw(char screen[HEIGHT][WIDTH], ScreenCoords a, ScreenCoords b, char c);
Vec3 MultiplyByMatrix(Matrix3x3 R, Vec3 v);
double rad(double angle);
void clearScreen();
void sleep_ms(int milliseconds);

int main() {
  char screen[HEIGHT][WIDTH];

  Vec3 vertices[8] = {{-1, -1, -1}, {1, -1, -1}, {-1, 1, -1}, {1, 1, -1},
                      {-1, -1, 1},  {1, -1, 1},  {-1, 1, 1},  {1, 1, 1}};

  int edges[12][2] = {{0, 1}, {0, 2}, {1, 3}, {2, 3}, {4, 5}, {4, 6},
                      {5, 7}, {6, 7}, {0, 4}, {1, 5}, {2, 6}, {3, 7}};

  double angleX = 0.0, angleY = 0.0, angleZ = 0.0;

  while (1) {
    Matrix3x3 Rx = {{1, 0, 0},
                    {0, cos(rad(angleX)), -sin(rad(angleX))},
                    {0, sin(rad(angleX)), cos(rad(angleX))}};
    Matrix3x3 Ry = {{cos(rad(angleY)), 0, sin(rad(angleY))},
                    {0, 1, 0},
                    {-sin(rad(angleY)), 0, cos(rad(angleY))}};
    Matrix3x3 Rz = {{cos(rad(angleZ)), -sin(rad(angleZ)), 0},
                    {sin(rad(angleZ)), cos(rad(angleZ)), 0},
                    {0, 0, 1}};

    for (int i = 0; i < HEIGHT; i++)
      for (int j = 0; j < WIDTH; j++)
        screen[i][j] = ' ';

    Vec3 rotated[8];
    for (int i = 0; i < 8; i++) {
      Vec3 v = vertices[i];
      v = MultiplyByMatrix(Rx, v);
      v = MultiplyByMatrix(Ry, v);
      v = MultiplyByMatrix(Rz, v);
      rotated[i] = v;
    }

    for (int i = 0; i < 12; i++) {
      Vec2 pa = project(rotated[edges[i][0]]);
      Vec2 pb = project(rotated[edges[i][1]]);
      ScreenCoords A = ResolveScreenCoords(pa);
      ScreenCoords B = ResolveScreenCoords(pb);
      draw(screen, A, B, '#');
    }

    char buffer[(WIDTH + 1) * HEIGHT + 1];
    int idx = 0;
    for (int i = 0; i < HEIGHT; i++) {
      for (int j = 0; j < WIDTH; j++)
        buffer[idx++] = screen[i][j];
      buffer[idx++] = '\n';
    }
    buffer[idx] = '\0';

    clearScreen();
    printf("%s", buffer);

    angleX += 5.0;
    angleY += 5.0;
    angleZ += 5.0;
    sleep_ms(50);
  }

  return 0;
}

Vec3 add(Vec3 a, Vec3 b) { return (Vec3){a.x + b.x, a.y + b.y, a.z + b.z}; }
Vec3 scale(Vec3 v, double s) { return (Vec3){v.x * s, v.y * s, v.z * s}; }
double dot(Vec3 a, Vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
double magnitude(Vec3 v) { return sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }
Vec3 normalize(Vec3 v) {
  double m = magnitude(v);
  return (Vec3){v.x / m, v.y / m, v.z / m};
}
Vec2 project(Vec3 v) {
  double z = v.z + CAMERA_DISTANCE;
  return (Vec2){v.x / z * SCALE, v.y / z * SCALE};
}
ScreenCoords ResolveScreenCoords(Vec2 v) {
  return (ScreenCoords){(int)(WIDTH / 2 + v.x * WIDTH / HEIGHT),
                        (int)(HEIGHT / 2 - v.y)};
}
void draw(char screen[HEIGHT][WIDTH], ScreenCoords a, ScreenCoords b, char c) {
  int steps = fmax(fabs(a.x - b.x), fabs(a.y - b.y));
  for (int i = 0; i <= steps; i++) {
    double t = (double)i / steps;
    int x = (int)round(a.x + (b.x - a.x) * t);
    int y = (int)round(a.y + (b.y - a.y) * t);
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
      screen[y][x] = c;
  }
}
Vec3 MultiplyByMatrix(Matrix3x3 R, Vec3 v) {
  return add(add(scale(R.basisX, v.x), scale(R.basisY, v.y)),
             scale(R.basisZ, v.z));
}
double rad(double angle) { return angle * (M_PI / 180.0); }

void clearScreen() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

void sleep_ms(int milliseconds) {
#ifdef _WIN32
  Sleep(milliseconds);
#else
  usleep(milliseconds * 1000);
#endif
}
