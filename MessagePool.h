#ifndef MESSAGE_POOL
#define MESSAGE_POOL

#include <vector>
#include <mutex>

struct Message;

class MessagePool {
public:
    MessagePool();

    void swap(std::mutex& mutex);

    std::vector<std::shared_ptr<Message>>* get_previous() const;
    std::vector<std::shared_ptr<Message>>* get_current() const;

    void clear_previous();
    void clear_current();
private:
    std::vector<std::shared_ptr<Message>> _first_array;
    std::vector<std::shared_ptr<Message>> _second_array;

    std::vector<std::shared_ptr<Message>>* _previous;
    std::vector<std::shared_ptr<Message>>* _current;
};

#endif