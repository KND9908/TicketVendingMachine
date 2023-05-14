#pragma once
#include "Money.h"
#include "Server.h"
#include <memory>

//�����@�̃f�[�^����ѓ�����Ǘ�����N���X
class TicketVendingMachine
{
	//�����@�ɓ������������enum�@�����Ɠd�q�}�l�[
	enum MoneyType
	{
		MoneyType_None,//����
		MoneyType_Cash,//����
		MoneyType_EMoney,//�d�q�}�l�[
	};

	//���݂̌����@�̏��
	enum State
	{
		State_Standby,//�ҋ@��
		State_Selecting,//����I��
		State_Paying,//�x������
		State_Change,//���ނ蒆
		State_Finish//�������
	};

	//�����̕ԋp���o�[�̉����t���O
	bool _isReturnLever = false;

	bool moneyempty = true;

	bool iscsvread = false;

	//�T�[�o�[�̃C���X�^���X
	std::unique_ptr<Server> m_server;

	//���ݓ�������Ă������@�����l�͖���
	MoneyType m_moneyType = MoneyType_None;

	//���݂̌����@�̏�ԁ@�����l�͑ҋ@��
	State m_state = State_Standby;

	//�����@�ɓ������ꂽ�����̍��v���z�@�����l��0�~
	int m_totalMoney = 0;

	//�����@�ɓ������ꂽ�����@csv�̌`���ɍ��킹�邽��[0]~[7]�܂ł͌����̐��A[8]�͓d�q�}�l�[�̋��z���i�[����
	int m_money[8] = { 0 };

	//�����@����ԋp���邨�ނ�@csv�̌`���ɍ��킹�邽��[0]~[7]�܂ł͌����̐��A[8]�͓d�q�}�l�[�̋��z���i�[����
	int m_changeMoney[8] = { 0 };

	//�����ōw������ꍇ�̐ؕ��̉��i�@
	int m_ticketPrice = 130;

	//�ؕ��̍w������
	int m_ticketNum = 0;

	//���ނ�
	int m_change = 0;

	//���݂̌����@�̏�Ԃɍ��킹�ď������Ǘ�����֐�
	void Command();

	//�����@�̊e��ʂōs������
	void Selecting();//�ؕ��I�����
	void Paying();//�x�����
	void Change();//��v���ʉ��

	//�����@�ɓ������ꂽ�����̍��v���v�Z���鏈��
	void CalcTotalMoney();

	//�R���\�[����ʂ̌��h���̂��߂Ƀ��C���������֐�
	void DisplayLine();

	//�����@�ɓ�������Ă�����z���i�[���Ă���csv�t�@�C���Ƃ̃f�[�^�̓ǂݏ������s������
	void ReadCsvFile();
	void WriteCsvFile();

public:
	//�R���X�g���N�^
	TicketVendingMachine();

	//�����@�����̋��K�̏�Ԃ��Ď�����֐�
	void Monitor();

	//�����@�̑ҋ@��Ԓ��ɍs���֐�
	void Standby();
};