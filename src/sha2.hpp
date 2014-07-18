/*
* Project: sha2
* Author: kevr <kevr@nixcode.us>
* License: MIT
* Description: A SHA2 hashing program. [possibly a library?]
*/
#ifndef __SHA2_HPP__
#define __SHA2_HPP__

namespace crypto
{
    typedef unsigned int uint;
    typedef unsigned char uchar;

    template<uint ScheduleSize, uint DataSize>
    struct context
    {
    public:
        context();
        ~context();

    private:
        uint schedule[ScheduleSize];
        uint data[DataSize];

    };
};

#endif

