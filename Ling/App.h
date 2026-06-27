#pragma once
namespace Ling {
	class App
	{
	public:
		static void init();
		static App* get();
		~App();
		void quit(const int& code);
	private:
		App();
	};
}


