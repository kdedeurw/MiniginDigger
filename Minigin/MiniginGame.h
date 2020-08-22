#pragma once
#include "WindowInfo.h" //allowed to be included
struct SDL_Window;

enum class Key;
namespace dae
{
	class MiniginGame
	{
	public:
		MiniginGame(const char* pTitle = "Minigin", int w = 640, int h = 480, int msPF = 16);
		virtual ~MiniginGame();

		void Start();
		void AddFPSScene(float x = 0, float y = 480) const;
		void DisableDemoScene();
		void ForceQuit();
		void SetForceQuitKey(Key key);

	protected:
		bool m_IsQuit = false;

		//override me! (Don't forget to set ResourceManager's DataPath)
		virtual void LoadGame();
		//override me for extra functionality in the main update loop (debugging purposes)
		virtual void Update() {};
		//override me for extra functionality in the main render loop (debugging purposes)
		virtual void Render() {};

	private:
		Key m_QuitKey;
		SDL_Window* m_pWindow = nullptr;
		WindowInfo m_WindowInfo;

		void InitializeSDL();
		void Initialize();
		void Cleanup();
	};
}