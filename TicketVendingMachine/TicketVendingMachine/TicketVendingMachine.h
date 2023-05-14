#pragma once
#include "Money.h"
#include "Server.h"
#include <memory>

//券売機のデータおよび動作を管理するクラス
class TicketVendingMachine
{
	//券売機に投入される金種のenum　現金と電子マネー
	enum MoneyType
	{
		MoneyType_None,//無し
		MoneyType_Cash,//現金
		MoneyType_EMoney,//電子マネー
	};

	//現在の券売機の状態
	enum State
	{
		State_Standby,//待機中
		State_Selecting,//券種選択中
		State_Paying,//支払い中
		State_Change,//お釣り中
		State_Finish//取引完了
	};

	//お金の返却レバーの押下フラグ
	bool _isReturnLever = false;

	bool moneyempty = true;

	bool iscsvread = false;

	//サーバーのインスタンス
	std::unique_ptr<Server> m_server;

	//現在投入されている金種　初期値は無し
	MoneyType m_moneyType = MoneyType_None;

	//現在の券売機の状態　初期値は待機中
	State m_state = State_Standby;

	//券売機に投入された現金の合計金額　初期値は0円
	int m_totalMoney = 0;

	//券売機に投入された現金　csvの形式に合わせるため[0]~[7]までは現金の数、[8]は電子マネーの金額を格納する
	int m_money[8] = { 0 };

	//券売機から返却するお釣り　csvの形式に合わせるため[0]~[7]までは現金の数、[8]は電子マネーの金額を格納する
	int m_changeMoney[8] = { 0 };

	//現金で購入する場合の切符の価格　
	int m_ticketPrice = 130;

	//切符の購入枚数
	int m_ticketNum = 0;

	//お釣り
	int m_change = 0;

	//現在の券売機の状態に合わせて処理を管理する関数
	void Command();

	//券売機の各画面で行う処理
	void Selecting();//切符選択画面
	void Paying();//支払画面
	void Change();//会計結果画面

	//券売機に投入されたお金の合計を計算する処理
	void CalcTotalMoney();

	//コンソール画面の見栄えのためにラインを引く関数
	void DisplayLine();

	//券売機に投入されている金額を格納しているcsvファイルとのデータの読み書きを行う処理
	void ReadCsvFile();
	void WriteCsvFile();

public:
	//コンストラクタ
	TicketVendingMachine();

	//券売機内部の金銭の状態を監視する関数
	void Monitor();

	//券売機の待機状態中に行う関数
	void Standby();
};