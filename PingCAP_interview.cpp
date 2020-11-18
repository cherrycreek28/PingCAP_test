//Written by Qingchuan Yang杨晴川 for the PingCAP interview. 
//The idea is as follows.
//Each time we load 1GB of the file into memory. (We can safely use malloc(), because the max size that malloc() allows is much greater than 1GB. I can't find the advantage of loading more bytes either.)
//We first find the first string and save it in [std::string first_string].  
//Then we find the next non-space char in the memory, and compare it with the first char in the first string. 
//-If they are same, continue, compare the following chars with the rest of [first_string]. If both two chars are space, this means we've found two identical strings, and then we find the next non-space char and start the comparison all over again. 
//-If they are not, break, and save the existing string to [std::string ret_string].
//Finally, we append next chars onto the [ret_string] until meeting a space. [ret_string] is the result. 

#include <string>
#include <iostream>

#define MB_t 1024

int f(std::string& ret, std::string file_name)
{
    //Idea: we grab the first string. 
    std::string first_string; 
    std::string ret_string;

    int index_to_compare = -1; 
    //std::string file_name = "a.txt"; 
    int first_string_done_flag = 0;
    int ret_string_found_flag = 0;
    FILE* f = nullptr;
    if (!(f = fopen(file_name.c_str(), "r+")))
        return -1;
    char* current_GB = (char* )malloc(1024 * MB_t); 

    for (int i = 0; i < 100; i++)
    {
        //Load 1GB of file into memory.
        int rc = 0; 
        rc = fseek(f, i * (1024 * MB_t), SEEK_SET);
        if (rc != 0)
        {
            free(current_GB); 
            return -1; 
        }
        
        if (fread(current_GB, (1024 * MB_t), 1, f) != 1)
        {
            free(current_GB); 
            return -1; 
        }

        for (long j = 0; j < 1024 * MB_t; j++)
        {
            //The flag is used to show if the first_string is determined. 
            if (first_string_done_flag == 0)
            {
                //The first_string is determined when meeting the first space && length of first_stirng > 0.
                if (*(current_GB + j) == ' ' && first_string.size() != 0)
                {
                    first_string_done_flag = 1; 
                    index_to_compare = 0;
                }
                else
                {
                    first_string += *(current_GB + j);
                }
            }
            else
            {
                //The flag is used to show if the ret_string is found.
                //If not: 
                if (ret_string_found_flag == 0)
                {
                    //If we meet the space:
                    if (*(current_GB + j) == ' ')
                    {
                        if (index_to_compare == first_string.size())
                        {
                            index_to_compare = 0; 
                        }
                    }
                    else
                    {
                        if (index_to_compare < first_string.size() && *(current_GB + j) == first_string[index_to_compare])
                            index_to_compare++;
                        else
                        {
                            ret_string = std::string(first_string.begin(), first_string.begin() + index_to_compare);
                            ret_string += *(current_GB + j); 
                            ret_string_found_flag = 1; 
                        }
                    }
                }
                //Now we've found the ret_string, we want to concatenate char to it. 
                else
                {
                    if (*(current_GB + j) == ' ')
                    {
                        ret = ret_string; 
                        free(current_GB);
                        return 0; 
                    }
                    else
                    {
                        ret_string += *(current_GB + j);
                    }
                }
            }
        }
    }
    free(current_GB); 
    return -1;
}

int main()
{
    std::string ret; 
    std::string file_name{"a.txt"};
    if (f(ret, file_name) == 0)
    {
        std::cout<<ret<<std::endl;
    }
    else
    {
        std::cout<<"Problem"<<std::endl;
    }
    return 0;
}
