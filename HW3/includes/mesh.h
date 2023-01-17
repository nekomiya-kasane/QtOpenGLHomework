#ifndef MESH_H
#define MESH_H

#include "common.h"

struct Mesh {
  ~Mesh();

  std::vector<float> data;
  std::vector<unsigned int> indices;

  size_t size;

  //  size_t positionSize = 0;
  //  size_t normalSize = 0;
  //  size_t indicesSize = 0;
  //  size_t texcoordSize = 0;
  //  size_t colorSize = 0;

  //  float* positions = nullptr;
  //  float* normals = nullptr;
  //  size_t* indices = nullptr;
  //  float* texcoords = nullptr;
  //  float* colors = nullptr;

  void Clear();
};

void CreateSphere(unsigned int iLongitude, unsigned int iLatitude, Mesh& oMesh);

#endif  // MESH_H
