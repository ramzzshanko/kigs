#ifndef _OPENGLCAMERA_H
#define _OPENGLCAMERA_H

#include "Camera.h"
//#include "GLSLDeferredFilter.h"


class OpenGLCamera : public Camera
{
public:
    DECLARE_CLASS_INFO(OpenGLCamera,Camera,Renderer)

    OpenGLCamera(const kstl::string& name,DECLARE_CLASS_NAME_TREE_ARG);

protected:

	//void SetAmbient(kfloat r, kfloat g, kfloat b) override;

	bool ProtectedSetActive(TravState* state) override;
	virtual void PlatformProtectedSetActive(TravState* state);

	void ProtectedRelease(TravState* state) override;
	virtual void PlatformProtectedRelease(TravState* state);

	virtual ~OpenGLCamera();

#ifdef WUP
	bool myNeedNearFarUpdate = true;
	void NotifyUpdate(unsigned int labelid) override;
#endif
};    

#endif //_OPENGLCAMERA_H
