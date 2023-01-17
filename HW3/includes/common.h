#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <string>
#include <functional>
#include <memory>
#include <map>
#include <unordered_map>
#include <chrono>

#include <QVector2D>
#include <QVector3D>
#include <QVector4D>

#include <QMatrix3x3>
#include <QMatrix4x4>

extern void check_gl_error(char const*, int);

#define OGL_CHECKPOINT_ALWAYS() do {                                \
        check_gl_error( __FILE__, __LINE__ );             \
    } while(0)                                                      \

#if defined(NDEBUG)
#	define OGL_CHECKPOINT_DEBUG()   do {} while(0)
#else
#	define OGL_CHECKPOINT_DEBUG()   OGL_CHECKPOINT_ALWAYS()
#endif

using Time     = decltype(std::chrono::high_resolution_clock::now());
using Duration = float;

#endif // COMMON_H
