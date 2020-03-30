#include "Dxlib.h"
#include "TicketMachine.h"
#include "CardServer.h"
#include "Cash.h"

const int price_cash = 130; // 現金の場合の価格
const int price_card = 124; // 電子マネーの場合の価格

const int color_white = 0xffffff; // 白色のコード
const int color_red   = 0xff2222; // 赤色のコード

const int comment_offsetY = 450;	// 案内文字の描画位置オフセット
const int draw_offsetX    = 200;	// 文字描画のオフセット
const int draw_offsetY    = 70;		// 文字描画のオフセット

// 券売機
using namespace std;

unique_ptr<TicketMachine, TicketMachine::TicketMachineDeleter> TicketMachine::s_Instance(new TicketMachine());

// 支払い方法を取得する
PaymentType TicketMachine::GetPayment()
{
	return this->paymentType;
}

// 電子マネーを入れる
bool TicketMachine::PutCard()
{
	// ICカードや現金が既に入っていたら処理しない
	if (!(paymentType == PaymentType::MAX))
	{
		return false;
	}

	// 電子マネーに設定する
	paymentType = PaymentType::CARD;
	cardMoney = lpCardServer.GetMoneyState();

	return true;
}

// 現金を入れる
bool TicketMachine::PutMoney(int inputMoney)
{
	// ICカードが既に入っていたら処理しない
	if (!(paymentType == PaymentType::MAX || paymentType == PaymentType::CASH))
	{
		return false;
	}

	if (!cashMoney.first.try_emplace(inputMoney, 1).second)
	{
		// 金種が存在した場合、カウントを増やす
		cashMoney.first[inputMoney]++;
	}

	// 現金に設定する
	paymentType = PaymentType::CASH;

	return true;
}

// 支払い処理
bool TicketMachine::Payment()
{
	return (paySuccess = (funcPay[paymentType]()));
}

bool TicketMachine::IsSuccess()
{
	return paySuccess;
}

// お釣りを返す
MapInt TicketMachine::GetChange()
{
	return cashMoney.second;
}

// 終了処理
void TicketMachine::PaymentEnd()
{
	Clear();
}

// 描画処理
void TicketMachine::Draw()
{
	funcDraw[paymentType]();
}

// 変数を初期化する
void TicketMachine::Clear()
{
	paymentType = PaymentType::MAX;
	paySuccess = false;

	// 電子マネーの初期化
	cardMoney = { 0, 0 };

	// 現金の初期化
	cashMoney.first.clear();
	cashMoney.second.clear();
}

// 各MoneyTypeの支払い処理を設定する
void TicketMachine::SetFuncPay()
{
	// 何も入ってない場合は何もしない
	funcPay.try_emplace(PaymentType::MAX, [this]() {
		return false;
	});

	// 電子マネー
	funcPay.try_emplace(PaymentType::CARD, [this]() {
		if (lpCardServer.Payment(price_card))
		{
			// 支払い成功
			// 残高をサーバーから取得する
			cardMoney = lpCardServer.GetMoneyState();
			return true;
		}
		else
		{
			// 支払い失敗
			return false;
		}
	});

	// 現金
	funcPay.try_emplace(PaymentType::CASH, [this]() {
		if (lpCash.Payment(cashMoney.first, price_cash))
		{
			// 支払い成功
			cashMoney.second = lpCash.GetChange();
			return true;
		}
		else
		{
			// 支払い失敗
			return false;
		}
		});
}

