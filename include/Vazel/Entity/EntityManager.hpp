#include "entity.hpp"
#include <exception>
#include <iostream>

namespace vazel {

    class EntityTypeError : std::exception {

        private:
            std::string _e;

        EntityTypeError(char const *e)
        {
            _e = std::string("EntityTypeError: ") + e;
        }

        const char *what()
        {
            return _e.c_str();
        }
    };

	class EntityManger {
	};
}