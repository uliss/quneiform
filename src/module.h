#ifndef MODULE_H
#define MODULE_H

#include <stdexcept>

namespace CIF {

    class ModuleInitException : public std::runtime_error {
    public:
        ModuleInitException(const std::string& msg, int return_code)
                : std::runtime_error(msg), return_code_(return_code)
        {}

        int return_code() const {
            return return_code_;
        }
    private:
        int return_code_;
    };
}

#endif // MODULE_H
