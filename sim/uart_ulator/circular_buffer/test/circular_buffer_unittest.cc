#include "circular_buffer.h"

#include <gtest/gtest.h>

#include <list>
#include <time.h>


const size_t test_buffer_size = 4;


TEST(CircularBuffer, no_bytes_in_buffer_after_init)
{
	CircularBuffer buffer(test_buffer_size);

	ASSERT_EQ(0, buffer.number_of_bytes_in_buffer());
}

TEST(CircularBuffer, cannot_pop_from_empty_buffer)
{
	CircularBuffer buffer(test_buffer_size);

	char c;
	ASSERT_FALSE(buffer.pop(c));
}

TEST(CircularBuffer, one_byte_in_buffer_after_byte_pushed)
{
	CircularBuffer buffer(test_buffer_size);

	ASSERT_TRUE(buffer.push('j'));

	ASSERT_EQ(1, buffer.number_of_bytes_in_buffer());
}

TEST(CircularBuffer, can_pop_byte_pushed)
{
	CircularBuffer buffer(test_buffer_size);

	ASSERT_TRUE(buffer.push('j'));

	char c;
	ASSERT_TRUE(buffer.pop(c));
	ASSERT_EQ(c, 'j');
}

TEST(CircularBuffer, wont_push_past_full)
{
	CircularBuffer buffer(test_buffer_size);

	ASSERT_TRUE(buffer.push('j'));
	ASSERT_TRUE(buffer.push('k'));
	ASSERT_TRUE(buffer.push('l'));
	ASSERT_TRUE(buffer.push('m'));
	ASSERT_FALSE(buffer.push('n'));
}

TEST(CircularBuffer, reports_correct_number_when_full)
{
	CircularBuffer buffer(test_buffer_size);

	ASSERT_TRUE(buffer.push('j'));
	ASSERT_TRUE(buffer.push('k'));
	ASSERT_TRUE(buffer.push('l'));
	ASSERT_TRUE(buffer.push('m'));
	ASSERT_EQ(4, buffer.number_of_bytes_in_buffer());
}

TEST(CircularBuffer, reports_correct_number_when_one_less_than_full)
{
	CircularBuffer buffer(test_buffer_size);

	ASSERT_TRUE(buffer.push('j'));
	ASSERT_TRUE(buffer.push('k'));
	ASSERT_TRUE(buffer.push('l'));
	ASSERT_EQ(3, buffer.number_of_bytes_in_buffer());
}

TEST(CircularBuffer, can_fill_and_empty_queue_with_correct_values)
{
	CircularBuffer buffer(test_buffer_size);

	ASSERT_TRUE(buffer.push('j'));
	ASSERT_TRUE(buffer.push('k'));
	ASSERT_TRUE(buffer.push('l'));
	ASSERT_TRUE(buffer.push('m'));

	char c;
	ASSERT_TRUE(buffer.pop(c));
	ASSERT_EQ(c, 'j');

	ASSERT_TRUE(buffer.pop(c));
	ASSERT_EQ(c, 'k');

	ASSERT_TRUE(buffer.pop(c));
	ASSERT_EQ(c, 'l');

	ASSERT_TRUE(buffer.pop(c));
	ASSERT_EQ(c, 'm');

	ASSERT_EQ(0, buffer.number_of_bytes_in_buffer());
}

TEST(CircularBuffer, reports_correct_count_on_wrap)
{
	char c;
	CircularBuffer buffer(test_buffer_size);

	ASSERT_TRUE(buffer.push('j'));
	ASSERT_TRUE(buffer.push('k'));

	ASSERT_TRUE(buffer.pop(c));
	ASSERT_EQ(c, 'j');

	ASSERT_TRUE(buffer.pop(c));
	ASSERT_EQ(c, 'k');

	ASSERT_TRUE(buffer.push('l'));
	ASSERT_TRUE(buffer.push('m'));

	ASSERT_TRUE(buffer.push('n'));
	ASSERT_TRUE(buffer.push('o'));

	ASSERT_EQ(4, buffer.number_of_bytes_in_buffer());

	ASSERT_TRUE(buffer.pop(c));
	ASSERT_EQ(c, 'l');

	ASSERT_TRUE(buffer.pop(c));
	ASSERT_EQ(c, 'm');

	ASSERT_EQ(2, buffer.number_of_bytes_in_buffer());

	ASSERT_TRUE(buffer.pop(c));
	ASSERT_EQ(c, 'n');

	ASSERT_TRUE(buffer.pop(c));
	ASSERT_EQ(c, 'o');

	ASSERT_EQ(0, buffer.number_of_bytes_in_buffer());
}

TEST(CircularBuffer, random_push_pop)
{
	srand((unsigned)time(NULL));
	std::list<char> test_buffer;

	char c;
	CircularBuffer buffer(test_buffer_size);

	for(size_t i = 0; i < 100; i++)
	{
		unsigned operation = rand() % 2;
		switch(operation)
		{
			case 0:	// push
				c = (char)(rand() % (127 - 32)) + 32;
				if (test_buffer.size() < test_buffer_size)
				{
					ASSERT_TRUE(buffer.push(c));
					test_buffer.push_back(c);
				}
				else
				{
					ASSERT_FALSE(buffer.push(c));
				}
			break;
			case 1:	// pop
				if (test_buffer.size() == 0)
				{
					ASSERT_FALSE(buffer.pop(c));
				}
				else
				{
					ASSERT_TRUE(buffer.pop(c));
					ASSERT_EQ(test_buffer.front(), c);
					test_buffer.pop_front();
				}
			break;
		}
	}
}