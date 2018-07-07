#ifndef _CAMERA_H_
#define _CAMERA_H_
#include <linmath.h>

class Camera
{
    private:
        vec3 eye = {0.0f, 0.0f, 0.0f};
        vec3 up = {0.0f, 1.0f, 0.0f};
        vec3 forward = {0.0f, 0.0f, 1.0f};

    public:
        Camera();

        void setEye(float x, float y, float z);
        void lookAt(vec3 target);

        float* getEye()  { return eye; };
        float* getUp()      { return up; };
        float* getForward() { return forward; };
};

#endif