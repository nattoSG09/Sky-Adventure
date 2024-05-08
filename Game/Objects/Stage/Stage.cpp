#include "Stage.h"

// インクルード
#include "StageObject.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/DirectX/Direct3D.h"


namespace {
	// 文字列内の全ての "\\" を "/" に置換する関数
	void ReplaceBackslashes(string& str) {
		size_t found = str.find("\\");
		while (found != string::npos) {
			str.replace(found, 1, "/"); // "\\" を "/" に置換する
			found = str.find("\\", found + 1); // 次の "\\" を検索する
		}
	}

	// カレントディレクトリからの相対パスを取得する関数
	std::string GetAssetsRelativePath(const std::string& absolutePath) {
		const std::string assetsFolder = "Assets\\";
		// "Assets\\" の位置を検索
		size_t assetsPos = absolutePath.find(assetsFolder);
		if (assetsPos != std::string::npos) {
			// "Assets\\" の部分を除いた、それ以降の部分を取得
			std::string relativePath = absolutePath.substr(assetsPos + assetsFolder.size());
			return relativePath;
		}
		else {
			// "Assets\\" が見つからない場合は、元のファイルパスを返す
			return absolutePath;
		}
	}

}

Stage::Stage(GameObject* _parent)
	:GameObject(_parent,"Stage"),objects_()
{
}

void Stage::Initialize()
{
}

void Stage::Update()
{
}

void Stage::Draw()
{
#ifdef _DEBUG
	DrawWorldOutliner();
	DrawDatails();
#endif // _DEBUG

}

void Stage::Release()
{
}

void Stage::Save(json& _saveObj)
{
	// オブジェクト群を保存
	for (auto obj : objects_)obj->Save(_saveObj[obj->GetObjectName()]);
}

void Stage::Load(json& _loadObj)
{
	// ステージオブジェクトをすべて削除
	DeleteAllStageObject();

	for (auto it = _loadObj.begin(); it != _loadObj.end();++it) {
		
		// オブジェクトのインスタンスを生成
		StageObject* obj = CreateStageObject(it.key(), it.value()["modelFilePath_"], this);
		
		// オブジェクト情報を読込
		obj->Load(it.value());

		// オブジェクトをリストに追加
		AddStageObject(obj);
	}
}

