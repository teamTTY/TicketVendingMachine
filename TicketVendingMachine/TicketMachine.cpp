#include "Dxlib.h"
#include "TicketMachine.h"
#include "CardServer.h"
#include "Cash.h"

const int price_cash = 130; // �����̏ꍇ�̉��i
const int price_card = 124; // �d�q�}�l�[�̏ꍇ�̉��i

const int color_white = 0xffffff; // ���F�̃R�[�h
const int color_red   = 0xff2222; // �ԐF�̃R�[�h

const int comment_offsetY = 450;	// �ē������̕`��ʒu�I�t�Z�b�g
const int draw_offsetX    = 200;	// �����`��̃I�t�Z�b�g
const int draw_offsetY    = 70;		// �����`��̃I�t�Z�b�g

// �����@
using namespace std;

unique_ptr<TicketMachine, TicketMachine::TicketMachineDeleter> TicketMachine::s_Instance(new TicketMachine());

// �x�������@���擾����
PaymentType TicketMachine::GetPayment()
{
	return this->paymentType;
}

// �d�q�}�l�[������
bool TicketMachine::PutCard()
{
	// IC�J�[�h�⌻�������ɓ����Ă����珈�����Ȃ�
	if (!(paymentType == PaymentType::MAX))
	{
		return false;
	}

	// �d�q�}�l�[�ɐݒ肷��
	paymentType = PaymentType::CARD;
	cardMoney = lpCardServer.GetMoneyState();

	return true;
}

// ����������
bool TicketMachine::PutMoney(int inputMoney)
{
	// IC�J�[�h�����ɓ����Ă����珈�����Ȃ�
	if (!(paymentType == PaymentType::MAX || paymentType == PaymentType::CASH))
	{
		return false;
	}

	if (!cashMoney.first.try_emplace(inputMoney, 1).second)
	{
		// ���킪���݂����ꍇ�A�J�E���g�𑝂₷
		cashMoney.first[inputMoney]++;
	}

	// �����ɐݒ肷��
	paymentType = PaymentType::CASH;

	return true;
}

// �x��������
bool TicketMachine::Payment()
{
	return (paySuccess = (funcPay[paymentType]()));
}

bool TicketMachine::IsSuccess()
{
	return paySuccess;
}

// ���ނ��Ԃ�
MapInt TicketMachine::GetChange()
{
	return cashMoney.second;
}

// �I������
void TicketMachine::PaymentEnd()
{
	Clear();
}

// �`�揈��
void TicketMachine::Draw()
{
	funcDraw[paymentType]();
}

// �ϐ�������������
void TicketMachine::Clear()
{
	paymentType = PaymentType::MAX;
	paySuccess = false;

	// �d�q�}�l�[�̏�����
	cardMoney = { 0, 0 };

	// �����̏�����
	cashMoney.first.clear();
	cashMoney.second.clear();
}

// �eMoneyType�̎x����������ݒ肷��
void TicketMachine::SetFuncPay()
{
	// ���������ĂȂ��ꍇ�͉������Ȃ�
	funcPay.try_emplace(PaymentType::MAX, [this]() {
		return false;
	});

	// �d�q�}�l�[
	funcPay.try_emplace(PaymentType::CARD, [this]() {
		if (lpCardServer.Payment(price_card))
		{
			// �x��������
			// �c�����T�[�o�[����擾����
			cardMoney = lpCardServer.GetMoneyState();
			return true;
		}
		else
		{
			// �x�������s
			return false;
		}
	});

	// ����
	funcPay.try_emplace(PaymentType::CASH, [this]() {
		if (lpCash.Payment(cashMoney.first, price_cash))
		{
			// �x��������
			cashMoney.second = lpCash.GetChange();
			return true;
		}
		else
		{
			// �x�������s
			return false;
		}
		});
}

