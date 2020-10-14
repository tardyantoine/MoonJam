#ifndef POINT_H
#define POINT_H

class Point {
 public:
  Point(float x, float y);
  Point() {}

  Point operator+(const Point& a) {
    Point b(0, 0);
    b.mX = this->mX + a.mX;
    b.mY = this->mY + a.mY;
    return b;
  }

  Point operator-(const Point& a) {
    Point b(0, 0);
    b.mX = this->mX - a.mX;
    b.mY = this->mY - a.mY;
    return b;
  }

  Point operator*(float m) {
    Point b(0, 0);
    b.mX = this->mX * m;
    b.mY = this->mY * m;
    return b;
  }

  float mX;
  float mY;
};

#endif