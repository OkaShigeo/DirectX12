#pragma once
#include <cmath>
#include <cstdint>

namespace Math
{
	template<typename T>
	struct Vector2
	{
	public:
		/** コンストラクタ */
		Vector2() {
			x = y = T(0);
		}
		/** コンストラクタ
		 * @param x X要素
		 * @param y Y要素
		 */
		Vector2(const T& x, const T& y) {
			value[0] = x;
			value[1] = y;
		}
		/** コンストラクタ
		 * @param val 要素
		 */
		Vector2(const T& val) {
			(*this) = Vector2(val, val);
		}
		/** コンストラクタ
		 * @param vec 二次元ベクトル
		 */
		Vector2(const Vector2& vec) {
			(*this) = Vector2(vec.x, vec.y);
		}
		/*＊ デストラクタ */
		~Vector2() {}

	public:
		/*＊ 大きさ 
		 * @return ベクトルの大きさ
		 */
		T Length(void) const {
			return std::sqrt((x * x) + (y * y));
		}
		/*＊ 正規化 
		 * @return 正規化ベクトル
		 */
		Vector2 Normalize(void) const {
			return (*this) / Length();
		}
		/*＊ 内積 
		 * @param 二次元ベクトル
		 * @return 二つのベクトルとの内角
		 */
		T Dot(const Vector2& vec) const {
			return (x * vec.x) + (y * vec.y);
		}

		/*----------オペレータオーバーロード関数----------*/
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

	using Vec2  = Vector2<std::int32_t>;
	using Vec2f = Vector2<float>;


	template<typename T>
	struct Vector3
	{
	public:
		/*＊ コンストラクタ */
		Vector3() {
			x = y = z = T(0);
		}
		/** コンストラクタ
		 * @param x X要素
		 * @param y Y要素
		 * @param z Z要素
		 */
		Vector3(const T& x, const T& y, const T& z) {
			value[0] = x;
			value[1] = y;
			value[2] = z;
		}
		/** コンストラクタ
		 * @param val 要素
		 */
		Vector3(const T& val) {
			(*this) = Vector3(val, val, val);
		}
		/** コピーコンストラクタ
		 * @param vec 三次元ベクトル
		 */
		Vector3(const Vector3& vec) {
			(*this) = Vector3(vec.x, vec.y, vec.z);
		}
		/*＊ デストラクタ */
		~Vector3() {}

	public:
		/*＊ 大きさ 
		 * @return ベクトルの大きさ
		 */
		T Length(void) const {
			return std::sqrt((x * x) + (y * y) + (z * z));
		}
		/** 正規化 
		 * @return 正規化ベクトル
		 */
		Vector3 Normalize(void) const {
			return (*this) / Length();
		}
		/*＊ 内積
		 * @param 三次元ベクトル
		 * @return 二つのベクトルとの内角
		 */
		T Dot(const Vector3& vec) const {
			return (x * vec.x) + (y * vec.y) + (z * vec.z);
		}

		/*----------オペレータオーバーロード関数----------*/
	public:
		Vector3 operator-(void) const {
			return Vector3(-x, -y, -z);
		}
		Vector3 operator+(const T& val) const {
			return Vector3(x + val, y + val, z + val);
		}
		Vector3 operator-(const T& val) const {
			return Vector3(x - val, y - val, z - val);
		}
		Vector3 operator*(const T& val) const {
			return Vector3(x * val, y * val, z * val);
		}
		Vector3 operator/(const T& val) const {
			return Vector3(x / val, y / val, z / val);
		}
		void operator+=(const T& val) {
			x += val;
			y += val;
			z += val;
		}
		void operator-=(const T& val) {
			x -= val;
			y -= val;
			z -= val;
		}
		void operator*=(const T& val) {
			x *= val;
			y *= val;
			z *= val;
		}
		void operator/=(const T& val) {
			x /= val;
			y /= val;
			z /= val;
		}
		void operator=(const T& val) {
			x = y = z = val;
		}
		Vector3 operator+(const Vector3& vec) const {
			return Vector3(x + vec.x, y + vec.y, z + vec.z);
		}
		Vector3 operator-(const Vector3& vec) const {
			return Vector3(x - vec.x, y - vec.y, z - vec.z);
		}
		Vector3 operator*(const Vector3& vec) const {
			return Vector3(x * vec.x, y * vec.y, z * vec.z);
		}
		Vector3 operator/(const Vector3& vec) const {
			return Vector3(x / vec.x, y / vec.y, z / vec.z);
		}
		void operator+=(const Vector3& vec) {
			x += vec.x;
			y += vec.y;
			z += vec.z;
		}
		void operator-=(const Vector3& vec) {
			x -= vec.x;
			y -= vec.y;
			y -= vec.z;
		}
		void operator*=(const Vector3& vec) {
			x *= vec.x;
			y *= vec.y;
			z *= vec.z;
		}
		void operator/=(const Vector3& vec) {
			x /= vec.x;
			y /= vec.y;
			z /= vec.z;
		}
		void operator=(const Vector3& vec) {
			x = vec.x;
			y = vec.y;
			z = vec.z;
		}

	public:
		/* 要素 */
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
