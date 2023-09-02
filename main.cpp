#include "Engine/Engine.h"
#include "externals/imgui/imgui.h"

#include <chrono>
#include <thread>
#include <numbers>
#include <memory>
#include <filesystem>

#include "Engine/WinApp/WinApp.h"

#include "Input/Gamepad/Gamepad.h"
#include "Input/KeyInput/KeyInput.h"
#include "Input/Mouse/Mouse.h"

#include "Drawers/PeraRender/PeraRender.h"
#include "Drawers/Model/Model.h"
#include "Drawers/Line/Line.h"
#include "Drawers/Texture2D/Texture2D.h"

#include "AudioManager/AudioManager.h"

#include "Utils/Camera/Camera.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリ初期化
	if(!Engine::Initialize("DirectXGame",Engine::Resolution::HD)){
		ErrorCheck::GetInstance()->ErrorTextBox(
			"InitializeDirect3D() : Engine::Initialize() Failed", 
			"WinMain"
		);
		return -1;
	}

	// フォントロード
	Engine::LoadFont("./Resources/Font/fonttest.spritefont");

	bool fullscreen = false;

	// カメラ
	Camera camera{ Camera::Mode::Othographic };
	
	// インスタンス
	Texture2D tex;

	// 通常読み込み(この関数が終わるまで進まない)
	tex.LoadTexture("./Resources/uvChecker.png");

	// こちらを使うと非同期に読み込みができる(読み込みが終わると描画される)
	//tex.ThreadLoadTexture("./Resource/uvChecker.png");

	// 描画関係初期化関数(任意のShaderの読み込みもできる)
	tex.Initialize();

	/// 
	/// メインループ
	/// 
	while (Engine::WindowMassage()) {
		// 描画開始処理
		Engine::FrameStart();

		// fps
#ifdef _DEBUG
		ImGui::SetNextWindowPos({});
		ImGui::SetNextWindowSizeConstraints({ 150,50 }, { 150,50 });
		ImGui::SetNextWindowCollapsed(false);
		ImGui::Begin("fps");
		ImGui::Text("Frame rate: %3.0f fps", ImGui::GetIO().Framerate);
		ImGui::End();
#endif

		// 入力処理
		Gamepad::Input();
		KeyInput::Input();
		Mouse::Input();

		/// 
		/// 更新処理
		/// 

		// フルスクリーン(ImGuiはフルスクリーン時バグる)
		if (KeyInput::Releaed(DIK_F11) || 
			((KeyInput::LongPush(DIK_LALT) || KeyInput::LongPush(DIK_RALT)) && KeyInput::Releaed(DIK_RETURN))) {
			fullscreen = !fullscreen;
			WinApp::GetInstance()->SetFullscreen(fullscreen);
		}

		camera.Update();

		tex.Update();

		///
		/// 更新処理ここまで
		/// 

		///
		/// 描画処理
		/// 
		
		// カメラ行列を渡す(描画の設定もここで変えれる)
		tex.Draw(camera.GetViewOthographics(), Pipeline::Normal);

		///
		/// 描画処理ここまで
		/// 


		// フレーム終了処理
		Engine::FrameEnd();

		// Escapeが押されたら終了
		if (KeyInput::Releaed(DIK_ESCAPE)) {
			break;
		}
	}

	// ライブラリ終了
	Engine::Finalize();

	return 0;
}