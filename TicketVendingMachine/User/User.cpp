#include "User.h"
#include <iostream>
#include <conio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>
#include <Windows.h>
#include <vector>

void User::Monitor() {
	while (true) {
		ReadCsvFile();
		//お釣りが発生しているかの確認
		bool isChange = false;
		for (int i = 0; i < std::size(m_changeMoney); i++) {
			if (m_changeMoney[i] > 0) {
				isChange = true;
				m_money[i] += m_changeMoney[i];
			}
		}
		if (isChange) {
			std::cout << "お釣りを受け取りました" << std::endl;
			std::cout << "10円:" << m_changeMoney[0] << "枚"
				<< "50円:" << m_changeMoney[1] << "枚"
				<< "100円:" << m_changeMoney[2] << "枚"
				<< "500円:" << m_changeMoney[3] << "枚"
				<< "1000円:" << m_changeMoney[4] << "枚"
				<< "5000円:" << m_changeMoney[5] << "枚"
				<< "10000円:" << m_changeMoney[6] << "枚"
				<< "電子マネー:" << m_changeMoney[7] << "円" << std::endl;
			//お釣りを受け取ったらCSVのお釣りデータをクリアする
			for (int i = 0; i < std::size(m_changeMoney); i++) {
				m_changeMoney[i] = 0;
			}
			WriteCsvFile();
		}
		// 500ms間隔でキーの入力を監視する
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}


void User::InsertCoin()
{
	//投入する金種を選択してください　とコンソールに表示する
	std::cout << "投入する金種を選択してください" << std::endl;
	std::cout << "現金:0 電子マネー:1" << std::endl;

	int Key = 0;
	while (true)
	{
		Key = _getch();

		if (Key == '0' || Key == '1')
		{
			break;
		}
		std::cout << "入力値が不正です。再度入力してください" << std::endl;
	}

	if (Key == '0')
	{
		std::cout << "投入する現金を選択してください" << std::endl;
		std::cout << "10円:0 50円:1 100円:2 500円:3 1000円:4 5000円:5 10000円:6" << std::endl;

		while (true)
		{
			Key = _getch();
			if (Key == '0' || Key == '1' || Key == '2' || Key == '3' || Key == '4' || Key == '5' || Key == '6')
			{
				if (m_money[Key - 48] < 1) {
					std::cout << "そのお金を所持していません。他の金額を選択してください。" << std::endl;
				}
				else {
					break;
				}
			}
			std::cout << "入力値が不正です。再度入力してください" << std::endl;
		}

		m_money[Key - 48]--;
		coinValues[Key - 48]++;
		switch (Key - 48) {
		case 0:
			std::cout << "10円";
			break;
		case 1:
			std::cout << "50円";
			break;
		case 2:
			std::cout << "100円";
			break;
		case 3:
			std::cout << "500円";
			break;
		case 4:
			std::cout << "1000円";
			break;
		case 5:
			std::cout << "5000円";
			break;
		case 6:
			std::cout << "10000円";
			break;
		}
		std::cout << "を1つ投入しました。" << std::endl;
	}
	else if (Key == '1')
	{
		coinValues[7] = m_money[7];
		m_money[7] = 0;
		std::cout << "電子マネーを投入しました。" << std::endl;
	}
	//CSVの値を変更する処理
	WriteCsvFile();
}

void User::ReturnCoin() {
	_isReturnLever = true;
	WriteCsvFile();
}

//csvファイルのデータを読み込む処理
void User::ReadCsvFile()
{
	// 読み込むCSVファイルのパスを指定する
	std::string filePath = "..//TicketVendingMachine_InsertCoin.csv";

	// CSVファイルを読み込むファイルストリームを作成する
	std::ifstream inputFile(filePath);

	// ファイルが開けなかった場合はエラーを出力して終了する
	if (!inputFile.is_open())
	{
		std::cerr << "ファイルを開けませんでした。" << std::endl;
		return;
	}

	// CSVファイルから読み込んだデータを格納するための配列を作成する
	std::vector<std::vector<std::string>> data;

	// CSVファイルから1行ずつ読み込み、データを配列に格納する
	std::string line;
	int lineNumber = 0; // 行数をカウントする変数を追加する
	while (getline(inputFile, line))
	{
		std::vector<std::string> record;
		std::string field;
		std::stringstream ss(line);

		while (getline(ss, field, ','))
		{
			record.push_back(field);
		}

		data.push_back(record);

		// 2行目の場合、カンマ区切りでint型の配列に格納する
		if (lineNumber == 1)
		{
			std::stringstream ss2(line);
			std::string coinValueStr;
			int i = 0;
			while (getline(ss2, coinValueStr, ','))
			{
				if (i < std::size(coinValues)) {
					coinValues[i] = std::stoi(coinValueStr);
				}
				else if (i < std::size(coinValues) + std::size(m_changeMoney)) {
					m_changeMoney[i - std::size(coinValues)] = std::stoi(coinValueStr);
				}
				else {
					_isReturnLever = std::stoi(coinValueStr);
				}
				i++;
			}
		}
		lineNumber++; // 行数をインクリメントする
	}
	// ファイルストリームを閉じる
	inputFile.close();

	return;
}

//csvファイルにデータを書き込む処理
void User::WriteCsvFile() {
	// 書き込むCSVファイルのパスを指定する
	std::string filePath = "..//TicketVendingMachine_InsertCoin.csv";

	// CSVファイルを書き込むファイルストリームを作成する
	std::ofstream outputFile(filePath);

	// ファイルが開けなかった場合はエラーを出力して終了する
	if (!outputFile.is_open())
	{
		std::cerr << "ファイルを開けませんでした。" << std::endl;
		return;
	}

	outputFile << "InsertMoney_10,InsertMoney_50,InsertMoney_100,"
		"InsertMoney_500,InsertMoney_1000,InsertMoney_5000,InsertMoney_10000,"
		"ICMoney, returnMoney_10, returnMoney_50, returnMoney_100, returnMoney_500,"
		"returnMoney_1000, returnMoney_5000, returnMoney_10000, returnICMoney,returnLever" << std::endl;

	// 2行目にvector配列の内容をカンマ区切りで書き出す
	for (auto iter = coinValues.begin(); iter != coinValues.end(); ++iter)
	{
		outputFile << *iter;
		outputFile << ",";
	}
	for (int j = 0; j < std::size(m_changeMoney); j++) {
		outputFile << m_changeMoney[j];
		if (j != std::size(m_changeMoney))
		{
			outputFile << ",";
		}
	}
	outputFile << _isReturnLever;

	outputFile << std::endl;

	// ファイルストリームを閉じる
	outputFile.close();
}

void User::ShowMenu(){
	while (true) {
		std::cout << "現在の所持金" << std::endl;
		std::cout << "10円:" << m_money[0] << std::endl;
		std::cout << "50円: " << m_money[1] << std::endl;
		std::cout << "100円: " << m_money[2] << std::endl;
		std::cout << "500円: " << m_money[3] << std::endl;
		std::cout << "1000円: " << m_money[4] << std::endl;
		std::cout << "5000円: " << m_money[5] << std::endl;
		std::cout << "10000円:" << m_money[6] << std::endl;
		std::cout << "電子マネー:" << m_money[7] <<"円" << std::endl;
		std::cout << "処理のコマンドを選択してください" << std::endl;
		std::cout << "お金を入れる:0　返却レバーを押す:1" << std::endl;
		std::cout << "-----------------------------------------------" << std::endl;

		//ユーザーの操作を受け付ける
		int Key = 0;
		while (true)
		{
			Key = _getch();

			if (Key == '0')
			{
				InsertCoin();
				break;
			}
			else if (Key == '1')
			{
				ReturnCoin();
				break;
			}
		}
	}
}