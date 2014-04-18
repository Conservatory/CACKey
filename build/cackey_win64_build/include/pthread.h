/* Thread_emulation.h */
/* Author: Johnson M. Hart */
/* Emulate the Pthreads model for the Win32 platform */
/* The emulation is not complete, but it does provide a subset */
/* required for a first project */
/* Source: http://world.std.com/~jmhart/opensource.htm */
/* The emulation is not complete, but it does provide a subset */
/* that will work with many well-behaved programs */
/* IF YOU ARE REALLY SERIOUS ABOUT THIS, USE THE OPEN SOURCE */
/* PTHREAD LIBRARY. YOU'LL FIND IT ON THE RED HAT SITE */

#ifndef _THREAD_EMULATION
#  define _THREAD_EMULATION

/* Thread management macros */
#  ifdef _WIN32
#    define _WIN32_WINNT 0x500 /* WINBASE.H - Enable SignalObjectAndWait */
#    include <process.h>
#    include <windows.h>
#    define THREAD_FUNCTION_PROTO THREAD_FUNCTION_RETURN (__stdcall *) (void *)
#    define THREAD_FUNCTION_RETURN unsigned int
#    define THREAD_SPECIFIC_INDEX DWORD
#    define pthread_t HANDLE
#    define pthread_attr_t DWORD
#    define pthread_create(thhandle, attr, thfunc, tharg) ((int) ((*thhandle = (HANDLE) _beginthreadex(NULL, 0, (THREAD_FUNCTION_PROTO) thfunc, tharg, 0, NULL)) == NULL))
#    define pthread_join(thread, result) ((WaitForSingleObject((thread), INFINITE) != WAIT_OBJECT_0) || !CloseHandle(thread))
#    define pthread_detach(thread) { if (((void *) thread) != NULL) { CloseHandle((void *) thread); }}
#    define thread_sleep(nms) Sleep(nms)
#    define pthread_cancel(thread) TerminateThread(thread, 0)
#    define ts_key_create(ts_key, destructor) {ts_key = TlsAlloc();}
#    define pthread_getspecific(ts_key) TlsGetValue(ts_key)
#    define pthread_setspecific(ts_key, value) TlsSetValue(ts_key, (void *)value)
#    define pthread_self() GetCurrentThreadId()
#  else
#    include <pthread.h>
#    define THREAD_FUNCTION_RETURN void *
#  endif

/* Syncrhronization macros: Win32->pthread */
#  ifdef _WIN32
#    define pthread_mutex_t HANDLE
#    define pthread_cond_t HANDLE
#    define pthread_mutex_lock(pobject) WaitForSingleObject(*pobject, INFINITE)
#    define pthread_mutex_unlock(pobject) (!ReleaseMutex(*pobject))
#    define pthread_mutex_init(pobject,pattr) ((*pobject=CreateMutex(NULL, FALSE, NULL)) == NULL)
#    define pthread_cond_init(pobject,pattr) (*pobject=CreateEvent(NULL, FALSE, FALSE, NULL))
#    define pthread_mutex_destroy(pobject) CloseHandle(*pobject)
#    define pthread_cond_destroy(pobject) CloseHandle(*pobject)
#    define pthread_cond_wait(pcv,pmutex) { SignalObjectAndWait(*pmutex, *pcv, INFINITE, FALSE); WaitForSingleObject(*pmutex, INFINITE); }
#    define pthread_cond_signal(pcv) SetEvent(*pcv)
#  endif

#endif
