#pragma once
#include <algorithm>
#include "Vector.h"

namespace lasmath {
	template<size_t ORDER>
	class Matrix
	{
	public:
		Matrix() :m_element{}
		{};
		template<typename... Args>
		Matrix(typename std::enable_if<(sizeof...(Args)+1 == (ORDER*ORDER)), float>::type xx, Args... args) :m_element{ xx,(float)args... }
		{};
		~Matrix() 
		{};

		// Return reference to vector axis
		Vector<ORDER>& operator[](size_t n) {
		//TODO trhow exception if n > ORDER
			return m_axis[n];
		};
		const Vector<ORDER>& operator[](size_t n) const {
			//TODO trhow exception if n > ORDER
			return m_axis[n];
		};

		// Cast matrix as an array of floats
		explicit operator float*() {
			// returns pointer to first element in union
			return m_element[0];
		};

		// Vector transformation
		Vector<ORDER> operator*(const Vector<ORDER>& v) const {
			//TODO comment vector transformation
			Vector<ORDER> transformed;
			for (size_t i = 0; i < ORDER; ++i) {
				for (size_t j = 0; j < ORDER; ++j) {
					transformed[i] += m_element[j][i] * v[j];
				}
			}
			return transformed;
		};

		// Matrix concatenation
		Matrix<ORDER> operator*(const Matrix<ORDER>& m) const {
			//TODO comment matrix transformation
			Matrix<ORDER> product;
			for (size_t i = 0; i < ORDER; ++i) {
				product[i] = (*this)*(m[i]);
			}
			return product;
		};

		// Set matrix to identity matrix
		void setIdentity() {
			//TODO comment
			for (size_t i = 0; i < ORDER; ++i) {
				m_axis[i] = {};
				m_element[i][i] = 1;
			}
		};

		// Set matrix to be a transformation matrix rotating by angle
		// Only enabled for Matrix<2>
		template<size_t ORD = ORDER>
		typename std::enable_if<ORD == 2, void>::type setRotate(float angle) {
			float rotate[ORD][ORD] = { { cosf(angle), sinf(angle) } ,{ -sinf(angle), cosf(angle) } };
			// Copy rotation matrix to elements
			std::copy(&rotate[0][0], &rotate[0][0] + (ORD*ORD), &m_element[0][0]);
		};

		// Set matrix to transformation matrix rotating around X axis by angle
		// Only enabled for Matrix<3> and Matrix<4>
		template<size_t ORD = ORDER>
		typename std::enable_if<ORD == 3 || ORD == 4, void>::type setRotateX(float angle) {
			// Set x axis as unit vector (1,0,0), set y and z axes as unit vectors rotated around x axis by angle
			float rotate[ORD][ORD] = { { 1,0,0 } ,{ 0, cosf(angle), sinf(angle) },{ 0, -sinf(angle), cosf(angle) } };
			// Set t axis as no translation for Matrix<4>
			if (ORD == 4) {
				rotate[3][3] = 1;
			}
			// Copy rotation matrix to elements
			std::copy(&rotate[0][0], &rotate[0][0] + (ORD*ORD), &m_element[0][0]);
		};

		// Set matrix to transformation matrix rotating around Y axis by angle
		// Only enabled for Matrix<3> and Matrix<4>
		template<size_t ORD = ORDER>
		typename std::enable_if<ORD == 3 || ORD == 4, void>::type setRotateY(float angle) {
			// Set y axis as unit vector (0,1,0), set x and z axes as unit vectors rotated around y axis by given angle
			float rotate[ORD][ORD] = { { cosf(angle), 0, -sinf(angle) } ,{0,1,0},{ sinf(angle), 0, cosf(angle) } };

			// Set t axis as no translation for Matrix<4>
			if (ORD == 4) {
				rotate[3][3] = 1;
			}
			// Copy rotation matrix to elements
			std::copy(&rotate[0][0], &rotate[0][0] + (ORD*ORD), &m_element[0][0]);
		};

		// Set matrix to transformation matrix rotating around Z axis by angle
		// Only enabled for Matrix<3> and Matrix<4>
		template<size_t ORD = ORDER>
		typename std::enable_if<ORD == 3 || ORD == 4, void>::type setRotateZ(float angle) {
			// Set z axis as unit vector (0,0,1), set x and y axes as unit vectors rotated around z axis by given angle
			float rotate[ORD][ORD] = { { cosf(angle), sinf(angle), 0 },{ -sinf(angle), cosf(angle), 0 },{0,0,1 } };
			
			// Set t axis as no translation for Matrix<4>
			if (ORD == 4) {
				//float tAxis[4] = { 0,0,0,1 };
				////HACK testing it works before rewriting
				//for (size_t i = 0; i < O; ++i) {
				//	m_element[3][i] = tAxis[i];
				//}
				rotate[3][3] = 1;
			}

			// Copy rotation matrix to elements
			std::copy(&rotate[0][0], &rotate[0][0] + (ORD*ORD), &m_element[0][0]);
		};

