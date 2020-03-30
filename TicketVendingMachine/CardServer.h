#pragma once
#include <utility>
#include <memory>

#define lpCardServer CardServer::GetInstance()

// �d�q�}�l�[�T�[�o�[
class CardServer
{
public:
	static CardServer& GetInstance()
	{
		return *s_Instance;
	}

	bool Payment(int price);				// ���Ϗ���
	std::pair<int, int> GetMoneyState();	// ���݂̎c���y�ш����z���擾����֐�(first�F�c���Asecond�F�����z)

private:
	struct CardServerDeleter
	{
		void operator()(CardServer* cardServer) const
		{
			delete cardServer;
		}
	};

	CardServer();
	~CardServer();

	static std::unique_ptr<CardServer, CardServerDeleter> s_Instance;

	static std::pair<int, int> payment;		// first:�c���Asecond:�����z
};

