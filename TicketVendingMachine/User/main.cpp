#include <iostream>
#include <conio.h>
#include <thread>
#include "User.h"

int main() {
	//ユーザのインスタンス
	std::unique_ptr<User> user = std::make_unique<User>();//後でスレッド処理にしよう

	std::thread monitorThread(&User::Monitor, user.get());
	user->ShowMenu();
}