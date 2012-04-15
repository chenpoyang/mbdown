#ifndef _SYNCHRO_H_
#define _SYNCHRO_H_

#include <pthread.h>
#include <stdio.h>

pthread_mutex_t m_protect_url_id;
pthread_mutex_t m_protect_url_root_url;
pthread_mutex_t m_protect_url_htable;

#define M_LOCK_ID		\
	pthread_mutex_lock(&m_protect_url_id);			\

#define M_UNLOCK_ID	\
	pthread_mutex_unlock(&m_protect_url_id);		\

#define M_LOCK_ROOTURL		\
	pthread_mutex_lock(&m_protect_url_root_url);			\

#define M_UNLOCK_ROOTURL	\
	pthread_mutex_unlock(&m_protect_url_root_url);		\

#define M_LOCK_HTABLE		\
	pthread_mutex_lock(&m_protect_url_htable);			\

#define M_UNLOCK_HTABLE	\
	pthread_mutex_unlock(&m_protect_url_htable);		\

/**
 * @brief	current thread wait some time
 *
 * @param	sec, the internal the thread wait
 */
void thread_wait(const int sec);

/* 初始化锁 */
void init_mutex();

/* 销毁锁 */
void destroy_mutex();

#endif // _SYNCHRO_H_