// �eMoneyType�̕`�揈����ݒ肷��
void TicketMachine::SetFuncDraw()
{
	// �x�����O�̕\��
	funcDraw.try_emplace(PaymentType::MAX, []() {
		DrawString(0, comment_offsetY + GetFontSize() / 2, "���̘g���̌�����IC�J�[�h��I�����N���b�N���ē������Ă��������B\n���������������猈�σ{�^���������Ă��������B", color_white);
	}
	);

	// �d�q�}�l�[�̏ꍇ�̕\��
	funcDraw.try_emplace(PaymentType::CARD, [this]() {
		if (paySuccess)
		{
			DrawString(0, comment_offsetY + GetFontSize() / 2, "���ϊ���\nIC�J�[�h���o���ۂ͎󂯎��{�^���������Ă��������B", color_white);

			DrawString(draw_offsetX, draw_offsetY, "�d�q�}�l�[", color_white);
			DrawFormatString(draw_offsetX + GetFontSize(), draw_offsetY + GetFontSize(), color_white, "  �c���@%d�~", cardMoney.first);
			DrawFormatString(draw_offsetX + GetFontSize(), draw_offsetY + GetFontSize() * 2, color_white, "�����z�@%d�~", cardMoney.second);
		}
		else
		{
			DrawString(0, comment_offsetY + GetFontSize() / 2, "���̘g����IC�J�[�h��I�����N���b�N���ē������Ă��������B\n���������������猈�σ{�^���������Ă��������B", color_white);

			DrawString(draw_offsetX, draw_offsetY, "�d�q�}�l�[", color_white);
			DrawFormatString(draw_offsetX + GetFontSize(), draw_offsetY + GetFontSize(), color_white, "�@�c���@%d�~", cardMoney.first);

			if (cardMoney.first < price_card)
			{
				DrawString(draw_offsetX, draw_offsetY + GetFontSize() * 3, "�c��������܂���", color_red, true);
			}
		}
	});

	// �����̏ꍇ�̕\��
	funcDraw.try_emplace(PaymentType::CASH, [this]() {
		if (paySuccess)
		{
			DrawString(0, comment_offsetY + GetFontSize() / 2, "���ϊ���\n���ނ���󂯎��ۂ͎󂯎��{�^���������Ă��������B", color_white);

			DrawString(draw_offsetX, draw_offsetY, "�������z", color_white);
			DrawString(draw_offsetX, draw_offsetY, "�@�@�@�@�@�@����", color_white);
			int moneyLine = 0;
			int totalMoney = 0;
			for (auto moneyData : cashMoney.first)
			{
				DrawFormatString(draw_offsetX + GetFontSize(), (draw_offsetY + GetFontSize()) + moneyLine * GetFontSize(), color_white, "%d�~", moneyData.first);
				DrawFormatString(draw_offsetX + GetFontSize(), (draw_offsetY + GetFontSize()) + moneyLine * GetFontSize(), color_white, "    �@�@�@%d��", moneyData.second);
				moneyLine++;
				totalMoney += (moneyData.first * moneyData.second);
			}

			DrawFormatString(draw_offsetX, (draw_offsetY + GetFontSize() * 2) + moneyLine * GetFontSize(), color_white, "���v�����z�@%d�~", totalMoney);

			DrawString(draw_offsetX * 2, draw_offsetY, "�ޑK�@", color_white);
			DrawString(draw_offsetX * 2, draw_offsetY, "�@�@�@�@�@ ����", color_white);
			int changeLine = 0;
			for (auto changeData : cashMoney.second)
			{
				DrawFormatString(draw_offsetX * 2, (draw_offsetY + GetFontSize()) + changeLine * GetFontSize(), color_white, "%d�~", changeData.first);
				DrawFormatString(draw_offsetX * 2, (draw_offsetY + GetFontSize()) + changeLine * GetFontSize(), color_white, "    �@�@�@ %d��", changeData.second);
				changeLine++;
			}
		}
		else
		{
			DrawString(0, comment_offsetY + GetFontSize() / 2, "���̘g���̌�����I�����N���b�N���ē������Ă��������B\n���������������猈�σ{�^���������Ă��������B", color_white);

			DrawString(draw_offsetX, draw_offsetY, "�������z", color_white);
			DrawString(draw_offsetX, draw_offsetY, "�@�@�@�@�@�@����", color_white);
			int moneyLine = 0;
			int totalMoney = 0;
			for (auto moneyData : cashMoney.first)
			{
				DrawFormatString(draw_offsetX + GetFontSize(), (draw_offsetY + GetFontSize()) + moneyLine * GetFontSize(), color_white, "%d�~", moneyData.first);
				DrawFormatString(draw_offsetX + GetFontSize(), (draw_offsetY + GetFontSize()) + moneyLine * GetFontSize(), color_white, "    �@�@�@%d��", moneyData.second);
				moneyLine++;
				totalMoney += (moneyData.first * moneyData.second);
			}

			DrawFormatString(draw_offsetX, (draw_offsetY + GetFontSize() * 2) + moneyLine * GetFontSize(), color_white, "���v�����z�@%d�~", totalMoney);

			if (totalMoney < price_cash)
			{
				DrawString(draw_offsetX, (draw_offsetY + GetFontSize() * 3) + moneyLine * GetFontSize(), "���z������܂���", color_red, true);
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
