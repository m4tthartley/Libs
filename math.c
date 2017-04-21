/*
	Vectors, matrices, quaternions
*/

#include <math.h>

#ifndef _GIANTJELLY_MATH_
#define _GIANTJELLY_MATH_

typedef union {
	struct {
		int x;
		int y;
	};
	struct {
		int u;
		int v;
	};
} int2;

typedef union {
	struct {
		int x;
		int y;
		int z;
	};
	struct {
		int r;
		int g;
		int a;
	};
	int2 xy;
} int3;

typedef union {
	struct {
		float x;
		float y;
	};
	struct {
		float u;
		float v;
	};
	float e[2];
} float2;

typedef union {
	struct {
		float x;
		float y;
		float z;
	};
	struct {
		float r;
		float g;
		float b;
	};
	float2 xy;
	float e[3];
} float3;

typedef union {
	struct {
		float x;
		float y;
		float z;
		float w;
	};
	struct {
		float r;
		float g;
		float b;
		float a;
	};
	float3 xyz;
	float2 xy;
	float e[4];
} float4;

typedef union {
	struct {
		float m00;
		float m01;
		float m02;
		float m03;
		float m10;
		float m11;
		float m12;
		float m13;
		float m20;
		float m21;
		float m22;
		float m23;
		float m30;
		float m31;
		float m32;
		float m33;
	};
	float e[16];
} mat4;

typedef float4 quaternion;

#define PI 3.14159265359f
#define PI2 (3.14159265359f*2.0f)

int2 make_int2(int x, int y) {
	int2 result = {x, y};
	return result;
}

int3 make_int3(int x, int y, int z) {
	int3 result = {x, y, z};
	return result;
}

float2 make_float2(float x, float y) {
	float2 result = {x, y};
	return result;
}

float3 make_float3(float x, float y, float z) {
	float3 result = {x, y, z};
	return result;
}

float4 make_float4(float x, float y, float z, float w) {
	float4 result = {x, y, z, w};
	return result;
}

float2 add2(float2 a, float2 b) {
	float2 result = {
		a.x + b.x,
		a.y + b.y,
	};
	return result;
}

float3 add3(float3 a, float3 b) {
	float3 result = {
		a.x + b.x,
		a.y + b.y,
		a.z + b.z,
	};
	return result;
}

float2 sub2(float2 a, float2 b) {
	float2 result = {
		a.x - b.x,
		a.y - b.y,
	};
	return result;
}

float3 sub3(float3 a, float3 b) {
	float3 result = {
		a.x - b.x,
		a.y - b.y,
		a.z - b.z,
	};
	return result;
}

float3 div3(float3 a, float3 b) {
	float3 result = {
		a.x / b.x,
		a.y / b.y,
		a.z / b.z,
	};
	return result;
}

float3 div3f(float3 a, float b) {
	float3 result = {
		a.x / b,
		a.y / b,
		a.z / b,
	};
	return result;
}

float length2(float2 v) {
	float result = sqrtf(v.x*v.x + v.y*v.y);
	if (isnan(result)) __debugbreak();
	return result;
}

float length3(float3 v) {
	float result = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
	return result;
}

float dist2(float2 a, float2 b) {
	float2 f = {a.x-b.x, a.y-b.y};
	float result = sqrtf(f.x*f.x + f.y*f.y);
	return result;
}

float3 normalize(float3 v) {
	float len = length3(v);
	float3 result = {
		v.x / len,
		v.y / len,
		v.z / len,
	};
	return result;
}

mat4 mat4_identity() {
	mat4 result = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1,
	};
	return result;
}

mat4 make_perspective_matrix(float fov, float aspect, float near, float far) {
	float f = 1.0f / tanf((fov/180.0f*PI) / 2.0f);
	mat4 mat = {
		f / aspect, 0, 0, 0,
		0, f, 0, 0,
		0, 0, (far + near) / (near - far), -1,
		0, 0, (2.0f * far * near) / (near - far), 0,
	};
	//memcpy(out, mat, sizeof(float)*16);
	return mat;
}

