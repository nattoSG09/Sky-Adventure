#include "KeyComponent.h"

#include "../../StageObject.h"
#include "../../../Player/Player.h"
#include "../../Stage.h"
#include "../../../../../Engine/ResourceManager/VFX.h"

KeyComponent::KeyComponent(StageObject* _holeder, Component* _parent)
	:Component(_holeder,"KeyComponent",Key,_parent)
{
}

void KeyComponent::Initialize()
{
	holder_->AddCollider(new SphereCollider(XMFLOAT3(0, 0, 0), 1.2f));
	prevContactPosition_ = holder_->transform_.position_;

}

void KeyComponent::Update()
{
    if (isHit_) {
        holder_->transform_.rotate_.y += count_;
        holder_->transform_.position_.y += 0.1f;

        holder_->SetScale(1.f / (count_ * 0.1f));
        count_ += 1.f;
        if (holder_->transform_.position_.y >= prevContactPosition_.y + 5.f) {
            // プレイヤーは`key`を取得
            ((Player*)holder_->FindObject("Player"))->GetKey();

            // 削除
            ((Stage*)holder_->FindObject("Stage"))->DeleteStageObject(holder_);
        }
    }
}

void KeyComponent::Release()
{
}

void KeyComponent::Save(json& _saveObj)
{
}

void KeyComponent::Load(json& _loadObj)
{
}

void KeyComponent::DrawData()
{
}

void KeyComponent::OnCollision(GameObject* _target)
{
    // プレイヤーが`myObject_`に触れたら...
    if (_target->GetObjectName() == "Player") {
        isHit_ = true;

        EmitterData data;
        data.position = holder_->transform_.position_;
        //火の粉
        data.delay = 0;
        data.number = 80;
        data.lifeTime = 100;
        data.positionRnd = XMFLOAT3(0.5, 0, 0.5);
        data.direction = XMFLOAT3(0, 1, 0);
        data.directionRnd = XMFLOAT3(90, 90, 90);
        data.speed = 0.25f;
        data.speedRnd = 1;
        data.accel = 0.93;
        data.size = XMFLOAT2(0.5, 0.5);
        data.sizeRnd = XMFLOAT2(0.4, 0.4);
        data.scale = XMFLOAT2(0.99, 0.99);
        data.color = XMFLOAT4(0.2, 0.2, 0.5, 1);
        data.deltaColor = XMFLOAT4(0, 0, 0, 0);
        data.gravity = 0.003f;
        VFX::Start(data);

    }
}
