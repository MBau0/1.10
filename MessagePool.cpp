#include "MessagePool.h"

MessagePool::MessagePool() :
    _current    ( &_first_array ),
    _previous   ( &_second_array)
{}

void MessagePool::swap(std::mutex& mutex) {
    std::lock_guard<std::mutex> guard(mutex);
    auto tmp = _previous;
    _previous = _current;
    _current = tmp;
}

std::vector<Message*>* MessagePool::get_previous() const {
    return _previous;
}

std::vector<Message*>* MessagePool::get_current() const {
    return _current;
}

void MessagePool::clear_previous() {
    _previous->clear();
}

void MessagePool::clear_current() {
    _current->clear();
}