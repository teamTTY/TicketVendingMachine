#pragma once
#include <memory>
#include <utility>
#include <functional>
#include <map>
#include <string>

#define lpTicketMachine TicketMachine::GetInstance()

// ���ς̎�ʂ����������ɂ͂����ɒǉ�����
enum class PaymentType {
	CASH,	// ����
	CARD,	// �J�[�h
	MAX
};

using FuncPayment = std::function<bool()>;
using FuncDraw	  = std::function<void()>;
using MapInt = std::map<int, int>;

// �����@
class TicketMachine
{
public:
	static TicketMachine& GetInstance()
	{
		return *s_Instance;
	}

	PaymentType GetPayment();		// �x�������@���擾����

	bool PutCard();					// �d�q�}�l�[������
	bool PutMoney(int inputMoney);	// ����������
	bool Payment();					// �x��������
	bool IsSuccess();				// �x���������A���s��Ԃ�
	MapInt GetChange();				// ���ނ��Ԃ�
	void PaymentEnd();				// �I������

	void Draw();					// �`�揈��

private:
	void Clear();		// �ϐ�������������

	void SetFuncPay();	// �eMoneyType�̎x����������ݒ肷��
	void SetFuncDraw();	// �eMoneyType�̕`�揈����ݒ肷��

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

	std::map<PaymentType, FuncPayment> funcPay;		// �eMoneyType�̎x����������ێ�����
	std::map<PaymentType, FuncDraw>    funcDraw;	// �eMoneyType�̕`�揈����ێ�����

	bool paySuccess;				// �x�������������ǂ������Ǘ�����t���O

	PaymentType paymentType;		// �x�������@

	std::pair<int, int> cardMoney;		// �d�q�}�l�[�c��	(first�F�c���@�@�@second�F�����z)
	std::pair<MapInt, MapInt> cashMoney;// ����				(first�F��������@second�F���ނ�)

	int* coinImage;
};

