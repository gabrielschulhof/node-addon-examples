#include <napi.h>

static Napi::Value CreateClosure(const Napi::CallbackInfo& info) {
  // Create a reference on the heap to the object we wish to use in the inner
  // function.
  Napi::ObjectReference* ref = new Napi::ObjectReference;
  *ref = Napi::Persistent(info[0].As<Napi::Object>());

  // Create the inner function which will have access to the above-created
  // reference.
  Napi::Function fn =
    Napi::Function::New(info.Env(), [](const Napi::CallbackInfo& info) {
      // The function receives the reference created in the outside scope as
      // its `info.Data()`.
      Napi::ObjectReference* ref =
        static_cast<Napi::ObjectReference*>(info.Data());

      // We may now use `ref`.
      ref->Set("closureWasCalled", true);
    }, "closure", ref);

  // Delete `ref` when we know that the inner function won't be using it
  // anymore, because `fn` is itself being garbage-collected.
  fn.AddFinalizer([](Napi::Env env, Napi::ObjectReference* ref) {
    delete ref;
  }, ref);

  return fn;
}

static Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports["createClosure"] = Napi::Function::New(env, CreateClosure);
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
