#pragma once

#include "../../../Engine/GameObject/GameObject.h"

class Player : public GameObject
{
private:
	int modelHandle_;	// ���f���ԍ�

	bool isActive_;		// ���쒆���ǂ���
	bool isAnim_;		// �A�j���[�V�������쒆���ǂ���
	bool hasKey_;		// ���������Ă��邩�ǂ���
	bool isAlive_;
public:
	Player(GameObject* _parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void Move(XMVECTOR dir, float speed);

	void Stop() { isActive_ = false; isAnim_ = false; }
	void Boot() { isActive_ = true; isAnim_ = true; }
	void GetKey() { hasKey_ = true; }
	void LostKey() { hasKey_ = false; }
	bool HasKey() { return hasKey_; }

	void Dead() { isAlive_ = false; }
private:
	void Shot();
};

