#pragma once
#include <utility>
#include <memory>

#define lpCardServer CardServer::GetInstance()

// 電子マネーサーバー
class CardServer
{
public:
	static CardServer& GetInstance()
	{
		return *s_Instance;
	}

	bool Payment(int price);				// 決済処理
	std::pair<int, int> GetMoneyState();	// 現在の残高及び引去額を取得する関数(first：残高、second：引去額)

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

	static std::pair<int, int> payment;		// first:残高、second:引去額
};

