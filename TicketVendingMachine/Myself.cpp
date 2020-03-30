#include <Dxlib.h>
#include "VECTOR2.h"
#include "Myself.h"

const int screen_sizeX = 800; // 画面の横の解像度
const int screen_sizeY = 600; // 画面の縦の解像度

const int color_white = 0xffffff; // 白色のコード
const int color_black = 0x000000; // 黒色のコード

const int money_sizeX = 100;	// お金の横のサイズ
const int money_sizeY = 50;		// お金の縦のサイズ

const int pay_btn_sizeX = 200;	// 支払いボタンの横のサイズ
const int pay_btn_sizeY = 50;	// 支払いボタンの縦のサイズ

const int font_size = 18;		// フォントのサイズ

const int money_type_num = 8;	// 支払うお金の種類の数

// 買い手
using namespace std;

unique_ptr<Myself, Myself::MyselfDeleter> Myself::s_Instance(new Myself());

void Myself::Run()
{
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		mouse->Update();

		// 入金処理
		auto PutMoney = [this](int money) {
			if (havingCash[money] != 0)
			{
				if (lpTicketMachine.PutMoney(money))
				{
					havingCash[money]--;
				}
			}
		};

		// クリックした
		if (mouse->GetOnClick())
		{
			VECTOR2 pos = mouse->GetClickPos();

			// 支払い処理
			if ((pos.x <= money_sizeX) && (pos.y <= (money_sizeY * (money_type_num))) && (!lpTicketMachine.IsSuccess()))
			{
				if (pos.y <= (money_sizeY * (money_type_num - 1)))
				{
					// 現金支払い
					PutMoney(moneyType[(pos.y / money_sizeY)]);
				}
				else
				{
					// 電子マネー支払い
					lpTicketMachine.PutCard();
				}
			}

			// ボタン処理
			if ((pos.x >= screen_sizeX / 2) && (pos.y >= money_sizeY * (money_type_num - 1))
				&& (pos.x <= screen_sizeX / 2 + (money_sizeX / 2 + pay_btn_sizeX)) && (pos.y <= money_sizeY * (money_type_num - 1) + pay_btn_sizeY))
			{
				if (lpTicketMachine.IsSuccess())
				{
					// お釣りを所持金に足す
					if (lpTicketMachine.GetPayment() == PaymentType::CASH)
					{
						for (auto change : lpTicketMachine.GetChange())
						{
							havingCash[change.first] += change.second;
						}
					}

					// お釣り及びカードの取り出し
					lpTicketMachine.PaymentEnd();
				}
				else
				{
					// 決済処理
					lpTicketMachine.Payment();
				}
			}
		}

		// 出力処理
		ClsDrawScreen();
		Draw();
		DrawUI();
		lpTicketMachine.Draw();
		ScreenFlip();
	}
}

// 描画処理
void Myself::Draw()
{
	// 所持金表示
	int moneyLine = 1;
	for (auto moneyData : havingCash)
	{
		if (moneyData.second >= 10)
		{
			DrawFormatString(money_sizeX + 25, moneyLine * money_sizeY - (money_sizeY / 2) - (font_size / 2), color_white, "%d 枚", moneyData.second);
		}
		else
		{
			DrawFormatString(money_sizeX + 25, moneyLine * money_sizeY - (money_sizeY / 2) - (font_size / 2), color_white, " %d 枚", moneyData.second);
		}
		moneyLine++;
	}

	// 切符の値段表示
	DrawString(screen_sizeX / 2 - font_size, money_sizeY / 2, "切符の価格　現金：130円　電子マネー：124円", color_white);
}

