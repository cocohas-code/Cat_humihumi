#include "MyGameMain.h"

//ーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//猫ふみふみ
//ーーーーーーーーーーーーーーーーーーーーーーーーーーーー

//ゲーム情報
DG::Image::SP imgBG, imgCat, GS;
DG::Font::SP fontA, fontB, fontHS;
//ゲーム状態　　　準備　ゲーム中　終了
enum class Stata { Ready, Normal, Done };

struct gameState
{
	Stata stata;
	int score;
	int highscore;
	int timecnt;
	int AnimCount;
	int limit;
};
gameState cat;
string filepath;

//関数の宣言
bool Savedata_Save(int& s_);
bool Savedata_Load(int& s_);
void Ready_Render();
void Normal_Render();
void Done_Render();


//-----------------------------------------------------------------------------
//初期化処理
//機能概要：プログラム起動時に１回実行される（素材などの準備を行う）
//-----------------------------------------------------------------------------
void  MyGameMain_Initialize()
{
	//画像の読み込み
	imgBG = DG::Image::Create("./data/image/BG.png");			//背景
	imgCat = DG::Image::Create("./data/image/catImage.png");    //猫
	GS = DG::Image::Create("./data/image/gamestata.jpg");       //ルール説明
	//文字の読み込み
	fontA = DG::Font::Create("HGS 教科書体", 20, 40);
	fontB = DG::Font::Create("HGS 教科書体", 10, 20);
	cat.stata = Stata::Ready;									//最初は準備
	filepath = "./data/text/highscore.txt";

	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//キーボードの入力を受け取る
	// アナログスティックL
	XI::AnalogAxisKB Is = { DIK_LEFT,DIK_RIGHT,DIK_UP,DIK_DOWN };
	XI::AnalogAxisKB rs = { DIK_NUMPAD4,DIK_NUMPAD6,DIK_NUMPAD8,DIK_NUMPAD2 };
	XI::AnalogTriggerKB tg = { DIK_E,DIK_R };
	//十字キーとボタン
	XI::KeyDatas_KB
		key = {
			{ DIK_Z, XI::VGP::B1 },{ DIK_X, XI::VGP::B2},
			{ DIK_C, XI::VGP::B3 },{ DIK_V, XI::VGP::B4},
			{ DIK_A, XI::VGP::SE },{ DIK_S, XI::VGP::ST},
			{ DIK_Q, XI::VGP::L1}, { DIK_W, XI::VGP::R1},
			{ DIK_D, XI::VGP::L3},{ DIK_NUMPAD5, XI::VGP::R3},
	};
	ge->in1 = XI::GamePad::CreateKB(Is, rs, tg, key);

	//ゲームパッドの入力を受け取るオブジェクトを生成する
	XI::KeyDatas_GP but = {
		{XI::RGP::B01,XI::VGP::B1},{XI::RGP::B02,XI::VGP::B2},
		{XI::RGP::B03,XI::VGP::B3},{XI::RGP::B04,XI::VGP::B4},
		{XI::RGP::B05,XI::VGP::L1},{XI::RGP::B06,XI::VGP::R1},
		{XI::RGP::B07,XI::VGP::SE},{XI::RGP::B08,XI::VGP::ST},
		{XI::RGP::B09,XI::VGP::L3},{XI::RGP::B10,XI::VGP::R3},
	};
	auto wgp = XI::GamePad::CreateGP(0, but);
	//キーボードとゲームパッド結合
	XI::GamePad::Link(ge->in1, wgp);
	ge->in2 = XI::GamePad::CreateGP(1, but);
	ge->in3 = XI::GamePad::CreateGP(2, but);
	ge->in4 = XI::GamePad::CreateGP(3, but);
	ge->mouse = XI::Mouse::Create(2, 2);
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーー

	//初期化
	cat.limit = 11 * 60;
	cat.timecnt = 10;
	cat.score = 0;
	cat.AnimCount = 0;
	cat.highscore = 0;
	Savedata_Load(cat.highscore);

}
//-----------------------------------------------------------------------------
//解放処理
//機能概要：プログラム終了時に１回実行される（素材などの解放を行う）
//-----------------------------------------------------------------------------
void  MyGameMain_Finalize()
{
	imgCat.reset();
	imgCat.reset();
	GS.reset();
	fontA.reset();
	fontB.reset();
	fontHS.reset();
}
//-----------------------------------------------------------------------------
//更新処理
//機能概要：ゲームの１フレームに当たる処理
//-----------------------------------------------------------------------------
void  MyGameMain_UpDate()
{
	auto inp = ge->in1->GetState();

	switch (cat.stata) {
		//準備段階ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	case Stata::Ready:
		//Zキー押して
		if (true == inp.B1.down)
		{
			cat.stata = Stata::Normal;
		}
		break;
		//ゲームスタートーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	case Stata::Normal:
		cat.limit--;		//ゲームカウンタ
		cat.timecnt++;
		//０になったら、結果画面へ
		if (cat.limit < 0)
		{
			cat.stata = Stata::Done;
			cat.timecnt = 0;
		}
		if (true == inp.B1.down)
		{
			cat.stata = Stata::Normal;
			cat.score++;
			//猫のアニメーション
			//1、2、1、2で戻すようにする;
			cat.AnimCount = (cat.AnimCount % 2) + 1;
		}

		break;
		//ゲーム終了ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	case Stata::Done:
		cat.timecnt++;
		if (cat.timecnt >= 180)
		{
			//Zキーを押したら、タイトル画面へ戻る
			if (true == inp.B1.down)
			{
				//最初からリセット
				//TODO:後でスコアセーブデータを加えたい
				cat.stata = Stata::Ready;
				cat.limit = 11 * 60;
				cat.score = 0;

			}
		}
		if (cat.score > cat.highscore)
		{
			cat.highscore = cat.score;
			Savedata_Save(cat.highscore);
		}
		break;
	}

}
//-----------------------------------------------------------------------------
//描画処理
//機能概要：ゲームの１フレームに当たる表示処理 ２Ｄ
//-----------------------------------------------------------------------------
void  MyGameMain_Render2D()
{
	//背景
	ML::Box2D draw0(0, 0, 480, 270);
	ML::Box2D src0(0, 0, 480, 270);
	imgBG->Draw(draw0, src0);

	switch (cat.stata)
	{
	case Stata::Ready:
		Ready_Render();
		break;
	case Stata::Normal:
		Normal_Render();
		break;
	case Stata::Done:
		Done_Render();
		break;
	}

}
//-----------------------------------------------------------------------------
//描画処理
//機能概要：各ゲーム遷移のフォントの描画
//-----------------------------------------------------------------------------
//準備段階ーReadyーーーーーーーーーーーーーーーーーーーーーーーーーーーー
void Ready_Render()
{
	//ルール説明画像
	ML::Box2D draw(40, 40, 400, 200);
	ML::Box2D src(0, 0, 400, 200);
	GS->Draw(draw, src);

	//「Zキーでスタート」
	ML::Box2D textBox0(300, 250, 480, 270);
	string text0 = "Zキーでスタート";
	fontB->Draw(textBox0, text0, ML::Color(1.0f, 1.0f, 1.0f, 1.0f));

	//ここでハイスコアの書き出し（ロード）
	ML::Box2D textBox1(100, 250, 480, 270);
	string text1 = "ハイスコア：" + to_string(cat.highscore);
	fontB->Draw(textBox1, text1, ML::Color(1.0f, 1.0f, 1.0f, 1.0f));

}
//ゲーム内ーNormalーーーーーーーーーーーーーーーーーーーーーーーーーーーー
void Normal_Render()
{
	//一回押したら
	ML::Box2D draw1(160, 40, 150, 240);
	ML::Box2D src1(cat.AnimCount * 150, 0, 150, 240);
	imgCat->Draw(draw1, src1);


	//得点の表示
	ML::Box2D textBox0(0, 0, 480, 270);
	string text0 = "score:" + to_string(cat.score);
	fontA->Draw(textBox0, text0, ML::Color(1.0f, 1.0f, 1.0f, 1.0f));

	//カウントダウン
	ML::Box2D textBox1(400, 0, 480, 270);
	string text1 = to_string(cat.limit / 60);
	fontA->Draw(textBox1, text1, ML::Color(1.0f, 1.0f, 1.0f, 1.0f));
}
//リザルトーDoneーーーーーーーーーーーーーーーーーーーーーーーーーーーー
void Done_Render()
{
	//結果は…〇〇回
	ML::Box2D textBox0(0, 0, 480, 270);
	string text0 = "結果は..." + to_string(cat.score) + "ふみふみ";
	fontA->Draw(textBox0, text0, ML::Color(1.8f, 0.7f, 0.0f, 0.1f));
	if (cat.timecnt <= 180)
	{
		//少々待ってね！
		ML::Box2D textBox1(300, 250, 480, 270);
		string text1 = "少々待ってね";
		fontB->Draw(textBox1, text1, ML::Color(1.0f, 1.0f, 1.0f, 1.0f));
	}
	if (cat.timecnt >= 180)
	{
		//「Zキーでタイトルへ」
		ML::Box2D textBox2(300, 250, 480, 270);
		string text2 = "Zキーでタイトル";
		fontB->Draw(textBox2, text2, ML::Color(1.0f, 1.0f, 1.0f, 1.0f));
	}
}
//-----------------------------------------------------------------------------
//セーブ機能
//NOTE:ハイスコアをセーブする。
//-----------------------------------------------------------------------------
bool Savedata_Save(int& s_)
{
	ofstream fin(filepath);

	if (!fin)
	{
		//ファイルがない場合は保存しない
		return false;
	}

	fin << s_;

	fin.close();
	return true;
}
//-----------------------------------------------------------------------------
//ロード機能
//NOTE：ファイルを読み込む
//-----------------------------------------------------------------------------
bool Savedata_Load(int& s_)
{
	ifstream fin(filepath);

	if (!fin)
	{
		//ファイルがない場合は保存しない
		return false;
	}

	fin >> s_;

	fin.close();
	return true;
}


