#include "uart.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock-global/gmock-global.h>

#include "circular_buffer.h"

void CircularBuffer__CircularBuffer(CircularBuffer *instance, size_t buffer_size);
bool CircularBuffer__push(CircularBuffer *instance, char c);
bool CircularBuffer__pop(CircularBuffer *instance, char &c);
size_t CircularBuffer__number_of_bytes_in_buffer(const CircularBuffer *instance);

MOCK_GLOBAL_FUNC2(CircularBuffer__CircularBuffer, void(CircularBuffer *instance, size_t buffer_size));
MOCK_GLOBAL_FUNC2(CircularBuffer__push, bool(CircularBuffer *instance, char c));
MOCK_GLOBAL_FUNC2(CircularBuffer__pop, bool(CircularBuffer *instance, char &c));
MOCK_GLOBAL_FUNC1(CircularBuffer__number_of_bytes_in_buffer, size_t(const CircularBuffer *instance));

CircularBuffer::CircularBuffer(size_t buffer_size)
{
    CircularBuffer__CircularBuffer(this, buffer_size);
}

bool CircularBuffer::push(char c)
{
    return CircularBuffer__push(this, c);
}

bool CircularBuffer::pop(char &c)
{
    return CircularBuffer__pop(this, c);
}

size_t CircularBuffer::number_of_bytes_in_buffer() const
{
    return CircularBuffer__number_of_bytes_in_buffer(this);
}


class UartUlator_Test : public GlobalMockTest
{
    virtual void SetUp()
    {
        _rx_buffer = NULL;
        _tx_buffer = NULL;
        _rx_buffer_size = 0;
        _tx_buffer_size = 0;
	    testSetUp();
    }
    virtual void testSetUp()
    {
        ON_GLOBAL_CALL(CircularBuffer__CircularBuffer, CircularBuffer__CircularBuffer(testing::_, testing::_)).
                        WillByDefault(
                            [this](CircularBuffer *instance, size_t buffer_size)
                            {
                                assert(NULL == _tx_buffer);
                                if (!_rx_buffer)
                                {
                                    _rx_buffer = instance;
                                    _rx_buffer_size = buffer_size;
                                }
                                else if (!_tx_buffer)
                                {
                                    _tx_buffer = instance;
                                    _tx_buffer_size = buffer_size;
                                }
                                else
                                {
                                    // Can't handle more than two circular buffer instantiations per test.
                                    assert(0);
                                }
                            });
    }
    virtual void TearDown()
    {
    }
protected:
    CircularBuffer *_rx_buffer;
    CircularBuffer *_tx_buffer;
    size_t _rx_buffer_size;
    size_t _tx_buffer_size;
};

TEST_F(UartUlator_Test, instantiates_rx_and_tx_buffers)
{
    ASSERT_TRUE(NULL == _rx_buffer);
    ASSERT_TRUE(NULL == _tx_buffer);
    ASSERT_EQ(0, _rx_buffer_size);
    ASSERT_EQ(0, _tx_buffer_size);
    UartUlator uart(1024, 256);
    ASSERT_TRUE(NULL != _rx_buffer);
    ASSERT_TRUE(NULL != _tx_buffer);
    ASSERT_EQ(1024, _rx_buffer_size);
    ASSERT_EQ(256, _tx_buffer_size);
}


// Host interface
TEST_F(UartUlator_Test, host_available_calls_rx_buffer_number_of_bytes_in_buffer)
{
    UartUlator uart(1024, 1024);

    EXPECT_GLOBAL_CALL(CircularBuffer__number_of_bytes_in_buffer, CircularBuffer__number_of_bytes_in_buffer(_rx_buffer)).
                        WillOnce(testing::Return(66));

    ASSERT_EQ(66, uart.host_available());
}

