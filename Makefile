SRCS := \
tensorflow/lite/experimental/micro/debug_log.cc tensorflow/lite/experimental/micro/debug_log_numbers.cc tensorflow/lite/experimental/micro/micro_error_reporter.cc tensorflow/lite/experimental/micro/micro_interpreter.cc tensorflow/lite/experimental/micro/micro_mutable_op_resolver.cc tensorflow/lite/experimental/micro/simple_tensor_allocator.cc tensorflow/lite/experimental/micro/kernels/all_ops_resolver.cc tensorflow/lite/experimental/micro/kernels/depthwise_conv.cc tensorflow/lite/experimental/micro/kernels/fully_connected.cc tensorflow/lite/experimental/micro/kernels/softmax.cc tensorflow/lite/c/c_api_internal.c tensorflow/lite/core/api/error_reporter.cc tensorflow/lite/core/api/flatbuffer_conversions.cc tensorflow/lite/core/api/op_resolver.cc tensorflow/lite/kernels/kernel_util.cc tensorflow/lite/kernels/internal/quantization_util.cc  main.cpp tensorflow/lite/experimental/micro/examples/micro_speech/osx/audio_provider.cc tensorflow/lite/experimental/micro/examples/micro_speech/feature_provider.cc tensorflow/lite/experimental/micro/examples/micro_speech/micro_features/no_micro_features_data.cc tensorflow/lite/experimental/micro/examples/micro_speech/micro_features/yes_micro_features_data.cc tensorflow/lite/experimental/micro/examples/micro_speech/micro_features/tiny_conv_micro_features_model_data.cc tensorflow/lite/experimental/micro/examples/micro_speech/recognize_commands.cc tensorflow/lite/experimental/micro/examples/micro_speech/command_responder.cc tensorflow/lite/experimental/micro/examples/micro_speech/micro_features/micro_features_generator.cc tensorflow/lite/experimental/micro/examples/micro_speech/micro_features/micro_model_settings.cc tensorflow/lite/experimental/micro/examples/micro_speech/micro_features/fft.cc tensorflow/lite/experimental/micro/examples/micro_speech/micro_features/fft_util.cc tensorflow/lite/experimental/micro/examples/micro_speech/micro_features/filterbank.cc tensorflow/lite/experimental/micro/examples/micro_speech/micro_features/filterbank_util.cc tensorflow/lite/experimental/micro/examples/micro_speech/micro_features/frontend.cc tensorflow/lite/experimental/micro/examples/micro_speech/micro_features/frontend_util.cc tensorflow/lite/experimental/micro/examples/micro_speech/micro_features/log_lut.cc tensorflow/lite/experimental/micro/examples/micro_speech/micro_features/log_scale.cc tensorflow/lite/experimental/micro/examples/micro_speech/micro_features/log_scale_util.cc tensorflow/lite/experimental/micro/examples/micro_speech/micro_features/noise_reduction.cc tensorflow/lite/experimental/micro/examples/micro_speech/micro_features/noise_reduction_util.cc tensorflow/lite/experimental/micro/examples/micro_speech/micro_features/pcan_gain_control.cc tensorflow/lite/experimental/micro/examples/micro_speech/micro_features/pcan_gain_control_util.cc tensorflow/lite/experimental/micro/examples/micro_speech/micro_features/window.cc tensorflow/lite/experimental/micro/examples/micro_speech/micro_features/window_util.cc tensorflow/lite/experimental/micro/tools/make/downloads/kissfft/kiss_fft.c tensorflow/lite/experimental/micro/tools/make/downloads/kissfft/tools/kiss_fftr.c

OBJS := \
$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(SRCS)))

CXXFLAGS += -O3 -DNDEBUG --std=c++11 -g -DTF_LITE_STATIC_MEMORY -DTF_LITE_DISABLE_X86_NEON -DTF_LITE_DISABLE_X86_NEON -I. -I./third_party/gemmlowp -I./third_party/flatbuffers/include -I./third_party/kissfft
CCFLAGS +=  -DNDEBUG -g -DTF_LITE_STATIC_MEMORY -DTF_LITE_DISABLE_X86_NEON -DTF_LITE_DISABLE_X86_NEON -I. -I./third_party/gemmlowp -I./third_party/flatbuffers/include -I./third_party/kissfft

LDFLAGS +=  -lm -framework Foundation -framework AudioToolbox

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.c
	$(CC) $(CCFLAGS) $(INCLUDES) -c $< -o $@

micro_speech : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

all: micro_speech
