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

#ifndef _VECTOR3D_H_
#define _VECTOR3D_H_

#include <cmath>
#include "Matrix.h"

#define PI 3.141592654
#define TO_RADIANS(n) ((n) * PI / 180.0)
#define PSEUDO_ZERO 0.000001

template<class T>
class Vector3D
{
    public:
        Vector3D<T>();
        Vector3D<T>(const Vector3D<T>& inVector);
        Vector3D<T>(T inValue);
        Vector3D<T>(T inX, T inY, T inZ);
        Matrix<T> toMatrix() const;
        void set(T inValue);
        void set(T inX, T inY, T inZ);
        T get(int inIndex) const;
        void normalize();
        const Vector3D<T> normalized() const;
        bool isZero() const;
        void negate();
        float length() const;
        const Vector3D<T> negated() const;
        void processMatrix(const Matrix<T>& inMatrix);

        Vector3D<T>& operator=(const Vector3D<T>& inVector);
        Vector3D<T>& operator+=(const Vector3D<T>& inVector);
        Vector3D<T>& operator-=(const Vector3D<T>& inVector);
        const Vector3D<T> operator+(const Vector3D<T>& inVector) const;
        const Vector3D<T> operator-(const Vector3D<T>& inVector) const;
        const Vector3D<T> operator-() const;
        T operator*(const Vector3D<T>& inVector) const;
        const Vector3D<T> operator*(const T inRight) const;
        const Vector3D<T> operator^(const Vector3D<T>& inVector) const;
        bool operator==(const Vector3D<T>& inVector);
        T& operator[](int inIndex);
        T operator[](int inIndex) const;
        const T* array() const;

    private:
        T _vector[4];
};

template<class T>
Vector3D<T>::Vector3D()
{
    _vector[0] = 0;
    _vector[1] = 0;
    _vector[2] = 0;
    _vector[3] = 1;
}

template<class T>
Vector3D<T>::Vector3D(const Vector3D<T>& inVector)
{
    _vector[0] = inVector._vector[0];
    _vector[1] = inVector._vector[1];
    _vector[2] = inVector._vector[2];
    _vector[3] = inVector._vector[3];
}

template<class T>
Vector3D<T>::Vector3D(T inValue)
{
    _vector[0] = inValue;
    _vector[1] = inValue;
    _vector[2] = inValue;
    _vector[3] = 1;
}

template<class T>
Vector3D<T>::Vector3D(T inX, T inY, T inZ)
{
    _vector[0] = inX;
    _vector[1] = inY;
    _vector[2] = inZ;
    _vector[3] = 1;
}

template<class T>
Matrix<T> Vector3D<T>::toMatrix() const
{
    Matrix<T> outMatrix(4, 1);
    outMatrix[0] = _vector[0];
    outMatrix[1] = _vector[1];
    outMatrix[2] = _vector[2];
    outMatrix[3] = _vector[3];
    return outMatrix;
}

template<class T>
void Vector3D<T>::set(T inValue)
{
    _vector[0] = inValue;
    _vector[1] = inValue;
    _vector[2] = inValue;
}

template<class T>
void Vector3D<T>::set(T inX, T inY, T inZ)
{
    _vector[0] = inX;
    _vector[1] = inY;
    _vector[2] = inZ;
}

template<class T>
void Vector3D<T>::normalize()
{
    T length = sqrt((_vector[0] * _vector[0]) + (_vector[1] * _vector[1]) +
        (_vector[2] * _vector[2]));
    if (fabs(length) < PSEUDO_ZERO) return;
    _vector[0] /= length;
    _vector[1] /= length;
    _vector[2] /= length;
}

template<class T>
const Vector3D<T> Vector3D<T>::normalized() const
{
    Vector3D<T> outVector(*this);
    outVector.normalize();
    return outVector;
}

template<class T>
bool Vector3D<T>::isZero() const
{
    return fabs(_vector[0]) < PSEUDO_ZERO
        && fabs(_vector[1]) < PSEUDO_ZERO
        && fabs(_vector[2]) < PSEUDO_ZERO;
}

template<class T>
void Vector3D<T>::negate()
{
    _vector[0] = -_vector[0];
    _vector[1] = -_vector[1];
    _vector[2] = -_vector[2];
}

template<class T>
inline float Vector3D<T>::length() const
{
    return sqrt((_vector[0] * _vector[0]) + (_vector[1] * _vector[1]) +
                (_vector[2] * _vector[2]));
}

template<class T>
Vector3D<T>& Vector3D<T>::operator=(const Vector3D<T>& inVector)
{
    _vector[0] = inVector._vector[0];
    _vector[1] = inVector._vector[1];
    _vector[2] = inVector._vector[2];
    _vector[3] = inVector._vector[3];
    return *this;
}

