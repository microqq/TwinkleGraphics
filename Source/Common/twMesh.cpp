
#include "twMesh.h"
#include "twConsoleLog.h"

namespace TwinkleGraphics {

void SubMesh::Initialize(int32 indiceNum) {
  if (indiceNum != 0 && _indice == nullptr) {
    _indiceNum = indiceNum;
    _indice = new glm::uint32[indiceNum];
  }
}

Mesh::~Mesh() {
  Console::LogInfo("Mesh Deconstructed.\n");

  _submeshes.clear();

  SAFE_DEL(_verticePos);
  SAFE_DEL(_verticeColor);
  SAFE_DEL(_verticeBinormal);
  SAFE_DEL(_verticeNormal);
  SAFE_DEL(_verticeUV);
  SAFE_DEL(_verticeUV1);
  SAFE_DEL(_verticeUV2);
  SAFE_DEL(_verticeUV3);
  SAFE_DEL(_verticeUV4);
  SAFE_DEL(_verticeUV5);
  SAFE_DEL(_verticeUV6);
  SAFE_DEL(_verticeUV7);
}

/**
 * @brief
 * 创建球面网格的基础算法（https://zh.wikipedia.org/wiki/%E7%90%83%E5%BA%A7%E6%A8%99%E7%B3%BB）
 * 球坐标系由点 p 到原点 o 的距离 r，极角（pole angle）和方位角（azimuth
 * angle）构成，若已知这三个变量，
 * 可通过球坐标系转直角坐标系求得其直角坐标（x,y,z）
 * @param radius
 * @param longitudeCount
 * @param latitudeCount
 * @return MeshPtr
 */
MeshPtr Mesh::CreateSphereMeshStandard(float32 radius, int32 longitudeCount,
                                       int32 latitudeCount) {
  int32 rowCount = latitudeCount - 1;
  int32 colCount = longitudeCount;
  int32 num = colCount * rowCount + 2;

  MeshPtr mesh = std::make_shared<Mesh>();
  mesh->Initialize(num, MeshDataFlag::DEFAULT);

  int indiceNum = 6 * colCount + (rowCount - 1) * colCount * 6;
  SubMeshPtr submesh = std::make_shared<SubMesh>();
  submesh->Initialize(indiceNum);
  mesh->AddSubMesh(submesh);

  // submesh->Initialize(num, 0, 0, MeshDataFlag::DEFAULT);

  float32 uStep = glm::two_pi<float32>() / (float32)(longitudeCount - 1);
  float32 vStep = glm::pi<float32>() / (float32)latitudeCount;

  // pole points
  mesh->_verticePos[0].x = 0.0f;
  mesh->_verticePos[0].y = radius;
  mesh->_verticePos[0].z = 0.0f;

  mesh->_verticePos[num - 1].x = 0.0f;
  mesh->_verticePos[num - 1].y = -radius;
  mesh->_verticePos[num - 1].z = 0.0f;

  // compute vertice position
  float32 x, y, z;
  for (int32 i = 0, k = 1; i < rowCount; i++) {
    y = radius * cos(vStep * (i + 1));
    for (int32 j = 0; j < colCount; j++) {
      x = radius * sin(vStep * (i + 1)) * cos(uStep * j);
      z = radius * sin(vStep * (i + 1)) * sin(uStep * j);

      mesh->_verticePos[k++] = glm::vec3(x, y, z);
    }
  }

  for (int32 i = 0, j = 0; i < num - 2; i++) {
    int32 row = i / colCount;
    int32 col = i % colCount;
    if (0 == row) {
      if (col == colCount - 1) {
        submesh->_indice[j] = row * colCount + col + 1;
        submesh->_indice[j + 1] = row * colCount + 1;
        submesh->_indice[j + 2] = 0;
      } else {
        submesh->_indice[j] = row * colCount + col + 1;
        submesh->_indice[j + 1] = row * colCount + col + 2;
        submesh->_indice[j + 2] = 0;
      }

      j += 3;
    } else {
      int32 top_row = row - 1;
      if (col == colCount - 1) {
        submesh->_indice[j] = row * colCount + col + 1;
        submesh->_indice[j + 1] = top_row * colCount + 1;
        submesh->_indice[j + 2] = top_row * colCount + 1;

        submesh->_indice[j + 3] = row * colCount + col + 1;
        submesh->_indice[j + 4] = row * colCount + 1;
        submesh->_indice[j + 5] = top_row * colCount + 1;
      } else {
        submesh->_indice[j] = row * colCount + col + 1;
        submesh->_indice[j + 1] = top_row * colCount + col + 2;
        submesh->_indice[j + 2] = top_row * colCount + col + 1;

        submesh->_indice[j + 3] = row * colCount + col + 1;
        submesh->_indice[j + 4] = row * colCount + col + 2;
        submesh->_indice[j + 5] = top_row * colCount + col + 2;
      }

      j += 6;

      if (row == rowCount - 1) {
        if (col == colCount - 1) {
          submesh->_indice[j] = row * colCount + col + 1;
          submesh->_indice[j + 1] = num - 1;
          submesh->_indice[j + 2] = row * colCount + 1;
        } else {
          submesh->_indice[j] = row * colCount + col + 1;
          submesh->_indice[j + 1] = num - 1;
          submesh->_indice[j + 2] = row * colCount + col + 2;
        }

        j += 3;
      }
    }
  }

  return mesh;
}

/**
 * @brief 单位立方体均匀细分法（uniform subdive normalized cube）
 * 根据细分程度递归细分立方体六个面，新的顶点归一化，最后将得到顶点坐标乘以球半径
 * r
 *
 * @param radius
 * @param subdivide
 * @return MeshPtr
 */
MeshPtr Mesh::CreateSphereMeshNormalizedCube(float32 radius, int32 subdivide) {
  int32 rowCount, colCount;
  rowCount = colCount = subdivide + 1;
  int32 num = rowCount * colCount * 6;

  MeshPtr mesh = std::make_shared<Mesh>();
  mesh->Initialize(num, MeshDataFlag::DEFAULT);

  int32 indiceNum = subdivide * subdivide * 36;
  SubMeshPtr submesh = std::make_shared<SubMesh>();
  submesh->Initialize(indiceNum);

  mesh->AddSubMesh(submesh);

  // cube faces
  glm::vec3 faces[24] = {
      /**
       * @brief
       * 0 __ __ __ 3
       *  |        |
       *  |        |
       *  |__ __ __|
       * 1          2
       */

      // front face
      glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, 0.5f),
      glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f),

