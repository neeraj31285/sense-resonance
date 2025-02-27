
#include <iostream>

#include "Classifier.h"
#include "edge-impulse-sdk/classifier/ei_run_classifier.h"

namespace ai {

    void Classifier::runInference(const std::vector<float> &pSample, std::vector<std::pair<std::string, float>>& pClassification) 
    {
        signal_t signal;
        signal.total_length = pSample.size();
    
        signal.get_data = [&](size_t offset, size_t length, float *out_ptr) -> int {
            for (size_t i = 0; i < length; i++) {
                out_ptr[i] = static_cast<float>(pSample[offset + i]);
            }
            return 0;
        };
    
        ei_impulse_result_t result = {0};
        EI_IMPULSE_ERROR ei_error = run_classifier(&signal, &result);
    
        if (ei_error != EI_IMPULSE_OK) {
            std::cout << "Edge Impulse error: " << ei_error << std::endl;
            return;
        }

        if(pClassification.empty()) {
            for(const auto& inference : result.classification) {
                pClassification.push_back(std::make_pair(inference.label, inference.value));
            }
        }
        else {
            for(int i = 0; i < pClassification.size(); i++) {
                pClassification[i].second = result.classification[i].value;
            }
        }
    }
}