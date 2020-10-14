#ifndef POINT_H
#define POINT_H

class Point {
 public:
  Point(float x, float y);

  Point operator+(const Point& a) {
    Point b;
    b.mX = this->mX + a.mX;
    b.mY = this->mY + a.mY;
    return b;
  }

  Point operator-(const Point& a) {
    Point b;
    b.mX = this->mX - a.mX;
    b.mY = this->mY - a.mY;
    return b;
  }

  Point operator*(float m, const Point& a) {
    Point b;
    b.mX = a.mX * m;
    b.mY = a.mY * m;
    return b;
  }

  float mX;
  float mY;
};

#endif