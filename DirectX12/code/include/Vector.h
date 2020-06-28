#pragma once
#include <cstdint>

namespace st {
	/* 2次元ベクトル */
	template <typename T>
	struct Vector2
	{
	public:
		Vector2() {
			x = 0;
			y = 0;
		}
		Vector2(const Vector2& vec) {
			(*this) = vec;
		}
		Vector2(const T& x, const T& y) {
			this->x = x;
			this->y = y;
		}
		Vector2(const T& value) {
			(*this) = value;
		}

	public:
		void operator=(const Vector2& vec) {
			x = vec.x;
			y = vec.y;
		}
		void operator=(const T& value) {
			x = value;
			y = value;
		}

	public:
		/* 各要素 */
		union {
			struct {
				T x;
				T y;
			};
			T value[2];
		};
	};
	using Vec2 = Vector2<std::int32_t>;
	using Vec2f = Vector2<float>;

	/* 3次元ベクトル */
	template <typename T>
	struct Vector3
	{
	public:
		Vector3() {
			x = 0;
			y = 0;
			z = 0;
		}
		Vector3(const Vector3& vec) {
			(*this) = vec;
		}
		Vector3(const T& x, const T& y, const T& z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}
		Vector3(const T& value) {
			(*this) = value;
		}

	public:
		void operator=(const Vector3& vec) {
			x = vec.x;
			y = vec.y;
			z = vec.z;
		}
		void operator=(const T& value) {
			x = value;
			y = value;
			z = value;
		}

	public:
		/* 各要素 */
		union {
			struct {
				T x;
				T y;
				T z;
			};
			T value[3];
		};
	};
	using Vec3  = Vector3<std::int32_t>;
	using Vec3f = Vector3<float>;
}
