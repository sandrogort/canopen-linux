/******************************************************************************
   @author Sandro Gort

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
******************************************************************************/

/******************************************************************************
* INCLUDES
******************************************************************************/

#include "drv_timer_cpp.h"

#include "hw.h"
#include "app.h"

#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>

/******************************************************************************
* PRIVATE VARIABLES
******************************************************************************/

class TimerCountdown {
public:
    TimerCountdown() : timerThread_(&TimerCountdown::CountdownThread, this) {

    }

    ~TimerCountdown() {
        Stop();
    }

    void Reset() {
        std::lock_guard<std::mutex> lock(mutex_);
        count_ = 0;
    }

    void Reload(uint32_t reload) {
        std::lock_guard<std::mutex> lock(mutex_);
        count_ = reload;
    }

    uint32_t GetCount() {
        std::lock_guard<std::mutex> lock(mutex_);
        return count_;
    }

    void Stop() {
        if (timerThread_.joinable()) {
            endThread_ = true;
            timerThread_.join();
        }
    }

private:
    std::atomic<bool> endThread_ = false;
    std::thread timerThread_;
    std::mutex mutex_;
    uint32_t count_;

    void CountdownThread() {
        while (!endThread_) {
            if (Countdown()) {
                COTmrService(&Node.Tmr);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    }

    bool Countdown() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (count_ > 0) {
            count_--;
            return true;
        }
        return false;
    }
};

TimerCountdown Timer;

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

static void     DrvTimerInit   (uint32_t freq);
static void     DrvTimerStart  (void);
static uint8_t  DrvTimerUpdate (void);
static uint32_t DrvTimerDelay  (void);
static void     DrvTimerReload (uint32_t reload);
static void     DrvTimerStop   (void);

/******************************************************************************
* PUBLIC VARIABLE
******************************************************************************/

const CO_IF_TIMER_DRV Linux_Cpp_TimerDriver = {
    DrvTimerInit,
    DrvTimerReload,
    DrvTimerDelay,
    DrvTimerStop,
    DrvTimerStart,
    DrvTimerUpdate
};

/******************************************************************************
* PUBLIC FUNCTIONS
******************************************************************************/

/******************************************************************************
* PRIVATE FUNCTIONS
******************************************************************************/

static void DrvTimerInit(uint32_t freq)
{
    std::cout << __func__ << std::endl;
    Timer.Reset();
}

static void DrvTimerStart(void)
{
    // std::cout << __func__ << std::endl;
}

static uint8_t DrvTimerUpdate(void)
{
    uint32_t count = Timer.GetCount();
    // std::cout << __func__ << " " << count << std::endl;
    return (count == 0) ? 1u : 0u;
}

static uint32_t DrvTimerDelay(void)
{
    return Timer.GetCount();
}

static void DrvTimerReload(uint32_t reload)
{
    // std::cout << __func__ << std::endl;

    Timer.Reload(reload);
}

static void DrvTimerStop(void)
{
    // std::cout << __func__ << std::endl;

    Timer.Reset();
}
