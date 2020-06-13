#pragma once
#include <cstdint>

namespace st {
	/* �Q�����x�N�g�� */
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
		/* �e�v�f */
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
}
