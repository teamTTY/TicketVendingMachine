#pragma once
class VECTOR2;
class MouseCtl
{
public:
	MouseCtl();
	~MouseCtl();

	bool GetOnClick();		// �N���b�N���𔻒肷��
	bool GetClicking();		// �N���b�N��Ԃ𔻒肷��
	VECTOR2 GetClickPos();	// �N���b�N�ʒu�擾
	void Update();			// ���͏�ԍX�V����

private:
	char mouseData;			// ���݂̃}�E�X�̏��
	char mouseDataOld;		// 1�t���[���O�̃}�E�X�̏��
};

