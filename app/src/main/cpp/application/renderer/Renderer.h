//
// Created by sinigr on 3/21/18.
//

#ifndef OPENGLCOURSE_RENDERER_H
#define OPENGLCOURSE_RENDERER_H


class Renderer {
public:
    void onSurfaceCreated();

    void onSurfaceChanged(int width, int height);

    void onDrawFrame();
};


#endif //OPENGLCOURSE_RENDERER_H
