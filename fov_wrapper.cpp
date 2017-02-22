#include "fov.h"
#include "map.h"
#include "fov_wrapper.h"
#include "display.h"


class CFovWrapperImpl : public CFovWrapper
{
public:
	CFovWrapperImpl();
	~CFovWrapperImpl();
	virtual void Init() override;
	virtual void Draw() override;
	virtual void PlayerMove(int dx, int dy) override;
	virtual void SetDirection(fov_direction_type dir) override
	{
		direction = dir;
	}
	virtual void IncreaseView(int delta) override
	{
		radius += delta;

		// ×îÐ¡ÊÇ1
		if (radius < 1)
			radius = 1;
	}
	virtual void IncreaseAngle(int delta) override
	{
		angle -= delta;
		angle = angle <= 0 ? 0 : (angle >= 360 ? 360 : angle);
	}
	virtual void ChangeShape(fov_shape_type shape) override
	{
		fov_settings_set_shape(&fov_settings, FOV_SHAPE_CIRCLE_PRECALCULATE);
	}
	virtual void ToggleBeam() override
	{
		beam = !beam;
	}


private:
	MAP map;
	int pX;
	int pY;
	unsigned radius;
	fov_direction_type direction;
	float angle;
	bool beam;
	bool apply_to_opaque;
	fov_settings_type fov_settings;
};

CFovWrapper* CreateFovInstance()
{
	CFovWrapper* ret = new CFovWrapperImpl();
	ret->Init();

	return ret;
}



CFovWrapperImpl::CFovWrapperImpl()
	:map(3)
{
	pX = 25;
	pY = 20;
	radius = 20;
	direction = FOV_EAST;
	angle = 130.0f;
	beam = false;
	apply_to_opaque = true;
}
CFovWrapperImpl::~CFovWrapperImpl()
{
	fov_settings_free(&fov_settings);
}

static void applyImpl(void *map, int x, int y, int dx, int dy, void *src) {
	if (((MAP *)map)->onMap(x, y))
		((MAP *)map)->setSeen(x, y);
}

static bool opaqueImpl(void *map, int x, int y) {
	return ((MAP *)map)->blockLOS(x, y);
}


void CFovWrapperImpl::Init()
{
	fov_settings_init(&fov_settings);
	fov_settings_set_opacity_test_function(&fov_settings, opaqueImpl);
	fov_settings_set_apply_lighting_function(&fov_settings, applyImpl);
}
void CFovWrapperImpl::Draw()
{
	if (beam) {
		fov_beam(&fov_settings, &map, NULL, pX, pY, radius, direction, angle);
	}
	else {
		fov_circle(&fov_settings, &map, NULL, pX, pY, radius);
	}

	map.display();
	display_put_char('@', pX, pY, 0x00, 0xFF, 0x00);
}
void CFovWrapperImpl::PlayerMove(int dx, int dy)
{
	unsigned newx = pX + dx;
	unsigned newy = pY + dy;
	if (map.onMap(newx, newy)) {
		pX = newx;
		pY = newy;
	}
}
