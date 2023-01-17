#ifndef COMMON_H
#define COMMON_H

#include <QMatrix3x3>
#include <QMatrix4x4>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>

extern void check_gl_error(char const*, int);

#define OGL_CHECKPOINT_ALWAYS()         \
  do {                                  \
    check_gl_error(__FILE__, __LINE__); \
  } while (0)

#if defined(NDEBUG)
#define OGL_CHECKPOINT_DEBUG() \
  do {                         \
  } while (0)
#else
#define OGL_CHECKPOINT_DEBUG() OGL_CHECKPOINT_ALWAYS()
#endif

using Time = decltype(std::chrono::high_resolution_clock::now());
using Duration = float;

const float PI = 3.1415926f;

inline float ToDegree(float iRadian) { return iRadian / PI * 180; }
inline float ToRadian(float iDegree) { return iDegree / 180.0f * PI; }

#endif  // COMMON_H
