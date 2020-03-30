#include <Dxlib.h>
#include "MouseCtl.h"
#include "VECTOR2.h"

MouseCtl::MouseCtl()
{

}

MouseCtl::~MouseCtl()
{
}

// クリック時を判定する
bool MouseCtl::GetOnClick()
{
	if (mouseData && !mouseDataOld)
	{
		// クリックした
		return true;
	}
	// クリックしていない
	return false;
}

// クリック状態を判定する
bool MouseCtl::GetClicking()
{
	if (mouseData && mouseDataOld)
	{
		// クリックしている
		return true;
	}
	// クリックしていない
	return false;
}

// クリック位置取得
VECTOR2 MouseCtl::GetClickPos()
{
	VECTOR2 pos;
	GetMousePoint(&pos.x, &pos.y);
	return pos;
}

// 入力状態更新処理
void MouseCtl::Update()
{
	mouseDataOld = mouseData;
	mouseData = GetMouseInput();
}
