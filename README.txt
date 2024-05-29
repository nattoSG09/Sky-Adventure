**開発者情報**

- 氏名：菅原悠人（スガワラユウト）
- 連絡先：220369@jc-21.jp

**作品概要**
プレイヤーは、襲い掛かる敵を倒しながらゴールを目指す。鍵を拾い、南京錠の鍵を開けることを目的に進んでいくゲームである。


**アピールポイント**
- 肩越し３人称カメラでプレイヤーの視認性を向上させた
- ステージエディターにより、プログラマ以外の人のも簡単にステージ作成ができる
- Jsonファイルによるデータの管理によりデータとソースとの依存性を低くした


**開発期間**
6か月（※最初の３か月ほどは、DirectX11の学習、及び必要な機能の設計・製作）

**開発環境・言語**
- Microsoft VisualStudio 2022
- Windows API
- DirectX11
- Github(SourceTree)
- Maya
- IbisPaint

**謝辞**
以下のダウンロード素材を使用しています。

- https://kenney.itch.io/kenney-game-assets（ステージモデルに使用）

**ファイル内容**

```
├─Engine	ゲームエンジン関連
　├─Collider			コライダーによる当たり判定処理
　├─DirectX			DirectXによる描画処理など
　├─GameObject		ゲームオブジェクトに必要な処理
　├─ImGui			ImGui関連
　├─Json			Jsonを利用するために必要なもの
　├─ResourceManager	ハンドルで管理されたオブジェクトを管理する
├─Game		◇ゲームに必要な要素
　├─Objects			◇ゲームオブジェクト
　　├─Cameras		◇カメラの処理
　　├─Player			◇プレイヤーの処理
　　├─Stage			◇ステージ関連の処理
　　　├─Components		◇コンポーネント関連
　　　　├─KeyComponents		◇鍵コンポーネント処理
　　　　├─MoveComponents		◇動作コンポーネント処理
　　　　├─RotationComponents	◇回転コンポーネント処理
　　　　├─TargetComponents	◇対象コンポーネント処理
　　　　├─WarpComponents		◇ワープコンポーネント処理
　　├─Others		◇その他
　　　├─UI			◇UIにまつわるもの
　　├─Scenes		◇シーン関連ファイル
```

学校で提供されているエンジンのようなベースを使用しています。
◇マークがついているものは自身が作成したものです。
