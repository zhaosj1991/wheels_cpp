#include <iostream>
#include <unistd.h>
#include "../threadPool.h"

using namespace std;

class CHelloWork : public IWork
{
public:
    CHelloWork(int nWorkNum, int nCount) : 
    m_nWorkNum(nWorkNum),
    m_nCount(nCount)
    {}

    ~CHelloWork() {}

    void Process()
    {
        for (size_t i = 0; i < m_nCount; i++)
        {
            cout << "threadID: "<< this_thread::get_id() << " CHelloWork " << m_nWorkNum << " : "  << i << endl;
            sleep(1);
        }

        cout << "threadID: "<< this_thread::get_id() << " CHelloWork " \
            << m_nWorkNum << "  over ************************" << endl;
    }

private:
    int m_nWorkNum;
    int m_nCount;
};

int main()
{
    CThreadPool tp(10);
    for (size_t i = 0; i < 10; i++)
    {
        tp.Push(make_shared<CHelloWork>(i, 10));
        sleep(1);
        usleep(i*50000);
    }

    cout << "main over ..." << endl;
    return 0;
}