		// Set matrix to a transformation matrix rotating to orientation given by Euler angles
		// Only enabled for Matrix<3> and Matrix<4>
		template<size_t ORD = ORDER>
		typename std::enable_if<ORD == 3 || ORD == 4, void>::type setEulerRotate(float alpha, float beta, float gamma) {
			//z-x-z rotation
			//TODO further commenting
			float cosAlpha = cosf(alpha);
			float sinAlpha = sinf(alpha);
			float cosBeta = cosf(beta);
			float sinBeta = sinf(beta);
			float cosGamma = cosf(gamma);
			float sinGamma = sinf(gamma);

			float rotate[ORD][ORD] = { { cosAlpha*cosGamma - cosBeta*sinAlpha*sinGamma, cosGamma*sinAlpha + cosAlpha*cosBeta*sinGamma, sinBeta*sinGamma },
										{ -cosAlpha*sinGamma - cosBeta*cosGamma*sinAlpha, cosAlpha*cosBeta*cosGamma - sinAlpha*sinGamma, cosGamma*sinBeta },
										{ sinAlpha*sinBeta, -cosAlpha*sinBeta, cosBeta } };
			// Set t axis as no translation for Matrix<4>
			if (ORD == 4) {
		
				rotate[3][3] = 1;
			}
			// Copy rotation matrix to elements
			std::copy(&rotate[0][0], &rotate[0][0] + (ORD*ORD), &m_element[0][0]);
		};

		// Set matrix to a transformation matrix rotating to orientation given by Tait-Bryan angles
		// Only enabled for Matrix<3> and Matrix<4>
		template<size_t ORD = ORDER>
		typename std::enable_if<ORD == 3 || ORD == 4, void>::type setTaitBryanRotate(float yaw, float pitch, float roll) {
			//z-y-x rotation
			//TODO further commenting
			float cosYaw = cosf(yaw);
			float sinYaw = sinf(yaw);
			float cosPitch = cosf(pitch);
			float sinPitch = sinf(pitch);
			float cosRoll = cosf(roll);
			float sinRoll = sinf(roll);

			float rotate[ORD][ORD] = { { cosYaw*cosPitch, sinYaw*cosPitch, -sinPitch },
										{ cosYaw*sinPitch*sinRoll - sinYaw*cosRoll, cosYaw*cosRoll + sinYaw*sinPitch*sinRoll, cosPitch*sinRoll },
										{ sinYaw*sinRoll + cosYaw*sinPitch*cosRoll, sinYaw*sinPitch*cosRoll - cosYaw*sinRoll, cosPitch*cosRoll } };

			// Set t axis as no translation for Matrix<4>
			if (ORD == 4) {
				rotate[3][3] = 1;
			}
			// Copy rotation matrix to elements
			std::copy(&rotate[0][0], &rotate[0][0] + (ORD*ORD), &m_element[0][0]);
		};

		// Calculates inverse of this matrix and copies it to dest
		// Returns true if matrix is invertable, returns false if singular or poorly conditioned
		bool calculateInverse(Matrix<ORDER>& dest) {
			Matrix<ORDER> inverse;
			inverse.setIdentity();			//Start with an identity matrix
			bool invertable = invertTransform<ORDER>((float*)inverse);
			if (invertable) {
				dest = inverse;
			}
			return invertable;
		};

		//TODO undo transformation tests
		bool transformByInverse(Matrix<ORDER>& target) {//TODO pick better names
			Matrix<ORDER> tgtCopy = target;
			bool invertable = invertTransform<ORDER>((float*)tgtCopy);
			if (invertable) {
				target = tgtCopy;
			}
			return invertable;
		}
		//TODO undo transformation on vector tests
		bool transformByInverse(Vector<ORDER>& target) {//TODO pick better names
			Vector<ORDER> tgtCopy = target;
			bool invertable = invertTransform<1>((float*)tgtCopy);
			if (invertable) {
				target = tgtCopy;
			}
			return invertable;
		}

	protected:
		union {
			Vector<ORDER> m_axis[ORDER];
			float m_element[ORDER][ORDER];
		};
		
		// Compares float arrays and returns true if all members are equal
		template<size_t ROWS>
		static bool areColumnsEqual(float* first, float* second) {
			//TODO add tolerance if necessary
			for (size_t i = 0; i < ROWS; ++i) {
				if (first[i] != second[i]) {
					return false;
				}
			}
			return true;
		}