TEST_F(UartUlator_Test, host_read_returns_negative_1_if_rx_buffer_pop_fails)
{
    UartUlator uart(1024, 1024);

    EXPECT_GLOBAL_CALL(CircularBuffer__pop, CircularBuffer__pop(_rx_buffer, testing::_)).
                        WillOnce(testing::Return(false));

    ASSERT_EQ(-1, uart.host_read());
}

TEST_F(UartUlator_Test, host_read_calls_pop_on_rx_buffer_if_not_empty)
{
    UartUlator uart(1024, 1024);

    EXPECT_GLOBAL_CALL(CircularBuffer__pop, CircularBuffer__pop(_rx_buffer, testing::_)).
                        WillOnce([](CircularBuffer *instance, char &c){ c = (int)'j'; return true; });

    ASSERT_EQ((int('j')), uart.host_read());
}

TEST_F(UartUlator_Test, host_write_returns_0_if_tx_buffer_push_fails)
{
    UartUlator uart(1024, 256);

    EXPECT_GLOBAL_CALL(CircularBuffer__push, CircularBuffer__push(_tx_buffer, 'k')).
                        WillOnce(testing::Return(false));

    ASSERT_EQ(0, uart.host_write('k'));
}

TEST_F(UartUlator_Test, host_write_returns_1_if_tx_buffer_push_succeeds)
{
    UartUlator uart(1024, 256);

    EXPECT_GLOBAL_CALL(CircularBuffer__push, CircularBuffer__push(_tx_buffer, 'k')).
                        WillOnce(testing::Return(true));

    ASSERT_EQ(1, uart.host_write('k'));
}


// Device interface
TEST_F(UartUlator_Test, device_available_calls_tx_buffer_number_of_bytes_in_buffer)
{
    UartUlator uart(1024, 256);

    EXPECT_GLOBAL_CALL(CircularBuffer__number_of_bytes_in_buffer, CircularBuffer__number_of_bytes_in_buffer(_tx_buffer)).
                        WillOnce(testing::Return(19));

    ASSERT_EQ(19, uart.device_available());
}

TEST_F(UartUlator_Test, device_read_returns_negative_1_if_tx_buffer_pop_fails)
{
    UartUlator uart(1024, 1024);

    EXPECT_GLOBAL_CALL(CircularBuffer__pop, CircularBuffer__pop(_tx_buffer, testing::_)).
                        WillOnce(testing::Return(false));

    ASSERT_EQ(-1, uart.device_read());
}

TEST_F(UartUlator_Test, device_read_calls_pop_on_tx_buffer_if_not_empty)
{
    UartUlator uart(1024, 1024);

    EXPECT_GLOBAL_CALL(CircularBuffer__pop, CircularBuffer__pop(_tx_buffer, testing::_)).
                        WillOnce([](CircularBuffer *instance, char &c){ c = (int)'j'; return true; });

    ASSERT_EQ((int('j')), uart.device_read());
}

TEST_F(UartUlator_Test, device_write_returns_0_if_rx_buffer_push_fails)
{
    UartUlator uart(1024, 1024);

    EXPECT_GLOBAL_CALL(CircularBuffer__push, CircularBuffer__push(_rx_buffer, 'k')).
                        WillOnce(testing::Return(false));

    ASSERT_EQ(0, uart.device_write('k'));
}

TEST_F(UartUlator_Test, device_write_returns_1_if_rx_buffer_push_succeeds)
{
    UartUlator uart(1024, 256);

    EXPECT_GLOBAL_CALL(CircularBuffer__push, CircularBuffer__push(_rx_buffer, 'k')).
                        WillOnce(testing::Return(true));

    ASSERT_EQ(1, uart.device_write('k'));
}


// Host Serial Interface
TEST_F(UartUlator_Test, can_get_host_serial_interface)
{
    UartUlator uart(1024, 256);

    ASSERT_TRUE(NULL != uart.get_host_interface());
}


// Device Serial Interface
TEST_F(UartUlator_Test, can_get_device_serial_interface)
{
    UartUlator uart(1024, 256);

    ASSERT_TRUE(NULL != uart.get_device_interface());
}
