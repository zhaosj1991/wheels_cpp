#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <deque>
#include <mutex>
#include <memory>
#include <thread>
#include <vector>
#include <condition_variable>

#define DEFAULT_THREAD_NUM  5

class IWork
{
public:
    IWork() = default;
    virtual  ~IWork() = default;
    virtual void Process() = 0;
};

// replaced by std::lock_guard & std::unique_lock
class CAutoLock
{
public:
    CAutoLock(std::mutex &lock): m_mutex(lock) { m_mutex.lock(); }
    ~CAutoLock() { m_mutex.unlock(); }

private:
    std::mutex &m_mutex;
};

class CThreadPool
{
public:
    CThreadPool(uint16_t ui16ThreadNum = DEFAULT_THREAD_NUM);
    ~CThreadPool();
    
    void Push(std::shared_ptr<IWork>);
    void Destroy();

private:
    void Task();

private:
    std::vector<std::shared_ptr<std::thread>> m_vecThread;
    std::deque<std::shared_ptr<IWork>> m_workqueue;
    std::mutex m_wqLock;
    std::condition_variable m_wqCond;
    uint16_t m_ui16ThreadNum;
    bool m_run;
};

#endif