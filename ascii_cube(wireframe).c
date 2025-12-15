#include <math.h>
#include <stdio.h>
#include <windows.h>


#define WIDTH 80 //width of the projection in the terminal
#define HEIGHT 40 //height of the projection in the terminal
#define SCALE 20.0 //dictates how zoomed in the cube should look
#define CAMERA_DISTANCE 3.0 //the distance of the camera from the cube
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif


typedef struct {
    double x, y;
} Vec2;
/*
define type 2d vector for projecting the cube on a 2d plane, made up of:
    x: coordinate along the x axis
    y: coordinate along the y axis
*/


typedef struct {
    double x, y, z;
} Vec3;
/*
define type 3d vector made up of:
    x: the coordinate along the x axis
    y: the coordinate along the y axis
    z: the coordinate along the z axis
*/


typedef struct {
    Vec3 center, edgeU, edgeV, normal;
} Face;
/*
define type face made up of:
    normal: a 3d vector describing the position of the face's center relative to the center of the coordinate system
    edgeU: a 3d vector describing the distance between the center of the face and its top/bottom side 
    edgeV: a 3d vector describing the distance between the center of the face and its left/right side
    normal: a 3d vector describing the direction the face is pointing
*/


typedef struct {
    int x, y;
} ScreenCoords;
/*
integer version of Vec2, meant to symbolize coordinates on our terminal "screen"
*/


typedef struct {
    Vec3 basisX, basisY, basisZ;
} Matrix3x3;
/*
define type 3x3 matrix, made up of the 3 basis vectors of the transformed coordinate system
each basis vector represents 1 row of the matrix
*/


Vec3 add(Vec3 vectorA, Vec3 vectorB);
Vec3 scale(Vec3 vector, double scalar);
double dot(Vec3 vectorA, Vec3 vectorB);
double magnitude(Vec3 vector);
Vec3 normalize(Vec3 vector);
Vec2 project(Vec3 vector);
ScreenCoords ResolveScreenCoords(Vec2 vector);
void draw(char screen[HEIGHT][WIDTH], ScreenCoords pointA, ScreenCoords pointB, char c);
Vec3 MultiplyByMatrix(Matrix3x3 R, Vec3 vector);
double rad(double angle);
//function prototypes


int main() {
    char screen[HEIGHT][WIDTH];
    for(int i=0; i<HEIGHT; i++) {
        for(int j=0; j<WIDTH; j++) {
            screen[i][j] = ' ';
        }
    } //initializing the screen on which the cube will be projected as a character array filled with spaces

    Vec3 vertices[8] = {
        {-1, -1, -1}, {1, -1, -1}, {-1, 1, -1}, {1, 1, -1}, //back face
        {-1, -1, 1}, {1, -1, 1}, {-1, 1, 1}, {1, 1, 1} //front face
    }; //array of 3d vectors representing vertices

    int edges[12][2] = {
        {0, 1}, {0, 2}, {1, 3}, {2, 3}, //back face
        {4, 5}, {4, 6}, {5, 7}, {6, 7}, //front face
        {0, 4}, {1, 5}, {2, 6}, {3, 7} //connections between the front and back face
    }; //array storing indexes of array vertices which are to be connected together via sides of the cube

    double angleX = 0.0, angleY = 0.0, angleZ = 0.0;
    //angles by which we will be rotating

    while(1) {
        Matrix3x3 Rx = {
            {1, 0, 0},
            {0, cos(rad(angleX)), -sin(rad(angleX))},
            {0, sin(rad(angleX)), cos(rad(angleX))}
        }; //initialize the rotation matrix for the x axis

        Matrix3x3 Ry = {
            {cos(rad(angleY)), 0, sin(rad(angleY))},
            {0, 1, 0},
            {-sin(rad(angleY)), 0, cos(rad(angleY))}
        }; //initialize the rotation matrix for the y axis

        Matrix3x3 Rz = {
            {cos(rad(angleZ)), -sin(rad(angleZ)), 0},
            {sin(rad(angleZ)), cos(rad(angleZ)), 0},
            {0, 0, 1}
        }; //initialize the rotation matrix for the z axis

        for(int i=0; i<HEIGHT; i++) {
            for(int j=0; j<WIDTH; j++) {
                screen[i][j] = ' ';
            }
        } //clears the screen buffer

        Vec3 rotated_vertices[8];
        for(int i=0; i<8; i++) {
            Vec3 v = vertices[i];
            v = MultiplyByMatrix(Rx, v);
            v = MultiplyByMatrix(Ry, v);
            v = MultiplyByMatrix(Rz, v);
            rotated_vertices[i] = v;
        } //copies and rotates the vertices

        for(int i=0; i<12; i++) {
            Vec2 pointa = project(rotated_vertices[edges[i][0]]);
            Vec2 pointb = project(rotated_vertices[edges[i][1]]);
            ScreenCoords pointA = ResolveScreenCoords(pointa);
            ScreenCoords pointB = ResolveScreenCoords(pointb);
            draw(screen, pointA, pointB, '#');
        } //loops over edges and draws each one

        char buffer[(WIDTH + 1) * HEIGHT + 1]; // +1 per line for \n, +1 for null terminator
        int idx = 0;

        for(int i = 0; i < HEIGHT; i++) {
            for(int j = 0; j < WIDTH; j++) {
                buffer[idx++] = screen[i][j];
            }
            buffer[idx++] = '\n';
        }
        buffer[idx] = '\0';

        system("cls");
        printf("%s", buffer);
        //appends all characters to a single string and prints all at once

        angleX+=5.0;
        angleY+=5.0;
        angleZ+=5.0;
        //update the rotation angles

        Sleep(50); //delay in milliseconds
    }

    return 0;
}


