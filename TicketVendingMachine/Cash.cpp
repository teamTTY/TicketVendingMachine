#include <DxLib.h>
#include "Cash.h"

// ����
using namespace std;

unique_ptr<Cash, Cash::CashDeleter> Cash::s_Instance(new Cash());

// ���Ϗ���
bool Cash::Payment(MapInt inputMoney, int price)
{
	for (auto moneyData : inputMoney)
	{
		for (int cnt = 0; cnt < moneyData.second; cnt++)
		{
			price -= moneyData.first;
			if (price <= 0)
			{
				// �܂��������c���Ă����炻�̋��z�����ނ�Ƃ���
				if ((cnt + 1) < moneyData.second)
				{
					price -= ((moneyData.second - (cnt + 1)) * moneyData.first);
				}
				break;
			}
		}
	}

	// �������z�Ŏx�����Ȃ������ꍇ�͎��s��Ԃ�
	if (price > 0)
	{
		return false;
	}

	int surplus = (-price); // ���ނ肪����ۂ̓}�C�i�X�l�ɂȂ邽�߃v���X�l�ɕϊ�
	ChangeCalculation(surplus);
	return true;
}

// ���ނ�v�Z����
bool Cash::ChangeCalculation(int surplus)
{
	ChangeClear();
	for (auto type : moneyType)
	{
		while (type <= surplus)
		{
			change.try_emplace(type, 0);

			// �����𑝂₷
			change[type]++;

			surplus -= type;
		}
	}
	return true;
}

// ���ނ��Ԃ�
MapInt Cash::GetChange()
{
	return change;
}

// ���ނ�map������������
void Cash::ChangeClear()
{
	change.clear();
}

Cash::Cash()
{
	// ���ނ�p�̍~���̋��탊�X�g�����
	moneyType.emplace_back(10000);
	moneyType.emplace_back(5000);
	moneyType.emplace_back(1000);
	moneyType.emplace_back(500);
	moneyType.emplace_back(100);
	moneyType.emplace_back(50);
	moneyType.emplace_back(10);
}

Cash::~Cash()
{
}
