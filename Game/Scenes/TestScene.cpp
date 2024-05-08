#include "TestScene.h"

// インクルード
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
	// `スカイスフィア`を形成
	Instantiate<SkySphere>(this);

	// `ステージ`を形成
	Stage* pStage = Instantiate<Stage>(this); {

		// ステージ情報を読み込む
		json loadObj;
		JsonReader::Load("Datas/stage01.json", loadObj);
		pStage->Load(loadObj);
	}

#ifdef _DEBUG
	// `デバッグ用カメラ`を形成し、初期位置を設定
	((DebugCamera*)Instantiate<DebugCamera>(this))->SetPosition(XMFLOAT3(20, 40, -80));

#else
	// `プレイヤー`を形成
	Player* pPlayer = Instantiate<Player>(this); {

		// プレイヤーの位置を設定
		pPlayer->SetPosition(0, 10, 0);
	}

	// `リリース用カメラ`を形成
	TPSCamera* pTpsCamera = Instantiate<TPSCamera>(this); {

		// 見る対象を`プレイヤー`に設定
		pTpsCamera->SetTarget(pPlayer);
	}

	// `クロスヘア`のUIを形成
	Instantiate<CrossHair>(this);

#endif // _DEBUG
}
