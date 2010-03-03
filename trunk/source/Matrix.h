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

#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iomanip>
using namespace std;

template<class T>
class Matrix
{
    public:
        Matrix();
        Matrix(const Matrix<T>& inMatrix);
        Matrix(int inRows, int inCols);
        Matrix(int inEdge);
        virtual ~Matrix();

        int rows() const;
        int cols() const;
        int size() const;
        int lastRow() const;
        int lastCol() const;
        bool square() const;
        T at(int inIndex) const;
        T at(int inRow, int inCol) const;
        int toIndex(int inRow, int inCol) const;
        void set(int inRow, int inCol, T inValue);
        T determinant() const;
        Matrix<T> subMatrix(int inRow, int inCol, int inHeight, int inWidth);
        Matrix<T> minorMatrix(int inRow, int inCol) const;
        void transpose();
        const Matrix<T> transposed();

        Matrix<T>& operator=(const Matrix<T>& inMatrix);
        Matrix<T>& operator+=(const Matrix<T>& inMatrix);
        Matrix<T>& operator-=(const Matrix<T>& inMatrix);
        Matrix<T>& operator*=(const Matrix<T>& inMatrix);
        Matrix<T>& operator+=(const T inValue);
        Matrix<T>& operator-=(const T inValue);
        Matrix<T>& operator*=(const T inValue);
        Matrix<T>& operator/=(const T inValue);
        T& operator()(int inRow, int inCol);
        T operator()(int inRow, int inCol) const;
        T& operator[](int inIndex);
        T operator[](int inIndex) const;
        bool operator==(const Matrix<T>& inMatrix) const;
        bool operator!=(const Matrix<T>& inMatrix) const;
        const Matrix<T> operator+(const Matrix<T>& inMatrix) const;
        const Matrix<T> operator-(const Matrix<T>& inMatrix) const;
        const Matrix<T> operator*(const Matrix<T>& inMatrix) const;
        const Matrix<T> operator+(const T inValue) const;
        const Matrix<T> operator-(const T inValue) const;
        const Matrix<T> operator*(const T inValue) const;
        const Matrix<T> operator/(const T inValue) const;
        T* array();

    private:
        void copy(const Matrix<T>& inMatrix);

        int _rows;
        int _cols;
        int _size;
        T* _matrix;
};

template<class T>
Matrix<T>::Matrix()
{
    srand(time(NULL));
    int edge = rand() % 8 + 2;
    _rows = edge;
    _cols = edge;
    _size = edge * edge;
    _matrix = new T[_size];

    for (int i = 0; i < _size; ++i) _matrix[i] = rand() % 10;
}

template<class T>
Matrix<T>::Matrix(int inRows, int inCols)
{
    _rows = inRows > 0 ? inRows : 1;
    _cols = inCols > 0 ? inCols : 1;
    _size = _rows * _cols;
    _matrix = new T[_size];

    for (int i = 0; i < _size; ++i) _matrix[i] = 0;
}

template<class T>
Matrix<T>::Matrix(int inEdge)
{
    // produces a square matrix
    _rows = inEdge > 0 ? inEdge : 1;
    _cols = _rows;
    _size = _rows * _cols;
    _matrix = new T[_size];

    // default to identity matrix
    for (int i = 0; i < _size; ++i)
        _matrix[i] = (i % (_cols + 1) == 0 ? 1 : 0);
}

template<class T>
Matrix<T>::Matrix(const Matrix<T>& inMatrix)
{
    copy(inMatrix);
}

template<class T>
Matrix<T>::~Matrix()
{
    delete [] _matrix;
}

template<class T>
inline int Matrix<T>::rows() const
{
    return _rows;
}

template<class T>
inline int Matrix<T>::cols() const
{
    return _cols;
}

template<class T>
inline int Matrix<T>::size() const
{
    return _size;
}

template<class T>
inline int Matrix<T>::lastRow() const
{
    return _rows - 1;
}

template<class T>
inline int Matrix<T>::lastCol() const
{
    return _cols - 1;
}

template<class T>
bool Matrix<T>::square() const
{
    return _rows == _cols;
}

template<class T>
void Matrix<T>::copy(const Matrix<T>& inMatrix)
{
    _rows = inMatrix._rows;
    _cols = inMatrix._cols;
    _size = inMatrix._size;
    _matrix = new T[_size];

    for (int i = 0; i < _size; ++i) _matrix[i] = inMatrix._matrix[i];
}

template<class T>
void Matrix<T>::set(int inRow, int inCol, T inValue)
{
    if (inRow < 0 || inCol < 0) return;

    inRow %= _rows;
    inCol %= _cols;
    _matrix[(inRow * _cols) + inCol] = inValue;
}

