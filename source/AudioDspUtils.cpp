
//#include "AdacConfig.h"
#include "AudioDspUtils.h"

#include <cmath>
#include <cstring>

namespace {

	float computeRMS(const std::vector<float>& pAudioSample) 
	{
		float sum = 0.0f;
		for (int i = 0; i < pAudioSample.size(); i++) {
			sum += (pAudioSample[i] * pAudioSample[i]);  // Square each sample
		}
		return sqrt(sum / pAudioSample.size());  // Take square root of mean
	}
}


namespace adac 
{
	void AudioDspUtils::scaleFloatToPCM(std::vector<float>& pAudioSample) 
	{
		for (int i = 0; i < pAudioSample.size(); i++) {
			float scaledSample = pAudioSample[i] * 32768.0f;  // Convert to 16-bit PCM
			// Clip to valid range
			if (scaledSample > 32767.0f) scaledSample = 32767.0f;
			if (scaledSample < -32768.0f) scaledSample = -32768.0f;
			pAudioSample[i] = scaledSample;
		}
	}
	

    void AudioDspUtils::normalizeAndAmplify(std::vector<float> &pAudioSample)
    {
		float targetRMS = 0.20f;
		float minRMS = 1e-6f;  // Minimum threshold to prevent division by zero
	
		float rms = computeRMS(pAudioSample);
		if (rms < minRMS) {
			rms = minRMS;  // Prevent zero-division but still allow processing
		}
	
		float gain = targetRMS / rms;  // Compute gain factor
		for (int i = 0; i < pAudioSample.size(); i++) {
			pAudioSample[i] *= gain;
	
			// Clip to avoid distortion
			if (pAudioSample[i] > 1.0f) pAudioSample[i] = 1.0f;
			if (pAudioSample[i] < -1.0f) pAudioSample[i] = -1.0f;
		}
    }
}