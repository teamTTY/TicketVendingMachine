#pragma once
class VECTOR2;
class MouseCtl
{
public:
	MouseCtl();
	~MouseCtl();

	bool GetOnClick();		// クリック時を判定する
	bool GetClicking();		// クリック状態を判定する
	VECTOR2 GetClickPos();	// クリック位置取得
	void Update();			// 入力状態更新処理

private:
	char mouseData;			// 現在のマウスの状態
	char mouseDataOld;		// 1フレーム前のマウスの状態
};

