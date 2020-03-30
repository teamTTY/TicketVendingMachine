#include <DxLib.h>
#include "Cash.h"

// 現金
using namespace std;

unique_ptr<Cash, Cash::CashDeleter> Cash::s_Instance(new Cash());

// 決済処理
bool Cash::Payment(MapInt inputMoney, int price)
{
	for (auto moneyData : inputMoney)
	{
		for (int cnt = 0; cnt < moneyData.second; cnt++)
		{
			price -= moneyData.first;
			if (price <= 0)
			{
				// まだ枚数が残っていたらその金額分お釣りとする
				if ((cnt + 1) < moneyData.second)
				{
					price -= ((moneyData.second - (cnt + 1)) * moneyData.first);
				}
				break;
			}
		}
	}

	// 投入金額で支払えなかった場合は失敗を返す
	if (price > 0)
	{
		return false;
	}

	int surplus = (-price); // お釣りがある際はマイナス値になるためプラス値に変換
	ChangeCalculation(surplus);
	return true;
}

// お釣り計算処理
bool Cash::ChangeCalculation(int surplus)
{
	ChangeClear();
	for (auto type : moneyType)
	{
		while (type <= surplus)
		{
			change.try_emplace(type, 0);

			// 枚数を増やす
			change[type]++;

			surplus -= type;
		}
	}
	return true;
}

// お釣りを返す
MapInt Cash::GetChange()
{
	return change;
}

// お釣りmapを初期化する
void Cash::ChangeClear()
{
	change.clear();
}

Cash::Cash()
{
	// お釣り用の降順の金種リストを作る
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
