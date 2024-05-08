#pragma once

#include "../Component.h"

#include <DirectXMath.h>
using namespace DirectX;

class WarpComponent : public Component
{
private:
	XMFLOAT3 destination_;	// ���[�v��̈ʒu
	bool isRide_;			// ���[�v�n�_�ɏ���Ă��邩�ǂ���
	float count_;			// �o�ߎ���
public:
	WarpComponent(StageObject* _holder, Component* _parent);
	
	void Initialize() override;
	void Update() override;
	void Release() override;

	void Save(json& _saveObj) override;
	void Load(json& _loadObj) override;

	void DrawData() override;

	void OnCollision(GameObject* _target) override;
};

