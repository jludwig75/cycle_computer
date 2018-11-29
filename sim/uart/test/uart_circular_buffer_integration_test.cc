#include "uart.h"

#include <gtest/gtest.h>


TEST(uart_buffer_integration_test, send_byte_to_device)
{
    UartUlator uart(4, 4);

    SerialInterface *host = uart.get_host_interface();
    SerialInterface *device = uart.get_device_interface();

    ASSERT_EQ(0, device->available());
    ASSERT_EQ(0, host->available());

    ASSERT_TRUE(host->write('j'));

    ASSERT_EQ(1, device->available());
    ASSERT_EQ(0, host->available());    // Make sure nothing is available here

    ASSERT_EQ(-1, host->read());    // Make sure nothing is available here
    ASSERT_EQ((int)'j', device->read());
}

TEST(uart_buffer_integration_test, send_max_bytes_to_device)
{
    UartUlator uart(4, 4);

    SerialInterface *host = uart.get_host_interface();
    SerialInterface *device = uart.get_device_interface();

    ASSERT_EQ(0, device->available());
    ASSERT_EQ(0, host->available());

    ASSERT_TRUE(host->write('j'));
    ASSERT_TRUE(host->write('k'));
    ASSERT_TRUE(host->write('l'));
    ASSERT_TRUE(host->write('m'));
    ASSERT_FALSE(host->write('n')); // This would overflow

    ASSERT_EQ(4, device->available());
    ASSERT_EQ(0, host->available());    // Make sure nothing is available here

    ASSERT_EQ(-1, host->read());    // Make sure nothing is available here
    ASSERT_EQ((int)'j', device->read());
    ASSERT_EQ((int)'k', device->read());
    ASSERT_EQ((int)'l', device->read());
    ASSERT_EQ((int)'m', device->read());
    ASSERT_EQ(-1, device->read());  // This should now be empty
}

TEST(uart_buffer_integration_test, write_overflow_is_dropped)
{
    UartUlator uart(4, 4);

    SerialInterface *host = uart.get_host_interface();
    SerialInterface *device = uart.get_device_interface();

    ASSERT_TRUE(host->write('j'));
    ASSERT_TRUE(host->write('k'));
    ASSERT_TRUE(host->write('l'));
    ASSERT_TRUE(host->write('m'));
    ASSERT_FALSE(host->write('n')); // This would overflow

    ASSERT_EQ((int)'j', device->read());
    ASSERT_EQ((int)'k', device->read());
    ASSERT_EQ((int)'l', device->read());
    ASSERT_EQ((int)'m', device->read());    // We get 'm' not 'n'
    ASSERT_EQ(-1, device->read());  // This should now be empty
}

TEST(uart_buffer_integration_test, send_max_bytes_to_host)
{
    UartUlator uart(4, 4);

    SerialInterface *host = uart.get_host_interface();
    SerialInterface *device = uart.get_device_interface();

    ASSERT_EQ(0, host->available());
    ASSERT_EQ(0, device->available());

    ASSERT_TRUE(device->write('j'));
    ASSERT_TRUE(device->write('k'));
    ASSERT_TRUE(device->write('l'));
    ASSERT_TRUE(device->write('m'));
    ASSERT_FALSE(device->write('n')); // This would overflow

    ASSERT_EQ(4, host->available());
    ASSERT_EQ(0, device->available());    // Make sure nothing is available here

    ASSERT_EQ(-1, device->read());    // Make sure nothing is available here
    ASSERT_EQ((int)'j', host->read());
    ASSERT_EQ((int)'k', host->read());
    ASSERT_EQ((int)'l', host->read());
    ASSERT_EQ((int)'m', host->read());
    ASSERT_EQ(-1, host->read());  // This should now be empty
}

TEST(uart_buffer_integration_test, send_max_bytes_to_host_using_array_write)
{
    UartUlator uart(4, 4);

    SerialInterface *host = uart.get_host_interface();
    SerialInterface *device = uart.get_device_interface();

    ASSERT_EQ(0, host->available());
    ASSERT_EQ(0, device->available());

    ASSERT_TRUE(device->write("jklm", 4));
    ASSERT_FALSE(device->write('n')); // This would overflow

    ASSERT_EQ(4, host->available());
    ASSERT_EQ(0, device->available());    // Make sure nothing is available here

    ASSERT_EQ(-1, device->read());    // Make sure nothing is available here
    ASSERT_EQ((int)'j', host->read());
    ASSERT_EQ((int)'k', host->read());
    ASSERT_EQ((int)'l', host->read());
    ASSERT_EQ((int)'m', host->read());
    ASSERT_EQ(-1, host->read());  // This should now be empty
}