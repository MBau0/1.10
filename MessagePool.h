#ifndef MESSAGE_POOL
#define MESSAGE_POOL

#include <vector>
#include <mutex>

struct Message;

class MessagePool {
public:
    MessagePool();

    void swap(std::mutex& mutex);

    std::vector<Message*>* get_previous() const;
    std::vector<Message*>* get_current() const;

    void clear_previous();
    void clear_current();
private:
    std::vector<Message*> _first_array;
    std::vector<Message*> _second_array;

    std::vector<Message*>* _previous;
    std::vector<Message*>* _current;
};

#endif