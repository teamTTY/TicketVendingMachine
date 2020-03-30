#pragma once
#include <memory>
#include <map>
#include <vector>
#include <list>
#include "MouseCtl.h"
#include "TicketMachine.h"

#define lpMyself Myself::GetInstance()

// ������
class Myself
{
public:
	static Myself& GetInstance()
	{
		return *s_Instance;
	}

	void Run();
	void Draw();			// �`�揈��
	void DrawUI();			// UI�`�揈��
	bool GetSysSuccess();	// �V�X�e���������̐����A���s��Ԃ�

private:
	bool SysInit();
	void SysEnd();

	struct MyselfDeleter
	{
		void operator()(Myself* myself) const
		{
			delete myself;
		}
	};

	Myself();
	~Myself();

	static std::unique_ptr<Myself, MyselfDeleter> s_Instance;

	std::unique_ptr<MouseCtl> mouse;	// �L�[�{�[�h���͂��Ǘ�����

	bool sysSuccess;					// �V�X�e���������̐����A���s�t���O

	MapInt havingCash;					// �����Ă��邨��  (first�F����Asecond�F����)

	std::map<std::string, int> images;	// �摜�n���h��

	std::vector<int> moneyType;			// �x������@

};

