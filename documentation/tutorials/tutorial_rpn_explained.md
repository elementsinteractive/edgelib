:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# RPN Explained

## Why dynamic registration?
Dynamic registration is useful, because the developer only needs to provide a formula. Registration keys will be calculated with this formula, based on a unique ID from the device of the customer. A reseller will handle the purchases, and with dynamic registration they can handle the registration of products as well.

Product registration is a good way to prevent piracy. Because each customer needs to register their product, they can't just download a full version for free. It takes more effort to play commercial games for free this way.

## About RPN
RPN stands for [Reverse Polish notation](http://en.wikipedia.org/wiki/Reverse_Polish_notation) and is a popular method for calculating dynamic registration keys. It is supported by many resellers and is also supported by EDGELIB. The developer needs a formula to calculate an unlock code from a device ID when implementing this method. When this formula has been defined, it is being used in the game itself, and on the reseller website for that game (reseller websites enable you to enter an RPN string for each product and platform).

There is an input value (the device ID) and an output value (the unlock code) which is the calculated value of the RPN formula. Each character of the input value is evaluated by this formula until the last character is reached. The result becomes the output value.

An RPN string is a formula that can look like this: `i c + key + 10 *`. Here is an explanation of each component of this string:

* i: a variable, the value is the position of the input character
* c: a variable, the value is the ASCII value of the input character
* key: a cache variable which stores the result after interpreting a character. Starts at 0.
* Fixed numbers used for calculation
* Calculation symbols like: +, -, *, /

## Calculating RPN
Now follows a step by step guide on what actually happens when calculating a registration key, based on the formula. The input string (device ID) will be: `Dev`.

RPN string: i c + key + 10 * 
Character 1: 'D' 
Value i: 0 
Value c: 68 
Value key: 0 
Formula: 0 68 + 0 + 10 * 
Cache step 1: 0 
Cache step 2: 0 68 
Cache step 3: 0 68 + (0 + 68 = 68) 
Cache step 4: 68 0 
Cache step 5: 68 0 + (68 + 0 = 68) 
Cache step 6: 68 10 
Cache step 7: 68 10 * (68 * 10 = 680) 
Value key: 680

RPN string: i c + key + 10 * 
Character 2: 'e' 
Value i: 1 
Value c: 101 
Value key: 680 
Formula: 1 101 + 680 + 10 * 
Cache step 1: 1 
Cache step 2: 1 101 
Cache step 3: 1 101 + (1 + 101 = 102) 
Cache step 4: 102 680 
Cache step 5: 102 680 + (102 + 680 = 782) 
Cache step 6: 782 10 
Cache step 7: 782 10 * (782 * 10 = 7820) 
Value key: 7820

RPN string: i c + key + 10 * 
Character 3: 'v' 
Value i: 2 
Value c: 118 
Value key: 7820 
Formula: 2 118 + 7820 + 10 * 
Cache step 1: 2 
Cache step 2: 2 118 
Cache step 3: 2 118 + (2 + 118 = 120) 
Cache step 4: 120 7820 
Cache step 5: 120 7820 + (120 + 7820 = 7940) 
Cache step 6: 7940 10 
Cache step 7: 7940 10 * (7940 * 10 = 79400) 
Value key: 79400

The result is 79400. Using the variable 'key' is essential when defining an RPN string. This value stores the calculation result of one character, and needs to carry over.

## RPN method rules
Software resellers implementing RPN also have a set of rules regarding the input string and result. The result cannot be larger than 65535, because it needs to fit in a 2-byte integer value. In the previous case the result will be truncated: 79400 % 65536 = 13864. There is also another rule regarding the input string. When this string contains more than 10 characters, only the first 5 and last 5 will be processed.

## Chapters
* **RPN Explained**
* [Using the EDGELIB registration class](tutorial_rpn_registrationclass.md)

