#include "PadLockComponent.h"

#include "../../StageObject.h"
#include "../../../Player/Player.h"
#include "../../Stage.h"
#include "../../../../../Engine/SceneManager.h"
PadLockComponent::PadLockComponent(StageObject* _holeder, Component* _parent)
	:Component(_holeder,"PadLockComponent",PadLock,_parent),isLock_(true)
{
}

void PadLockComponent::Initialize()
{
	holder_->AddCollider(new SphereCollider(XMFLOAT3(0, 0, 0), 1.2f));
}

void PadLockComponent::Update()
{
	if (isLock_ == false) {
		holder_->SetScale( 1 / count_);
		count_ += 0.01f;

		if (count_ > 1.f) {
			((Stage*)holder_->FindObject("Stage"))->DeleteStageObject(holder_);
			((SceneManager*)holder_->FindObject("SceneManager"))->ChangeScene(SCENE_ID_CLEAR, TID_WHITEOUT, 2.f);
		}
	}
}

void PadLockComponent::Release()
{
}

void PadLockComponent::Save(json& _saveObj)
{
}

void PadLockComponent::Load(json& _loadObj)
{
}

void PadLockComponent::DrawData()
{
}

void PadLockComponent::OnCollision(GameObject* _target)
{
	if (_target->GetObjectName() == "Player") {
		Player* p = (Player*)holder_->FindObject("Player");

		if (p->HasKey()) {
			this->Open();
			p->LostKey();
		}

	}
}
