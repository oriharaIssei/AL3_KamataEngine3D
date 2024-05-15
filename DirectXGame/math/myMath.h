#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include <Matrix4x4.h>

#include <vector>

float Lerp(const float &t, const float &start, const float &end);
Vector2 Lerp(const float &t, const Vector2 &start, const Vector2 &end);
Vector3 Lerp(const float &t, const Vector3 &start, const Vector3 &end);

Vector3 Slerp(const float &t, const Vector3 &start, const Vector3 &end);

Vector3 CatmullRomInterpolation(const Vector3 &p0, const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, float t);
Vector3 CatmullRomInterpolation(const std::vector<Vector3> &points, float t);

Matrix4x4 InverseRenderingPipelineMatrix(const Matrix4x4 &view, const Matrix4x4 &projection, const Matrix4x4 &viewPort);
Vector3 ScreenToWorld(const Vector3 &screenPos, const Matrix4x4 &view, const Matrix4x4 &projection, const Matrix4x4 &viewPort);