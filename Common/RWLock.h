#pragma once
#ifndef __COMMON_RW_LOCK_H__
#define __COMMON_RW_LOCK_H__

#include <type_traits>
#include <cassert>
#include "SpinLock.h"

class RWLock {
	enum LockType : size_t{ NO_LOCK, READ_LOCK, WRITE_LOCK };
public:
	RWLock() : reader_count_(0), lock_type_(0) {}

	void AcquireReadLock() {
		while (1) {
			while (lock_type_ == WRITE_LOCK);
			spinlock_.Lock();
			if (lock_type_ == WRITE_LOCK) {
				spinlock_.Unlock();
			}
			else {
				if (lock_type_ == NO_LOCK) {
					lock_type_ = READ_LOCK;
					reader_count_ = reader_count_ + 1;
				}
				else {
					assert(lock_type_ == READ_LOCK);
					reader_count_ = reader_count_ + 1;
				}
				spinlock_.Unlock();
				return;
			}
		}
	}

	bool TryReadLock() {
		bool rt = false;
		spinlock_.Lock();
		if (lock_type_ == NO_LOCK) {
			lock_type_ = READ_LOCK;
			reader_count_ = reader_count_ + 1;
			rt = true;
		}
		else if (lock_type_ == READ_LOCK) {
			reader_count_ = reader_count_ + 1;
			rt = true;
		}
		else {
			rt = false;
		}
		spinlock_.Unlock();
		return rt;
	}

	void AcquireWriteLock() {
		while (1) {
			while (lock_type_ != NO_LOCK);
			spinlock_.Lock();
			if (lock_type_ != NO_LOCK) {
				spinlock_.Unlock();
			}
			else {
				assert(lock_type_ == NO_LOCK);
				lock_type_ = WRITE_LOCK;
				spinlock_.Unlock();
				return;
			}
		}
	}

	bool TryWriteLock() {
		bool rt = false;
		spinlock_.Lock();
		if (lock_type_ == NO_LOCK) {
			lock_type_ = WRITE_LOCK;
			rt = true;
		}
		else {
			rt = false;
		}
		spinlock_.Unlock();
		return rt;
	}

	void ReleaseReadLock() {
		spinlock_.Lock();
		reader_count_ = reader_count_ - 1;
		if (reader_count_ == 0) {
			lock_type_ = NO_LOCK;
		}
		spinlock_.Unlock();
	}

	void ReleaseWriteLock() {
		spinlock_.Lock();
		lock_type_ = NO_LOCK;
		spinlock_.Unlock();
	}

	bool ExistsWriteLock() const {
		return (lock_type_ == WRITE_LOCK);
	}

	SpinLock spinlock_;
	volatile size_t reader_count_;
	volatile size_t lock_type_;
};
static_assert(std::is_trivial_v<RWLock> == false);
static_assert(std::is_standard_layout_v<RWLock> == true);

#endif
