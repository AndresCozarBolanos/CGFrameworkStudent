#include "camera.h"

#include "main/includes.h"
#include <iostream>

static inline float dot3(const Vector3& a, const Vector3& b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

static inline Vector3 cross3(const Vector3& a, const Vector3& b)
{
    return Vector3(
        a.y*b.z - a.z*b.y,
        a.z*b.x - a.x*b.z,
        a.x*b.y - a.y*b.x
    );
}

static inline Vector3 normalize3(const Vector3& v)
{
    float len = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
    if (len <= 1e-8f) return Vector3(0,0,0);
    return Vector3(v.x/len, v.y/len, v.z/len);
}

Camera::Camera()
{
	view_matrix.SetIdentity();
	SetOrthographic(-1,1,1,-1,-1,1);
}

Vector3 Camera::GetLocalVector(const Vector3& v)
{
	Matrix44 iV = view_matrix;
	if (iV.Inverse() == false)
		std::cout << "Matrix Inverse error" << std::endl;
	Vector3 result = iV.RotateVector(v);
	return result;
}

Vector3 Camera::ProjectVector(Vector3 pos)
{
	Vector4 pos4 = Vector4(pos.x, pos.y, pos.z, 1.0);
	Vector4 result = viewprojection_matrix * pos4;
	if (type == ORTHOGRAPHIC)
		return result.GetVector3();
	else
		return result.GetVector3() / result.w;
}

void Camera::Rotate(float angle, const Vector3& axis)
{
	Matrix44 R;
	R.MakeRotationMatrix(angle, axis);
	Vector3 new_front = R * (center - eye);
	center = eye + new_front;
	UpdateViewMatrix();
}

void Camera::Move(Vector3 delta)
{
	Vector3 localDelta = GetLocalVector(delta);
	eye = eye - localDelta;
	center = center - localDelta;
	UpdateViewMatrix();
}

void Camera::SetOrthographic(float left, float right, float top, float bottom, float near_plane, float far_plane)
{
	type = ORTHOGRAPHIC;

	this->left = left;
	this->right = right;
	this->top = top;
	this->bottom = bottom;
	this->near_plane = near_plane;
	this->far_plane = far_plane;

	UpdateProjectionMatrix();
}

void Camera::SetPerspective(float fov, float aspect, float near_plane, float far_plane)
{
	type = PERSPECTIVE;

	this->fov = fov;
	this->aspect = aspect;
	this->near_plane = near_plane;
	this->far_plane = far_plane;

	UpdateProjectionMatrix();
}

void Camera::LookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
{
	this->eye = eye;
	this->center = center;
	this->up = up;

	UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
    view_matrix.SetIdentity();

    // Right-handed LookAt (estilo OpenGL)
    Vector3 f = normalize3(center - eye);      // forward
    Vector3 s = normalize3(cross3(f, up));     // right
    Vector3 u = cross3(s, f);                  // true up

    // OJO: llenamos la matriz para que sea consistente con OpenGL
    // (y con que luego haces viewprojection = projection * view)

    view_matrix.M[0][0] =  s.x; view_matrix.M[1][0] =  s.y; view_matrix.M[2][0] =  s.z; view_matrix.M[3][0] = -dot3(s, eye);
    view_matrix.M[0][1] =  u.x; view_matrix.M[1][1] =  u.y; view_matrix.M[2][1] =  u.z; view_matrix.M[3][1] = -dot3(u, eye);
    view_matrix.M[0][2] = -f.x; view_matrix.M[1][2] = -f.y; view_matrix.M[2][2] = -f.z; view_matrix.M[3][2] =  dot3(f, eye);
    view_matrix.M[0][3] = 0.0f; view_matrix.M[1][3] = 0.0f; view_matrix.M[2][3] = 0.0f; view_matrix.M[3][3] = 1.0f;

    UpdateViewProjectionMatrix();
}


// Create a projection matrix
void Camera::UpdateProjectionMatrix()
{
    projection_matrix.SetIdentity();

    if (type == PERSPECTIVE)
    {
        // fov está en grados. trig en radianes.
        float fov_rad = fov * DEG2RAD;
        float f = 1.0f / tanf(fov_rad * 0.5f);

        float n = near_plane;
        float fa = far_plane;

        // OpenGL NDC z en [-1,1]
        projection_matrix.M[0][0] = f / aspect;
        projection_matrix.M[1][1] = f;

        projection_matrix.M[2][2] = (fa + n) / (n - fa);
        projection_matrix.M[2][3] = -1.0f;

        projection_matrix.M[3][2] = (2.0f * fa * n) / (n - fa);
        projection_matrix.M[3][3] = 0.0f;

        // El resto ya está a 0 por SetIdentity + sobrescrituras
        projection_matrix.M[0][1] = projection_matrix.M[0][2] = projection_matrix.M[0][3] = 0.0f;
        projection_matrix.M[1][0] = projection_matrix.M[1][2] = projection_matrix.M[1][3] = 0.0f;
        projection_matrix.M[2][0] = projection_matrix.M[2][1] = 0.0f;
        projection_matrix.M[3][0] = projection_matrix.M[3][1] = 0.0f;
    }
    else // ORTHOGRAPHIC
    {
        float l = left, r = right, b = bottom, t = top;
        float n = near_plane, fa = far_plane;

        projection_matrix.M[0][0] =  2.0f / (r - l);
        projection_matrix.M[1][1] =  2.0f / (t - b);
        projection_matrix.M[2][2] = -2.0f / (fa - n);

        projection_matrix.M[3][0] = -(r + l) / (r - l);
        projection_matrix.M[3][1] = -(t + b) / (t - b);
        projection_matrix.M[3][2] = -(fa + n) / (fa - n);
        projection_matrix.M[3][3] =  1.0f;

        projection_matrix.M[0][1] = projection_matrix.M[0][2] = projection_matrix.M[0][3] = 0.0f;
        projection_matrix.M[1][0] = projection_matrix.M[1][2] = projection_matrix.M[1][3] = 0.0f;
        projection_matrix.M[2][0] = projection_matrix.M[2][1] = projection_matrix.M[2][3] = 0.0f;
        projection_matrix.M[3][3] = 1.0f;
    }

    UpdateViewProjectionMatrix();
}


void Camera::UpdateViewProjectionMatrix()
{
	viewprojection_matrix = projection_matrix * view_matrix;
}

Matrix44 Camera::GetViewProjectionMatrix()
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();

	return viewprojection_matrix;
}

// The following methods have been created for testing.
// Do not modify them.

void Camera::SetExampleViewMatrix()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);
	glGetFloatv(GL_MODELVIEW_MATRIX, view_matrix.m );
}

void Camera::SetExampleProjectionMatrix()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (type == PERSPECTIVE)
		gluPerspective(fov, aspect, near_plane, far_plane);
	else
		glOrtho(left,right,bottom,top,near_plane,far_plane);

	glGetFloatv(GL_PROJECTION_MATRIX, projection_matrix.m );
	glMatrixMode(GL_MODELVIEW);
}