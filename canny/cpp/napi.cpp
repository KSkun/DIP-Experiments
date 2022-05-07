#include <napi.h>

#include "image.h"

static Napi::String hello(const Napi::CallbackInfo &info) {
    // Napi::Env is the opaque data structure containing the environment in which the request is being run.
    // We will need this env when we want to create any new objects inside of the node.js environment
    Napi::Env env = info.Env();

    // Create a C++ level variable
    std::string helloWorld = "Hello, world!";

    // Return a new javascript string that we copy-construct inside of the node.js environment
    return Napi::String::New(env, helloWorld);
}

static Napi::Boolean loadImage(const Napi::CallbackInfo &info) {
    auto path = info[0].ToString().Utf8Value();
    auto success = canny::loadImage(path);
    return Napi::Boolean::New(info.Env(), success);
}

static Napi::String getImage(const Napi::CallbackInfo &info) {
    auto id = info[0].ToNumber().Int32Value();
    auto image = canny::getImage(id);
    return Napi::String::New(info.Env(), image);
}

static Napi::String doProcess(const Napi::CallbackInfo &info) {
    auto sigma = info[0].ToNumber().FloatValue();
    auto gradKern = info[1].ToNumber().Int32Value();
    auto thrLow = info[2].ToNumber().FloatValue(), thrHigh = info[3].ToNumber().FloatValue();
    canny::gaussianFilter(sigma);
    canny::getGradient(gradKern);
    canny::nonMaxSuppression();
    canny::doubleThreshold(thrLow, thrHigh);
    canny::connectEdges();
    return Napi::String::New(info.Env(), "");
}

static Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "hello"),
                Napi::Function::New(env, hello));
    exports.Set(Napi::String::New(env, "loadImage"),
                Napi::Function::New(env, loadImage));
    exports.Set(Napi::String::New(env, "getImage"),
                Napi::Function::New(env, getImage));
    exports.Set(Napi::String::New(env, "doProcess"),
                Napi::Function::New(env, doProcess));
    return exports;
}

NODE_API_MODULE(nodecanny, Init)
