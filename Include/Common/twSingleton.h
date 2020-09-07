
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
private:
    Singleton() = default;
};


template <typename T>
T &Singleton<T>::Instance()
{
    static T instance{};
    return instance;
}
} // namespace TwinkleGraphics

#endif