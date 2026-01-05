#ifndef UTILS_HPP
#define UTILS_HPP
#include <iostream>
#include <thread>
#include <atomic>
#include <array>
// #include <fstrea>


//we make SPSC and MPSC classes here. These are wait free. 

namespace core{
    namespace utils{

        template <typename T, size_t Capacity>
        class LockFreeSPSC{
            private:
            static constexpr size_t CACHE_LINE_SIZE = 64;
            //need array for explicit declaration of size of the ring buffer
            std::array<T, Capacity> queue_;
            //need a pointer for the head and tail and cache align them
            alignas(CACHE_LINE_SIZE) std::atomic<size_t> head_{0};
            alignas(CACHE_LINE_SIZE) std::atomic<size_t> tail_{0};
            //cache tail so we dont Poll everytime
            alignas(CACHE_LINE_SIZE) size_t cached_tail_{0};
            alignas(CACHE_LINE_SIZE) size_t cached_head_{0};

            public:

            //add a method do we can push
            bool push_data(const T&data)
            {
                //head is owned by the producer so its fine to have memory order relaxed
                const size_t curr_head = head_.load(std::memory_order_relaxed);
                const size_t next_head = (curr_head+1) % Capacity;

                //check if queue is full
                if(next_head == cached_tail_)
                {
                    //check with the original value and we need to acquire because its owned by the consumer
                    cached_tail_ = tail_.load(std::memory_order_acquire);
                    if(next_head == cached_tail_)
                    {
                        //buffer is full. DROP!
                        return false;
                    }
                }

                //write data
                queue_[curr_head] = data;

                //update the atomic
                head_.store(next_head, std::memory_order_release);
                return true;
            }

            //add method to get data
            bool get_data(T &data)
            {
                //get the read index
                const size_t curr_tail = tail_.load(std::memory_order_relaxed);

                //compare it to the cached head
                if(curr_tail == cached_head_)
                {
                    //check against the original head
                    cached_head_ = head_.load(std::memory_order_acquire);
                    if(curr_tail == cached_head_)
                    {
                        //buffer is empty. DROP!
                        return false;
                    }
                }

                //read the data
                data = queue_[curr_tail];
                const size_t next_tail = (curr_tail+1)%Capacity;
                //update atomic
                tail_.store(next_tail, std::memory_order_release);
                return true;
            }
        };
    }

}
#endif