// UI描画処理
void Myself::DrawUI()
{
	// 支払うお金のUI表示
	auto payType = lpTicketMachine.GetPayment();
	if (payType == PaymentType::CASH)
	{
		DrawGraph(0, 0, images["act_cash"], true);
	}
	else if (payType == PaymentType::CARD)
	{
		DrawGraph(0, 0, images["act_card"], true);
	}
	else
	{
		DrawGraph(0, 0, images["money"], true);
	}

	// 仕切り線
	DrawLine(0, money_sizeY * (money_type_num + 1), screen_sizeX, money_sizeY * (money_type_num + 1), color_white, true);
	DrawLine(money_sizeX * 2, 0, money_sizeX * 2, money_sizeY * (money_type_num + 1), color_white, true);

	// ボタンの描画
	auto DrawBtn = [this](int offsetX, const char* name) {
		SetFontSize(font_size * 2);

		VECTOR2 pos = mouse->GetClickPos();

		if ((mouse->GetClicking()) && (pos.x >= screen_sizeX / 2) && (pos.y >= money_sizeY * (money_type_num - 1))
			&& (pos.x <= screen_sizeX / 2 + (money_sizeX / 2 + pay_btn_sizeX)) && (pos.y <= money_sizeY * (money_type_num - 1) + pay_btn_sizeY))
		{
			DrawGraph((screen_sizeX - money_sizeX * 2) - pay_btn_sizeX, money_sizeY * (money_type_num - 1), images["btn_push"], true);
			DrawString((screen_sizeX - money_sizeX * 2) - pay_btn_sizeX + 50 - offsetX + 1, money_sizeY * (money_type_num - 1) + font_size / 2 + 1, name, color_black);
		}
		else
		{
			DrawGraph((screen_sizeX - money_sizeX * 2) - pay_btn_sizeX, money_sizeY * (money_type_num - 1), images["btn"], true);
			DrawString((screen_sizeX - money_sizeX * 2) - pay_btn_sizeX + 50 - offsetX, money_sizeY * (money_type_num - 1) + font_size / 2, name, color_black);
		}

		SetFontSize(font_size);
	};

	if (lpTicketMachine.IsSuccess())
	{
		// 受け取りボタン
		DrawBtn(font_size, "受け取り");
	}
	else
	{
		// 決済ボタン
		DrawBtn(0, "決 済");
	}
}

// システム初期化の成功、失敗を返す
bool Myself::GetSysSuccess()
{
	return sysSuccess;
}

bool Myself::SysInit()
{
	// システム処理
	SetGraphMode(screen_sizeX, screen_sizeY, 16);
	ChangeWindowMode(true);
	SetWindowText("TicketVendingMachine");

	if (DxLib_Init() == -1)
	{
		sysSuccess = false;
		return false;
	}

	mouse = make_unique<MouseCtl>();

	sysSuccess = true;

	// UI配置されてるお金を追加
	moneyType.emplace_back(10);
	moneyType.emplace_back(50);
	moneyType.emplace_back(100);
	moneyType.emplace_back(500);
	moneyType.emplace_back(1000);
	moneyType.emplace_back(5000);
	moneyType.emplace_back(10000);

	// 保持する現金を追加
	havingCash.try_emplace(   10, 15);
	havingCash.try_emplace(   50,  3);
	havingCash.try_emplace(  100,  2);
	havingCash.try_emplace(  500,  1);
	havingCash.try_emplace( 1000,  1);
	havingCash.try_emplace( 5000,  1);
	havingCash.try_emplace(10000,  1);

	// 画像読み込み
	images.try_emplace("money", LoadGraph("image/money.png"));
	images.try_emplace("act_cash", LoadGraph("image/active_cash.png"));
	images.try_emplace("act_card", LoadGraph("image/active_card.png"));
	images.try_emplace("btn", LoadGraph("image/btn.png"));
	images.try_emplace("btn_push", LoadGraph("image/btn_push.png"));

	return true;
}

void Myself::SysEnd()
{
	DxLib_End();
}

Myself::Myself()
{
	SysInit();

	SetFontSize(font_size);
}

Myself::~Myself()
{
	SysEnd();
}