      // left face
      glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f),
      glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, 0.5f),

      // back face
      glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f),
      glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, -0.5f),

      // right face
      glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f),
      glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, -0.5f),

      // top face
      glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, 0.5f),
      glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, -0.5f),

      // bottom face
      glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, -0.5f),
      glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, 0.5f)};

  // compute vertice & indice
  int32 verticeIndex = 0;
  int32 elementIndex = 0;
  int32 faceIndex = -1;
  float32 step = 1.0f / subdivide;
  for (int32 i = 0; i < 24; i += 4) {
    faceIndex = i / 4;
    verticeIndex = faceIndex * rowCount * colCount;
    for (int32 j = 0; j < rowCount; j++) {
      for (int32 k = 0; k < colCount; k++) {
        // double linear interpolation
        glm::vec3 colTemp1 =
            (faces[i + 3] - faces[i]) * (float32)k * step + faces[i];
        glm::vec3 colTemp2 =
            (faces[i + 2] - faces[i + 1]) * (float32)k * step + faces[i + 1];
        glm::vec3 coord =
            (colTemp2 - colTemp1) * (float32)j * step + colTemp1;
        coord = glm::normalize(coord);
        coord *= radius;

        // submesh->_indice[elementIndex++] = verticeIndex;

        if (j != rowCount - 1 && k != colCount - 1) {
          submesh->_indice[elementIndex] = verticeIndex;
          submesh->_indice[elementIndex + 1] = verticeIndex + colCount;
          submesh->_indice[elementIndex + 2] = verticeIndex + colCount + 1;

          submesh->_indice[elementIndex + 3] = verticeIndex;
          submesh->_indice[elementIndex + 4] = verticeIndex + colCount + 1;
          submesh->_indice[elementIndex + 5] = verticeIndex + 1;

          elementIndex += 6;
        }

        mesh->_verticePos[verticeIndex++] = coord;
      }
    }
  }

  return mesh;
}

