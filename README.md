# Multi-threaded Mathproblem
This repository contains a brute-force attempt at a certain math problem. The point here however, is to show some multithreaded code
that is working together with string_view in C++. On my personal laptop (MSI GL65 9SD, Intel Core i7-9750H (6 cores, 12 threads), it 
can check all numbers 11 to 100,000,000 in about 1.9s. The same settings for a single thread takes 14.3s. So the application of
multi-threading increases the speed by a factor of about 7.5.

Note: when using the -O3 tag on the compiler, I can get all numbers from 11 to 100,000,000 in 0.5s. On a single thread, it takes about
3s when the -O3 tag is used.

Also note that maximum hardware_concurrency heats up your CPU as it takes 100% of its capacity. My CPU can reach 95 degrees Celcius
when 12 threads are used, it is rated for 100 degrees.

The program takes 3 optional arguments. The first is a threshold value you want to look for, this is the minimum number of steps a
certain number needs to be included in your final result. The second is the lower bound of the interval you want to check and the third
is the upper bound of this interval.

## Single
The single program is able to check a single number and will also print all the steps for this particular number. It takes the number
you wish to check as input.

## The math problem explained
Take any number with multiple digits (i.e. >=10) and multiply all the digits together. Then take the result and multiply all the digits 
again. Continue this pattern until you reach a single digit number (i.e. result is element of [0,9]). The result of the problem is the
number of times you can multiply numbers together, before you reach the single digit.
The recordholder number is 277777788888899 which has 11 steps.
The smallest number with 10 steps is 3778888999.
Numberphile did a great video on this as well, you can view it here: https://www.youtube.com/watch?v=Wim9WJeDTHQ