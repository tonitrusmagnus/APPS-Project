/**
 * @file queue.hpp
 * @brief Definition of the IntQueue class, a queue implementation for integers with additional functionalities.
 */

#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <queue>
#include <cmath>

/**
 * @class IntQueue
 * @brief A queue implementation for integers with additional functionalities.
 */
class IntQueue
{
private:
    std::queue<int> queue_; /**< Standard queue to store integer values. */
    int length_; /**< Maximum length of the queue. */

public:
    /**
     * @brief Constructor for the IntQueue class.
     * @param length The maximum length of the queue.
     */
    IntQueue(int length);

    /**
     * @brief Sets the maximum length of the queue.
     * @param length The new maximum length of the queue.
     */
    void setLength(int length);

    /**
     * @brief Adds a value to the queue.
     * @param value The integer value to be added to the queue.
     */
    void add(int value);

    /**
     * @brief Clears all elements from the queue.
     */
    void clear();

    /**
     * @brief Returns the latest value added to the queue.
     * @return The latest integer value.
     */
    int latest();

    /**
     * @brief Returns the oldest value in the queue.
     * @return The oldest integer value.
     */
    int oldest();

    /**
     * @brief Calculates and returns the average value of the queue.
     * @return The average value as a floating-point number.
     */
    float average();

    /**
     * @brief Returns the current length of the queue.
     * @return The current length of the queue.
     */
    int length();

    /**
     * @brief Returns the lowest value in the queue.
     * @return The lowest integer value.
     */
    int lowest();

    /**
     * @brief Returns the highest value in the queue.
     * @return The highest integer value.
     */
    int highest();
};

#endif // QUEUE_HPP
