#include "gps_ulator.h"

#include "serial_interface.h"
#include "mcu_time.h"
#include "interrupt_ulator/interrupt_ulator.h"

#include <pthread.h>


GpsUlator::GpsUlator(SerialInterface *device_serial_interface) :
    _device_serial_interface(device_serial_interface)
{
    _transactions.push_back(GpsTransaction( 500 * 1000, GpsTransaction::NMEA_sentence, "$test sentence 1\n"));
    _transactions.push_back(GpsTransaction(1000 * 1000, GpsTransaction::PPS_pulse));
    _transactions.push_back(GpsTransaction(1200 * 1000, GpsTransaction::NMEA_sentence, "$test sentence 2\n"));
    _transactions.push_back(GpsTransaction(2000 * 1000, GpsTransaction::PPS_pulse));
    _transactions.push_back(GpsTransaction(2300 * 1000, GpsTransaction::NMEA_sentence, "$test sentence 3\n"));
    _transactions.push_back(GpsTransaction(3000 * 1000, GpsTransaction::PPS_pulse));
    _transactions.push_back(GpsTransaction(3100 * 1000, GpsTransaction::NMEA_sentence, "$test sentence 4\n"));
}

bool GpsUlator::start()
{
    _exit_thread = false;
    int ret = pthread_create(&_thread_id, NULL, gps_thread, this);
    return ret == 0;
}

void GpsUlator::stop()
{
    _exit_thread = true;
    pthread_join(_thread_id, NULL);
}

static void send_string(SerialInterface * serial, const std::string &str)
{
    /// @todo The correct behavior is probably to not prevent buffer overflows
    /// and just drop data that can't be written to the UART. We'll do this
    /// for now to make it easier until the app is running.
    size_t bytes_to_send = str.length();
    size_t total_bytes_sent = 0;
    while (bytes_to_send > 0)
    {
        size_t bytes_sent = serial->write(str.substr(total_bytes_sent).c_str(), bytes_to_send);
        total_bytes_sent += bytes_sent;
        bytes_to_send -= bytes_sent;
    }
}

void GpsUlator::run()
{
    timespec tv;
    tv.tv_sec = 0;
    tv.tv_nsec = 1000000;   // 1 ms
    while(!_exit_thread)
    {
        if (!_transactions.empty())
        {
            if (mcu_microseconds() >= _transactions.front().transaction_time_us())
            {
                GpsTransaction transaction = _transactions.front();
                _transactions.pop_front();

                if (transaction.transaction_type() == GpsTransaction::NMEA_sentence)
                {
                    send_string(_device_serial_interface, transaction.get_nmea_sentence());
                }
                else if (transaction.transaction_type() == GpsTransaction::PPS_pulse)
                {
                    InterruptUlator.trigger_interrupt(25, InterruptMode_Rising);
                }
            }
        }
        nanosleep(&tv, NULL);
    }
}

void *GpsUlator::gps_thread(void *instance)
{
    GpsUlator *gps = (GpsUlator *)instance;
    gps->run();

    return NULL;
}
