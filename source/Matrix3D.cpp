/**
 *  This file is part of Zero2D.
 *
 *  Zero2D is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Zero2D is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Zero2D.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Matrix3D.h"

#include <iomanip>
using namespace std;

const float Matrix3D::mIdentity[16] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f};

Matrix3D::Matrix3D()
{
    // default to identity matrix
    loadIdentity();
}

Matrix3D::Matrix3D(const Matrix3D& inMatrix)
{
    copy(inMatrix);
}

void Matrix3D::loadIdentity()
{
    memcpy(mData, mIdentity, 16 * sizeof(float));
}

void Matrix3D::rotate(float inDegrees, float inX, float inY, float inZ)
{
    /// arbitrary rotation about an axis
    /// http://www.opengl.org/sdk/docs/man/xhtml/glRotate.xml

    float r = TO_RADIANS(inDegrees);
    float c = cos(r);
    float ci = 1.0f - c;
    float s = sin(r);
    Matrix3D transform;

    transform[0] = inX * inX * ci + c;
    transform[4] = inX * inY * ci - (inZ * s);
    transform[8] = inX * inZ * ci + (inY * s);
    transform[1] = inY * inX * ci + (inZ * s);
    transform[5] = inY * inY * ci + c;
    transform[9] = inY * inZ * ci - (inX * s);
    transform[2] = inX * inZ * ci - (inY * s);
    transform[6] = inY * inZ * ci + (inX * s);
    transform[10] = inZ * inZ * ci + c;

    multiply(transform);
}

void Matrix3D::rotateX(float inDegrees)
{
    float r = TO_RADIANS(inDegrees);
    float c = cos(r);
    float s = sin(r);
    Matrix3D transform;

    transform[5] = c;
    transform[9] = -s;
    transform[6] = s;
    transform[10] = c;

    multiply(transform);
}

void Matrix3D::rotateY(float inDegrees)
{
    float r = TO_RADIANS(inDegrees);
    float c = cos(r);
    float s = sin(r);
    Matrix3D transform;

    transform[0] = c;
    transform[8] = s;
    transform[2] = -s;
    transform[10] = c;

    multiply(transform);
}

void Matrix3D::rotateZ(float inDegrees)
{
    float r = TO_RADIANS(inDegrees);
    float c = cos(r);
    float s = sin(r);
    Matrix3D transform;

    transform[0] = c;
    transform[4] = -s;
    transform[1] = s;
    transform[5] = c;

    multiply(transform);
}

void Matrix3D::scale(float inScale)
{
    Matrix3D transform;

    transform[0] = inScale;
    transform[5] = inScale;
    transform[10] = inScale;

    multiply(transform);
}

void Matrix3D::scale(float inX, float inY, float inZ)
{
    Matrix3D transform;

    transform[0] = inX;
    transform[5] = inY;
    transform[10] = inZ;

    multiply(transform);
}

void Matrix3D::translate(float inX, float inY, float inZ)
{
    Matrix3D transform;

    transform[12] = inX;
    transform[13] = inY;
    transform[14] = inZ;

    multiply(transform);
}

void Matrix3D::frustrum(float inLeft, float inRight, float inBottom,
    float inTop, float inNear, float inFar)
{
    Matrix3D transform;

    transform[0] = (2 * inNear) / (inRight - inLeft);
    transform[8] = (inRight + inLeft) / (inRight - inLeft);
    transform[5] = (2 * inNear) / (inTop - inBottom);
    transform[9] = (inTop + inBottom) / (inTop - inBottom);
    transform[10] = (inFar + inNear) / (inNear - inFar);
    transform[14] = (2.0f * inFar * inNear) / (inNear - inFar);
    transform[11] = -1.0f;
    transform[15] = 0.0f;

    multiply(transform);
}

void Matrix3D::perspective(float inFieldOfView, float inRatio, float inNear,
    float inFar)
{
    /// adaptation of gluPerspective
    /// http://www.opengl.org/sdk/docs/man/xhtml/gluPerspective.xml
    float r = TO_RADIANS(inFieldOfView);
    float f = 1.0f / tan(r / 2.0f);

    Matrix3D transform;

    transform[0] = f / inRatio;
    transform[5] = f;
    transform[10] = (inFar + inNear) / (inNear - inFar);
    transform[14] = (2 * inFar * inNear) / (inNear - inFar);
    transform[11] = -1.0f;
    transform[15] = 0.0f;

    multiply(transform);
}

void Matrix3D::orthographic(float inLeft, float inRight, float inBottom,
    float inTop, float inNear, float inFar)
{
    Matrix3D transform;

    transform[0] = 2.0f / (inRight - inLeft);
    transform[12] = (inRight + inLeft) / (inRight - inLeft);
    transform[5] = 2.0f / (inTop - inBottom);
    transform[13] = (inTop + inBottom) / (inTop - inBottom);
    transform[10] = 2.0f / (inNear - inFar);
    transform[14] = (inFar + inNear) / (inFar - inNear);

    multiply(transform);
}

void Matrix3D::orthographic(float inRange, float inRatio)
{
    if (inRatio < 1.0f)
    {
        orthographic(-inRange, inRange, -inRange / inRatio, inRange / inRatio,
            -10.0f, 10.0f);
    }
    else
    {
        orthographic(-inRange * inRatio, inRange * inRatio, -inRange, inRange,
            -10.0f, 10.0f);
    }
}

Matrix3D& Matrix3D::operator=(const Matrix3D& inMatrix)
{
    if (this != &inMatrix) copy(inMatrix);
    return *this;
}

//Matrix3D& Matrix3D::operator*=(const Matrix3D& inMatrix)
void Matrix3D::multiply(const Matrix3D& inMatrix)
{
    Matrix3D result;

    // keep for reference
//    for (size_t i = 0; i < 4; ++i)
//    {
//        for (size_t j = 0; j < 4; ++j)
//        {
//            float value = 0.0f;
//            for (size_t k = 0; k < 4; ++k)
//                value += (*this)(i, k) * inMatrix(k, j);
//
//            result(i, j) = value;
//        }
//    }

    result[0] = (mData[0] * inMatrix[0]) + (mData[4] * inMatrix[1])
        + (mData[8] * inMatrix[2]) + (mData[12] * inMatrix[3]);
    result[4] = (mData[0] * inMatrix[4]) + (mData[4] * inMatrix[5])
        + (mData[8] * inMatrix[6]) + (mData[12] * inMatrix[7]);
    result[8] = (mData[0] * inMatrix[8]) + (mData[4] * inMatrix[9])
        + (mData[8] * inMatrix[10]) + (mData[12] * inMatrix[11]);
    result[12] = (mData[0] * inMatrix[12]) + (mData[4] * inMatrix[13])
        + (mData[8] * inMatrix[14]) + (mData[12] * inMatrix[15]);
    result[1] = (mData[1] * inMatrix[0]) + (mData[5] * inMatrix[1])
        + (mData[9] * inMatrix[2]) + (mData[13] * inMatrix[3]);
    result[5] = (mData[1] * inMatrix[4]) + (mData[5] * inMatrix[5])
        + (mData[9] * inMatrix[6]) + (mData[13] * inMatrix[7]);
    result[9] = (mData[1] * inMatrix[8]) + (mData[5] * inMatrix[9])
        + (mData[9] * inMatrix[10]) + (mData[13] * inMatrix[11]);
    result[13] = (mData[1] * inMatrix[12]) + (mData[5] * inMatrix[13])
        + (mData[9] * inMatrix[14]) + (mData[13] * inMatrix[15]);
    result[2] = (mData[2] * inMatrix[0]) + (mData[6] * inMatrix[1])
        + (mData[10] * inMatrix[2]) + (mData[14] * inMatrix[3]);
    result[6] = (mData[2] * inMatrix[4]) + (mData[6] * inMatrix[5])
        + (mData[10] * inMatrix[6]) + (mData[14] * inMatrix[7]);
    result[10] = (mData[2] * inMatrix[8]) + (mData[6] * inMatrix[9])
        + (mData[10] * inMatrix[10]) + (mData[14] * inMatrix[11]);
    result[14] = (mData[2] * inMatrix[12]) + (mData[6] * inMatrix[13])
        + (mData[10] * inMatrix[14]) + (mData[14] * inMatrix[15]);
    result[3] = (mData[3] * inMatrix[0]) + (mData[7] * inMatrix[1])
        + (mData[11] * inMatrix[2]) + (mData[15] * inMatrix[3]);
    result[7] = (mData[3] * inMatrix[4]) + (mData[7] * inMatrix[5])
        + (mData[11] * inMatrix[6]) + (mData[15] * inMatrix[7]);
    result[11] = (mData[3] * inMatrix[8]) + (mData[7] * inMatrix[9])
        + (mData[11] * inMatrix[10]) + (mData[15] * inMatrix[11]);
    result[15] = (mData[3] * inMatrix[12]) + (mData[7] * inMatrix[13])
        + (mData[11] * inMatrix[14]) + (mData[15] * inMatrix[15]);


/** The program below was used to generate the code above.

#include <iostream>
#include <fstream>
using namespace std;

size_t toIndex(size_t inRow, size_t inCol)
{
    return inCol * 4 + inRow;
}

int main(int argc, char** argv)
{
    ofstream fout;
    fout.open("code.txt");
    for (size_t i = 0; i < 4; ++i)
    {
        for (size_t j = 0; j < 4; ++j)
        {
            fout << "result[" << toIndex(i, j) << "] =";
            for (size_t k = 0; k < 4; ++k)
            {
                if (k > 0) fout << " +";
                fout << " (mData[" << toIndex(i, k) << "] * inMatrix["
                    << toIndex(k, j) << "])";
            }
            fout << ';' << endl;
        }
    }
    fout.close();
    return 0;
}

**/

    copy(result);
}

