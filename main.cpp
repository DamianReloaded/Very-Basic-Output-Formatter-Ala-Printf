namespace impl {
  template<char... S> struct str {
    static constexpr char data[] = { S..., '\0' };
  };

  template<char... S>
  constexpr char str<S...>::data[];

  template<char C, int N, char const * const * Ptr, char... S>
  struct explode_ { typedef typename explode_<*(*Ptr+N+1), N+1, Ptr, S..., C>::list list; };

  template<int N, char const * const * Ptr, char... S>
  struct explode_<'\0', N, Ptr, S...> { typedef str<S...> list; };

  template<char const * const * Ptr>
  using explode = typename explode_<**Ptr, 0, Ptr>::list;

  template<class...>
  struct join { typedef str<> result; };

  template<char... A, char... B, class... Ts>
  struct join<str<A...>, str<B...>, Ts...> {
    typedef typename join<str<A..., ',', ' ', B...>, Ts...>::result result;
  };

  template<char... A>
  struct join<str<A...>> {
    typedef str<A..., '\n'> result;
  };
}

#include <cstddef>


template<class T> struct format_m;
template<class T> struct format_m<T*>      { static constexpr char const * fmt = "%p";  };
template<> struct format_m<int          >  { static constexpr char const * fmt = "%d";  };
template<> struct format_m<float        >  { static constexpr char const * fmt = "%.4f";  };
template<> struct format_m<double        >  { static constexpr char const * fmt = "%.4f";  };
template<> struct format_m<char const*  >  { static constexpr char const * fmt = "%s";  };
template<> struct format_m<char         >  { static constexpr char const * fmt = "'%c'";  };
template<> struct format_m<unsigned long>  { static constexpr char const * fmt = "%lu"; };


template<class... Ts>
constexpr char const * build_fmt () {
  return impl::join<impl::explode<&format_m<Ts>::fmt>...>::result::data;
}

/* ...................................... */

#include <cstdio>
#include <type_traits>

template<class... Ts>
void auto_printf (Ts... args) {
  constexpr char const * fmt = build_fmt<Ts...> ();
  printf (fmt, args...);
}

//--------------------------------------------------------
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
            case 'd': ss << std::fixed << std::setprecision(4) << value; break;
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

#include <time.h>

int main()
{
    clock_t t;
    clock_t t1, t2, t3;

    t = clock();
    for (size_t i=0; i<10000; i++) auto_printf ("Hello " "world" "!",123.2312);
    t1 = clock() - t;

    t = clock();
    for (size_t i=0; i<10000; i++) owl::out("{s} world{s} {d}\n","Hello",'!',123.2312);
    t2 = clock() - t;

    t = clock();
    for (size_t i=0; i<10000; i++) printf("Hello ! %f \n",123.2312);
    t3 = clock() - t;

    printf ("auto_printf: took me %d clicks (%f seconds).\n",t1,((float)t1)/CLOCKS_PER_SEC);
    printf ("owl::out   : took me %d clicks (%f seconds).\n",t2,((float)t2)/CLOCKS_PER_SEC);
    printf ("printf     : took me %d clicks (%f seconds).\n",t3,((float)t3)/CLOCKS_PER_SEC);


    return 0;
}

