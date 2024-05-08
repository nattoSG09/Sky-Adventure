#include "TestScene.h"

// �C���N���[�h
#include "../../Engine/ImGui/imgui.h"
#include "../Objects/Cameras/DebugCamera.h"
#include "../Objects/Cameras/TPSCamera.h"
#include "../Objects/Player/Player.h"
#include "../Objects/Stage/SkySphere.h"
#include "../Objects/Stage/Stage.h"
#include "../Others/UI/CrossHair.h"

TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene"),imageHandle_(-1)
{
}

void TestScene::Initialize()
{
	// `�X�J�C�X�t�B�A`���`��
	Instantiate<SkySphere>(this);

	// `�X�e�[�W`���`��
	Stage* pStage = Instantiate<Stage>(this); {

		// �X�e�[�W����ǂݍ���
		json loadObj;
		JsonReader::Load("Datas/stage01.json", loadObj);
		pStage->Load(loadObj);
	}

#ifdef _DEBUG
	// `�f�o�b�O�p�J����`���`�����A�����ʒu��ݒ�
	((DebugCamera*)Instantiate<DebugCamera>(this))->SetPosition(XMFLOAT3(20, 40, -80));

#else
	// `�v���C���[`���`��
	Player* pPlayer = Instantiate<Player>(this); {

		// �v���C���[�̈ʒu��ݒ�
		pPlayer->SetPosition(0, 10, 0);
	}

	// `�����[�X�p�J����`���`��
	TPSCamera* pTpsCamera = Instantiate<TPSCamera>(this); {

		// ����Ώۂ�`�v���C���[`�ɐݒ�
		pTpsCamera->SetTarget(pPlayer);
	}

	// `�N���X�w�A`��UI���`��
	Instantiate<CrossHair>(this);

#endif // _DEBUG
}
