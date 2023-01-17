#include "mesh.h"

#include <cmath>

/*###################################################
##  函数: CreateSphere
##  函数描述： Generate sphere mesh
##  参数描述： 无
#####################################################*/
void CreateSphere(unsigned int iLongitude, unsigned int iLatitude,
                  Mesh& oMesh) {
  const float PI = 3.1415926535897f;

  oMesh.Clear();

  unsigned int n = (iLatitude + 1) * iLongitude;
  oMesh.size = n;

  oMesh.data.reserve(11 * n);
  oMesh.indices.reserve(6 * iLongitude * iLatitude);

  // positions, normals, texcoord, color
  // index

  for (unsigned int i = 0; i <= iLatitude; i++) {
    float y = std::cos(i * PI / iLatitude);

    for (unsigned int j = 0; j < iLongitude; j++) {
      float x = static_cast<float>(std::sin(i * PI / iLatitude) *
                                   std::sin(2 * j * PI / iLongitude));
      float z = static_cast<float>(std::sin(i * PI / iLatitude) *
                                   std::cos(2 * j * PI / iLongitude));
      // positions
      oMesh.data.push_back(x);
      oMesh.data.push_back(y);
      oMesh.data.push_back(z);

      // normals
      oMesh.data.push_back(x);
      oMesh.data.push_back(y);
      oMesh.data.push_back(z);

      // texcoord
      oMesh.data.push_back(static_cast<float>(i) /
                           static_cast<float>(iLatitude));
      oMesh.data.push_back(static_cast<float>(j) /
                           static_cast<float>(iLongitude));

      // color
      oMesh.data.push_back((x + 1.0f) / 2.0f);
      oMesh.data.push_back((y + 1.0f) / 2.0f);
      oMesh.data.push_back((z + 1.0f) / 2.0f);
    }
  }

  // index
  for (unsigned int i = 0; i < iLatitude; i++) {
    for (unsigned int j = 0; j < iLongitude; j++) {
      auto p1 = iLongitude * i + j;
      auto p2 = iLongitude * (i + 1) + j;
      auto p3 = iLongitude * (i + 1) + j + 1;
      auto p4 = iLongitude * i + j + 1;

      p1 = p1 % n;
      p2 = p2 % n;
      p3 = p3 % n;
      p4 = p4 % n;

      oMesh.indices.push_back(p4);
      oMesh.indices.push_back(p1);
      oMesh.indices.push_back(p2);
      oMesh.indices.push_back(p4);
      oMesh.indices.push_back(p2);
      oMesh.indices.push_back(p3);
    }
  }
}

Mesh::~Mesh() { Clear(); }

void Mesh::Clear() {
  indices.clear();
  data.clear();
  size = 0;
}