/**
 * @brief 细分Icosahedron
 * Icoshahedron是一个12个顶点的20面体，细分的方法是每一个三角形拆解成四个三角形
 *
 *         /\               /\
 *        /  \             /__\
 *       /    \   -----   /\  /\
 *      /__  __\         /__\/__\
 *
 * 球坐标系下， 首先生成一个正二十面体， 除两个极点（pole point）外，
 * 其他十个顶点根据极角 ((+26.565°/-26.565°)（tan^(-1)(0.5)) 和 方位角 72° * n
 * (0 <= n <= 5) 生成
 *
 * @param radius
 * @param subdivide
 * @return MeshPtr
 */
MeshPtr Mesh::CreateSphereMeshIcosahedron(float32 radius, int32 subdivide) {
  int32 num = 20 * ((subdivide + 1) + (subdivide + 1) * (subdivide) / 2);
  MeshPtr mesh = std::make_shared<Mesh>();
  mesh->Initialize(num, MeshDataFlag::DEFAULT);

  int32 indiceNum = 60 * subdivide * subdivide;
  SubMeshPtr submesh = std::make_shared<SubMesh>();
  submesh->Initialize(indiceNum);
  mesh->AddSubMesh(submesh);

  glm::vec3 icoVertice[12] = {};
  uint32 icotriangleIndice[60] = {};
  CreateIconsahedron(icoVertice, icotriangleIndice, 1.0f);
  // CreateIconsahedron(submesh->GetVerticePos(), submesh->GetIndice(), radius);

  int32 verticeIndex = 0;
  int32 indiceIndex = 0;
  glm::vec3 p0, p1, p2, p, pHelper1, pHelper2;
  glm::vec3 edge0, edge1;
  for (int32 i = 0; i < 60; i += 3) {
    p0 = icoVertice[icotriangleIndice[i]];
    p1 = icoVertice[icotriangleIndice[i + 1]];
    p2 = icoVertice[icotriangleIndice[i + 2]];

    edge0 = p1 - p0;
    edge1 = p2 - p0;

    for (int32 row = 0, rowCount = subdivide + 1; row < rowCount; row++) {
      pHelper1 = p0 + edge0 * (float32)row / (float32)subdivide;
      pHelper2 = p0 + edge1 * (float32)row / (float32)subdivide;

      int32 col = 0;
      int32 col_count = row + 1;
      for (; col < col_count; col++) {
        if (row == 0) {
          p = pHelper1;
        } else {
          p = pHelper1 + (pHelper2 - pHelper1) * (float32)col / (float32)row;
        }
        p = glm::normalize(p);
        p *= radius;

        if (row != 0) {
          if (col != 0) {
            submesh->_indice[indiceIndex] = verticeIndex + col;
            submesh->_indice[indiceIndex + 1] = verticeIndex + col - row - 1;
            submesh->_indice[indiceIndex + 2] = verticeIndex + col - 1;

            indiceIndex += 3;

            if (col != col_count - 1) {
              submesh->_indice[indiceIndex] = verticeIndex + col;
              submesh->_indice[indiceIndex + 1] = verticeIndex + col - row;
              submesh->_indice[indiceIndex + 2] =
                  verticeIndex + col - row - 1;

              indiceIndex += 3;
            }
          }
        }

        // submesh->_indice[indiceIndex] = indiceIndex++;

        mesh->_verticePos[verticeIndex + col] = p;
      }

      verticeIndex += col_count;
    }
  }

  return mesh;
}

