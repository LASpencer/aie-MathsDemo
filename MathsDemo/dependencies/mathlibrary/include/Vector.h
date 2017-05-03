#pragma once
#include <math.h>
#include <type_traits>
#include <cstdarg>
#include <stdexcept>
#include <string>

namespace lasmath {
	template<size_t DIM>
	class Vector
	{
	public:
		Vector() : m_component{}
		{};

		template<typename... Args>
		Vector(typename std::enable_if<(sizeof...(Args)+1 == DIM), float>::type x, Args... args) :m_component{ x,(float)args... } 
		{
		};

		~Vector()
		{};

		// return reference to vector component
		float& operator[](size_t n) {
			if (n >= DIM) {
				throw std::range_error("Vector component out of range");
			}
			return m_component[n];
		};
		const float& operator[](size_t n) const {
			if (n >= DIM) {
				throw std::range_error("Vector component out of range");
			}
			return m_component[n];
		};

		// cast vector as array of floats
		explicit operator float*() {
			// Return array of components
			return m_component;
		};


		// Add and assignment
		Vector<DIM> operator+=(const Vector<DIM>& b) {
			for (size_t i = 0; i < DIM; ++i) {
				this->m_component[i] += b[i];
			}
			return *this;
		}

		// Subtract and assignment
		Vector<DIM> operator-=(const Vector<DIM>& b) {
			for (size_t i = 0; i < DIM; ++i) {
				this->m_component[i] -= b[i];
			}
			return *this;
		}

		// Scale and assignment
		Vector<DIM> operator*=(float f) {
			for (size_t i = 0; i < DIM; ++i) {
				this->m_component[i] *= f;
			}
			return *this;
		}

		// cast as vector of different dimension
		template<size_t D>
		explicit operator Vector<D>()
		{
			Vector<D> vec;
			for (size_t i = 0; i < D&&i<DIM; ++i) {
				vec[i] = m_component[i];
			}
			return vec;
		};

		

		// Dot multiplication of two vectors
		float dot(const Vector<DIM>& b) const {
			float product = 0.0f;
			// Iterate over corresponding components, multiplying them and adding to total
			for (size_t i = 0; i < DIM; ++i) {
				product += m_component[i] * b[i];
			}
			return product;
		};

// Cross multiplication of two vectors
template <size_t D = DIM>
typename std::enable_if<D == 3||D==4, Vector<D>>::type cross(const Vector<D>& b) const {
			Vector<D> product;
			product[0] = m_component[1] * b[2] - m_component[2] * b[1];
			product[1] = m_component[2] * b[0] - m_component[0] * b[2];
			product[2] = m_component[0] * b[1] - m_component[1] * b[0];
			// m_component[3] remains 0 in Vector<4>
			return product;
		};


		// Returns square of vector's magnitude
		float magnitudeSquared() const {
			// Square of magnitude is quicker to calculate than magnitude and is
			// useful when magnitude only needs to be compared to a value
			float magSquared = 0.0f;
			for (size_t i = 0; i < DIM; ++i) {
				magSquared += m_component[i] * m_component[i];
			}
			return magSquared;
		};

		// Returns magnitude of vector
		float magnitude() const {
			// Calculate magnitude through pythagoras equation
			// Take square root of sum of components squared
			return sqrtf(magnitudeSquared());
		};

		// Converts vector to a unit vector with same direction
		bool normalise() {
			// Divide all components by magnitude
			float mag = magnitude();
			if (mag != 0.0f && !isinf(mag) && !isnan(mag)) {
				float magReciprocal = 1.0f / mag;
				for (size_t i = 0; i < DIM; ++i) {
					m_component[i] *= magReciprocal;
				}
				return true;
			}
			else {
				return false;
			}
		};

		/* Compares magnitude of vector to value given.
		Returns 0 if equal, -1 if vector magnitude is lower, 1 if vector magnitude is greater
		*/
		int compareMagnitude(float f) const {
			float difference = magnitudeSquared() - (f*f);
			if (difference < 0.0) {
				return -1;
			}
			else if (difference > 0.0) {
				return 1;
			}
			else {
				return 0;
			}
		};

		// Returns true if all components are 0.0f
		bool isZeroVector() const {
			for (size_t i = 0; i < DIM; ++i) {
				if (m_component[i] != 0.0f) {
					return false;
				}
			}
			return true;
		}

		// Returns true if any component is infinite
		bool isInfinite() const {
			for (size_t i = 0; i < DIM; ++i) {
				if (isinf(m_component[i])) {
					return true;
				}
			}
			return false;
		}

		// Returns true if any component is not a number
		bool isNAN() const {
			for (size_t i = 0; i < DIM; ++i) {
				if (isnan(m_component[i])) {
					return true;
				}
			}
			return false;
		}

	protected:
		float m_component[DIM];
	};

	// typedefs for common vector dimensions 
	typedef Vector<2> Vector2;
	typedef Vector<3> Vector3;
	typedef Vector<4> Vector4;

	// Arithmetic operators
	template<size_t DIM>
	Vector<DIM> operator+(const Vector<DIM>& a, const Vector<DIM>& b) {
		// Add corresponding vector components to get components of resultant vector
		Vector<DIM> sum;
		for (size_t i = 0; i < DIM; ++i) {
			sum[i] = a[i] + b[i];
		}
		return sum;
	};
	template<size_t DIM>
	Vector<DIM> operator-(const Vector<DIM>& a, const Vector<DIM>& b) {
		// Subtract corresponding vector components to get components of resultant vector
		Vector<DIM> difference;
		for (size_t i = 0; i < DIM; ++i) {
			difference[i] = a[i] - b[i];
		}
		return difference;
	};
	template<size_t DIM>
	Vector<DIM> operator*(const Vector<DIM>& v, float f) {
		// Multiply all components by f to get components of resultant vector
		Vector<DIM> product;
		for (size_t i = 0; i < DIM; ++i) {
			product[i] = v[i] *f;
		}
		return product;
	};
	template<size_t DIM>
	Vector<DIM> operator*(float f, const Vector<DIM>& v) {
		return v * f;
	};

	// Compare magnitudes of vectors
	// These operators use magnitudeSquared as the exact difference doesn't need to be calculated

	template<size_t DIM>
	bool operator>(const Vector<DIM>& a, const Vector<DIM>& b) {
		return a.magnitudeSquared() > b.magnitudeSquared();
	};
	template<size_t DIM>
	bool operator<(const Vector<DIM>& a, const Vector<DIM>& b) {
		return b > a;
	};

	// Vectors are equal if all components are equal
	template<size_t DIM>
	bool operator==(const Vector<DIM>& a, const Vector<DIM>& b) {
		for (size_t i = 0; i < DIM; ++i) {
			if (a[i] != b[i]) {
				return false;
			}
		}
		return true;
	};
	template<size_t DIM>
	bool operator!=(const Vector<DIM>& a, const Vector<DIM>& b) {
		return !(a == b);
	};
}