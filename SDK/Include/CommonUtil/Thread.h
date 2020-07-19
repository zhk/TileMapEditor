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

		//创建线程对象后等待开始，必须传入一个IThreadStarter
		bool	Create(IThreadStarter* starter);

		//线程函数开始运行
		void	Start();

		//使调用线程挂起一段时间，-1会一直等待，直到线程结束
		bool	Join(int time);

		//退出线程，线程函数会运行结束
		void	Exit();

		//强行终止线程
		void	Abort();

		//得到线程对象的ID，注意创建后才有效
		threadID_t	GetID();

		//得到调用线程的ID
		static threadID_t	GetCurrentThreadID();

	protected:
		static void ThreadRun(LPVOID lpParameter);

		void		_WaitStart();

		void		_CloseHandles();

		HANDLE		_thread;
		HANDLE		_beginEvent;	//开始信号
		HANDLE		_exitEvent;		//发送需要结束信号，延迟结束
		HANDLE		_endEvent;		//结束信号
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

		//创建线程对象后等待开始，必须传入一个IThreadStarter
		bool	Create(IThreadStarter* starter);

		//线程函数开始运行
		void	Start();

		//使调用线程挂起一段时间，-1会一直等待，直到线程结束
		bool	Join(int time);

		//退出线程，线程函数会运行结束
		void	Exit();

		//强行终止线程
		void	Abort();

		//得到线程对象的ID，注意创建后才有效
		pthread_t	GetID();

		//得到调用线程的ID
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