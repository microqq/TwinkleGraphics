#ifndef TW_READERMANAGER_H
#define TW_READERMANAGER_H

#include <future>
#include <vector>

#include "twResource.h"

namespace TwinkleGraphics
{
    class IReaderManager
    {
    public:
        template <class T>
        void RemoveFutures(std::vector<std::future<ReadResult<T>>> &futures)
        {
            using Future = typename std::future<ReadResult<T>>;
            using ReadStatus = typename ReadResult<T>::Status;

            futures.erase(
                std::remove_if(futures.begin(), futures.end(), [](Future &future) {
                    bool ret = future.valid() && TaskFinished(future);
                    if (ret)
                    {
                        ReadResult<T> result = future.get();
                        ResourceReader::Ptr reader = result.GetReader();
                        if (reader != nullptr)
                        {
                            ReaderOption *option = reader->GetReaderOption();
                            if (option != nullptr)
                            {
                                ReadStatus status = result.GetStatus();
                                if (ReadStatus::SUCCESS == status)
                                {
                                    option->OnReadSuccess(result.GetSharedObject());
                                }
                                else if (ReadStatus::FAILED == status)
                                {
                                    option->OnReadFailed();
                                }
                            }
                        }
                    }
                    else //loading or wait load?
                    {
                    }

                    return ret;
                }),
                futures.end());
        }

        virtual ~IReaderManager() {}

    protected:
        IReaderManager() {}
    };
} // namespace TwinkleGraphics

#endif