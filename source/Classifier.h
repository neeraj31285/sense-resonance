#pragma once


#include <vector>
#include <string>

namespace ai {

    class Classifier 
    {
    public:
        static void runInference(const std::vector<float>& pSample,
                                 std::vector<std::pair<std::string, float>>& pClassification);
    };
}