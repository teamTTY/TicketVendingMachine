#include <DxLib.h>
#include "CardServer.h"

// �d�q�}�l�[�T�[�o�[
using namespace std;

std::pair<int, int> CardServer::payment;

unique_ptr<CardServer, CardServer::CardServerDeleter> CardServer::s_Instance(new CardServer());

// ���Ϗ���
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

// ���݂̎c���y�ш����z���擾����֐�(first�F�c���Asecond�F�����z)
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