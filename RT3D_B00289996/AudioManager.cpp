#include "AudioManager.h"
#include <iostream>
namespace B00289996 {
	AudioManager::AudioManager() {
		/* Initialize default output device */
		if(!BASS_Init(-1, 44100, 0, 0, NULL)) std::cout << "Can't initialize device" << std::endl;
	}

	AudioManager & AudioManager::GetInstance() {
		static AudioManager instance; // create the static instance of this singleton
		return instance;
	}

	AudioManager::~AudioManager() {
		BASS_Free(); // release resources allocated by Bass 
	}

	void AudioManager::PlayClip(const std::string & fileName) {
		HSAMPLE sample;
		if(audioMap.count(fileName) == 0) { // if the file is not already loaded
			// Try to load the sample
			if(!(sample = BASS_SampleLoad(FALSE, fileName.c_str(), 0, 0, 3, BASS_SAMPLE_OVER_POS))) std::cout << "Couldn't Load Audio Clip" << std::endl; // if couldn't be loaded print error
			else audioMap.insert(std::pair<std::string, HSAMPLE>(fileName, sample)); // else store sample in map
		}
		else {
			sample = audioMap[fileName]; // retrieve sample from map
		}

		// try to play the sample
		HCHANNEL ch = BASS_SampleGetChannel(sample, FALSE);
		BASS_ChannelSetAttribute(ch, BASS_ATTRIB_FREQ, 0);
		BASS_ChannelSetAttribute(ch, BASS_ATTRIB_VOL, 0.5);
		BASS_ChannelSetAttribute(ch, BASS_ATTRIB_PAN, -1);
		if(!BASS_ChannelPlay(ch, FALSE)) std::cout << "Can't Play Audio Clip" << std::endl;
		else BASS_Start();
	}
}
