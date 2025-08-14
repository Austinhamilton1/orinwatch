#pragma once
#include <string>
#include <cstddef>
#include <mutex>

namespace orin_watch {
	class SharedBuffer {
	private:
		int shm_fd;				// File descriptor for shared memory
		void *mapped_mem;		// Raw pointer to shared memory
		std::size_t mem_size;	// Size of the memory region
		bool owner;				// Does this buffer own the memory region
		std::string shm_name;	// Name of the shared memory region
		std::mutex mMutex;		// Synchronization of reads/writes
	public:
		/* Constructor/Destructor */
		SharedBuffer(const std::string& name, std::size_t size, bool create);
		~SharedBuffer();

		/* Size of buffer */
		std::size_t size() const;

		/* Read/write to and from buffer */
		void read(void *buffer, std::size_t size, std::size_t offset);
		void write(const void *data, std::size_t size, std::size_t offset);

		/* Determine if the buffer has new data */
		bool poll();
	};
};
