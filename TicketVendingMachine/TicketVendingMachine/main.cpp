#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>
#include <Windows.h>
#include "TicketVendingMachine.h"

int main()
{
	//券売機のインスタンスをスマートポインタで生成
	std::unique_ptr<TicketVendingMachine> ticketVendingMachine = std::make_unique<TicketVendingMachine>();

	std::cout << "券売機のプログラム" << std::endl << std::endl;
	// キーボード入力を監視するスレッドを開始
	std::thread monitorThread(&TicketVendingMachine::Monitor, ticketVendingMachine.get());

	//券売機は待機中より始まるため最初にこの処理から始める。
	ticketVendingMachine->Standby();

	return 0;
}

