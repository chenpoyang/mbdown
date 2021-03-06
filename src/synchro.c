#include <time.h>
#include <pthread.h>

#include "synchro.h"

/**
 * @brief    current thread wait some time
 *
 * @param    sec, the internal the thread wait
 */
void thread_wait(const int sec)
{
    struct timespec timeout;
    pthread_mutex_t mutex;
    pthread_cond_t cond;


    /* there is no need to wait */
    if (sec <= 0)
    {
        return;
    }

    /**
     * @param    mutex: for synchronization
     * @param    cond: wait sec seconds
     */
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_mutex_lock(&mutex);

    /* wait till : current time + sec */
    timeout.tv_sec = time(NULL) + sec;
    timeout.tv_nsec = 0;
    pthread_cond_timedwait(&cond, &mutex, &timeout);

    pthread_mutex_unlock(&mutex);
    pthread_mutex_destroy(&mutex);
}

/* 初始化锁 */
void init_mutex()
{
    pthread_mutex_init(&m_protect_url_id, NULL);
    pthread_mutex_init(&m_protect_url_root_url, NULL);
    pthread_mutex_init(&m_protect_url_htable, NULL);
}

/* 销毁锁 */
void destroy_mutex()
{
    pthread_mutex_destroy(&m_protect_url_id);
    pthread_mutex_destroy(&m_protect_url_root_url);
    pthread_mutex_destroy(&m_protect_url_htable);
}

