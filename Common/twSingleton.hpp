
#ifndef TW_SINGLETON_H
#define TW_SINGLETON_H

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
    // static T* getInstance()
    // {
    //     static bool init = []() -> bool {
    //         _instance = new T;
    //         return true;
    //     }();

    //     return _instance;
    // }

    Singleton(const Singleton<T> &copy) = delete;
    Singleton(Singleton<T> &&) = delete;
    Singleton<T> &operator=(const Singleton<T> &copy) = delete;
    Singleton<T> &operator=(Singleton<T> &&) = delete;

    T *operator->() { return _instance; }
    const T *operator->() const { return _instance; }
    T &operator*() { return *_instance; }
    const T &operator*() const { return *_instance; }

    //protected:
    Singleton()
    {
        static bool init = []() -> bool {
            _instance = new T;
            return true;
        }();
    }

private:
    static T *_instance;
};

template <typename T>
T *Singleton<T>::_instance = nullptr;
} // namespace TwinkleGraphics

#endif