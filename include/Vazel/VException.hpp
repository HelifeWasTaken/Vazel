#pragma once

#include <exception>
#include <string>

namespace vazel
{

    class VException : public std::exception
    {
      public:
        virtual const char* what() const throw()
        {
            return "Vazel Default Exception";
        }
    };

} // namespace vazel