#include "TicketVendingMachine.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <conio.h>
#include <thread>
#include <chrono>

TicketVendingMachine::TicketVendingMachine() {
	m_server = std::make_unique<Server>();
}

void TicketVendingMachine::Standby() {
	//ユーザーによって画面が操作された状態を想定
	//実際の券売機を想定すると、画面をタッチしたあと、購入する切符を選択する画面に遷移するため、
	//本関数では、コマンドを受けてから券種選択中の状態に遷移する処理を行う。
	std::cout << "*ご利用のボタンを押下してください*" << std::endl;
	std::cout << "券売機の画面をタッチする:0" << std::endl;
	DisplayLine();

	int Key = _getch();
	//今回のプログラムでは、ハード面の考慮を行うことが出来ないため、
	//仮の操作として0キーが押下された場合、券売機の処理を実行するようにする。
	if (Key == '0') {
		m_state = State_Selecting;
		//券種選択中の処理を行う
		Selecting();
	}
}

void TicketVendingMachine::Selecting() {
	//券種選択中の処理を行う　
	//今回は仕様として券種が1種類のみのため、具体的な選択の処理は省略する。
	//実際の券売機で一度の購入処理で購入できる数が8枚までとのことなので今回の仕様でも8枚まで購入可能とする。
	//実際の券売機の想定として、券種が今後拡張されることを考慮し本関数を実装する。

	//券種選択中の処理を行う
	//本課題では切符は1種類のためコンソールの切符の選択肢は１種類のみになりますが、
	//実際の券売機を想定した場合には複数の種類の切符を購入することが出来る仕様を元に、
	//今後の拡張性を考え切符の種類を選択する画面の設計は行う。
	std::cout << "*ご利用の切符を選択してください*" << std::endl;
	std::cout << "切符1:0" << std::endl;
	DisplayLine();

	while (true) {
		int Key = _getch();
		if (Key == '0') {
			break;
		}
	}

	//購入枚数を受け取る
	while (true) {
		//切符の購入枚数を受け付ける状態にする。
		std::cout << "切符の購入枚数を入力してください。購入できる切符の最大枚数は8枚までです。" << std::endl;
		DisplayLine();
		std::string input;
		std::cin >> input;

		if (!std::all_of(input.begin(), input.end(), isdigit)) {
			std::cout << "数値のみ入力してください。" << std::endl;
			DisplayLine();
		}
		else if (input.length() > 1 || std::stoi(input) < 1) {
			std::cout << "入力数値を修正してください。購入できる切符の最大枚数は8枚までです。" << std::endl;
			DisplayLine();
		}
		else {
			m_ticketNum = std::stoi(input);
			std::cout << "購入枚数は" << m_ticketNum << "枚でよろしいですか？" << std::endl;
			std::cout << "はい：Y" << std::endl;
			std::cout << "いいえ：N" << std::endl;
			DisplayLine();

			int Key = _getch();
			if (Key == 'Y' || Key == 'y') {
				m_state = State_Paying;
				Paying();
				break;
			}
			else if (Key == 'N' || Key == 'n') {
				//処理はないが、Nボタンを押下されたケースとして条件分岐を明示的に記載する
			}
		}
	}
}

void TicketVendingMachine::Paying() {
	//精算処理を行う
	//お金がまだ機会に投入されていない場合、
	//または投入金額が切符の合計金額未満の場合、お金または電子マネーを投入してくださいという文章を表示する

	int totalmoney = 0;
	if (m_moneyType == MoneyType_Cash) {
		m_ticketPrice = 130;
	}
	else if (m_moneyType == MoneyType_EMoney) {
		m_ticketPrice = 124;
	}

	totalmoney = m_ticketNum * m_ticketPrice;

	std::cout << "お支払金額は" << totalmoney << "円です。" << std::endl;
	std::cout << "お金または電子マネーを投入してください" << std::endl;
	DisplayLine();

	while (true) {
		CalcTotalMoney();
		if (totalmoney <= m_money[7] || totalmoney <= m_totalMoney) {
			while (iscsvread) {
				//csvを読み込んでいる最中の場合、終了するまで待つ。
			}
			moneyempty = false;//追加のお金投入を行わせないようにする
			if (m_moneyType == MoneyType_EMoney) {
				//サーバとの通信処理を行う。
				//データサーバの内容の更新が無事行えた場合には電子マネーの残高を減らす。
				if (m_server->Pay(&m_money[7], totalmoney)) {
					std::cout << "サーバとの通信でエラーが発生したため、取引を中止します。" << std::endl;
					DisplayLine();

					//返却
					m_changeMoney[7] = m_money[7];
					m_money[7] = 0;
					WriteCsvFile();
					m_state = State_Standby;
				}
				else {
					m_state = State_Change;
					//お釣り中の処理を行う
					Change();
				}
			}
			else {
				//現金を減額する。
				m_totalMoney -= totalmoney;
				m_state = State_Change;
				//お釣り中の処理を行う
				Change();
			}
			break;
		}
	}
}

