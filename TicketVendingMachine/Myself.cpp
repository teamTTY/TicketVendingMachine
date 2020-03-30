#include <Dxlib.h>
#include "VECTOR2.h"
#include "Myself.h"

const int screen_sizeX = 800; // ��ʂ̉��̉𑜓x
const int screen_sizeY = 600; // ��ʂ̏c�̉𑜓x

const int color_white = 0xffffff; // ���F�̃R�[�h
const int color_black = 0x000000; // ���F�̃R�[�h

const int money_sizeX = 100;	// �����̉��̃T�C�Y
const int money_sizeY = 50;		// �����̏c�̃T�C�Y

const int pay_btn_sizeX = 200;	// �x�����{�^���̉��̃T�C�Y
const int pay_btn_sizeY = 50;	// �x�����{�^���̏c�̃T�C�Y

const int font_size = 18;		// �t�H���g�̃T�C�Y

const int money_type_num = 8;	// �x���������̎�ނ̐�

// ������
using namespace std;

unique_ptr<Myself, Myself::MyselfDeleter> Myself::s_Instance(new Myself());

void Myself::Run()
{
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		mouse->Update();

		// ��������
		auto PutMoney = [this](int money) {
			if (havingCash[money] != 0)
			{
				if (lpTicketMachine.PutMoney(money))
				{
					havingCash[money]--;
				}
			}
		};

		// �N���b�N����
		if (mouse->GetOnClick())
		{
			VECTOR2 pos = mouse->GetClickPos();

			// �x��������
			if ((pos.x <= money_sizeX) && (pos.y <= (money_sizeY * (money_type_num))) && (!lpTicketMachine.IsSuccess()))
			{
				if (pos.y <= (money_sizeY * (money_type_num - 1)))
				{
					// �����x����
					PutMoney(moneyType[(pos.y / money_sizeY)]);
				}
				else
				{
					// �d�q�}�l�[�x����
					lpTicketMachine.PutCard();
				}
			}

			// �{�^������
			if ((pos.x >= screen_sizeX / 2) && (pos.y >= money_sizeY * (money_type_num - 1))
				&& (pos.x <= screen_sizeX / 2 + (money_sizeX / 2 + pay_btn_sizeX)) && (pos.y <= money_sizeY * (money_type_num - 1) + pay_btn_sizeY))
			{
				if (lpTicketMachine.IsSuccess())
				{
					// ���ނ���������ɑ���
					if (lpTicketMachine.GetPayment() == PaymentType::CASH)
					{
						for (auto change : lpTicketMachine.GetChange())
						{
							havingCash[change.first] += change.second;
						}
					}

					// ���ނ�y�уJ�[�h�̎��o��
					lpTicketMachine.PaymentEnd();
				}
				else
				{
					// ���Ϗ���
					lpTicketMachine.Payment();
				}
			}
		}

		// �o�͏���
		ClsDrawScreen();
		Draw();
		DrawUI();
		lpTicketMachine.Draw();
		ScreenFlip();
	}
}

// �`�揈��
void Myself::Draw()
{
	// �������\��
	int moneyLine = 1;
	for (auto moneyData : havingCash)
	{
		if (moneyData.second >= 10)
		{
			DrawFormatString(money_sizeX + 25, moneyLine * money_sizeY - (money_sizeY / 2) - (font_size / 2), color_white, "%d ��", moneyData.second);
		}
		else
		{
			DrawFormatString(money_sizeX + 25, moneyLine * money_sizeY - (money_sizeY / 2) - (font_size / 2), color_white, " %d ��", moneyData.second);
		}
		moneyLine++;
	}

	// �ؕ��̒l�i�\��
	DrawString(screen_sizeX / 2 - font_size, money_sizeY / 2, "�ؕ��̉��i�@�����F130�~�@�d�q�}�l�[�F124�~", color_white);
}

// UI�`�揈��
void Myself::DrawUI()
{
	// �x����������UI�\��
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

	// �d�؂��
	DrawLine(0, money_sizeY * (money_type_num + 1), screen_sizeX, money_sizeY * (money_type_num + 1), color_white, true);
	DrawLine(money_sizeX * 2, 0, money_sizeX * 2, money_sizeY * (money_type_num + 1), color_white, true);

	// �{�^���̕`��
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
		// �󂯎��{�^��
		DrawBtn(font_size, "�󂯎��");
	}
	else
	{
		// ���σ{�^��
		DrawBtn(0, "�� ��");
	}
}

// �V�X�e���������̐����A���s��Ԃ�
bool Myself::GetSysSuccess()
{
	return sysSuccess;
}

bool Myself::SysInit()
{
	// �V�X�e������
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

	// UI�z�u����Ă邨����ǉ�
	moneyType.emplace_back(10);
	moneyType.emplace_back(50);
	moneyType.emplace_back(100);
	moneyType.emplace_back(500);
	moneyType.emplace_back(1000);
	moneyType.emplace_back(5000);
	moneyType.emplace_back(10000);

	// �ێ����錻����ǉ�
	havingCash.try_emplace(   10, 15);
	havingCash.try_emplace(   50,  3);
	havingCash.try_emplace(  100,  2);
	havingCash.try_emplace(  500,  1);
	havingCash.try_emplace( 1000,  1);
	havingCash.try_emplace( 5000,  1);
	havingCash.try_emplace(10000,  1);

	// �摜�ǂݍ���
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
