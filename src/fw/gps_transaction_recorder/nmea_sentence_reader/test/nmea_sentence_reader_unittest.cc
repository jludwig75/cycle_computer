#include "nmea_sentence_reader.h"

#include <gtest/gtest.h>

#include "serial_interface.h"

#include <string>
#include <assert.h>


class StringSerialInterface : public SerialInterface
{
public:
    StringSerialInterface(const std::string str) :
        _str(str),
        _current_offset(0)
    {

    }
	virtual size_t available() const
    {
        if (_current_offset > _str.length())
        {
            return 0;
        }

        return _str.length() - _current_offset;
    }
	virtual int read()
    {
        if (_current_offset >= _str.length())
        {
            return -1;
        }

        return _str.at(_current_offset++);
    }
	virtual size_t write(uint8_t c)
    {
        assert(false);
    }
private:
    std::string _str;
    size_t _current_offset;
};

const size_t max_sentence_character_length = 20;

TEST(NmeaSentenceReader, reads_empty_line)
{
    std::string test_string = "$\n";
    StringSerialInterface serial(test_string);
    char sentence_buffer[max_sentence_character_length + 1];
    NmeaSentenceReader reader(sentence_buffer, max_sentence_character_length + 1);

    ASSERT_FALSE(reader.parse_char(serial.read()));
    ASSERT_TRUE(reader.parse_char(serial.read()));

    ASSERT_TRUE(sentence_buffer == test_string);
}

TEST(NmeaSentenceReader, reads_empty_lines_separated_by_dollar)
{
    std::string test_string = "$$\n";
    StringSerialInterface serial(test_string);
    char sentence_buffer[max_sentence_character_length + 1];
    NmeaSentenceReader reader(sentence_buffer, max_sentence_character_length + 1);

    ASSERT_FALSE(reader.parse_char(serial.read()));
    ASSERT_TRUE(reader.parse_char(serial.read()));

    ASSERT_TRUE(sentence_buffer == test_string.substr(0, 1));

    ASSERT_TRUE(reader.parse_char(serial.read()));
    ASSERT_TRUE(sentence_buffer == test_string.substr(1));
}

TEST(NmeaSentenceReader, reads_non_empty_line)
{
    std::string test_string = "$arbitrary text\n";
    StringSerialInterface serial(test_string);
    char sentence_buffer[max_sentence_character_length + 1];
    NmeaSentenceReader reader(sentence_buffer, max_sentence_character_length + 1);

    while(serial.available() > 1)
    {
        ASSERT_FALSE(reader.parse_char(serial.read()));
    }
    // Last char
    ASSERT_TRUE(reader.parse_char(serial.read()));

    ASSERT_TRUE(sentence_buffer == test_string);
}

TEST(NmeaSentenceReader, reads_sentence_that_does_not_begin_with_dollar)
{
    std::string test_string = "arbitrary text\n";
    StringSerialInterface serial(test_string);
    char sentence_buffer[max_sentence_character_length + 1];
    NmeaSentenceReader reader(sentence_buffer, max_sentence_character_length + 1);

    while(serial.available() > 1)
    {
        ASSERT_FALSE(reader.parse_char(serial.read()));
    }
    // Last char
    ASSERT_TRUE(reader.parse_char(serial.read()));

    ASSERT_TRUE(sentence_buffer == test_string);
}

TEST(NmeaSentenceReader, starts_new_sentence_at_dollar_sign)
{
    std::string test_string = "1234$12\n";
    StringSerialInterface serial(test_string);
    char sentence_buffer[max_sentence_character_length + 1];
    NmeaSentenceReader reader(sentence_buffer, max_sentence_character_length + 1);

    for(size_t i = 0; i < 4; i++)
    {
        ASSERT_FALSE(reader.parse_char(serial.read()));
    }
    // $ is read on 5th character, but will start a new string
    // at $ not the next character so we will be notified about
    // the string termination when we read $, the first character
    // of the first line.
    ASSERT_TRUE(reader.parse_char(serial.read()));

    // Just a lenght of, 4, because the $ that terminated the
    // sentence parsing will be in the next sentence.
    ASSERT_TRUE(sentence_buffer == test_string.substr(0, 4));

    for(size_t i = 0; i < 2; i++)
    {
        ASSERT_FALSE(reader.parse_char(serial.read()));
    }
    // Last char for full sentence buffer
    ASSERT_TRUE(reader.parse_char(serial.read()));

    ASSERT_TRUE(sentence_buffer == test_string.substr(4));
}

TEST(NmeaSentenceReader, reads_line_that_is_too_long)
{
    std::string test_string = "$very long arbitrary text\n";
    StringSerialInterface serial(test_string);
    char sentence_buffer[max_sentence_character_length + 1];
    NmeaSentenceReader reader(sentence_buffer, max_sentence_character_length + 1);

    for(size_t i = 0; i < max_sentence_character_length - 1; i++)
    {
        ASSERT_FALSE(reader.parse_char(serial.read()));
    }
    // Last char for full sentence buffer
    ASSERT_TRUE(reader.parse_char(serial.read()));

    ASSERT_TRUE(sentence_buffer == test_string.substr(0, 20));

    while(serial.available() > 1)
    {
        ASSERT_FALSE(reader.parse_char(serial.read()));
    }
    // Last char
    ASSERT_TRUE(reader.parse_char(serial.read()));

    ASSERT_TRUE(sentence_buffer == test_string.substr(20));
}