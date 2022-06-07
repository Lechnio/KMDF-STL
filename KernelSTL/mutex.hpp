#pragma once

#include "common.hpp"
#include <fltKernel.h>

namespace tiny {
	class mutex {
	public:
		mutex& operator=(const mutex&) = delete;
		mutex(const mutex&) = delete;

		inline mutex() {
			KeInitializeGuardedMutex(&_nativeHandle);
		}

		inline void lock() {
			KeAcquireGuardedMutex(&_nativeHandle);
		}

		inline bool try_lock() {
			KeTryToAcquireGuardedMutex(&_nativeHandle);
		}

		inline void unlock() {
			KeReleaseGuardedMutex(&_nativeHandle);
		}
	private:
		KGUARDED_MUTEX _nativeHandle;
	};

	class shared_mutex {
	public:
		shared_mutex& operator=(const shared_mutex&) = delete;
		shared_mutex(const shared_mutex&) = delete;

		inline shared_mutex() {
			FltInitializePushLock(&_nativeHandle);
		}

		inline ~shared_mutex() {
			FltDeletePushLock(&_nativeHandle);
		}

		inline void lock() {
			FltAcquirePushLockExclusiveEx(&_nativeHandle, 0);
		}

		inline void lock_shared() {
			FltAcquirePushLockSharedEx(&_nativeHandle, 0);
		}

		inline bool try_lock() {
			lock();
			return true;
		}

		inline bool try_lock_shared() {
			lock_shared();
			return false;
		}

		inline void unlock() {
			FltReleasePushLockEx(&_nativeHandle, 0);
		}

		inline void unlock_shared() {
			FltReleasePushLockEx(&_nativeHandle, 0);
		}
	private:
		EX_PUSH_LOCK _nativeHandle;
	};

	template <typename T>
	class scoped_lock {
	public:
		scoped_lock& operator=(const scoped_lock&) = delete;
		scoped_lock(const scoped_lock&) = delete;

		inline explicit scoped_lock(T& obj) : _obj(obj) {
			_obj.lock();
		}

		inline ~scoped_lock() {
			_obj.unlock();
		}

	private:
		T& _obj;
	};
}
