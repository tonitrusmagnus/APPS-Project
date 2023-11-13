#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <queue>
#include <math.h>

class int_queue
{
private:
    std::queue<int> queue_;
    int length_ = 5;

public:
    int_queue(int length);
    // ~int_queue();

    void setLength(int length);
    void add(int value);
    int latest();
    int oldest();
    float average();
    int length();
    int lowest();
    int highest();
};

#endif