Matrix3D Matrix3D::inverse()
{
    Matrix3D outMatrix;
    inverse(outMatrix);
    return outMatrix;
}

void Matrix3D::inverse(Matrix3D& inMatrix)
{
#define SWAP_ROWS(a, b) { float *_tmp = a; (a)=(b); (b)=_tmp; }

        float wtmp[4][8];
        float m0, m1, m2, m3, s;
        float *r0, *r1, *r2, *r3;

        r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];

        r0[0] = at(0, 0), r0[1] = at(0, 1),
        r0[2] = at(0, 2), r0[3] = at(0, 3),
        r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,
        r1[0] = at(1, 0), r1[1] = at(1, 1),
        r1[2] = at(1, 2), r1[3] = at(1, 3),
        r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,
        r2[0] = at(2, 0), r2[1] = at(2, 1),
        r2[2] = at(2, 2), r2[3] = at(2, 3),
        r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,
        r3[0] = at(3, 0), r3[1] = at(3, 1),
        r3[2] = at(3, 2), r3[3] = at(3, 3),
        r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;

        if (fabs(r3[0]) > fabs(r2[0])) SWAP_ROWS(r3, r2);
        if (fabs(r2[0]) > fabs(r1[0])) SWAP_ROWS(r2, r1);
        if (fabs(r1[0]) > fabs(r0[0])) SWAP_ROWS(r1, r0);
        if (0.0 == r0[0]) return;

        m1 = r1[0] / r0[0];
        m2 = r2[0] / r0[0];
        m3 = r3[0] / r0[0];
        s = r0[1];
        r1[1] -= m1 * s;
        r2[1] -= m2 * s;
        r3[1] -= m3 * s;
        s = r0[2];
        r1[2] -= m1 * s;
        r2[2] -= m2 * s;
        r3[2] -= m3 * s;
        s = r0[3];
        r1[3] -= m1 * s;
        r2[3] -= m2 * s;
        r3[3] -= m3 * s;
        s = r0[4];

        if (s != 0.0)
        {
            r1[4] -= m1 * s;
            r2[4] -= m2 * s;
            r3[4] -= m3 * s;
        }
        s = r0[5];
        if (s != 0.0)
        {
            r1[5] -= m1 * s;
            r2[5] -= m2 * s;
            r3[5] -= m3 * s;
        }
        s = r0[6];
        if (s != 0.0)
        {
            r1[6] -= m1 * s;
            r2[6] -= m2 * s;
            r3[6] -= m3 * s;
        }
        s = r0[7];
        if (s != 0.0)
        {
            r1[7] -= m1 * s;
            r2[7] -= m2 * s;
            r3[7] -= m3 * s;
        }

        if (fabs(r3[1]) > fabs(r2[1])) SWAP_ROWS(r3, r2);
        if (fabs(r2[1]) > fabs(r1[1])) SWAP_ROWS(r2, r1);
        if (0.0 == r1[1]) return;

        m2 = r2[1] / r1[1];
        m3 = r3[1] / r1[1];
        r2[2] -= m2 * r1[2];
        r3[2] -= m3 * r1[2];
        r2[3] -= m2 * r1[3];
        r3[3] -= m3 * r1[3];
        s = r1[4];
        if (0.0 != s)
        {
            r2[4] -= m2 * s;
            r3[4] -= m3 * s;
        }
        s = r1[5];
        if (0.0 != s)
        {
            r2[5] -= m2 * s;
            r3[5] -= m3 * s;
        }
        s = r1[6];
        if (0.0 != s)
        {
            r2[6] -= m2 * s;
            r3[6] -= m3 * s;
        }
        s = r1[7];
        if (0.0 != s)
        {
            r2[7] -= m2 * s;
            r3[7] -= m3 * s;
        }

        if (fabs(r3[2]) > fabs(r2[2])) SWAP_ROWS(r3, r2);
        if (0.0 == r2[2]) return;

        m3 = r3[2] / r2[2];
        r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
        r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6], r3[7] -= m3 * r2[7];

        if (0.0 == r3[3]) return;

        s = 1.0 / r3[3];
        r3[4] *= s;
        r3[5] *= s;
        r3[6] *= s;
        r3[7] *= s;

        m2 = r2[3];
        s = 1.0 / r2[2];
        r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
        r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
        m1 = r1[3];
        r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
        r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
        m0 = r0[3];
        r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
        r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;

        m1 = r1[2];
        s = 1.0 / r1[1];
        r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
        r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
        m0 = r0[2];
        r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
        r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;

        m0 = r0[1];
        s = 1.0 / r0[0];
        r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
        r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);

        inMatrix(0, 0) = r0[4];
        inMatrix(0, 1) = r0[5];
        inMatrix(0, 2) = r0[6];
        inMatrix(0, 3) = r0[7];
        inMatrix(1, 0) = r1[4];
        inMatrix(1, 1) = r1[5];
        inMatrix(1, 2) = r1[6];
        inMatrix(1, 3) = r1[7];
        inMatrix(2, 0) = r2[4];
        inMatrix(2, 1) = r2[5];
        inMatrix(2, 2) = r2[6];
        inMatrix(2, 3) = r2[7];
        inMatrix(3, 0) = r3[4];
        inMatrix(3, 1) = r3[5];
        inMatrix(3, 2) = r3[6];
        inMatrix(3, 3) = r3[7];
}

ostream& operator<<(ostream& inStream, const Matrix3D& inMatrix)
{
    inStream << setprecision(2);
    for (size_t i = 0; i < 4; ++i)
    {
        if (i) inStream << endl;
        for (size_t j = 0; j < 4; ++j)
        {
            inStream << setw(6) << inMatrix(i, j);
        }
    }

    return inStream;
}
