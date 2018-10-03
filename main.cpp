#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>

namespace owl
{
    template<typename S, typename T, typename... Targs>
    void manage(S& ss, const char type, T value)
    {
        switch (type)
        {
            case 's': ss << value; break;
            case 'd': ss << std::fixed << std::setprecision(1) << value; break;
        }
    }

    template<typename S>
    void tprintf2(S& ss, const char* format) // base function
    {
        ss << format;
    }

    template<typename S,typename T, typename... Targs>
    S& tprintf2(S& ss, const char* format, T value, Targs... Fargs) // recursive variadic function
    {
        for ( ; *format != '\0'; format++ )
        {
            if ( *format == '{' )
            {
                if ( *(format+2) == '}' )
                {
                    format++;
                    char type = *format;
                    format++;
                    manage(ss,type,value);
                }
               tprintf2(ss, format+1, Fargs...); // recursive call
               return ss;
            }
            ss << *format;
        }
        return ss;
    }

    template<typename T, typename... Targs>
    void out(const char* format, T value, Targs... Fargs) // recursive variadic function
    {
        tprintf2(std::cout, format, value, Fargs...);
    }

    template<typename T, typename... Targs>
    std::string fmt(const char* format, T value, Targs... Fargs) // recursive variadic function
    {
        std::stringstream ss;
        tprintf2(ss, format, value, Fargs...);
        return ss.str();
    }
}
int main()
{
    owl::out("{s} world{s} {d}\n","Hello",'!',123.2312);
    std::cout << owl::fmt("{s} world{s} {d}\n","Hello",'!',123.2312);
    return 0;
}


/* // Using stdout //
#include <vector>
#include <iostream>
#include <iomanip>

template<typename T, typename... Targs>
void manage(std::ostream& ss, const char type, T value)
{
    switch (type)
    {
        case 's': ss << value; break;
        case 'd': ss << std::fixed << std::setprecision(1) << value; break;
    }
}

void tprintf2(std::ostream& ss, const char* format) // base function
{
    ss << format;
}

template<typename T, typename... Targs>
std::ostream& tprintf2(std::ostream& ss, const char* format, T value, Targs... Fargs) // recursive variadic function
{
    for ( ; *format != '\0'; format++ )
    {
        if ( *format == '{' )
        {
            if ( *(format+2) == '}' )
            {
                format++;
                char type = *format;
                format++;
                manage(ss,type,value);
            }
           tprintf2(ss, format+1, Fargs...); // recursive call
           return ss;
        }
        ss << *format;
    }
    return ss;
}

template<typename T, typename... Targs>
std::ostream& tprintf(const char* format, T value, Targs... Fargs) // recursive variadic function
{
    std::ostream& ss = std::cout;
    return tprintf2(ss, format, value, Fargs...);
}

int main()
{
    tprintf("{s} world{s} {d}\n","Hello",'!',123.2312);
    return 0;
}
*/

/*
// using stringstream //
#include <vector>
#include <iostream>
#include <iomanip>
#include <limits>

template<typename T, typename... Targs>
void manage(std::stringstream& ss, const char type, T value)
{
    switch (type)
    {
        case 's': ss << value; break;
        case 'd': ss << std::setprecision(std::numeric_limits<double>::digits10) << value; break;
    }
}

void tprintf2(std::stringstream& ss, const char* format) // base function
{
    ss << format;
}

template<typename T, typename... Targs>
std::string tprintf2(std::stringstream& ss, const char* format, T value, Targs... Fargs) // recursive variadic function
{
    for ( ; *format != '\0'; format++ )
    {
        if ( *format == '{' )
        {
            if ( *(format+2) == '}' )
            {
                format++;
                char type = *format;
                format++;
                manage(ss,type,value);
            }
           tprintf2(ss, format+1, Fargs...); // recursive call
           return ss.str();
        }
        ss << *format;
    }
    return ss.str();
}

template<typename T, typename... Targs>
std::string tprintf(const char* format, T value, Targs... Fargs) // recursive variadic function
{
    std::stringstream ss;
    return tprintf2(ss, format, value, Fargs...);
}

int main()
{
    std::cout << tprintf("{s} world{s} {d}\n","Hello",'!',123.2312);
    return 0;
}

*/
