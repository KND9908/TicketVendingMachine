#pragma once
#include "Money.h"
#include <vector>

//券売機を操作するユーザークラス
//疑似的にユーザーの操作模した行動のできる治具ソフトです。
//実際のユーザーの想定操作を参考に
//ユーザーが券売機に対して行う操作をシミュレータとして実装しています。
class User
{
	//返却レバーを押下したかを判定するフラグ
	bool _isReturnLever = false;

	//現在所持している各お金の金種　
	//初期値は以下の通り
	//10 円玉 15 枚
	//50 円玉 3 枚
	//100 円玉 2 枚
	//500 円玉 1 枚
	//1000 円札 1 枚
	//5000 円札 1 枚
	//10000 円札 1 枚
	//所持している電子マネーの残高　初期値は 1000円
	int m_money[8] = { 15, 3, 2, 1, 1, 1, 1 ,1000 };

	//お釣りの金種
	int m_changeMoney[8] = { 0 };

	//csvファイルからデータを読み書きする処理
	void ReadCsvFile();
	void WriteCsvFile();

	//現在の券売機に格納されている金種
	std::vector<int> coinValues = { 0,0,0,0,0,0,0,0 };

public:
	//メニュー画面を表示する処理
	void ShowMenu();

	//券売機にコインを投入する処理
	void InsertCoin();

	//返却レバーを押下する処理
	void ReturnCoin();

	//券売機およびユーザーの状態を監視する処理
	void Monitor();
};