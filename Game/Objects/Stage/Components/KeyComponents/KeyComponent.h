#pragma once
#include "../Component.h"

#include <DirectXMath.h>
using namespace DirectX;

// X�����ɕ��s�ړ����s���R���|�[�l���g�̃N���X
class KeyComponent : public Component
{
private:
	bool isHit_;					// �ڐG�������ǂ���
	float count_;					// ���Ԍo��
	XMFLOAT3 prevContactPosition_;	// �ڐG�������_�ł̈ʒu

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_holeder">�ۗL��</param>
	KeyComponent(StageObject* _holeder, Component* _parent);

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �J��
	/// </summary>
	void Release() override;

	/// <summary>
	/// �ۑ�
	/// </summary>
	/// <param name="_saveObj">�ۑ����</param>
	void Save(json& _saveObj) override;

	/// <summary>
	/// �Ǎ�
	/// </summary>
	/// <param name="_loadObj">�Ǎ����</param>
	void Load(json& _loadObj) override;

	/// <summary>
	/// ImGui�\��
	/// </summary>
	void DrawData() override;

	void OnCollision(GameObject* _target) override;

};

