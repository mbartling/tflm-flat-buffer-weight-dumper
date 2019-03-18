// Dont just my gross hacky code
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

#include "tensorflow/lite/experimental/micro/kernels/all_ops_resolver.h"
#include "tensorflow/lite/experimental/micro/micro_error_reporter.h"
#include "tensorflow/lite/experimental/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"
#include "tensorflow/lite/kernels/internal/tensor_ctypes.h"
#include "tensorflow/lite/core/api/flatbuffer_conversions.h"
#include "tensorflow/lite/schema/schema_generated.h"



using namespace std;

template<typename T> 
void dump_data(T* data, int len){
    // Eww
    // Only print out linear representation cuz that's all I need
    cout << "~~~[" << endl;
    for(int i = 0; i < len; i++)
        cout << data[i] << endl;
    cout << "~~~]" << endl;
}

void DumpWeights(const tflite::Tensor* tensor,
    const flatbuffers::Vector<flatbuffers::Offset<tflite::Buffer>>* flat_buffers){
    void* tensor_data = nullptr;
    int numElems = 0;
    //if (auto* buffer = tensor->buffer()) {
    if (auto* buffer = (*flat_buffers)[tensor->buffer()]) {
        if (auto* array = buffer->data()) {
            if (numElems = array->size()) {
                cerr << numElems << endl;
                tensor_data =
                    const_cast<char*>(reinterpret_cast<const char*>(array->data()));
            }
        }
    }
    if(tensor_data == nullptr)
        return;

    switch (tensor->type()){
        case tflite::TensorType_FLOAT32:
            {
                auto* data1 = reinterpret_cast<float*>(tensor_data);
                dump_data(data1, numElems/sizeof(float));
                break;
            }
        case tflite::TensorType_INT32:
            {
                auto* data2 = reinterpret_cast<int32_t*>(tensor_data);
                dump_data(data2, numElems/sizeof(int32_t));
                break;
            }
        case tflite::TensorType_UINT8:
            {
                auto* data3 = reinterpret_cast<uint8_t*>(tensor_data);
                cout << hex;
                dump_data(data3, numElems);
                cout << dec;
                break;
            }
        case tflite::TensorType_INT64:
            {
                auto* data4 = reinterpret_cast<int64_t*>(tensor_data);
                dump_data(data4, numElems/sizeof(int64_t));
                break;
            }
        case tflite::TensorType_INT16:
            {
                auto* data5 = reinterpret_cast<int16_t*>(tensor_data);
                dump_data(data5, numElems/sizeof(int16_t));
                break;
            }
        case tflite::TensorType_INT8:
            {
                cout << hex;
                auto* data6 = reinterpret_cast<int8_t*>(tensor_data);
                cout << dec;
                dump_data(data6, numElems);
                break;
            }
    }
};
int main(int argc, char* argv[]) {
    // Set up logging.
    tflite::MicroErrorReporter micro_error_reporter;
    tflite::ErrorReporter* error_reporter = &micro_error_reporter;

    assert(argc == 2);
    const char* tfile = argv[1];
    cerr << tfile << endl;

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
        ::tflite::GetModel(buffer.data());
    if (model->version() != TFLITE_SCHEMA_VERSION) {
        error_reporter->Report(
                "Model provided is schema version %d not equal "
                "to supported version %d.\n",
                model->version(), TFLITE_SCHEMA_VERSION);
        return 1;
    }

    const flatbuffers::Vector<flatbuffers::Offset<tflite::Buffer>>* buffers =
        model->buffers();
    auto* subgraphs = model->subgraphs();

    if (subgraphs->size() != 1) {
        error_reporter->Report("Only 1 subgraph is currently supported.\n");
        return -1;
    }

    auto subgraph_ = (*subgraphs)[0];
    auto tensors_ = subgraph_->tensors();

   // for (int i = 0; i < subgraph_->inputs()->Length(); ++i) {
   //     const int tensor_index = subgraph_->inputs()->Get(i);
   //     const auto* tensor = tensors_->Get(tensor_index);
   //     DumpWeights(tensor, buffers);
   // }
    for (int i = 0; i < tensors_->Length(); ++i) {
        const auto* tensor = tensors_->Get(i);
        DumpWeights(tensor, buffers);
    }

    return 0;

}
