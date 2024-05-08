#include "sceneManager.h"

// �C���N���[�h
#include "ResourceManager/Model.h"
#include "ResourceManager/Image.h"
#include "ResourceManager/Audio.h"

// �e�V�[�� �̃C���N���[�h
#include "../Game/Scenes/TestScene.h"
#include "../Game/Scenes/SplashScene.h"
#include "../Game/Scenes/ClearScene.h"
#include "../Game/Scenes/TitleScene.h"

//�R���X�g���N�^
SceneManager::SceneManager(GameObject * parent)
	: GameObject(parent, "SceneManager"),currentSceneID_(),nextSceneID_(),tmpID_()
{
}

//������
void SceneManager::Initialize()
{
	//�ŏ��̃V�[��������
#ifdef _DEBUG
	currentSceneID_ = SCENE_ID_TEST;
	nextSceneID_ = currentSceneID_;
	Instantiate<TestScene>(this);
#else
	currentSceneID_ = SCENE_ID_SPLASH;
	nextSceneID_ = currentSceneID_;
	Instantiate<SplashScene>(this);
#endif // _DEBUG

}

//�X�V
void SceneManager::Update()
{
	//�g�����W�V�������s���A�V�[���ؑւ̃^�C�~���O�ŃV�[����ύX����
	if (Transition::IsChangePoint())nextSceneID_ = tmpID_;

	//���̃V�[�������݂̃V�[���ƈႤ�@���@�V�[����؂�ւ��Ȃ���΂Ȃ�Ȃ�
	if (currentSceneID_ != nextSceneID_)
	{
		//���̃V�[���̃I�u�W�F�N�g��S�폜
		KillAllChildren();

		//���[�h�����f�[�^��S�폜
		Audio::Release();
		Model::AllRelease();
		Image::AllRelease();

		//���̃V�[�����쐬
		switch (nextSceneID_)
		{
		case SCENE_ID_TEST: Instantiate<TestScene>(this); break;
		case SCENE_ID_TITLE: Instantiate<TitleScene>(this); break;
		case SCENE_ID_SPLASH: Instantiate<SplashScene>(this); break;
		case SCENE_ID_CLEAR: Instantiate<ClearScene>(this); break;
		}
		Audio::Initialize();
		currentSceneID_ = nextSceneID_;
	}
}

//�`��
void SceneManager::Draw()
{
}

//�J��
void SceneManager::Release()
{
}

//�V�[���؂�ւ��i���ۂɐ؂�ւ��̂͂��̎��̃t���[���j
void SceneManager::ChangeScene(SCENE_ID next, TRANSITION_ID _type, float _time)
{
	//�g�����W�V���������쒆�̓V�[���J�ڂ��s��Ȃ�
	if (!Transition::IsActive()) {
		//�g�����W�V�������g��Ȃ��ꍇ�A�V�[��ID���Z�b�g
		if (!Transition::SetTransition(_type))nextSceneID_ = next;

		//���Ԃ��Z�b�g
		Transition::SetTime(_time);

		//�g�����W�V�������J�n���A�V�[��ID���Z�b�g
		Transition::Start(); tmpID_ = next;
	}
}
void SceneManager::ChangeScene(SCENE_ID next, TRANSITION_ID _type) { ChangeScene(next, _type, 2); }
void SceneManager::ChangeScene(SCENE_ID next) { ChangeScene(next, TID_NONE); }

