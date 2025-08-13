#include <fcntl.h> 		// shm_open flags
#include <sys/mman.h>	// mmap
#include <unistd.h>		// ftruncate, close
#include <cstring>		// memcpy
#include <string>
#include <cstddef>
#include <stdexcept>

#include "buffer.h"

/*
 * Initialize a shared memory space. Opens a new
 * virtual file descriptor and tries to mmap the
 * buffer at that location.
 *
 * Arguments:
 *    const std::string& name - The virtual file name (e.g., /orin_watch).
 *    std::size_t size - The size of the buffer.
 *    bool create - Is this buffer being created for the first time (true) or simply accessed?
 */
orin_watch::SharedBuffer::SharedBuffer(const std::string& name, std::size_t size, bool create) 
	: shm_fd(-1), mapped_mem(nullptr), mem_size(size), owner(create), shm_name(name) {
	
	/* Open the shared memory */
	int flags = create ? (O_CREAT | O_RDWR) : O_RDWR;
	shm_fd = shm_open(shm_name.c_str(), flags, 0666);
	if(shm_fd < 0) {
		throw std::runtime_error("orin_watch::SharedBuffer::SharedBuffer - Failed to open shared memory");
	}

	/* If creating, need to size the buffer */
	if(create) {
		if(ftruncate(shm_fd, mem_size) < 0) {
			close(shm_fd);
			shm_unlink(shm_name.c_str());
			throw std::runtime_error("orin_watch::SharedBuffer::SharedBuffer - Failed to set shared memory size");
		}
	}

	/* Map the shared memory */
	mapped_mem = mmap(nullptr, mem_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if(mapped_mem == MAP_FAILED) {
		close(shm_fd);
		if(owner) shm_unlink(shm_name.c_str());
		throw std::runtime_error("orin_watch::SharedBuffer::SharedBuffer - Failed to map shared memory");
	}
}

/*
 * Deinitialize a shared buffer. Unmap the buffer,
 * close the file, and if the buffer is the owner, unlink  
 * the, shared virtual memory.
 */
orin_watch::SharedBuffer::~SharedBuffer() {
	if(mapped_mem) munmap(mapped_mem, mem_size);
	if(shm_fd >= 0) close(shm_fd);
	if(owner) shm_unlink(shm_name.c_str());
}

/*
 * Returns the size of the shared buffer.
 *
 * Returns:
 *    std::size_t - The size of the buffer.
 */
std::size_t orin_watch::SharedBuffer::size() const {
	return mem_size;
}

/*
 * Read size bytes from the buffer starting at offset.
 *
 * Arguments:
 *    void *buffer - Read data into this location.
 *    std::size_t size - Read this many bytes.
 *    std::size_t offset - Read starting at this offset.
 */
void orin_watch::SharedBuffer::read(void *buffer, std::size_t size, std::size_t offset) {
	std::lock_guard<std::mutex> guard(mMutex);
	memcpy(buffer, ((char *)mapped_mem) + offset, size);
}

/*
 * Write size bytes to the buffer starting at offset.
 *
 * Arguments:
 *    void *data - Write this data to the buffer.
 *    std::size_t size - Write this many bytes.
 *    std::size_t offset - Write starting at this offset.
 */
void orin_watch::SharedBuffer::write(void *data, std::size_t size, std::size_t offset) {
	std::lock_guard<std::mutex> guard(mMutex);
	memcpy(((char *)mapped_mem) + offset, data, size);
}
