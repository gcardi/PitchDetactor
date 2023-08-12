//---------------------------------------------------------------------------

#pragma hdrstop

#include <System.Classes.hpp>

#include "LogAsync.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

namespace PitchDet {

LogAsync::LogAsync( CallbackType Callback )
  : callback_{ Callback }
{
    Run();
}
//---------------------------------------------------------------------------

LogAsync::~LogAsync()
{
    stopped_ = true;
    condVar_.notify_all();
    thConsumer_.join();
}
//---------------------------------------------------------------------------

void LogAsync::DoLogMessage( String Text, TColor Color )
{
    std::unique_lock<std::mutex> Lock( mutex_ );
    queue_.emplace( Text, Color );
    condVar_.notify_one();
}
//---------------------------------------------------------------------------

void LogAsync::Run()
{
    thConsumer_ =
        std::thread(
            [this](){
                LogItemType Item;
                while ( !stopped_ ) {
                    {
                        std::unique_lock<std::mutex> Lock( mutex_ );
                        condVar_.wait(
                            Lock, [&](){ return !queue_.empty() || stopped_; }
                        );

                        if ( !stopped_ ) {
                            if ( !queue_.empty() ) {
                                Item = std::move( queue_.front() );
                                queue_.pop();
                            }
                        }
                        else {
                            break;
                        }
                    }
                    // Manda elemento alla GUI
                    TThread::Synchronize(
                        (TThread*)nullptr,
                        [this,&Item](){
                            callback_( Item.first, Item.second );
                        }
                    );
                }
                {
                    std::unique_lock<std::mutex> Lock( mutex_ );
                    while ( !queue_.empty() ) {
                        Item = std::move( queue_.front() );
                        queue_.pop();
                        // Manda elemento alla GUI
                        TThread::Synchronize(
                            (TThread*)nullptr,
                            [this,&Item](){
                                callback_( Item.first, Item.second );
                            }
                        );
                    }
                }
            }
        );
}

} // End of namespace RTTLogger
