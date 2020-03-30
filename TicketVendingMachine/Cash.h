#pragma once
#include <utility>
#include <memory>
#include <map>
#include <list>

#define lpCash Cash::GetInstance()

using MapInt = std::map<int, int>;

// ����
class Cash
{
public:
	static Cash& GetInstance()
	{
		return *s_Instance;
	}

	bool Payment(MapInt inputMoney, int price);		// ���Ϗ���
	bool ChangeCalculation(int surplus);			// ���ނ�v�Z����
	MapInt GetChange();								// ���ނ��Ԃ�

private:
	void ChangeClear();	// ���ނ�map������������

	struct CashDeleter
	{
		void operator()(Cash* cash) const
		{
			delete cash;
		}
	};

	Cash();
	~Cash();

	static std::unique_ptr<Cash, CashDeleter> s_Instance;

	MapInt change;	// ���ނ�@(first�F����@second�F����)
	std::list<int>	moneyType;	// ���ނ���o���ۂ̋��탊�X�g
};

