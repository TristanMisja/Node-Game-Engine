#ifndef _H_UTIL_
#define _H_UTIL_

#include <string>
#include <iostream>
#include <time.h>
#include <math.h>
#include <chrono>
#include <thread>
#include <glad/glad.h>

#ifdef _WIN32
#include <Windows.h>
void sleep(float milliseconds) { Sleep(milliseconds); }
#else
#include <unistd.h>
void sleep(float milliseconds) { usleep(milliseconds / 1000); }
#endif

// The issue with this is that while it is a lot more precise 
// it uses 10x more CPU than with the regular sleep(). It's still better than delay()
// though, so I will try using this instead of delay() when possible.
void preciseSleep(double seconds) {

    static double estimate = 5e-3;
    static double mean = 5e-3;
    static double m2 = 0;
    static int64_t count = 1;

    while (seconds > estimate) {
        auto start = std::chrono::high_resolution_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        auto end = std::chrono::high_resolution_clock::now();

        double observed = (end - start).count() / 1e9;
        seconds -= observed;

        ++count;
        double delta = observed - mean;
        mean += delta / count;
        m2 += delta * (observed - mean);
        double stddev = sqrt(m2 / (count - 1));
        estimate = mean + stddev;
    }

    // spin lock
    auto start = std::chrono::high_resolution_clock::now();
    while ((std::chrono::high_resolution_clock::now() - start).count() / 1e9 < seconds);
}

bool endsWith(std::string const& input, std::string const& suffix)
{
	if (input.size() < suffix.size()) return false; // "a" cannot end in "aa"
	return std::equal(begin(suffix), end(suffix), end(input) - suffix.size());
}

void delay(int milliseconds) // Only use if extremely precise delays are required, use preciseSleep otherwise
{
	long pause;
	clock_t now, then;

	pause = milliseconds * (CLOCKS_PER_SEC / 1000);
	now = then = clock();
	while ((now - then) < pause)
		now = clock();
}

void eraseLines(int count)
{
	count++;
	if (count > 0) {
		std::cout << "\x1b[2K"; // Delete current line
		// i=1 because we included the first line
		for (int i = 1; i < count; i++) {
			std::cout
				<< "\x1b[1A" // Move cursor up one
				<< "\x1b[2K"; // Delete the entire line
		}
		std::cout << "\r"; // Resume the cursor at beginning of line
	}
}

#endif