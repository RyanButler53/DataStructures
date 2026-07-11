#include <gtest/gtest.h>
#include <sstream>
#include <stdexcept>
#include "quack/quack.hpp"

class QuackTest : public ::testing::Test {
protected:
    Quack<int> q_;
};

// 1. Test baseline initialization states
TEST_F(QuackTest, InitialState) {
    EXPECT_TRUE(q_.empty());
    EXPECT_EQ(q_.size(), 0);
}

// 2. Test standard stack-like pushes and pops (LIFO)
TEST_F(QuackTest, PushAndPopLIFO) {
    q_.push(10);
    q_.push(20);
    q_.push(30);

    EXPECT_EQ(q_.size(), 3);
    EXPECT_FALSE(q_.empty());
    EXPECT_EQ(q_.front(), 10);
    EXPECT_EQ(q_.back(), 30);

    // Pop removes from the back (stack behavior)
    q_.pop();
    EXPECT_EQ(q_.back(), 20);
    EXPECT_EQ(q_.size(), 2);

    q_.pop();
    EXPECT_EQ(q_.back(), 10);

    q_.pop();
    EXPECT_TRUE(q_.empty());
}

// 3. Test standard queue-like operations (FIFO)
TEST_F(QuackTest, PushAndDequeueFIFO) {
    q_.push(1);
    q_.push(2);
    q_.push(3);

    // Dequeue removes from the front (queue behavior)
    q_.dequeue();
    EXPECT_EQ(q_.front(), 2);
    EXPECT_EQ(q_.size(), 2);

    q_.dequeue();
    EXPECT_EQ(q_.front(), 3);

    q_.dequeue();
    EXPECT_TRUE(q_.empty());
}

// 4. Test front-loading operations
TEST_F(QuackTest, PushFrontOperations) {
    q_.push_front(100);
    q_.push_front(200);
    
    EXPECT_EQ(q_.front(), 200);
    EXPECT_EQ(q_.back(), 100);
    EXPECT_EQ(q_.size(), 2);
}

// 5. Test stream and string serialization overrides
TEST_F(QuackTest, Serialization) {
    q_.push(5);
    q_.push(10);
    q_.push(15);

    // Test operator<< stream extraction override
    std::stringstream ss;
    ss << q_;
    EXPECT_EQ(ss.str(), q_.to_string());
}

// 6. Test boundary conditions and internal buffer reshuffling triggers
TEST_F(QuackTest, InterleavedReshuffleStress) {
    // Interleaving queue and stack steps forcing data to transfer between arrays
    for (int i = 0; i < 5; ++i) {
        q_.push(i);
    }
    
    q_.dequeue(); // Consumes front element, potentially triggering reshuffle
    EXPECT_EQ(q_.front(), 1);
    
    q_.push_front(99);
    EXPECT_EQ(q_.front(), 99);
    EXPECT_EQ(q_.back(), 4);
    
    q_.pop(); // Consumes back element
    EXPECT_EQ(q_.back(), 3);
}

// 7. Test exception protections for empty configurations
TEST_F(QuackTest, EmptyAccessExceptions) {
    // Asserting that accessing elements or mutating empty bounds raises a valid error
    // (Adjust the logic_error/runtime_error specs to match your implementation targets)
    EXPECT_THROW(q_.front(), std::logic_error);
    EXPECT_THROW(q_.back(), std::logic_error);
    EXPECT_THROW(q_.pop(), std::logic_error);
    EXPECT_THROW(q_.dequeue(), std::logic_error);
}
