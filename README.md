Written by Qingchuan Yang杨晴川 for the PingCAP interview. 
The idea is as follows.
Each time we load 1GB of the file into memory. (We can safely use malloc(), because the max size that malloc() allows is much greater than 1GB. I can't find the advantage of loading more bytes either.)
We first find the first string and save it in [std::string first_string].  
Then we find the next non-space char in the memory, and compare it with the first char in the first string. 
-If they are same, continue, compare the following chars with the rest of [first_string]. If both two chars are space, this means we've found two identical strings, and then we find the next non-space char and start the comparison all over again. 
-If they are not, break, and save the existing string to [std::string ret_string].
Finally, we append next chars onto the [ret_string] until meeting a space. [ret_string] is the result. 