template<class T>
T Matrix<T>::at(int inIndex) const
{
    if (inIndex < 0 || inIndex >= _size) inIndex = 0;
    return _matrix[inIndex];
}

template<class T>
T Matrix<T>::at(int inRow, int inCol) const
{
    if (inRow < 0 || inCol < 0) return 0;

    inRow %= _rows;
    inCol %= _cols;
    return _matrix[(inRow * _cols) + inCol];
}

template<class T>
int Matrix<T>::toIndex(int inRow, int inCol) const
{
    if (inRow < 0 || inCol < 0) return 0;
    inRow %= _rows;
    inCol %= _cols;
    return (inRow * _cols) + inCol;
}

template<class T>
T Matrix<T>::determinant() const
{
    if (_rows != _cols)
    {
        return 0;
    }
    else if (_rows == 1)
    {
        return _matrix[0];
    }
    else if (_rows == 2)
    {
        return (_matrix[0] * _matrix[3]) - (_matrix[1] * _matrix[2]);
    }

    T outValue = 0;

    for (int i = 0; i < _rows; ++i)
    {
        Matrix m = minorMatrix(0, i);
        int neg = ((i % 2 == 0) ? 1 : -1);
        outValue += neg * at(0, i) * m.determinant();
    }

    return outValue;
}

template<class T>
Matrix<T> Matrix<T>::subMatrix(int inRow, int inCol, int inHeight, int inWidth)
{
    int bottom = inRow + inHeight - 1;
    int right = inCol + inWidth - 1;

    if (inHeight < 1 || inWidth < 1 || inRow < 0 || bottom >= _rows || inCol < 0
        || right >= _cols)
        return Matrix(1, 1);

    Matrix<T> outMatrix(inHeight, inWidth);
    for (int i = 0; i < inHeight; ++i)
    {
        for (int j = 0; j < inWidth; ++j)
        {
            outMatrix(i, j) = at(i + inRow, j + inCol);
        }
    }

    return outMatrix;
}

template<class T>
Matrix<T> Matrix<T>::minorMatrix(int inRow, int inCol) const
{
    if (_rows != _cols || inRow < 0 || inRow >= _rows || inCol < 0
        || inCol >= _cols)
        return Matrix(1, 1);

    int p = 0;

    Matrix outMatrix(_rows - 1);

    for (int i = 0; i < _rows; ++i)
    {
        if (i != inRow)
        {
            int q = 0;
            for (int j = 0; j < _cols; ++j)
            {
                if (j != inCol)
                {
                    outMatrix(p, q) = at(i, j);
                    ++q;
                }
            }
            ++p;
        }
    }

    return outMatrix;
}

template<class T>
void Matrix<T>::transpose()
{
    if (_rows < 2 || _rows != _cols) return;
    for (int i = 0; i < _rows; ++i)
    {
        for (int j = i + 1; j < _cols; ++j)
        {
            T t = at(i, j);
            (*this)(i, j) = at(j, i);
            (*this)(j, i) = t;
        }
    }
}

template<class T>
const Matrix<T> Matrix<T>::transposed()
{
    Matrix<T> outMatrix(*this);
    outMatrix.transpose();
    return outMatrix;
}

template<class T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& inMatrix)
{
    if (this != &inMatrix)
    {
        delete [] _matrix;
        copy(inMatrix);
    }
    return *this;
}

template<class T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& inMatrix)
{
    if (_rows == inMatrix._rows && _cols == inMatrix._cols)
    {
        for (int i = 0; i < _size; ++i) _matrix[i] += inMatrix._matrix[i];
    }
    return *this;
}

template<class T>
Matrix<T>& Matrix<T>::operator+=(const T inValue)
{
    for (int i = 0; i < _size; ++i) _matrix[i] += inValue;
    return *this;
}

template<class T>
Matrix<T>& Matrix<T>::operator-=(const Matrix<T>& inMatrix)
{
    if (_rows == inMatrix._rows && _cols == inMatrix._cols)
    {
        for (int i = 0; i < _size; ++i) _matrix[i] -= inMatrix._matrix[i];
    }
    return *this;
}

template<class T>
Matrix<T>& Matrix<T>::operator-=(const T inValue)
{
    for (int i = 0; i < _size; ++i) _matrix[i] -= inValue;
    return *this;
}

template<class T>
Matrix<T>& Matrix<T>::operator*=(const Matrix<T>& inMatrix)
{
    if (_cols != inMatrix._rows) return *this;
    Matrix<T> result = *this * inMatrix;
    delete [] _matrix;
    copy(result);
    return *this;
}

template<class T>
Matrix<T>& Matrix<T>::operator*=(const T inValue)
{
    for (int i = 0; i < _size; ++i) _matrix[i] *= inValue;
    return *this;
}

