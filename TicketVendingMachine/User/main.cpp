#include <iostream>
#include <conio.h>
#include <thread>
#include "User.h"

int main() {
	//���[�U�̃C���X�^���X
	std::unique_ptr<User> user = std::make_unique<User>();//��ŃX���b�h�����ɂ��悤

	std::thread monitorThread(&User::Monitor, user.get());
	user->ShowMenu();
}