void Mesh::CreateIconsahedron(glm::vec3 *vertice, uint32 *indice,
                              float32 radius) {
  vertice[0] = glm::vec3(0.0f, radius, 0.0f);
  vertice[11] = glm::vec3(0.0f, -radius, 0.0f);

  indice[0] = 0;
  indice[11] = 11;

  float32 poleAngle = acot(0.5f);
  float32 azimuthStep = glm::radians<float32>(72.0f);
  float32 cosPoleAngle = cos(poleAngle);
  float32 sinPoleAngle = sin(poleAngle);

  float32 cosHalfAzimuth = cos(-azimuthStep * 0.5f);
  float32 sinHalfAzimuth = sin(-azimuthStep * 0.5f);

  float32 x = radius * sinPoleAngle;
  float32 y = radius * cosPoleAngle;
  float32 z = x;
  for (int32 i = 0, j = 5; j < 10; i++, j++) {
    float32 cosAzimuth = cos(azimuthStep * i);
    float32 sinAzimuth = sin(azimuthStep * i);

    float32 newX = x * cosAzimuth;
    float32 newZ = z * sinAzimuth;

    // float32 newX = x * cosAzimuth + z * sinAzimuth;
    // float32 newZ = -x * sinAzimuth + z * cosAzimuth;

    vertice[i + 1] = glm::vec3(newX, y, newZ);

    float32 newX1 = newX * cosHalfAzimuth - newZ * sinHalfAzimuth;
    float32 newZ1 = newX * sinHalfAzimuth + newZ * cosHalfAzimuth;

    vertice[j + 1] = glm::vec3(newX1, -y, newZ1);

    // if(j == 5)
    //     vertice[j + 1] = glm::vec3(newX1, -y, newZ1);
    // else
    //     vertice[16 - j] = glm::vec3(newX1, -y, newZ1);
  }

  for (int32 i = 0, j = 0; i < 10; i++) {
    if (i < 5) {
      indice[j] = 0;
      if (i == 4)
        indice[j + 1] = 1;
      else
        indice[j + 1] = i + 2;
      indice[j + 2] = i + 1;

      j += 3;
    } else {
      indice[j] = 11;
      indice[j + 1] = i + 1;
      if (i == 9)
        indice[j + 2] = 6;
      else
        indice[j + 2] = i + 2;

      j += 3;
    }
  }

  for (int32 i = 0, j = 30; i < 5; i++) {
    if (i != 0) {
      indice[j] = i + 1;
      indice[j + 1] = i + 6;
      indice[j + 2] = i;

      indice[j + 3] = i + 1;
      if (i != 4)
        indice[j + 4] = i + 7;
      else
        indice[j + 4] = 6;
      indice[j + 5] = i + 6;
    } else {
      indice[j] = 1;
      indice[j + 1] = 6;
      indice[j + 2] = 5;

      indice[j + 3] = 1;
      indice[j + 4] = 7;
      indice[j + 5] = 6;
    }

    j += 6;
  }
}

MeshPtr Mesh::CreateQuadMesh(float32 x_size, float32 y_size) {
  MeshPtr mesh = std::make_shared<Mesh>();
  mesh->Initialize(4, MeshDataFlag::DEFAULT);

  SubMeshPtr submesh = std::make_shared<SubMesh>();
  submesh->Initialize(6);
  mesh->AddSubMesh(submesh);

  float32 halfXSize = x_size * 0.5f;
  float32 halfYSize = y_size * 0.5f;

  int32 indiceNum = 6;
  submesh->_indiceNum = indiceNum;
  submesh->_indice = new uint32[indiceNum]{};
  submesh->_indice[0] = 0;
  submesh->_indice[1] = 1;
  submesh->_indice[2] = 2;

  submesh->_indice[3] = 0;
  submesh->_indice[4] = 2;
  submesh->_indice[5] = 3;

  mesh->_verticePos[0] = glm::vec3(-halfXSize, halfYSize, 0.0f);
  mesh->_verticePos[1] = glm::vec3(-halfXSize, -halfYSize, 0.0f);
  mesh->_verticePos[2] = glm::vec3(halfXSize, -halfYSize, 0.0f);
  mesh->_verticePos[3] = glm::vec3(halfXSize, halfYSize, 0.0f);

  return mesh;
}

