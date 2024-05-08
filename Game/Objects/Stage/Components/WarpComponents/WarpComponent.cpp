#include "WarpComponent.h"

#include "../../StageObject.h"
#include "../../../Player/Player.h"
#include "../../../../../Engine/ResourceManager/VFX.h"
#include "../../../../../Engine/ImGui/imgui.h"


#define REFERENCE_XMFLOAT3(p) p.x,p.y,p.z // XMFLOAT3型の変数をコンマ区切りで表示する


WarpComponent::WarpComponent(StageObject* _holder, Component* _parent)
	:Component(_holder,"WarpComponent",Warp,_parent)
{
}

void WarpComponent::Initialize()
{
	holder_->AddCollider(new SphereCollider(XMFLOAT3(0, 2, 0), 1.0f));
}

void WarpComponent::Update()
{
	if (isRide_ == false) return;

    // エフェクトを適当に出力
    {
        EmitterData data;
        data.position = holder_->GetPosition();
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
    
    count_++;

    Player* p = (Player*)holder_->FindObject("Player");

    p->SetScale(0.5f / (count_ * 0.1f),1.f, 0.5f / (count_ * 0.1f));

    if (count_ >= 1.f * 60) {
        p->SetScale(1.f);
        p->SetPosition(destination_);
        p->Boot();
        isRide_ = false;
    }
}

void WarpComponent::Release()
{
}

void WarpComponent::Save(json& _saveObj)
{
    _saveObj["destination_"] = { REFERENCE_XMFLOAT3(destination_) };
}

void WarpComponent::Load(json& _loadObj)
{
    destination_ = { _loadObj["destination_"][0].get<float>(),_loadObj["destination_"][1].get<float>(), _loadObj["destination_"][2].get<float>() };
}

void WarpComponent::DrawData()
{
    ImGui::DragFloat3("destination_", &destination_.x, 0.1f);
}

void WarpComponent::OnCollision(GameObject* _target)
{
    if (_target->GetObjectName() == "Player") {
        isRide_ = true; ((Player*)holder_->FindObject("Player"))->Stop();
    }
}
