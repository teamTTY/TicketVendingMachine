#pragma once
#include <memory>
#include <utility>
#include <functional>
#include <map>
#include <string>

#define lpTicketMachine TicketMachine::GetInstance()

// 決済の種別が増えた時にはここに追加する
enum class PaymentType {
	CASH,	// 現金
	CARD,	// カード
	MAX
};

using FuncPayment = std::function<bool()>;
using FuncDraw	  = std::function<void()>;
using MapInt = std::map<int, int>;

// 券売機
class TicketMachine
{
public:
	static TicketMachine& GetInstance()
	{
		return *s_Instance;
	}

	PaymentType GetPayment();		// 支払い方法を取得する

	bool PutCard();					// 電子マネーを入れる
	bool PutMoney(int inputMoney);	// 現金を入れる
	bool Payment();					// 支払い処理
	bool IsSuccess();				// 支払い成功、失敗を返す
	MapInt GetChange();				// お釣りを返す
	void PaymentEnd();				// 終了処理

	void Draw();					// 描画処理

private:
	void Clear();		// 変数を初期化する

	void SetFuncPay();	// 各MoneyTypeの支払い処理を設定する
	void SetFuncDraw();	// 各MoneyTypeの描画処理を設定する

	struct TicketMachineDeleter
	{
		void operator()(TicketMachine* ticketMachine) const
		{
			delete ticketMachine;
		}
	};

	TicketMachine();
	~TicketMachine();

	static std::unique_ptr<TicketMachine, TicketMachineDeleter> s_Instance;

	std::map<PaymentType, FuncPayment> funcPay;		// 各MoneyTypeの支払い処理を保持する
	std::map<PaymentType, FuncDraw>    funcDraw;	// 各MoneyTypeの描画処理を保持する

	bool paySuccess;				// 支払いが成功かどうかを管理するフラグ

	PaymentType paymentType;		// 支払い方法

	std::pair<int, int> cardMoney;		// 電子マネー残高	(first：残高　　　second：引去額)
	std::pair<MapInt, MapInt> cashMoney;// 現金				(first：投入金種　second：お釣り)

	int* coinImage;
};