// 各MoneyTypeの描画処理を設定する
void TicketMachine::SetFuncDraw()
{
	// 支払い前の表示
	funcDraw.try_emplace(PaymentType::MAX, []() {
		DrawString(0, comment_offsetY + GetFontSize() / 2, "左の枠内の現金かICカードを選択しクリックして入金してください。\n入金が完了したら決済ボタンを押してください。", color_white);
	}
	);

	// 電子マネーの場合の表示
	funcDraw.try_emplace(PaymentType::CARD, [this]() {
		if (paySuccess)
		{
			DrawString(0, comment_offsetY + GetFontSize() / 2, "決済完了\nICカードを出す際は受け取りボタンを押してください。", color_white);

			DrawString(draw_offsetX, draw_offsetY, "電子マネー", color_white);
			DrawFormatString(draw_offsetX + GetFontSize(), draw_offsetY + GetFontSize(), color_white, "  残高　%d円", cardMoney.first);
			DrawFormatString(draw_offsetX + GetFontSize(), draw_offsetY + GetFontSize() * 2, color_white, "引去額　%d円", cardMoney.second);
		}
		else
		{
			DrawString(0, comment_offsetY + GetFontSize() / 2, "左の枠内のICカードを選択しクリックして入金してください。\n入金が完了したら決済ボタンを押してください。", color_white);

			DrawString(draw_offsetX, draw_offsetY, "電子マネー", color_white);
			DrawFormatString(draw_offsetX + GetFontSize(), draw_offsetY + GetFontSize(), color_white, "　残高　%d円", cardMoney.first);

			if (cardMoney.first < price_card)
			{
				DrawString(draw_offsetX, draw_offsetY + GetFontSize() * 3, "残高が足りません", color_red, true);
			}
		}
	});

	// 現金の場合の表示
	funcDraw.try_emplace(PaymentType::CASH, [this]() {
		if (paySuccess)
		{
			DrawString(0, comment_offsetY + GetFontSize() / 2, "決済完了\nお釣りを受け取る際は受け取りボタンを押してください。", color_white);

			DrawString(draw_offsetX, draw_offsetY, "投入金額", color_white);
			DrawString(draw_offsetX, draw_offsetY, "　　　　　　枚数", color_white);
			int moneyLine = 0;
			int totalMoney = 0;
			for (auto moneyData : cashMoney.first)
			{
				DrawFormatString(draw_offsetX + GetFontSize(), (draw_offsetY + GetFontSize()) + moneyLine * GetFontSize(), color_white, "%d円", moneyData.first);
				DrawFormatString(draw_offsetX + GetFontSize(), (draw_offsetY + GetFontSize()) + moneyLine * GetFontSize(), color_white, "    　　　%d枚", moneyData.second);
				moneyLine++;
				totalMoney += (moneyData.first * moneyData.second);
			}

			DrawFormatString(draw_offsetX, (draw_offsetY + GetFontSize() * 2) + moneyLine * GetFontSize(), color_white, "合計投入額　%d円", totalMoney);

			DrawString(draw_offsetX * 2, draw_offsetY, "釣銭　", color_white);
			DrawString(draw_offsetX * 2, draw_offsetY, "　　　　　 枚数", color_white);
			int changeLine = 0;
			for (auto changeData : cashMoney.second)
			{
				DrawFormatString(draw_offsetX * 2, (draw_offsetY + GetFontSize()) + changeLine * GetFontSize(), color_white, "%d円", changeData.first);
				DrawFormatString(draw_offsetX * 2, (draw_offsetY + GetFontSize()) + changeLine * GetFontSize(), color_white, "    　　　 %d枚", changeData.second);
				changeLine++;
			}
		}
		else
		{
			DrawString(0, comment_offsetY + GetFontSize() / 2, "左の枠内の現金を選択しクリックして入金してください。\n入金が完了したら決済ボタンを押してください。", color_white);

			DrawString(draw_offsetX, draw_offsetY, "投入金額", color_white);
			DrawString(draw_offsetX, draw_offsetY, "　　　　　　枚数", color_white);
			int moneyLine = 0;
			int totalMoney = 0;
			for (auto moneyData : cashMoney.first)
			{
				DrawFormatString(draw_offsetX + GetFontSize(), (draw_offsetY + GetFontSize()) + moneyLine * GetFontSize(), color_white, "%d円", moneyData.first);
				DrawFormatString(draw_offsetX + GetFontSize(), (draw_offsetY + GetFontSize()) + moneyLine * GetFontSize(), color_white, "    　　　%d枚", moneyData.second);
				moneyLine++;
				totalMoney += (moneyData.first * moneyData.second);
			}

			DrawFormatString(draw_offsetX, (draw_offsetY + GetFontSize() * 2) + moneyLine * GetFontSize(), color_white, "合計投入額　%d円", totalMoney);

			if (totalMoney < price_cash)
			{
				DrawString(draw_offsetX, (draw_offsetY + GetFontSize() * 3) + moneyLine * GetFontSize(), "金額が足りません", color_red, true);
			}
		}
	});
}

TicketMachine::TicketMachine()
{
	SetFuncPay();
	SetFuncDraw();
	Clear();
}

TicketMachine::~TicketMachine()
{
}
