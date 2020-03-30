#pragma once
#include <memory>
#include <map>
#include <vector>
#include <list>
#include "MouseCtl.h"
#include "TicketMachine.h"

#define lpMyself Myself::GetInstance()

// 買い手
class Myself
{
public:
	static Myself& GetInstance()
	{
		return *s_Instance;
	}

	void Run();
	void Draw();			// 描画処理
	void DrawUI();			// UI描画処理
	bool GetSysSuccess();	// システム初期化の成功、失敗を返す

private:
	bool SysInit();
	void SysEnd();

	struct MyselfDeleter
	{
		void operator()(Myself* myself) const
		{
			delete myself;
		}
	};

	Myself();
	~Myself();

	static std::unique_ptr<Myself, MyselfDeleter> s_Instance;

	std::unique_ptr<MouseCtl> mouse;	// キーボード入力を管理する

	bool sysSuccess;					// システム初期化の成功、失敗フラグ

	MapInt havingCash;					// 持っているお金  (first：金種、second：枚数)

	std::map<std::string, int> images;	// 画像ハンドル

	std::vector<int> moneyType;			// 支払い手法

};

