#include <DxLib.h>
#include "CardServer.h"

// 電子マネーサーバー
using namespace std;

std::pair<int, int> CardServer::payment;

unique_ptr<CardServer, CardServer::CardServerDeleter> CardServer::s_Instance(new CardServer());

// 決済処理
bool CardServer::Payment(int price)
{
	int num = payment.first;
	num -= price;

	if (num < 0)
	{
		return false;
	}
	payment.first = num;
	payment.second = price;
	return true;
}

// 現在の残高及び引去額を取得する関数(first：残高、second：引去額)
pair<int, int> CardServer::GetMoneyState()
{
	return payment;
}

CardServer::CardServer()
{
	payment = { 1000, 0 };
}

CardServer::~CardServer()
{
}