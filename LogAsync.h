//---------------------------------------------------------------------------

#ifndef LogAsyncH
#define LogAsyncH

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <utility>
#include <atomic>
#include <functional>

#include "ILog.h"

namespace PitchDet {

class LogAsync : public ILog {
public:
    using CallbackType = std::function<void(String,TColor)>;

    LogAsync( CallbackType Callback );
    LogAsync( LogAsync const & ) = delete;
    LogAsync& operator=( LogAsync const & ) = delete;
    ~LogAsync();
protected:
    virtual void DoLogMessage( String Text, TColor Color ) override;
    virtual void DoClearLog() override {}
private:
    using LogItemType = std::pair<String,TColor>;

    CallbackType callback_;
    std::atomic_bool stopped_;
    std::queue<LogItemType> queue_;
    std::mutex mutex_;
    std::condition_variable condVar_;
    std::thread thConsumer_;

    void Run();
};

} // End of namespace RTTLogger


//---------------------------------------------------------------------------
#endif
