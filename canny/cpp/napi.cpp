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

static Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "hello"),
                Napi::Function::New(env, hello));
    exports.Set(Napi::String::New(env, "loadImage"),
                Napi::Function::New(env, loadImage));
    return exports;
}

NODE_API_MODULE(nodecanny, Init)
