#include "gps_ulator.h"

#include "serial_interface.h"
#include "mcu_time.h"

#include <pthread.h>


GpsUlator::GpsUlator(SerialInterface *device_serial_interface)
{
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
            if (micros() >= _transactions.front().transaction_time_us())
            {
                GpsTransaction transaction = _transactions.front();
                _transactions.pop_front();

                if (transaction.transaction_type() == GpsTransaction::NMEA_sentence)
                {
                    send_string(_device_serial_interface, transaction.get_nmea_sentence());
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