mat4 make_ortho_matrix(float left, float top, float right, float bottom, float n, float f) {
	mat4 result = {
		2 / right-left, 0, 0, right+left / right-left,
		0, 2 / top-bottom, 0, top+bottom / top-bottom,
		0, 0, -2 / f-n, f+n / f-n,
		0, 0, 0, 1,
	};
	return result;
}

void mat4_apply_mul(mat4 *out, mat4 mat2) {
	float mat1[16];
	memcpy(mat1, out, sizeof(float)*16);
	// todo: this copying and zeroing isn't actually needed
	for (int i = 0; i < 16; ++i) out->e[i] = 0;

	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			for (int i = 0; i < 4; ++i) {
				out->e[row*4 + col] += mat1[row*4 + i] * mat2.e[i*4 + col];
			}
		}
	}
}

mat4 mat4_mul(mat4 m1, mat4 m2) {
	mat4 out = {0};

	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			for (int i = 0; i < 4; ++i) {
				out.e[row*4 + col] += m1.e[row*4 + i] * m2.e[i*4 + col];
			}
		}
	}

	return out;
}

void mat4_apply_translate(mat4 *m, float3 pos) {
	mat4 result = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		pos.x, pos.y, pos.z, 1,
	};
	mat4_apply_mul(m, result);
}

mat4 mat4_translate(float3 pos) {
	mat4 result = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		pos.x, pos.y, pos.z, 1,
	};
	return result;
}

void mat4_apply_rotate_x(mat4 *m, float rads) {
	mat4 result = {
		1, 0,          0,           0,
		0, cosf(rads), -sinf(rads), 0,
		0, sinf(rads), cosf(rads),  0,
		0, 0,          0,           1,
	};
	mat4_apply_mul(m, result);
}
void mat4_apply_rotate_y(mat4 *m, float rads) {
	mat4 result = {
		cosf(rads),  0, sinf(rads), 0,
		0,           1, 0,          0,
		-sinf(rads), 0, cosf(rads), 0,
		0,           0, 0,          1,
	};
	mat4_apply_mul(m, result);
}
void mat4_apply_rotate_z(mat4 *m, float rads) {
	mat4 result = {
		cosf(rads), -sinf(rads), 0, 0,
		sinf(rads), cosf(rads),  0, 0,
		0,          0,           1, 0,
		0,          0,           0, 1,
	};
	mat4_apply_mul(m, result);
}

// todo: this seems terribly inefficient
void mat4_apply_euler(mat4 *m, float3 rotation) {
	mat4_apply_rotate_x(m, rotation.x);
	mat4_apply_rotate_y(m, rotation.y);
	mat4_apply_rotate_z(m, rotation.z);
}

mat4 mat4_euler_rotation(float3 euler) {
	mat4 result = mat4_identity();
	mat4_apply_rotate_x(&result, euler.x);
	mat4_apply_rotate_y(&result, euler.y);
	mat4_apply_rotate_z(&result, euler.z);
	return result;
}

mat4 euler_to_mat4(float3 euler) {
	mat4 result = mat4_identity();
	mat4_apply_euler(&result, euler);
	return result;
}

void float4_apply_mat4(float4 *out, mat4 mat) {
	float4 f = *out;
	memset(out, 0, sizeof(float4));

	for (int col = 0; col < 4; ++col) {
		for (int i = 0; i < 4; ++i) {
			out->e[col] += f.e[i] * mat.e[i*4 + col];
		}
	}
}

void float4_apply_perspective(float4 *out, mat4 mat) {
	float4 f = *out;
	memset(out, 0, sizeof(float4));

	for (int col = 0; col < 4; ++col) {
		for (int i = 0; i < 4; ++i) {
			out->e[col] += f.e[i] * mat.e[i*4 + col];
		}
	}

	out->e[0] /= out->e[3];
	out->e[1] /= out->e[3];
	out->e[2] /= out->e[3];
}

