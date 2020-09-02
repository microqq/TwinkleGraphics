
#ifndef TW_SINGLETON_H
#define TW_SINGLETON_H

#include <memory>
#include <mutex>


namespace TwinkleGraphics
{
/**
 * @brief 
 * 
 * @tparam T 
 */
template <typename T>
class Singleton
{
public:
    Singleton(const Singleton<T> &copy) = delete;
    Singleton(Singleton<T> &&) = delete;
    Singleton<T> &operator=(const Singleton<T> &copy) = delete;
    Singleton<T> &operator=(Singleton<T> &&) = delete;

    static T& Instance();
    // static T& GetInstance();

private:
    Singleton() = default;

// private:
//     static std::unique_ptr<T> _instance;
//     static std::once_flag _once;
};

// template <typename T>
// std::unique_ptr<T> Singleton<T>::_instance = nullptr;

// template <typename T>
// std::once_flag Singleton<T>::_once;

template <typename T>
T &Singleton<T>::Instance()
{
    static T instance{};
    return instance;
}

// template <typename T>
// T &Singleton<T>::GetInstance()
// {
//     std::call_once(_once, []() {
//         _instance.reset(new T);
//     });
//     return *_instance.get();
// }

} // namespace TwinkleGraphics

#endif