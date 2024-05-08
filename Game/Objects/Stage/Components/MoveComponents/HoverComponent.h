#pragma once
#include "../Component.h"

class HoverComponent : public Component
{
private:
	float direction_;
	float tmpPositionY_;
	float speed_;
	float min_;
	float max_;
public:
	HoverComponent(StageObject* _holder, Component* _parent);

	void Initialize() override;
	void Update() override;
	void Release() override;

	void Save(json& _saveObj) override;
	void Load(json& _loadObj) override;

	void DrawData() override;
};

