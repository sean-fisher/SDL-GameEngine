#include "stdafx.h"
#include "Audio.h"

#include "SDLerrors.h"

namespace SimE {

	Audio::Audio() {
	}


	Audio::~Audio() {
		destroy();
	}

	void SimE::Audio::init() {
		// combo of MIX_INIT_FAC, MIX_INIT_MOD, MIX_INIT_MP3, MIX_INIT_OGG
		if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1) {
			fatalError("Mix_Init error: " + std::string(Mix_GetError()));
		}

		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
			fatalError("Mix_OpenAudio error: " + std::string(Mix_GetError()));
		}
		m_isInitialized = true;
	}

	void SimE::Audio::destroy() {
		if (m_isInitialized) {
			m_isInitialized = false;
			Mix_Quit();
		}
	}
	SoundEffect Audio::loadSoundEffect(const std::string & filepath) {
		auto it = m_effectMap.find(filepath);

		SoundEffect effect;

		if (it == m_effectMap.end()) {
			// load the effect
			Mix_Chunk* chunk = Mix_LoadWAV(filepath.c_str());
			if (chunk == nullptr) {
				fatalError("Sound effect was not found! Error: " + std::string(Mix_GetError()));
			}
			effect.m_chunk = chunk;
			m_effectMap[filepath] = chunk;
		} else {
			// is already cached
			effect.m_chunk = it->second;
		}
		return effect;
	}
	Music Audio::loadMusic(const std::string & filepath) {
		auto it = m_musicMap.find(filepath);

		Music music;

		if (it == m_musicMap.end()) {
			// load the effect
			Mix_Music* chunk = Mix_LoadMUS(filepath.c_str());
			if (chunk == nullptr) {
				fatalError("Sound effect was not found! Error: " + std::string(Mix_GetError()));
			}
			music.m_music = chunk;
			m_musicMap[filepath] = chunk;
		} else {
			// is already cached
			music.m_music = it->second;
		}
		return music;
	}
	void SoundEffect::play(int loops) {
		if (Mix_PlayChannel(-1, m_chunk, loops) == -1) {
			fatalError("Sound Effect did not play correctly! Error: " + std::string(Mix_GetError()));
		}
	}
	void Music::play(int loops) {
		if (Mix_PlayMusic(m_music, loops) == -1) {
			fatalError("Sound Effect did not play correctly! Error: " + std::string(Mix_GetError()));
		}
	}
	void Music::pause() {
		Mix_PauseMusic();
	}
	void Music::resume() {
		Mix_ResumeMusic();
	}
	void Music::stop() {
		Mix_HaltMusic();
	}
}