MeshPtr Mesh::CreateCubeMesh(float32 size) {
  MeshPtr mesh = std::make_shared<Mesh>();
  mesh->Initialize(8, MeshDataFlag::DEFAULT);

  SubMeshPtr submesh = std::make_shared<SubMesh>();
  submesh->Initialize(36);
  mesh->AddSubMesh(submesh);

  int32 index = 0;
  // front
  submesh->_indice[index++] = 0;
  submesh->_indice[index++] = 1;
  submesh->_indice[index++] = 2;

  submesh->_indice[index++] = 0;
  submesh->_indice[index++] = 2;
  submesh->_indice[index++] = 3;

  // left
  submesh->_indice[index++] = 4;
  submesh->_indice[index++] = 5;
  submesh->_indice[index++] = 1;

  submesh->_indice[index++] = 4;
  submesh->_indice[index++] = 1;
  submesh->_indice[index++] = 0;

  // right
  submesh->_indice[index++] = 3;
  submesh->_indice[index++] = 2;
  submesh->_indice[index++] = 6;

  submesh->_indice[index++] = 3;
  submesh->_indice[index++] = 6;
  submesh->_indice[index++] = 7;

  // back
  submesh->_indice[index++] = 7;
  submesh->_indice[index++] = 6;
  submesh->_indice[index++] = 5;

  submesh->_indice[index++] = 7;
  submesh->_indice[index++] = 5;
  submesh->_indice[index++] = 4;

  // top
  submesh->_indice[index++] = 4;
  submesh->_indice[index++] = 0;
  submesh->_indice[index++] = 3;

  submesh->_indice[index++] = 4;
  submesh->_indice[index++] = 3;
  submesh->_indice[index++] = 7;

  // bottom
  submesh->_indice[index++] = 1;
  submesh->_indice[index++] = 5;
  submesh->_indice[index++] = 6;

  submesh->_indice[index++] = 1;
  submesh->_indice[index++] = 6;
  submesh->_indice[index++] = 2;

  float32 halfXSize = size * 0.5f;
  float32 halfYSize = size * 0.5f;
  float32 half_z_size = size * 0.5f;

  mesh->_verticePos[0] = glm::vec3(-halfXSize, halfYSize, half_z_size);
  mesh->_verticePos[1] = glm::vec3(-halfXSize, -halfYSize, half_z_size);
  mesh->_verticePos[2] = glm::vec3(halfXSize, -halfYSize, half_z_size);
  mesh->_verticePos[3] = glm::vec3(halfXSize, halfYSize, half_z_size);

  mesh->_verticePos[4] = glm::vec3(-halfXSize, halfYSize, -half_z_size);
  mesh->_verticePos[5] = glm::vec3(-halfXSize, -halfYSize, -half_z_size);
  mesh->_verticePos[6] = glm::vec3(halfXSize, -halfYSize, -half_z_size);
  mesh->_verticePos[7] = glm::vec3(halfXSize, halfYSize, -half_z_size);

  return mesh;
}

/**
 * @brief use geometry shader render line mesh
 *
 * @param points
 * @param num
 * @return MeshPtr
 */
MeshPtr Mesh::CreateLineMesh(glm::vec3 *points, int32 num) {
  MeshPtr mesh = std::make_shared<Mesh>();
  mesh->Initialize(num, MeshDataFlag::DEFAULT);

  // use line_adjency
  int32 indiceNum = (num - 1) * 4;
  SubMeshPtr submesh = std::make_shared<SubMesh>();
  submesh->Initialize(indiceNum);
  mesh->AddSubMesh(submesh);

  int32 indiceIndex = 0;
  for (int32 i = 0; i < num - 1; i++) {
    if (i == 0) {
      submesh->_indice[indiceIndex++] = 0;
    } else {
      submesh->_indice[indiceIndex++] = i - 1;
    }

    submesh->_indice[indiceIndex++] = i;
    submesh->_indice[indiceIndex++] = i + 1;

    if (i == num - 2) {
      submesh->_indice[indiceIndex++] = num - 1;
    } else {
      submesh->_indice[indiceIndex++] = i + 2;
    }
  }

  if (points != nullptr) {
    for (int32 i = 0; i < num; i++) {
      mesh->_verticePos[i] = points[i];
    }
  }

  return mesh;
}

