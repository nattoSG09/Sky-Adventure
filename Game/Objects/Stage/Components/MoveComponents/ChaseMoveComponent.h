#pragma once
#include "../Component.h"
#include "../../../../Others/Circle.h"

class ChaseMoveComponent : public Component
{
private:
	Circle recognitionDistance_;
	XMFLOAT2 center_;
	float speed_;
public:
	ChaseMoveComponent(StageObject* _holder, Component* _parent);
	
	void Initialize() override;
	void Update() override;
	void Release() override;

	void Save(json& _saveObj) override;
	void Load(json& _loadObj) override;

	void DrawData() override;
};

