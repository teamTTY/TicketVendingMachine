#include <Dxlib.h>
#include "MouseCtl.h"
#include "VECTOR2.h"

MouseCtl::MouseCtl()
{

}

MouseCtl::~MouseCtl()
{
}

// �N���b�N���𔻒肷��
bool MouseCtl::GetOnClick()
{
	if (mouseData && !mouseDataOld)
	{
		// �N���b�N����
		return true;
	}
	// �N���b�N���Ă��Ȃ�
	return false;
}

// �N���b�N��Ԃ𔻒肷��
bool MouseCtl::GetClicking()
{
	if (mouseData && mouseDataOld)
	{
		// �N���b�N���Ă���
		return true;
	}
	// �N���b�N���Ă��Ȃ�
	return false;
}

// �N���b�N�ʒu�擾
VECTOR2 MouseCtl::GetClickPos()
{
	VECTOR2 pos;
	GetMousePoint(&pos.x, &pos.y);
	return pos;
}

// ���͏�ԍX�V����
void MouseCtl::Update()
{
	mouseDataOld = mouseData;
	mouseData = GetMouseInput();
}
