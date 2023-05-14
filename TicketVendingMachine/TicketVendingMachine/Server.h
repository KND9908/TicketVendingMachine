#pragma once
//電子マネーの処理を行うサーバとの通信を行うクラス
//サーバ環境の仕様やデータ管理のライブラリ等の定義が存在しないため、
//本課題では仮想的にサーバとの通信を行うクラスとして定義します。
class Server
{
	//サーバとの処理通信の結果を示すenum
	enum Result
	{
		Result_Success,//成功
		Result_Failed,//失敗
	};

	//サーバからの通信結果を代入する関数
	//本課題ではこの値を変更することで異常系の動作確認を行います。
	Result m_result = Result_Success;

	//引去額
	int withdraw = 0;

	//電子マネーの残高
	int m_eMoney = 0;

	//サーバとの通信を行う関数
	//引数：ユーザの所持している電子マネーの残高、および使用する金額
	//戻り値：処理の終了コード
	bool Connect(int* eMoney, int price);

public:
	//電子マネーのPay処理
	//引数：ユーザの所持している電子マネーの残高、および使用する金額
	//戻り値：処理完了後の電子マネーの残高
	int Pay(int* eMoney, int price);

};