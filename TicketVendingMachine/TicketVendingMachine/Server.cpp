#include "Server.h"
#include <iostream>

bool Server::Connect(int *eMoney, int price)
{
	//�T�[�o�Ƃ̒ʐM���s�������������ɏ���
	//��̓I�ȒʐM�����͏ȗ�����

	//�T�[�o�����ŏ����������s��ꂽ����return����
	//����̗v���ɂ����Ă͋�̓I�ȃT�[�o����̖߂�l�̎�茈�߂͂���܂���ł����̂ŁA
	//��̓I�ȏ����͏ȗ����܂�
	return m_result;
}

int Server::Pay(int *eMoney, int price)
{
	//�T�[�o�ƒʐM���s���B
	int result = Connect(eMoney, price);

	if (result == Result_Success)
	{
		*eMoney -= price;
		withdraw = price;
		m_eMoney = *eMoney;
	}
	else if (result == Result_Failed)
	{
		//�T�[�o�Ƃ̒ʐM�����s�����ꍇ�A�d�q�}�l�[�̎c���͕ύX���Ȃ�
		std::cout << "�T�[�o�Ƃ̒ʐM�Ɏ��s���܂����B" << std::endl;
		std::cout << "�d�q�}�l�[�̎c���͕ύX����܂���B" << std::endl;
	}

	return result;
}