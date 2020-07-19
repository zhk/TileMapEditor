/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			Thread.h
*	Description:
*			
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
===============================================================================*/
#pragma once 

#include "PreInclude.h"


namespace Cactus
{
	class COMMONUTIL_API IThreadStarter
	{
	public:
		virtual void Run() = 0;
	};
}

#ifdef WIN32

#	define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#	ifndef _WIN32_WINNT
#		define _WIN32_WINNT 0x403
#	endif
#	include <windows.h>

namespace Cactus
{

#	define	threadID_t	uint32

	class COMMONUTIL_API Thread : public Object
	{
	public:
		Thread();
		~Thread();

		//�����̶߳����ȴ���ʼ�����봫��һ��IThreadStarter
		bool	Create(IThreadStarter* starter);

		//�̺߳�����ʼ����
		void	Start();

		//ʹ�����̹߳���һ��ʱ�䣬-1��һֱ�ȴ���ֱ���߳̽���
		bool	Join(int time);

		//�˳��̣߳��̺߳��������н���
		void	Exit();

		//ǿ����ֹ�߳�
		void	Abort();

		//�õ��̶߳����ID��ע�ⴴ�������Ч
		threadID_t	GetID();

		//�õ������̵߳�ID
		static threadID_t	GetCurrentThreadID();

	protected:
		static void ThreadRun(LPVOID lpParameter);

		void		_WaitStart();

		void		_CloseHandles();

		HANDLE		_thread;
		HANDLE		_beginEvent;	//��ʼ�ź�
		HANDLE		_exitEvent;		//������Ҫ�����źţ��ӳٽ���
		HANDLE		_endEvent;		//�����ź�
		threadID_t	_ID;

		IThreadStarter* _starter;
	};
}

#else

#include <pthread.h>
#include <semaphore.h>

namespace Cactus
{

#	define	threadID_t	pthread_t

	class Thread : public Object
	{
	public:
		Thread();
		~Thread();

		//�����̶߳����ȴ���ʼ�����봫��һ��IThreadStarter
		bool	Create(IThreadStarter* starter);

		//�̺߳�����ʼ����
		void	Start();

		//ʹ�����̹߳���һ��ʱ�䣬-1��һֱ�ȴ���ֱ���߳̽���
		bool	Join(int time);

		//�˳��̣߳��̺߳��������н���
		void	Exit();

		//ǿ����ֹ�߳�
		void	Abort();

		//�õ��̶߳����ID��ע�ⴴ�������Ч
		pthread_t	GetID();

		//�õ������̵߳�ID
		static pthread_t	GetCurrentThreadID();

	protected:
		static void ThreadRun(void* lpParameter);

		void		_WaitStart();

		void		_CloseHandles();

		sem_t		_beginEvent;
		sem_t		_exitEvent;
		sem_t		_endEvent;

		pthread_t	_ID;

		IThreadStarter* _starter;
	};
}


#endif