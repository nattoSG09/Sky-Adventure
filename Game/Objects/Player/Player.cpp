#include "Player.h"

// インクルード
#include "../../../Engine/ResourceManager/Model.h"
#include "../../../Engine/DirectX/Input.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../Cameras/TPSCamera.h"
#include "../../../Engine/GameObject/Camera.h"
#include "../Stage/StageObject.h"
#include "../Stage/Stage.h"
#include "../Stage/Components/TargetComponents/TargetComponent.h"
#include "../../../Engine/ResourceManager/VFX.h"
#include "../../../Engine/SceneManager.h"

namespace {
	const float TWEAK_CAM_ROT_NUM = 25.f;
	const float REACH_DISTANCE = 50.f;
	const float ATTACK_DAMAGE = 15.f;
}

Player::Player(GameObject* _parent)
	:GameObject(_parent,"Player"),isActive_(true), isAlive_(true)
{
}

void Player::Initialize()
{
	// モデルの読込
	modelHandle_ = Model::Load("Models/Player/Firing Rifle.fbx");
	assert(modelHandle_ >= 0);

	// コライダーを追加
	AddCollider(new SphereCollider(XMFLOAT3(0, 0, 0), 1.2f));
}

void Player::Update()
{
	// アニメーションを行う
	{
		static bool prevAnim = false;
		if (isAnim_ == true) {
			if (prevAnim == false)Model::SetAnimFrame(modelHandle_, 0, 82, 1);
			prevAnim = true;
		}
		else {
			Model::SetAnimFrame(modelHandle_, 0, 0, 0);
			prevAnim = false;
		}
	}

	if (isAlive_ == false) {
		
		// 動きを停止
		isActive_ = false;
		isAnim_ = false;

		// Effectを出す
		{
			EmitterData data;

			//炎
			data.textureFileName = "Images/particle/cloudA.png";

			data.position = GetPosition();
			data.position.y += 3.f;
			data.delay = 0;
			data.number = 80;
			data.lifeTime = 30;
			data.direction = XMFLOAT3(0, 1, 0);
			data.directionRnd = XMFLOAT3(90, 90, 90);
			data.speed = 0.1f;
			data.speedRnd = 0.8;
			data.size = XMFLOAT2(1.2, 1.2);
			data.sizeRnd = XMFLOAT2(0.4, 0.4);
			data.scale = XMFLOAT2(1.05, 1.05);
			data.color = XMFLOAT4(0.5, 0.5, 0.1, 1);
			data.deltaColor = XMFLOAT4(0, -1.0 / 20, 0, -1.0 / 20);
			VFX::Start(data);

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
			data.size = XMFLOAT2(0.1, 0.1);
			data.sizeRnd = XMFLOAT2(0.4, 0.4);
			data.scale = XMFLOAT2(0.99, 0.99);
			data.color = XMFLOAT4(0.4, 0.2, 0.0, 1);
			data.deltaColor = XMFLOAT4(0, 0, 0, 0);
			data.gravity = 0.003f;
			VFX::Start(data);
		}

		((SceneManager*)FindObject("SceneManager"))->ChangeScene(SCENE_ID_CLEAR, TID_BLACKOUT, 2.f);
	}

	if (isActive_ == false)return;

	// アニメーションを行うかどうか
	isAnim_ = false;


	// 移動処理
	{
		TPSCamera* cam = (TPSCamera*)FindObject("TPSCamera");

		float angle = 0;
		if (cam != nullptr)angle = cam->GetAngle().y;

		// 方向を設定
		XMVECTOR dir{}; {
			// 視線ベクトルを取得
			XMVECTOR sightline = XMVector3Normalize(XMVectorSetY(Camera::GetSightline(), 0));

			// 前方に移動
			if (Input::IsKey(DIK_W)) {
				dir += sightline; isAnim_ = true;
				transform_.rotate_.y = angle - TWEAK_CAM_ROT_NUM;
			}

			// 後方に移動
			if (Input::IsKey(DIK_S)) {
				dir += -sightline; isAnim_ = true;
				transform_.rotate_.y = (angle - TWEAK_CAM_ROT_NUM) + 180;
			}

			// 左方に移動
			if (Input::IsKey(DIK_A)) {
				dir += XMVector3Transform(sightline, XMMatrixRotationY(XMConvertToRadians(-90))); isAnim_ = true;
				transform_.rotate_.y = (angle - TWEAK_CAM_ROT_NUM) - 90;
			}

			// 右方に移動
			if (Input::IsKey(DIK_D)) {
				dir += XMVector3Transform(sightline, XMMatrixRotationY(XMConvertToRadians(90))); isAnim_ = true;
				transform_.rotate_.y = (angle - TWEAK_CAM_ROT_NUM) + 90;
			}

			// 正規化して
			dir = XMVector3Normalize(dir);
		}

		// 速度を設定
		float speed = 0.1f;

		// 移動
		Move(dir, speed);
	}

#ifdef _DEBUG
	ImGui::Text("has key = %s", hasKey_ ? "true" : "false");

#endif // _DEBUG

	if (Input::IsMouseButtonDown(0))Shot();
}