void Stage::DrawWorldOutliner()
{
	// ImGuiで表示するウィンドウの設定を行う
	ImGui::SetNextWindowPos(ImVec2(Direct3D::screenWidth_ * 0.7f, 0));
	ImGui::SetNextWindowSize(ImVec2(Direct3D::screenWidth_ * 0.3f, Direct3D::screenHeight_ * 0.5f));
	
	// ウィンドウを表示
	ImGui::Begin("World Outliner", NULL,
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	{
		// - ActionButtons ------------------------------------------ 
		ImGui::Text("Stage control Buttons");

		// `追加`ボタンを表示
		if (ImGui::Button("Add")) {
			// 追加するオブジェクトの初期名を設定
			string name = "object" + std::to_string(objects_.size() + 1);

			//現在のカレントディレクトリを覚えておく
			char defaultCurrentDir[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

			// 追加するオブジェクトのモデルファイルパスを設定
			string filePath{}; {
				// 「ファイルを開く」ダイアログの設定用構造体を設定
				OPENFILENAME ofn; {
					TCHAR szFile[MAX_PATH] = {}; // ファイル名を格納するバッファ
					ZeroMemory(&ofn, sizeof(ofn)); // 構造体の初期化
					ofn.lStructSize = sizeof(ofn); // 構造体のサイズ
					ofn.lpstrFile = szFile; // ファイル名を格納するバッファ
					ofn.lpstrFile[0] = '\0'; // 初期化
					ofn.nMaxFile = sizeof(szFile); // ファイル名バッファのサイズ
					ofn.lpstrFilter = TEXT("FBXファイル(*.fbx)\0*.fbx\0すべてのファイル(*.*)\0*.*\0"); // フィルター（FBXファイルのみ表示）
					ofn.nFilterIndex = 1; // 初期選択するフィルター
					ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // フラグ（ファイルが存在すること、パスが存在することを確認）
					ofn.lpstrInitialDir = TEXT("."); // カレントディレクトリを初期選択位置として設定
				}

				// ファイルを選択するダイアログの表示
				if (GetOpenFileName(&ofn) == TRUE) {
					// ファイルパスを取得
					filePath = ofn.lpstrFile;

					// カレントディレクトリからの相対パスを取得
					filePath = GetAssetsRelativePath(filePath);

					// 文字列内の"\\"を"/"に置換
					ReplaceBackslashes(filePath);

					// ディレクトリを戻す
					SetCurrentDirectory(defaultCurrentDir);
				}
				else {
					// 作成を中断
					ImGui::End();
					return;
				}
			}

			// オブジェクトのリストに追加
			this->AddStageObject(CreateStageObject(name, filePath, this));
		}
		ImGui::SameLine();

		// `保存`ボタンを表示
		if (ImGui::Button("Save")) {
			//現在のカレントディレクトリを覚えておく
			char defaultCurrentDir[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

			// 保存先のファイルパスを取得
			string filePath{}; {
				// 「ファイルを保存」ダイアログの設定用構造体を設定
				OPENFILENAME ofn; {
					ZeroMemory(&ofn, sizeof(ofn));
					ofn.lStructSize = sizeof(OPENFILENAME);
					ofn.lpstrFilter = TEXT("objectData(*.json)\0*.json\0すべてのファイル(*.*)\0*.*\0\0");
					char fileName[MAX_PATH] = "無題.json";
					ofn.lpstrFile = fileName;
					ofn.nMaxFile = MAX_PATH;
					ofn.Flags = OFN_OVERWRITEPROMPT;
					ofn.lpstrDefExt = "json";
					ofn.nFilterIndex = 1; // 初期選択するフィルター
					ofn.lpstrInitialDir = TEXT("."); // カレントディレクトリを初期選択位置として設定
				}

				// ファイルを保存するダイアログの表示
				if (GetSaveFileName(&ofn) == TRUE) {
					// ファイルパスを取得
					filePath = ofn.lpstrFile;

					// カレントディレクトリからの相対パスを取得
					filePath = GetAssetsRelativePath(filePath);

					// 文字列内の"\\"を"/"に置換
					ReplaceBackslashes(filePath);

					// ディレクトリを戻す
					SetCurrentDirectory(defaultCurrentDir);
				}
				else {
					// 保存を中断
					ImGui::End();
					return;
				}
			}

			// 保存オブジェクトを作成
			json saveObj;

			// オブジェクトにデータを格納
			this->Save(saveObj);

			// ファイルにデータを保存
			JsonReader::Save(filePath, saveObj);
		}
		ImGui::SameLine();

		// `読込`ボタンを表示
		if (ImGui::Button("Load")) {
			//現在のカレントディレクトリを覚えておく
			char defaultCurrentDir[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

			// 読み込むファイルのパスを取得
			string filePath{}; {
				// 「ファイルを開く」ダイアログの設定用構造体を設定
				OPENFILENAME ofn; {
					TCHAR szFile[MAX_PATH] = {}; // ファイル名を格納するバッファ
					ZeroMemory(&ofn, sizeof(ofn)); // 構造体の初期化
					ofn.lStructSize = sizeof(ofn); // 構造体のサイズ
					ofn.lpstrFile = szFile; // ファイル名を格納するバッファ
					ofn.lpstrFile[0] = '\0'; // 初期化
					ofn.nMaxFile = sizeof(szFile); // ファイル名バッファのサイズ
					ofn.lpstrFilter = TEXT("JSONファイル(*.json)\0*.json\0すべてのファイル(*.*)\0*.*\0"); // フィルター（FBXファイルのみ表示）
					ofn.nFilterIndex = 1; // 初期選択するフィルター
					ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // フラグ（ファイルが存在すること、パスが存在することを確認）
					ofn.lpstrInitialDir = TEXT("."); // カレントディレクトリを初期選択位置として設定
				}

				// ファイルを選択するダイアログの表示
				if (GetOpenFileName(&ofn) == TRUE) {
					// ファイルパスを取得
					filePath = ofn.lpstrFile;

					// カレントディレクトリからの相対パスを取得
					filePath = GetAssetsRelativePath(filePath);

					// 文字列内の"\\"を"/"に置換
					ReplaceBackslashes(filePath);

					// ディレクトリを戻す
					SetCurrentDirectory(defaultCurrentDir);
				}
				else {
					// 作成を中断
					ImGui::End();
					return;
				}
			}

			// 読込オブジェクトを作成
			json loadObj;

			// ファイルからデータを読込
			JsonReader::Load(filePath, loadObj);

			// オブジェクトからデータを読込
			this->Load(loadObj);
		}
		ImGui::SameLine();

		// `削除`ボタンを表示
		if (ImGui::Button("Delete"))this->DeleteAllStageObject();

		// 区切りの為、ラインを表示
		ImGui::Separator();

		// - ObjectList ---------------------------------------------
		ImGui::BeginChild("ObjectList"); {
			// リストを表示
			for (int i = 0; i < objects_.size(); ++i)
				if (ImGui::Selectable(objects_[i]->GetObjectName().c_str(), selectedIndex_ == i)) {
					selectedIndex_ = i;
				}
		}
		ImGui::EndChild();
		// ----------------------------------------------------------
	}
	ImGui::End();
}

void Stage::DrawDatails()
{
	// ImGuiで表示するウィンドウの設定を行う
	ImGui::SetNextWindowPos(ImVec2(Direct3D::screenWidth_ * 0.7f, Direct3D::screenHeight_ * 0.5f));
	ImGui::SetNextWindowSize(ImVec2(Direct3D::screenWidth_ * 0.3f, Direct3D::screenHeight_ * 0.5f));

	// ウィンドウを表示
	ImGui::Begin("Datails", NULL,
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	{
		// 選択されているオブジェクトの詳細を表示
		if (selectedIndex_ >= 0 && selectedIndex_ < objects_.size()) {

			objects_[selectedIndex_]->DrawData();
		}
		else ImGui::Text("No information to display");
	}
	ImGui::End();
}

void Stage::AddStageObject(StageObject* _obj)
{
	// リストに追加
	if (_obj != nullptr)objects_.push_back(_obj);
}

void Stage::DeleteStageObject(StageObject* _obj)
{
	// オブジェクトを削除する
	_obj->KillMe();

	// オブジェクトのイテレータを取得する
	auto it = std::find(objects_.begin(), objects_.end(), _obj);

	// イテレータが見つかった場合、ベクターから削除する
	if (it != objects_.end()) objects_.erase(it);
}

void Stage::DeleteAllStageObject()
{
	// リスト内にある要素をすべて削除
	for (auto obj : objects_) obj->KillMe();
	objects_.clear();
}
