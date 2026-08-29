#include "MyGameMain.h"

//ーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//猫ふみふみ
//ーーーーーーーーーーーーーーーーーーーーーーーーーーーー

//ゲーム情報
DG::Image::SP imgBG, imgCat, GS;
DG::Font::SP fontA, fontB, fontC, fontD;
//ゲーム状態　　　準備　ゲーム中　終了
enum class Stata { Ready, Normal, Done, Load, Save};

struct Chara
{
	Stata stata;
	int score;
	int highscore;
	int limit; 
	int timecnt;
	int count;
};
Chara cat;

//関数の宣言
bool Savedata_Save();
bool Savedata_Load(int n_);

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
	fontB = DG::Font::Create("HGS 教科書体", 20, 40);
	fontC = DG::Font::Create("HGS 教科書体", 10, 20);
	fontD = DG::Font::Create("HGS 教科書体", 10, 20);
	cat.stata = Stata::Ready;									//最初は準備

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
	cat.count = 0;


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
	fontC.reset();
	fontD.reset();
}
//-----------------------------------------------------------------------------
//更新処理
//機能概要：ゲームの１フレームに当たる処理
//-----------------------------------------------------------------------------
void  MyGameMain_UpDate()
{
	auto inp = ge->in1->GetState();
	//準備段階ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	if (cat.stata == Stata::Ready)
	{
		//Zキー押して
		if (true == inp.B1.down)
		{
			cat.stata = Stata::Normal;
		}
	}
	//ゲームスタートーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	if (cat.stata == Stata::Normal)
	{
		cat.limit--;		//ゲームカウンタ
		cat.timecnt++;      //行動カウンタ
		//０になったら、結果画面へ
		if (cat.limit < 0)
		{
			cat.stata = Stata::Done;
			cat.timecnt = 0;
		}
		else {
			if (true == inp.B1.down)
			{
				cat.stata = Stata::Normal;
				//20秒立ったら
				if (cat.timecnt > 20)
				{
					cat.score++;
				}
				//猫ふみふみのアニメーション
				//1、2、1、2で戻すようにする
				cat.count += 1;
				if (cat.count == 3)
				{
					cat.count = 1;
				}

			}

		}
	}
	//ゲーム終了ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	if (cat.stata == Stata::Done)
	{
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
		
	}
	//スコアをセーブする
	if (cat.stata == Stata::Save)
	{

	}
	//TODO:ロード中とセーブを実装したい

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

	//フォント描画
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//準備段階
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	if (cat.stata == Stata::Ready)
	{
		//ルール説明画像
		ML::Box2D draw(40, 40, 400, 200);
		ML::Box2D src(0, 0, 400, 200);
		GS->Draw(draw, src);

		//「Zキーでスタート」
		ML::Box2D textBox(300, 250, 480, 270);
		string text = "Zキーでスタート";
		fontC->Draw(textBox, text, ML::Color(1.0f, 1.0f, 1.0f, 1.0f));
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//ゲーム開始
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	if (cat.stata == Stata::Normal)
	{
		//一回押したら
		ML::Box2D draw1(160, 40, 150, 240);
		ML::Box2D src1(cat.count * 150, 0, 150, 240);
		imgCat->Draw(draw1, src1);


		//得点の表示
		ML::Box2D textBox(0, 0, 480, 270);
		string text = "score:" + to_string(cat.score);
		fontA->Draw(textBox, text, ML::Color(1.0f, 1.0f, 1.0f, 1.0f));

		//カウントダウン
		ML::Box2D textBox2(400, 0, 480, 270);
		string text2 = to_string(cat.limit / 60);
		fontB->Draw(textBox2, text2, ML::Color(1.0f, 1.0f, 1.0f, 1.0f));
	}
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//ゲーム終了
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーーー
	if (cat.stata == Stata::Done)
	{

		//結果は…〇〇回
		ML::Box2D textBox2(0, 0, 480, 270);
		string text2 = "結果は..." + to_string(cat.score) + "ふみふみ";
		fontA->Draw(textBox2, text2, ML::Color(1.8f, 0.7f, 0.0f, 0.1f));
		if (cat.timecnt <= 180)
		{
			//少々待ってね！
			ML::Box2D textBox1(300, 250, 480, 270);
			string text1 = "ロード中・・・";
			fontD->Draw(textBox1, text1, ML::Color(1.0f, 1.0f, 1.0f, 1.0f));
		}
		if (cat.timecnt >= 180)
		{
			//「Zキーでタイトルへ」
			ML::Box2D textBox1(300, 250, 480, 270);
			string text1 = "Zキーでタイトル";
			fontD->Draw(textBox1, text1, ML::Color(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}
	
