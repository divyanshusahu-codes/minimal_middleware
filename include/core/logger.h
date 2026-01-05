// #include <atomic>
// #include <iostream>

// namespace core {

// class AsyncLogger {
// public:
//     // AsyncLogger

//     //we need to keep one logger for the entire middleware and have it be passed around somehow.
//     void try_log(const std::string & string_buffer)
//     {
//       //lets first see where the head and tail are
//       // size_t curr_head = head_.load(std::)
//     }

    



// private:
//   //make the logger non copyable and non movable
//   AsyncLogger(const AsyncLogger&) = delete;
//   AsyncLogger& operator=(cosnt AsyncLogger&) = delete;
//   AsyncLogger(AsyncLogger&&) = delete;



//   //lets make a static buffer so it does not get dynamically allocated during runtime
//   static constexpr size_t BUFFER_SIZE = 256;

//   //slot is a struct and will have a flag to notify if its available for read?
//   struct Slot{
//     char message[128];
//     std::atomic<bool> ready{false};
//   };

//   //we make BUFFER_SIZE slots which we are going to use a ring buffer 
//   static Slot slot_[BUFFER_SIZE];

//   alignas(64) std::atomic<size_t> head_{0};
//   alignas(64) std::atomic<size_t> tail_{0};


// }
// } // namespace core