void Player::Draw()
{
	Model::SetTransform(modelHandle_, transform_);
	Model::Draw(modelHandle_);
}

void Player::Release()
{
}

void Player::Move(XMVECTOR dir, float speed)
{
	XMStoreFloat3(&transform_.position_, XMLoadFloat3(&transform_.position_) + (dir * speed));
}

void Player::Shot()
{
	// 視線ベクトルを取得
	XMVECTOR sightLine = XMVector3Normalize(Camera::GetSightline());

	// ターゲットコンポーネントがついているオブジェクトをすべて取得
	vector<StageObject*> targets; {
		for (auto obj : ((Stage*)FindObject("Stage"))->GetStageObjects()) 
			if (obj->FindComponent(Target) == true)targets.push_back(obj);
	}

	// ターゲットに対してレイキャストを発射
	for (auto target : targets) {
		
		// 発射するレイを設定
		RayCastData sightRay; {
			XMStoreFloat3(&sightRay.dir, sightLine);
			sightRay.start = Camera::GetPosition();
		}
		
		//発射
		Model::RayCast(target->GatModelHandle(), &sightRay);

		if (sightRay.hit && sightRay.dist <= REACH_DISTANCE) {
			
			// Effectを出す
			{
				EmitterData data;

				//炎
				data.textureFileName = "Images/particle/cloudA.png";

				data.position = target->GetPosition();
				data.position.y += 3.f;
				data.delay = 0;
				data.number = 80;
				data.lifeTime = 30;
				data.direction = XMFLOAT3(0, 1, 0);
				data.directionRnd = XMFLOAT3(90, 90, 90);
				data.speed = 0.1f;
				data.speedRnd = 0.8;
				data.size = XMFLOAT2(1.2, 1.2);
				data.sizeRnd = XMFLOAT2(0.4, 0.4);
				data.scale = XMFLOAT2(1.05, 1.05);
				data.color = XMFLOAT4(0.5, 0.5, 0.1, 1);
				data.deltaColor = XMFLOAT4(0, -1.0 / 20, 0, -1.0 / 20);
				VFX::Start(data);

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
				data.size = XMFLOAT2(0.1, 0.1);
				data.sizeRnd = XMFLOAT2(0.4, 0.4);
				data.scale = XMFLOAT2(0.99, 0.99);
				data.color = XMFLOAT4(0.4, 0.2, 0.0, 1);
				data.deltaColor = XMFLOAT4(0, 0, 0, 0);
				data.gravity = 0.003f;
				VFX::Start(data);
			}

			// ダメージを与える
			((TargetComponent*)target->GetComponent(Target))->ReceiveDamage(ATTACK_DAMAGE);
		}
	}
}
