#ifndef _SYNCHRO_H_
#define _SYNCHRO_H_

#include <pthread.h>
#include <stdio.h>

pthread_mutex_t m_protect;

#define M_LOCK		\
	pthread_mutex_init(&m_protect, NULL);	\
	pthread_mutex_lock(&m_protect);			\

#define M_UNLOCK	\
	pthread_mutex_unlock(&m_protect);		\
	pthread_mutex_destroy(&m_protect);		\

/**
 * @brief	current thread wait some time
 *
 * @param	sec, the internal the thread wait
 */
void thread_wait(const int sec);

#endif // _SYNCHRO_H_
