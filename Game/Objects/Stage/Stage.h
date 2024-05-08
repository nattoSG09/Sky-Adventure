#pragma once

// �C���N���[�h
#include "../../../Engine/GameObject/GameObject.h"
#include "../../../Engine/Json/JsonReader.h"
#include "Components/Component.h"
#include <vector>

// using�錾
using std::vector;

// �O���錾
class StageObject;

// �X�e�[�W���Ǘ�����N���X
class Stage : public GameObject
{
private:
	vector<StageObject*> objects_;			// ���g���ۗL����X�e�[�W�I�u�W�F�N�g�Q
	int selectedIndex_;						// �I�𒆂̃I�u�W�F�N�g�̃C���f�b�N�X
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_parent">�e�I�u�W�F�N�g</param>
	Stage(GameObject* _parent);

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// ���
	/// </summary>
	void Release() override;

	/// <summary>
	/// �ۑ�
	/// </summary>
	/// <param name="_saveObj"></param>
	void Save(json& _saveObj);

	/// <summary>
	/// �Ǎ�
	/// </summary>
	/// <param name="_saveObj"></param>
	void Load(json& _loadObj);

	/// <summary>
	/// �X�e�[�W��ɑ��݂���I�u�W�F�N�g��R���g���[���p�l����ImGui�`��
	/// </summary>
	void DrawWorldOutliner();

	/// <summary>
	/// �I�����ꂽ�I�u�W�F�N�g�̏ڍ׏���ImGui�`��
	/// </summary>
	void DrawDatails();

	/// <summary>
	/// �I�u�W�F�N�g�����X�g�ɒǉ�
	/// </summary>
	/// <param name="_obj">�ǉ�����I�u�W�F�N�g�̃A�h���X</param>
	void AddStageObject(StageObject* _obj);

	/// <summary>
	/// �I�u�W�F�N�g�����X�g����폜
	/// </summary>
	/// <param name="_obj">�폜����I�u�W�F�N�g�̃A�h���X</param>
	void DeleteStageObject(StageObject* _obj);

	/// <summary>
	/// ���X�g���̃I�u�W�F�N�g�����ׂč폜
	/// </summary>
	void DeleteAllStageObject();

// getter:

	vector<StageObject*> GetStageObjects() { return objects_; }
};