Vec3 add(Vec3 vectorA, Vec3 vectorB) {
    Vec3 vectorC = {vectorA.x + vectorB.x, vectorA.y + vectorB.y, vectorA.z + vectorB.z};
    return vectorC;
} //adds 2 vectors

Vec3 scale(Vec3 vector, double scalar) {
    Vec3 result_vector = {vector.x * scalar, vector.y * scalar, vector.z * scalar};
    return result_vector;
} //scales a vector

double dot(Vec3 vectorA, Vec3 vectorB) {
    double scalar = vectorA.x * vectorB.x + vectorA.y * vectorB.y + vectorA.z * vectorB.z;
    return scalar;
} //returns the dot product of 2 vectors

double magnitude(Vec3 vector) {
    double magnitude = sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
    return magnitude;
} //returns the magnitude (length) of a vector

Vec3 normalize(Vec3 vector) {
    double vector_magnitude = magnitude(vector);
    Vec3 unit_vector = {vector.x / vector_magnitude, vector.y / vector_magnitude, vector.z / vector_magnitude};
    return unit_vector;
} //turns a vector into a unit vector facing the same direction (normalizes)

Vec2 project(Vec3 vector) {
    double z = vector.z + CAMERA_DISTANCE;
    Vec2 projection = {vector.x / z * SCALE, vector.y / z * SCALE};
    return projection;
} //projects a 3d vector onto a 2d plane

ScreenCoords ResolveScreenCoords(Vec2 vector) {
    ScreenCoords sc = {
        (int)(WIDTH / 2 + vector.x * WIDTH / HEIGHT),
        (int)(HEIGHT / 2 - vector.y)
    };
    return sc;
} //turns a 2d vector into a pair of integers meant to symbolize the coordinates in the terminal "screen"

void draw(char screen[HEIGHT][WIDTH], ScreenCoords pointA, ScreenCoords pointB, char c) {
    int steps = fmax(fabs(pointA.x - pointB.x), fabs(pointA.y - pointB.y));
    for(int i=0; i<=steps; i++) {
        double interpolation_factor = (double)i / steps;
        int x = (int)round(pointA.x + (pointB.x - pointA.x) * interpolation_factor);
        int y = (int)round(pointA.y + (pointB.y - pointA.y) * interpolation_factor);
        if(x>=0 && x<WIDTH && y>=0 && y<HEIGHT) {
            screen[y][x] = c;
        }
    }
} //draws a line of characters represented by the c variable on the "screen" from point A to point B

Vec3 MultiplyByMatrix(Matrix3x3 R, Vec3 vector) {
    Vec3 result = add(add(scale(R.basisX, vector.x), scale(R.basisY, vector.y)), scale(R.basisZ, vector.z));
    return result;
} //multiplies a 3d vector by a 3x3 matrix

double rad(double angle) {
    double result = angle * (M_PI / 180.0);
    return result;
} //converts from degrees to radians