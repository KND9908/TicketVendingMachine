#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>
#include <Windows.h>
#include "TicketVendingMachine.h"

int main()
{
	//�����@�̃C���X�^���X���X�}�[�g�|�C���^�Ő���
	std::unique_ptr<TicketVendingMachine> ticketVendingMachine = std::make_unique<TicketVendingMachine>();

	std::cout << "�����@�̃v���O����" << std::endl << std::endl;
	// �L�[�{�[�h���͂��Ď�����X���b�h���J�n
	std::thread monitorThread(&TicketVendingMachine::Monitor, ticketVendingMachine.get());

	//�����@�͑ҋ@�����n�܂邽�ߍŏ��ɂ��̏�������n�߂�B
	ticketVendingMachine->Standby();

	return 0;
}

