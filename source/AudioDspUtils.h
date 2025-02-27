#pragma once

#include <vector>

namespace adac {

	class AudioDspUtils 
	{
		AudioDspUtils() = delete;
		~AudioDspUtils() = delete;

	public:

		static void scaleFloatToPCM(std::vector<float>& pAudioSample);
		
		static void normalizeAndAmplify(std::vector<float> &pAudioSample);
	};
}
