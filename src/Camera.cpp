#include "Camera.h"

Camera::Camera()
{
    // noop
}

void Camera::setEye(float x, float y, float z)
{
    eye[0] = x;
    eye[1] = y;
    eye[2] = z;
}

void Camera::lookAt(vec3 target)
{
    vec3 centerToTarget;
    vec3_sub(centerToTarget, eye, target);

    vec3_add(forward, eye, centerToTarget);
    vec3_norm(forward, forward);

    // do up rotation?
}