#pragma once
//�d�q�}�l�[�̏������s���T�[�o�Ƃ̒ʐM���s���N���X
//�T�[�o���̎d�l��f�[�^�Ǘ��̃��C�u�������̒�`�����݂��Ȃ����߁A
//�{�ۑ�ł͉��z�I�ɃT�[�o�Ƃ̒ʐM���s���N���X�Ƃ��Ē�`���܂��B
class Server
{
	//�T�[�o�Ƃ̏����ʐM�̌��ʂ�����enum
	enum Result
	{
		Result_Success,//����
		Result_Failed,//���s
	};

	//�T�[�o����̒ʐM���ʂ�������֐�
	//�{�ۑ�ł͂��̒l��ύX���邱�Ƃňُ�n�̓���m�F���s���܂��B
	Result m_result = Result_Success;

	//�����z
	int withdraw = 0;

	//�d�q�}�l�[�̎c��
	int m_eMoney = 0;

	//�T�[�o�Ƃ̒ʐM���s���֐�
	//�����F���[�U�̏������Ă���d�q�}�l�[�̎c���A����юg�p������z
	//�߂�l�F�����̏I���R�[�h
	bool Connect(int* eMoney, int price);

public:
	//�d�q�}�l�[��Pay����
	//�����F���[�U�̏������Ă���d�q�}�l�[�̎c���A����юg�p������z
	//�߂�l�F����������̓d�q�}�l�[�̎c��
	int Pay(int* eMoney, int price);

};