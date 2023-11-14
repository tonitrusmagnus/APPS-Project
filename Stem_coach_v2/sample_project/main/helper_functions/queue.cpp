#include "queue.hpp"

int_queue::int_queue(int length)
{
    setLength(length);
}

void int_queue::setLength(int length)
{
    length_ = length;
}

void int_queue::add(int value)
{
    if (queue_.size() >= length_)
    {
        queue_.pop();
    }
    queue_.push(value);
}
int int_queue::length()
{
    return queue_.size();
}

int int_queue::latest()
{
    return queue_.back();
}

int int_queue::oldest()
{
    return queue_.front();
}
int int_queue::lowest()
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
int int_queue::highest()
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

float int_queue::average()
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

        // Calculate the mean
        average = round(sum / num_elements);
    }

    return average;
}

void int_queue::clear() {
    while(queue_.size() > 0){
        queue_.pop();
    }
}