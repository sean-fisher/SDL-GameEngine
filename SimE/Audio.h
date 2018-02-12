#pragma once
#include <SDL/SDL_mixer.h>
#include <string>
#include <map>

namespace SimE {

	class SoundEffect {
	public:
		friend class Audio;
		void play(int loops = 0);
	private:
		Mix_Chunk* m_chunk = nullptr;

	};
	class Music {
	public:
		friend class Audio;		
		void play(int loops = -1);
		void pause();
		void resume();
		void stop();
	private:
		Mix_Music* m_music = nullptr;

	};

	class Audio {
	public:
		Audio();
		~Audio();

		void init();
		void destroy();

		SoundEffect loadSoundEffect(const std::string & filepath);
		Music loadMusic(const std::string & filepath);

	private:
		std::map <std::string, Mix_Chunk*> m_effectMap;
		std::map<std::string, Mix_Music*> m_musicMap;1
		bool m_isInitialized = false;
	};

}