void TicketVendingMachine::Change() {
	//お釣りはその数字に最も近い硬貨および紙幣から支払われていくとする。
	if (m_moneyType == MoneyType_Cash) {
		std::cout << "切符とお釣りをお受け取りください。" << std::endl;
		std::cout << "お釣りは" << m_totalMoney << "円です。" << std::endl;
		DisplayLine();

		//お釣りはその金額に対して大きい紙幣を合わせて出力する。
		while (m_totalMoney != 0) {
			if (10000 <= m_totalMoney) {
				m_totalMoney -= 10000;
				m_changeMoney[6]++;
			}
			else if (5000 <= m_totalMoney) {
				m_totalMoney -= 5000;
				m_changeMoney[5]++;
			}
			else if (1000 <= m_totalMoney) {
				m_totalMoney -= 1000;
				m_changeMoney[4]++;
			}
			else if (500 <= m_totalMoney) {
				m_totalMoney -= 500;
				m_changeMoney[3]++;
			}
			else if (100 <= m_totalMoney) {
				m_totalMoney -= 100;
				m_changeMoney[2]++;
			}
			else if (50 <= m_totalMoney) {
				m_totalMoney -= 50;
				m_changeMoney[1]++;
			}
			else {
				m_totalMoney -= 10;
				m_changeMoney[0]++;
			}
		}
		std::cout << "投入された金種の枚数:" << std::endl;
		if (m_money[0] != 0) {
			std::cout << "10円:" << m_money[0] <<" 枚 "<<std::endl;
		}
		if (m_money[1] != 0) {
			std::cout << "50円:" << m_money[1] <<" 枚 "<<std::endl;
		}
		if (m_money[2] != 0) {
			std::cout << "100円:" << m_money[2] <<" 枚 "<<std::endl;
		}
		if (m_money[3] != 0) {
			std::cout << "500円:" << m_money[3] <<" 枚 "<<std::endl;
		}
		if (m_money[4] != 0) {
			std::cout << "1000円:" << m_money[4] <<" 枚 "<<std::endl;
		}
		if (m_money[5] != 0) {
			std::cout << "5000円:" << m_money[5] <<" 枚 "<<std::endl;
		}
		if (m_money[6] != 0) {
			std::cout << "10000円:" << m_money[6] <<" 枚 "<<std::endl;
		}


		std::cout << "お釣りの内訳:" << std::endl;
		if (m_changeMoney[0] != 0) {
			std::cout << "10円:" << m_changeMoney[0] << "枚" << std::endl;
		}
		if (m_changeMoney[1] != 0) {
			std::cout << "50円:" << m_changeMoney[1] << "枚" << std::endl;
		}
		if (m_changeMoney[2] != 0) {
			std::cout << "100円:" << m_changeMoney[2] << "枚" << std::endl;
		}
		if (m_changeMoney[3] != 0) {
			std::cout << "500円:" << m_changeMoney[3] << "枚" << std::endl;
		}
		if (m_changeMoney[4] != 0) {
			std::cout << "1000円:" << m_changeMoney[4] << "枚" << std::endl;
		}
		if (m_changeMoney[5] != 0) {
			std::cout << "5000円:" << m_changeMoney[5] << "枚" << std::endl;
		}
		if (m_changeMoney[6] != 0) {
			std::cout << "10000円:" << m_changeMoney[6] << "枚" << std::endl;
		}
		DisplayLine();

	}

	//電子マネーの場合はお残高のみ表示する
	else if (m_moneyType == MoneyType_EMoney) {
		std::cout << "切符と電子マネーをお受け取りください。" << std::endl;
		m_changeMoney[7] = m_money[7];
		m_money[7] = 0;
		std::cout << "電子マネーの残高:" << m_changeMoney[7] << "円" << std::endl;
		DisplayLine();

	}

	//機械の中のお金を整理する
	for (int i = 0; i < std::size(m_money); i++) {
		m_money[i] = 0;
	}

	WriteCsvFile();
	std::cout << "*ご利用ありがとうございました*" << std::endl;
	DisplayLine();

	//お釣りを出力したら、待機画面へ遷移する
	moneyempty = true;
	m_state = State_Standby;
	Standby();
}

void TicketVendingMachine::CalcTotalMoney() {
	m_totalMoney = 10 * m_money[0] + 50 * m_money[1] + 100 * m_money[2] + 500 * m_money[3] + 1000 * m_money[4]
		+ 5000 * m_money[5] + 10000 * m_money[6];

}

void TicketVendingMachine::DisplayLine() {
	//見栄えのためにラインを引く
	std::cout << "ーーーーーーーーーーーーーーーーーーーーーーーーーー" << std::endl;
}

