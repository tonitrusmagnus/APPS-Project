#include "queue.hpp"

IntQueue::IntQueue(int length)
{
    setLength(length);
}

void IntQueue::setLength(int length)
{
    length_ = length;
}

void IntQueue::add(int value)
{
    if (queue_.size() >= length_)
    {
        queue_.pop();
    }
    queue_.push(value);
}

int IntQueue::length()
{
    return queue_.size();
}

int IntQueue::latest()
{
    if (queue_.size() > 0)
    {
        return queue_.back();
    }
    return 0;
}

int IntQueue::oldest()
{
    return queue_.front();
}

int IntQueue::lowest()
{
    int min = 1000;
    if (queue_.size() > 0)
    {
        std::queue<int> temp_queue = queue_; // Create a temporary queue to preserve the original
        while (!temp_queue.empty())
        {
            if (min > temp_queue.front())
            {
                min = temp_queue.front();
            }
            temp_queue.pop();
        }
    }
    else
    {
        return -1;
    }
    return min;
}

int IntQueue::highest()
{
    int max = 0;
    if ((queue_.size()) > 0)
    {
        std::queue<int> temp_queue = queue_; // Create a temporary queue to preserve the original
        while (!temp_queue.empty())
        {
            if (max < temp_queue.front())
            {
                max = temp_queue.front();
            }
            temp_queue.pop();
        }
    }
    else
    {
        return -1;
    }
    return max;
}

float IntQueue::average()
{
    int sum = 0.0;
    int num_elements = queue_.size();
    int average = 0;

    if (num_elements > 0)
    {

        // Iterate through the queue and sum the values
        std::queue<int> temp_queue = queue_; // Create a temporary queue to preserve the original
        while (!temp_queue.empty())
        {
            sum += temp_queue.front();
            temp_queue.pop();
        }

        // Calculate the average
        average = round(sum / num_elements);
    }

    return average;
}

void IntQueue::clear() {
    while(queue_.size() > 0){
        queue_.pop();
    }
}