template<class T>
Matrix<T>& Matrix<T>::operator/=(const T inValue)
{
    for (int i = 0; i < _size; ++i) _matrix[i] /= inValue;
    return *this;
}

template<class T>
T& Matrix<T>::operator()(int inRow, int inCol)
{
    if (inRow < 0 || inCol < 0) return _matrix[0];

    inRow %= _rows;
    inCol %= _cols;
    return _matrix[(inRow * _cols) + inCol];
}

template<class T>
T Matrix<T>::operator()(int inRow, int inCol) const
{
    if (inRow < 0 || inCol < 0) return _matrix[0];

    inRow %= _rows;
    inCol %= _cols;
    return _matrix[(inRow * _cols) + inCol];
}

template<class T>
T& Matrix<T>::operator[](int inIndex)
{
    if (inIndex < 0 || inIndex >= _size) inIndex = 0;
    return _matrix[inIndex];
}

template<class T>
T Matrix<T>::operator[](int inIndex) const
{
    if (inIndex < 0 || inIndex >= _size) inIndex = 0;
    return _matrix[inIndex];
}

template<class T>
const Matrix<T> Matrix<T>::operator+(const Matrix<T>& inMatrix) const
{
    if (_rows != inMatrix._rows || _cols != inMatrix._cols) return Matrix(1, 1);

    Matrix outMatrix(*this);
    for (int i = 0; i < _size; ++i) outMatrix[i] += inMatrix[i];
    return outMatrix;
}

template<class T>
const Matrix<T> Matrix<T>::operator+(T inValue) const
{
    Matrix outMatrix(*this);
    for (int i = 0; i < _size; ++i) outMatrix[i] += inValue;
    return outMatrix;
}

template<class T>
const Matrix<T> Matrix<T>::operator-(const Matrix<T>& inMatrix) const
{
    if (_rows != inMatrix._rows || _cols != inMatrix._cols) return Matrix(1, 1);

    Matrix outMatrix(*this);
    for (int i = 0; i < _size; ++i) outMatrix[i] -= inMatrix[i];
    return outMatrix;
}

template<class T>
const Matrix<T> Matrix<T>::operator-(T inValue) const
{
    Matrix outMatrix(*this);
    for (int i = 0; i < _size; ++i) outMatrix[i] -= inValue;
    return outMatrix;
}

template<class T>
const Matrix<T> Matrix<T>::operator*(const Matrix<T>& inMatrix) const
{
    if (_cols != inMatrix._rows) return Matrix(1, 1);

    Matrix outMatrix(_rows, inMatrix._cols);

    for (int i = 0; i < outMatrix._rows; ++i)
    {
        for (int j = 0; j < outMatrix._cols; ++j)
        {
            T value = 0;
            for (int k = 0; k < _cols; ++k)
            {
                value += (at(i, k) * inMatrix.at(k, j));
            }
            outMatrix(i, j) = value;
        }
    }

    return outMatrix;
}

template<class T>
const Matrix<T> Matrix<T>::operator*(T inValue) const
{
    Matrix outMatrix(*this);
    for (int i = 0; i < _size; ++i) outMatrix[i] *= inValue;
    return outMatrix;
}

template<class T>
const Matrix<T> Matrix<T>::operator/(T inValue) const
{
    Matrix outMatrix(*this);
    for (int i = 0; i < _size; ++i) outMatrix[i] /= inValue;
    return outMatrix;
}

template<class T>
bool Matrix<T>::operator==(const Matrix<T>& inMatrix) const
{
    if (_rows != inMatrix._rows || _cols != inMatrix._cols) return false;

    for (int i = 0; i < _size; ++i)
    {
        if (at(i) != inMatrix.at(i)) return false;
    }

    return true;
}

template<class T>
bool Matrix<T>::operator!=(const Matrix<T>& inMatrix) const
{
    return !(*this == inMatrix);
}

template<class T>
T* Matrix<T>::array()
{
    return _matrix;
}

template<class T>
ostream& operator<<(ostream& inStream, const Matrix<T>& inMatrix)
{
    inStream << setprecision(3);
    for (int i = 0; i < inMatrix.size(); ++i)
    {
        if (i % inMatrix.cols() == 0 && i > 0) inStream << endl;
        inStream << setw(8) << inMatrix.at(i);
    }

    inStream << endl;

    return inStream;
}

template<class T>
istream& operator>>(istream& inStream, Matrix<T>& inMatrix)
{
    int rows;
    int cols;
    inStream >> rows >> cols;
    inMatrix = Matrix<T>(rows, cols);

    for (int i = 0; i < inMatrix.size(); ++i) inStream >> inMatrix[i];

    return inStream;
}

#endif