float3 normalize3(float3 v) {
	float len = length3(v);
	float3 result = {v.x/len, v.y/len, v.z/len};
	return result;
}

float3 cross3(float3 a, float3 b) {
	float3 result;
	result.x = a.y*b.z - a.z*b.y;
	result.y = a.z*b.x - a.x*b.z;
	result.z = a.x*b.y - a.y*b.x;
	return normalize3(result);
}

float dot3(float3 a, float3 b) {
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

float3 neg3(float3 v) {
	float3 result = {-v.x, -v.y, -v.z};
	return result;
}

mat4 mat4_camera(float3 position, float3 direction, float3 up) {
	float3 x = {0};
	float3 y = {0};
	float3 z = {0};

	float3 d = sub3(direction, position);

	z = /*neg3*/(normalize3(d));
	y = up;
	x = cross3(y, z);
	y = cross3(z, x);
	x = normalize3(x);
	y = normalize3(y);

	mat4 result = {
		-x.x, y.x, -z.x, 0,
		-x.y, y.y, -z.y, 0,
		-x.z, y.z, -z.z, 0,
		-dot3(x, position), -dot3(y, position), dot3(z, position), 1.0f,
	};
	/*mat4 lookat = {
		x.x, y.x, z.x, 0.0f,
		x.y, y.y, z.y, 0.0f,
		x.z, y.z, z.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	mat4 pos = {
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		-position.x, -position.y, -position.z, 1.0f,
	};*/
	/*mat4 lookat = {
		x.x, x.y, x.z, 0.0f,
		y.x, y.y, y.z, 0.0f,
		z.x, z.y, z.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	mat4 pos = {
		1.0f, 0.0f, 0.0f, -position.x,
		0.0f, 1.0f, 0.0f, -position.y,
		0.0f, 0.0f, 1.0f, -position.z,
		0.0f, 0.0f, 0.0f, 1.0f,
	};*/

	return result;//mat4_mul(lookat, pos);
}

quaternion quaternion_identity() {
	quaternion result = {0.0f, 0.0f, 0.0f, 1.0f};
	return result;
}

quaternion quaternion_mul(quaternion q1, quaternion q2) {
	quaternion q;
	q.w = q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z;
	q.x = q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y;
	q.y = q1.w*q2.y - q1.x*q2.z + q1.y*q2.w + q1.z*q2.x;
	q.z = q1.w*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.w;
	return q;
}

// change shouldn't be a quaternion cause it isn't a real one
// should just use seperate parameters instead
void quaternion_apply(quaternion *q, float3 axis, float angle) {
	quaternion local;
	local.w = cosf(angle/2.0f);
	local.x = axis.x * sinf(angle/2.0f);
	local.y = axis.y * sinf(angle/2.0f);
	local.z = axis.z * sinf(angle/2.0f);

	*q = quaternion_mul(local, *q);
}

mat4 quaternion_to_mat4(quaternion q) {
	float len = sqrtf(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
	float x = q.x / len;
	float y = q.y / len;
	float z = q.z / len;
	float w = q.w / len;

	/*mat4 result = {
	w*w + x*x - y*y - z*z, 2*x*y - 2*w*z, 2*x*z + 2*w*y, 0,
	2*x*y + 2*w*z, w*w - x*x + y*y - z*z, 2*y*z + 2*w*x, 0,
	2*x*z - 2*w*y, 2*y*z - 2*w*x, w*w - x*x - y*y + z*z, 0,
	0, 0, 0, 1,
	};*/

	// todo: optimize operation repetition
	mat4 result = {
		1 - 2*y*y - 2*z*z, 2*x*y - 2*w*z,     2*x*z + 2*w*y,     0,
		2*x*y + 2*w*z,     1 - 2*x*x - 2*z*z, 2*y*z + 2*w*x,     0,
		2*x*z - 2*w*y,     2*y*z - 2*w*x,     1 - 2*x*x - 2*y*y, 0,
		0, 0, 0, 1,
	};

	return result;
}

#endif