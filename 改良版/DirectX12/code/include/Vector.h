#pragma once
#include <cmath>

namespace Dx12
{
	template<typename T>
	class Vector2
	{
	public:
		/* コンストラクタ */
		Vector2() {
			x = y = T(0);
		}
		Vector2(const T& x, const T& y) {
			value[0] = x;
			value[1] = y;
		}
		Vector2(const T& val) {
			Vector2(val, val);
		}
		Vector2(const Vector2& vec) {
			Vector2(vec.x, vec.y);
		}
		/* デストラクタ */
		~Vector2() {}

	public:
		/* 大きさ */
		T Length(void) const {
			return std::sqrt((x * x) + (y * y));
		}
		/* 正規化 */
		Vector2 Normalize(void) const {
			return (*this) / Length();
		}
		/* 内積 */
		T Dot(const Vector2& vec) const {
			return (x * vec.x) + (y + vec.y);
		}

		/* 演算子オーバーロード */
	public:
		Vector2 operator-(void) const {
			return Vector2(-x, -y);
		}
		Vector2 operator+(const T& val) const {
			return Vector2(x + val, y + val);
		}
		Vector2 operator-(const T& val) const {
			return Vector2(x - val, y - val);
		}
		Vector2 operator*(const T& val) const {
			return Vector2(x * val, y * val);
		}
		Vector2 operator/(const T& val) const {
			return Vector2(x / val, y / val);
		}
		void operator+=(const T& val) {
			x += val;
			y += val;
		}
		void operator-=(const T& val) {
			x -= val;
			y -= val;
		}
		void operator*=(const T& val) {
			x *= val;
			y *= val;
		}
		void operator/=(const T& val) {
			x /= val;
			y /= val;
		}
		void operator=(const T& val) {
			x = y = val;
		}
		Vector2 operator+(const Vector2& vec) const {
			return Vector2(x + vec.x, y + vec.y);
		}
		Vector2 operator-(const Vector2& vec) const {
			return Vector2(x - vec.x, y - vec.y);
		}
		Vector2 operator*(const Vector2& vec) const {
			return Vector2(x * vec.x, y * vec.y);
		}
		Vector2 operator/(const Vector2& vec) const {
			return Vector2(x / vec.x, y / vec.y);
		}
		void operator+=(const Vector2& vec) {
			x += vec.x;
			y += vec.y;
		}
		void operator-=(const Vector2& vec) {
			x -= vec.x;
			y -= vec.y;
		}
		void operator*=(const Vector2& vec) {
			x *= vec.x;
			y *= vec.y;
		}
		void operator/=(const Vector2& vec) {
			x /= vec.x;
			y /= vec.y;
		}
		void operator=(const Vector2& vec) {
			x = vec.x;
			y = vec.y;
		}

	public:
		/* 要素 */
		union {
			struct {
				T x;
				T y;
			};
			T value[2];
		};
	};

	using Vec2  = Vector2<__int32>;
	using Vec2f = Vector2<float>;
}
