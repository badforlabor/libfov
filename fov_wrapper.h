#pragma once
#ifndef _FOV_WRAPPER_
#define _FOV_WRAPPER_

#include "fov.h"

class CFovWrapper
{
public:
	virtual void Init() {};
	virtual void Draw() {};
	virtual void PlayerMove(int dx, int dy) {};
	virtual void SetDirection(fov_direction_type dir) {};
	virtual void IncreaseView(int delta) {}
	virtual void IncreaseAngle(int delta) {}
	virtual void ChangeShape(fov_shape_type shape) {}
	virtual void ToggleBeam() {}
};

extern CFovWrapper* CreateFovInstance();


#endif