MeshPtr Mesh::CreateLineMeshEx(glm::vec4 *points, int32 num) {
  MeshPtr mesh = std::make_shared<Mesh>();
  mesh->Initialize(num, MeshDataFlag::DEFAULT);

  // use line_adjency
  int32 indiceNum = (num - 1) * 4;
  SubMeshPtr submesh = std::make_shared<SubMesh>();
  submesh->Initialize(indiceNum);
  mesh->AddSubMesh(submesh);

  int32 indiceIndex = 0;
  for (int32 i = 0; i < num - 1; i++) {
    if (i == 0) {
      submesh->_indice[indiceIndex++] = 0;
    } else {
      submesh->_indice[indiceIndex++] = i - 1;
    }

    submesh->_indice[indiceIndex++] = i;
    submesh->_indice[indiceIndex++] = i + 1;

    if (i == num - 2) {
      submesh->_indice[indiceIndex++] = num - 1;
    } else {
      submesh->_indice[indiceIndex++] = i + 2;
    }
  }

  if (points != nullptr) {
    for (int32 i = 0; i < num; i++) {
      mesh->_verticePos[i] = glm::vec3(points[i]);
    }
  }

  return mesh;
}

MeshPtr Mesh::CreateBezierLine(glm::vec3 *points, int32 num, int32 segments) {
  MeshPtr mesh = CreateLineMesh(nullptr, segments + 1);

  SubMeshPtr submesh = mesh->GetSubMesh(0);
  glm::vec3 *vertices = mesh->GetVerticePos();
  vertices[0] = points[0];
  vertices[segments] = points[num - 1];

  float32 uStep = 1.0f / (float32)segments;

  glm::vec3 *pointsHelper = new glm::vec3[num];
  for (int32 i = 1; i < segments; i++) {
    // memcpy((void *)pointsHelper, (void *)points, sizeof(glm::vec3) * num);
    vertices[i] = DeCasteljau(points, pointsHelper, num, uStep * i);

    // std::cout << "------------------------" << std::endl;
    // std::cout << "x:" << results[i].x << std::endl;
    // std::cout << "y:" << results[i].y << std::endl;
    // std::cout << "z:" << results[i].z << std::endl;
  }

  SAFE_DEL_ARR(pointsHelper);

  return mesh;
}

/**
 * @brief (refer http://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/)
 *
 * @param points
 * @param num
 * @param segments
 * @return MeshPtr
 */
MeshPtr Mesh::CreateQuadraticBezierLine(glm::vec3 *points, int32 segments) {
  return CreateBezierLine(points, 3, segments);
}

/**
 * @brief (refer http://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/)
 *
 * @param points
 * @param num
 * @param segments
 * @return MeshPtr
 */
MeshPtr Mesh::CreateCubicBezierLine(glm::vec3 *points, int32 segments) {
  return CreateBezierLine(points, 4, segments);
}

/**
 * @brief (refer http://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/)
 *
 * @param points
 * @param num
 * @param u
 * @return glm::vec3
 */
glm::vec3 Mesh::DeCasteljau(glm::vec3 *points, glm::vec3 *helper, int32 num,
                            float32 u) {
  int32 pointCount = num - 1;
  for (int32 i = 0, iteration = num - 1; i < iteration; i++) {
    for (int32 j = 0; j < pointCount; j++) {
      if (i == 0)
        helper[j] = (1.0f - u) * points[j] + u * points[j + 1];
      else {
        helper[j] = (1.0f - u) * helper[j] + u * helper[j + 1];
      }
    }
    --pointCount;
  }

  return helper[0];
}

} // namespace TwinkleGraphics
