#pragma once
#ifndef B00289996_AUDIO_MANAGER_H
#define B00289996_AUDIO_MANAGER_H
#include <bass.h>
#include <string>
#include <unordered_map>
namespace B00289996 {
	/// <summary>
	/// Plays Sounds Using BASS Audio Libraries
	/// </summary>
	class AudioManager {
	public:
		/// <summary>
		/// Gets the instance if this Singleton.
		/// </summary>
		/// <returns>the Singleton instance</returns>
		static AudioManager & GetInstance();
		/// <summary>
		/// Finalizes an instance of the <see cref="AudioManager"/> class.
		/// </summary>
		~AudioManager();
		/// <summary>
		/// Plays the clip specified by fileName.
		/// </summary>
		/// <param name="fileName">Name of the file.</param>
		void PlayClip(const std::string & fileName);
	private:
		/// <summary>
		/// Prevents a default instance of the <see cref="AudioManager"/> class from being created.
		/// </summary>
		AudioManager();
		/// <summary> used to prevent files being loaded multiple times </summary>
		std::unordered_map<std::string, HSAMPLE> audioMap;
	};
}
#endif // !B00289996_AUDIO_MANAGER_H
