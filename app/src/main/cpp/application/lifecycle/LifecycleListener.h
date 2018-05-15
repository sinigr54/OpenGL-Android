//
// Created by Alexander Chekh on 15.05.2018.
//

#ifndef OPENGLCOURSE_LIFECYCLELISTENER_H
#define OPENGLCOURSE_LIFECYCLELISTENER_H

class LifecycleListener {
public:

    virtual void onPause() = 0;

    virtual void onResume(void *env, void *context, void *activity) = 0;
};

#endif //OPENGLCOURSE_LIFECYCLELISTENER_H