void TicketVendingMachine::ReadCsvFile()
{
	iscsvread = true;
	// 読み込むCSVファイルのパスを指定する
	std::string filePath = "..//TicketVendingMachine_InsertCoin.csv";

	// CSVファイルを読み込むファイルストリームを作成する
	std::ifstream inputFile(filePath);

	// ファイルが開けなかった場合はエラーを出力して終了する
	if (!inputFile.is_open())
	{
		std::cerr << "ファイルを開くことができませんでした。" << std::endl;
		return;
	}

	// CSVファイルから読み込んだデータを格納するための配列を作成する
	std::vector<std::vector<std::string>> data;

	// CSVファイルから1行ずつ読み込み、データを配列に格納する
	std::string line;
	int lineNumber = 0; // 行数をカウントする変数を追加する

	bool chg = false;

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
				if (i < std::size(m_money)) {
					if (m_money[i] != std::stoi(coinValueStr)) {
						if (i < std::size(m_money) - 1) {//現金投入
							if (m_moneyType != MoneyType_EMoney)
							{
								m_moneyType = MoneyType_Cash;
							}
							m_money[i] = std::stoi(coinValueStr);
							chg = true;
						}
						else if (i == 7) {//電子マネー投入
							if (m_moneyType != MoneyType_Cash)
							{
								m_moneyType = MoneyType_EMoney;
							}
							m_money[i] = std::stoi(coinValueStr);
							chg = true;
						}
					}
				}
				else if (i < std::size(m_money) + std::size(m_changeMoney)) {//お釣りデータの取得
					m_changeMoney[i - std::size(m_money)] = std::stoi(coinValueStr);
				}
				else {//券売機の金銭返却レバーが押下されていないかの確認
					if (std::stoi(coinValueStr)) {
						_isReturnLever = true;
					}
				}

				i++;
			}
		}
		lineNumber++; // 行数をインクリメントする
	}
	if (chg) {
		std::cout << "お金が投入されました。" << std::endl;
		if (m_moneyType == MoneyType_Cash) {
			CalcTotalMoney();
			std::cout << "現在投入されている金額の合計:" << m_totalMoney << "円" << std::endl;
		}
		else if (m_moneyType == MoneyType_EMoney) {
			std::cout << "電子マネーの残高:" << m_money[7] << "円" << std::endl;
		}
	}

	// ファイルストリームを閉じる
	inputFile.close();

	iscsvread = false;
}

//csvファイルにデータを書き込む処理
void TicketVendingMachine::WriteCsvFile() {
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
	for (int i = 0; i < std::size(m_money); i++)
	{
		outputFile << m_money[i];
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

void TicketVendingMachine::Monitor()
{
	while (true) {

		if (moneyempty) {
			ReadCsvFile();

			//投入された金種に問題がないかを確認する
			bool iscasherror = false;
			bool isallzero = true;
			for (int i = 0; i < std::size(m_money); i++) {

				if (m_moneyType == MoneyType_EMoney && m_money[i] != 0 && i < std::size(m_money) - 1) {
					iscasherror = true;
					m_changeMoney[i] = m_money[i];
					m_money[i] = 0;
				}
				else if (m_moneyType == MoneyType_Cash && m_money[i] != 0 && i >= std::size(m_money) - 1) {
					m_changeMoney[i] = m_money[i];
					m_money[i] = 0;
					iscasherror = true;
				}
				if (m_money[i] != 0) {
					isallzero = false;
				}
			}
			if (_isReturnLever) {
				_isReturnLever = false;
				std::cout << "お金を返却します。" << std::endl;
				for (int i = 0; i < std::size(m_money); i++) {
					m_changeMoney[i] = m_money[i];
					m_money[i] = 0;
					m_moneyType = MoneyType_None;
				}
				WriteCsvFile();
			}

			if (iscasherror) {
				//既に電子マネーが投入されている場合はエラーメッセージを表示し、お金を返却する
				if (m_moneyType == MoneyType_EMoney) {
					std::cout << "既に電子マネーが投入されていますので現金でのお支払いはできません。" << std::endl;
					std::cout << "お支払方法は現金または電子マネーのどちらかのみの対応となっております。" << std::endl;
					std::cout << "お金を返却いたします。" << std::endl;
				}

				//既に現金が投入されている場合はエラーメッセージを表示し、お金を返却する
				else if (m_moneyType == MoneyType_Cash) {
					std::cout << "既に現金が投入されていますので電子マネーでのお支払はできません。" << std::endl;
					std::cout << "お支払方法は現金または電子マネーのどちらかのみの対応となっております。" << std::endl;
					std::cout << "電子マネーを返却いたします。" << std::endl;
				}
				WriteCsvFile();
			}
			//券売機の中にお金が何も入ってない場合は、券売機に投入されている金種の状態をNoneにする
			if (isallzero) {
				m_moneyType = MoneyType_None;
			}
		}
		// 500ms間隔でキーの入力を監視する
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}
