#include "threadPool.h"
#include <functional>

void CThreadPool::Task()
{
    while (m_run)
    {
        std::unique_lock<std::mutex> lck(m_wqLock);
        if (m_workqueue.size() == 0)
        {
            m_wqCond.wait(lck);
            if (!m_run)
                break;
        }
        
        std::shared_ptr<IWork> work = m_workqueue.front();
        m_workqueue.pop_front();
        lck.unlock();
        work->Process();
    }
}

CThreadPool::CThreadPool(uint16_t ui16ThreadNum):
    m_ui16ThreadNum(ui16ThreadNum),
    m_run(true)
{
    m_vecThread.reserve(2*ui16ThreadNum);
    for (size_t i = 0; i < ui16ThreadNum; i++)
    {
        auto pThread = std::make_shared<std::thread>(std::mem_fn(&CThreadPool::Task), this);
        m_vecThread.push_back(pThread);
    }
}

CThreadPool::~CThreadPool()
{
    if (m_run)
        Destroy();
}

void CThreadPool::Push(std::shared_ptr<IWork> work)
{
    std::unique_lock<std::mutex> lck(m_wqLock);
    m_workqueue.push_back(work);
    m_wqCond.notify_one();
}

void CThreadPool::Destroy()
{
    m_run = false;
    m_wqCond.notify_all();
    for (auto pThread : m_vecThread)
    {
        pThread->join();
    }
}