template<class T>
Vector3D<T>& Vector3D<T>::operator+=(const Vector3D<T>& inVector)
{
    _vector[0] += inVector._vector[0];
    _vector[1] += inVector._vector[1];
    _vector[2] += inVector._vector[2];
    return *this;
}

template<class T>
Vector3D<T>& Vector3D<T>::operator-=(const Vector3D<T>& inVector)
{
    _vector[0] -= inVector._vector[0];
    _vector[1] -= inVector._vector[1];
    _vector[2] -= inVector._vector[2];
    return *this;
}

template<class T>
const Vector3D<T> Vector3D<T>::operator+(const Vector3D<T>& inVector) const
{
    Vector3D<T> outVector;
    outVector._vector[0] = _vector[0] + inVector._vector[0];
    outVector._vector[1] = _vector[1] + inVector._vector[1];
    outVector._vector[2] = _vector[2] + inVector._vector[2];
    return outVector;
}

template<class T>
const Vector3D<T> Vector3D<T>::operator-(const Vector3D<T>& inVector) const
{
    Vector3D<T> outVector;
    outVector._vector[0] = _vector[0] - inVector._vector[0];
    outVector._vector[1] = _vector[1] - inVector._vector[1];
    outVector._vector[2] = _vector[2] - inVector._vector[2];
    return outVector;
}

template<class T>
const Vector3D<T> Vector3D<T>::operator-() const
{
    Vector3D<T> outVector(*this);
    outVector.negate();
    return outVector;
}

template<class T>
T Vector3D<T>::operator*(const Vector3D<T>& inVector) const
{
    return (_vector[0] * inVector._vector[0])
        + (_vector[1] * inVector._vector[1])
        + (_vector[2] * inVector._vector[2]);
}

template<class T>
const Vector3D<T> Vector3D<T>::operator*(const T inRight) const
{
    Vector3D<T> outVector;
    outVector._vector[0] = _vector[0] * inRight;
    outVector._vector[1] = _vector[1] * inRight;
    outVector._vector[2] = _vector[2] * inRight;
    return outVector;
}

template<class T>
const Vector3D<T> Vector3D<T>::operator^(const Vector3D<T>& inVector) const
{
    Vector3D<T> outVector;
    outVector[0] = (_vector[1] * inVector._vector[2]) -
        (_vector[2] * inVector._vector[1]);
    outVector[1] = (_vector[2] * inVector._vector[0]) -
        (_vector[0] * inVector._vector[2]);
    outVector[2] = (_vector[0] * inVector._vector[1]) -
        (_vector[1] * inVector._vector[0]);
    return outVector;
}

template<class T>
bool Vector3D<T>::operator==(const Vector3D<T>& inVector)
{
    return _vector[0] == inVector._vector[0]
        && _vector[1] == inVector._vector[1]
        && _vector[2] == inVector._vector[2]
        && _vector[3] == inVector._vector[3];
}

template<class T>
T& Vector3D<T>::operator[](int inIndex)
{
    return _vector[inIndex];
}

template<class T>
T Vector3D<T>::operator[](int inIndex) const
{
    return _vector[inIndex];
}

template<class T>
T Vector3D<T>::get(int inIndex) const
{
    if (inIndex < 0 || inIndex > 2) return 0.0;
    return _vector[inIndex];
}

template<class T>
inline const T* Vector3D<T>::array() const
{
    return _vector;
}

template<class T>
void Vector3D<T>::processMatrix(const Matrix<T>& inMatrix)
{
    _vector[0] = _vector[0] * inMatrix[0] + _vector[1] * inMatrix[4]
        + _vector[2] * inMatrix[8] + _vector[3] * inMatrix[12];
    _vector[1] = _vector[0] * inMatrix[1] + _vector[1] * inMatrix[5]
        + _vector[2] * inMatrix[9] + _vector[3] * inMatrix[13];
    _vector[2] = _vector[0] * inMatrix[2] + _vector[1] * inMatrix[6]
        + _vector[2] * inMatrix[10] + _vector[3] * inMatrix[14];
    _vector[3] = _vector[0] * inMatrix[3] + _vector[1] * inMatrix[7]
        + _vector[2] * inMatrix[11] + _vector[3] * inMatrix[15];
}

template<class T>
istream& operator>>(istream& inStream, Vector3D<T>& inVector)
{
    char comma; // used strictly to help ignore delimiter commas
    T x;
    T y;
    T z;
    inStream >> x >> comma >> y >> comma >> z;
    inVector[0] = x;
    inVector[1] = y;
    inVector[2] = z;
    return inStream;
}

template<class T>
ostream& operator<<(ostream& inStream, const Vector3D<T>& inVector)
{
    inStream << '(' << inVector[0] << ", " << inVector[1] << ", "
        << inVector[2] << ')';
    return inStream;
}

#endif
