# Multi-threaded Mathproblem
This repository contains a brute-force attempt at a certain math problem. The point here however, is to show some multithreaded code
that is working together with string_view in C++. On my personal laptop (MSI GL65 9SD, Intel Core i7-9750H (6 cores, 12 threads), it 
can check all numbers 11 to 100.000.000 in about 2.7s. The same settings for a single thread takes about 20s. So the application of
multi-threading increases the speed by a factor of about 7.4.

Note: when using the -O3 tag on the compiler, I can get all numbers from 11 to 100.000.000 in 0.8s. On a single thread, it takes about
5s when the -O3 tag is used.

Also note that maximum hardware_concurrency heats up your CPU as it takes 100% of its capacity. My CPU can reach 95 degrees Celcius
when 12 threads are used, it is rated for 100 degrees.

## The math problem explained
Take any number with multiple digits (i.e. >=10) and multiply all the digits together. Then take the result and multiply all the digits 
again. Continue this pattern until you reach a single digit number (i.e. result is element of [0,9]). The result of the problem is the
number of times you can multiply numbers together, before you reach the single digit.
Apparently, nobody has ever found a number, such that this result equals 10 or higher. Also from my tests, I cannot find any number 
such that the result is either 10 or above. It always seems to max out at 9.