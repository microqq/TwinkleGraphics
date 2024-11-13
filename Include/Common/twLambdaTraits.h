#ifndef TW_LAMBDA_H
#define TW_LAMBDA_H

/*
https://stackoverflow.com/questions/28746744/passing-capturing-lambda-as-function-pointer
https://stackoverflow.com/questions/7852101/c-lambda-with-captures-as-a-function-pointer


Using locally global (static) method it can be done as followed

template <class F>
auto cify_no_args(F&& f) {
  static F fn = std::forward<F>(f);
  return [] {
    return fn();
  };
}
Suppose we have

void some_c_func(void (*callback)());
So the usage will be

some_c_func(cify_no_args([&] {
  // code
}));
This works because each lambda has an unique signature so making it static is
not a problem. Following is a generic wrapper with variadic number of arguments
and any return type using the same method.

template <class F>
struct lambda_traits : lambda_traits<decltype(&F::operator())>
{ };

template <typename F, typename R, typename... Args>
struct lambda_traits<R(F::*)(Args...)> : lambda_traits<R(F::*)(Args...) const>
{ };

template <class F, class R, class... Args>
struct lambda_traits<R(F::*)(Args...) const> {
    using pointer = typename std::add_pointer<R(Args...)>::type;

    static pointer cify(F&& f) {
        static F fn = std::forward<F>(f);
        return [](Args... args) {
            return fn(std::forward<Args>(args)...);
        };
    }
};

template <class F>
inline lambda_traits<F>::pointer cify(F&& f) {
    return lambda_traits<F>::cify(std::forward<F>(f));
}
And similar usage

void some_c_func(int (*callback)(some_struct*, float));

some_c_func(cify([&](some_struct* s, float f) {
    // making use of "s" and "f"
    return 0;
}));

*/

namespace TwinkleGraphics {
template <class F>
struct lambda_traits : lambda_traits<decltype(&F::operator())> {};

template <typename F, typename R, typename... Args>
struct lambda_traits<R (F::*)(Args...)>
    : lambda_traits<R (F::*)(Args...) const> {};

template <class F, class R, class... Args>
struct lambda_traits<R (F::*)(Args...) const> {
  using pointer = typename std::add_pointer<R(Args...)>::type;

  static pointer cify(F &&f) {
    static F fn = std::forward<F>(f);
    return [](Args... args) { return fn(std::forward<Args>(args)...); };
  }
};

template <class F> inline typename lambda_traits<F>::pointer cify(F &&f) {
  return lambda_traits<F>::cify(std::forward<F>(f));
}
} // namespace TwinkleGraphics

#endif