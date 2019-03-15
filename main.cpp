#include <iostream>
#include <vector>
#include <cassert>

#include "tensorflow/lite/experimental/micro/kernels/all_ops_resolver.h"
#include "tensorflow/lite/experimental/micro/micro_error_reporter.h"
#include "tensorflow/lite/experimental/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"
#include "tensorflow/lite/kernels/internal/tensor_ctypes.h"


using namespace std;

template<typename T> dump_data(T* data, int len){
    for(int i = 0; i < len; i++)
        cout << data[i] << endl;
}

void DumpWeights(TfLiteTensor* tensor){

    /**
      kTfLiteNoType = 0,
      kTfLiteFloat32 = 1,
      kTfLiteInt32 = 2,
      kTfLiteUInt8 = 3,
      kTfLiteInt64 = 4,
      kTfLiteString = 5,
      kTfLiteBool = 6,
      kTfLiteInt16 = 7,
      kTfLiteComplex64 = 8,
      kTfLiteInt8 = 9,
      */
    int numElems = GetTensorShape(tensor);
    switch (tensor.type){
        case kTfLiteFloat32:
            auto data = GetTensorData<float>(tensor);
            dump_data(data, numElems);
            break;
        case kTfLiteInt32:
            auto data = GetTensorData<int32_t>(tensor);
            dump_data(data, numElems);
            break;
        case kTfLiteUInt8:
            auto data = GetTensorData<uint8_t>(tensor);
            dump_data(data, numElems);
            break;
        case kTfLiteInt64:
            auto data = GetTensorData<int64_t>(tensor);
            dump_data(data, numElems);
            break;
        case kTfLiteInt16:
            auto data = GetTensorData<int16_t>(tensor);
            dump_data(data, numElems);
            break;
        case kTfLiteInt8:
            auto data = GetTensorData<int16_t>(tensor);
            dump_data(data, numElems);
            break;
    }
};
int main(int argc, char* argv[]) {
    // Set up logging.
    tflite::MicroErrorReporter micro_error_reporter;
    tflite::ErrorReporter* error_reporter = &micro_error_reporter;

    assert(argc == 2);
    const char* tfile = argv[1];
    cout << tfile << endl;

    // Read toco flatbuffer into mem
    std::ifstream file(tfile, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if (file.read(buffer.data(), size))
    {
        /* worked! */
    }

    // Map the model into a usable data structure. This doesn't involve any
    // copying or parsing, it's a very lightweight operation.
    const tflite::Model* model =
        ::tflite::GetModel(g_tiny_conv_micro_features_model_data);
    if (model->version() != TFLITE_SCHEMA_VERSION) {
        error_reporter->Report(
                "Model provided is schema version %d not equal "
                "to supported version %d.\n",
                model->version(), TFLITE_SCHEMA_VERSION);
        return 1;
    }

    const flatbuffers::Vector<flatbuffers::Offset<Buffer>>* buffers =
        model->buffers();
    auto* subgraphs = model->subgraphs();

    if (subgraphs->size() != 1) {
        micro_error_reporter->Report("Only 1 subgraph is currently supported.\n");
        return;
    }

    auto subgraph_ = (*subgraphs)[0];
    auto tensors_ = subgraph_->tensors();

    for (int i = 0; i < subgraph_->inputs()->Length(); ++i) {
        const int tensor_index = subgraph_->inputs()->Get(i);
        const auto* tensor = tensors_->Get(tensor_index);

    }

}
