#include "Server.h"
#include <iostream>

bool Server::Connect(int *eMoney, int price)
{
	//サーバとの通信を行う処理をここに書く
	//具体的な通信処理は省略する

	//サーバ内部で処理が無事行われたかをreturnする
	//今回の要件においては具体的なサーバからの戻り値の取り決めはありませんでしたので、
	//具体的な処理は省略します
	return m_result;
}

int Server::Pay(int *eMoney, int price)
{
	//サーバと通信を行う。
	int result = Connect(eMoney, price);

	if (result == Result_Success)
	{
		*eMoney -= price;
		withdraw = price;
		m_eMoney = *eMoney;
	}
	else if (result == Result_Failed)
	{
		//サーバとの通信が失敗した場合、電子マネーの残高は変更しない
		std::cout << "サーバとの通信に失敗しました。" << std::endl;
		std::cout << "電子マネーの残高は変更されません。" << std::endl;
	}

	return result;
}