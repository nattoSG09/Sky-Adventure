#pragma once

#include "../Component.h"

class TargetComponent : public Component
{
private:
	float hp_;

public:
	TargetComponent(StageObject* _holder, Component* _parent);

	void Initialize() override;
	void Update() override;
	void Release() override;

	void Save(json& _saveObj) override;
	void Load(json& _loadObj) override;

	void DrawData() override;

	void ReceiveDamage(float _damage);

	void OnCollision(GameObject* _target) override;

};