		// Swaps rows in matrix
		template<size_t ROWS, size_t COLUMNS>
		static void swapRows(float** theMatrix, size_t first, size_t second) {
			//TODO argument exception if first or second > ROWS
			// Swap values of first and second row for every column
			for (size_t i = 0; i < COLUMNS; ++i) {
				float temp = theMatrix[i][first];
				theMatrix[i][first] = theMatrix[i][second];
				theMatrix[i][second] = temp;
			}
		}

		// Multiplies all elements in row by factor
		template<size_t ROWS, size_t COLUMNS>
		static void multiplyRow(float** theMatrix, size_t row, float factor) {
			//TODO argument exception if row > ROWS
			for (size_t i = 0; i < COLUMNS; ++i) {
				theMatrix[i][row] *= factor;
			}
		}

		// Adds source row multiplied by factor to target row
		template<size_t ROWS, size_t COLUMNS>
		static void addRow(float** theMatrix, size_t sourceRow, size_t targetRow, float factor) {
			//TODO argument exception if sourceRow or targetRow > ROWS
			for (size_t i = 0; i < COLUMNS; ++i) {
				theMatrix[i][targetRow] += factor * theMatrix[i][sourceRow];
			}
		}

		/*	Performs inverse of transform on result to either undo a transformation or get inverse matrix
			Returns true if inversion is possible, or false if transform is singular or too poorly conditioned
			transform = ORDERxORDER column major transformation matrix, will be converted to reduced echelon form
			result = ORDERxRESULT_COLUMNS column major matrix, will be transformed by inverse of transform
		*/
		template<size_t RESULT_COLUMNS>
		bool invertTransform(float* result) {
			//TODO get rid of transform and just use copy of self, this is no longer a static function
			// HACK this relies on a bunch of pointer arithmetic, since matrices aren't templated (yet?)
			Matrix<ORDER> rowReduce = *this;
			float* transform = (float*)rowReduce;
			Vector<ORDER> emptyColumn;
			bool matrixInvertable = true;
			// Create 2d arrays holding pointers to matrix elements, so [] operator rather than pointer arithmetic can be used
			float* transformArray[ORDER];
			float* resultArray[RESULT_COLUMNS];
			for (size_t col = 0; col < ORDER; ++col) {
				transformArray[col] = transform + (ORDER * col);
			}
			for (size_t col = 0; col < RESULT_COLUMNS; ++col) {
				resultArray[col] = result + (ORDER*col);
			}

			// Use row operations to make rowReduced into identity matrix
			for (size_t i = 0; i < ORDER && matrixInvertable; ++i) {
				float reducedColumn[ORDER] = { 0.0f };					// How current column will appear in reduced echelon form
				reducedColumn[i] = 1.0f;
				// Perform row operations until current column is in reduced echelon form
				if (!areColumnsEqual<ORDER>(transformArray[i], reducedColumn)) {
					if (areColumnsEqual<ORDER>(transformArray[i], (float*)emptyColumn)) {		// If column has no values, matrix must be singular
						matrixInvertable = false;
						break;
					}
					else if (transformArray[i][i] == 0.0f) {		// If pivot position is 0, swap with lower row
						bool rowFound = false;				// flag variable to check if row to swap with has been found
						for (size_t row = i + 1; row < ORDER; ++row) {
							if (transformArray[i][row] != 0.0f) {			// If row has value in relevant column
								swapRows<ORDER, ORDER>(transformArray, i, row);			// Swap row with pivot row
								swapRows<ORDER, RESULT_COLUMNS>(resultArray, i, row);	// Perform same operation on result matrix
								rowFound = true;
								break;
							}
						}
						if (!rowFound) {
							//If pivot position cannot be given a non-zero value without breaking echelon form
							//matrix cannot be inverted
							matrixInvertable = false;
							break;
						}
					}
					if (transformArray[i][i] != 1.0f) {									// If pivot position isn't 1
						float quotient = 1.0f / transformArray[i][i];
						multiplyRow<ORDER, ORDER>(transformArray, i, quotient);			// Divide row by pivot position value
						multiplyRow<ORDER, RESULT_COLUMNS>(resultArray, i, quotient);
					}
					// Now that leading entry of current row is 1, subtract from other rows so entries in that column are 0
					for (size_t row = 0; row < ORDER; ++row) {
						if (row!=i && transformArray[i][row] != 0.0f) {
							float factor = -transformArray[i][row];
							addRow<ORDER, ORDER>(transformArray, i, row, factor);
							addRow<ORDER, RESULT_COLUMNS>(resultArray, i, row, factor);
						}
					}
				}
			}
			return matrixInvertable;
		}
	};

	// typedefs for common matrix dimensions
	typedef Matrix<2> Matrix2;
	typedef Matrix<3> Matrix3;
	typedef Matrix<4> Matrix4;
}
