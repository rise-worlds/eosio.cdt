#pragma once

#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/Support/Program.h"
#include <stdlib.h>
#if defined(__APPLE__)
# include <crt_externs.h>
#elif !defined(_MSC_VER)
// Forward declare environ in case it's not provided by stdlib.h.
extern char **environ;
#endif

#include "whereami/whereami.hpp"
#include <vector>
#include <sstream>

namespace eosio { namespace cdt {

using uint128_t           = unsigned __int128;

uint64_t char_to_symbol( char c ) {
    if( c == '~' )
        return (c - '~') + 49;
    if( c == '`' )
        return (c - '`') + 48;
    if( c == ')' )
        return (c - ')') + 47;
    if( c == '(' )
        return (c - '(') + 46;
    if( c == '}' )
        return (c - '}') + 45;
    if( c == '{' )
        return (c - '{') + 44;
    if( c == ']' )
        return (c - ']') + 43;
    if( c == '[' )
        return (c - '[') + 42;
    if( c == '>' )
        return (c - '>') + 41;
    if( c == '<' )
        return (c - '<') + 40;
    if( c == ':' )
        return (c - ':') + 39;
    if( c == '_' )
        return (c - '_') + 38;
    if( c >= 'A' && c <= 'Z' )
        return (c - 'A') + 12;
    if( c >= 'a' && c <= 'z' )
        return (c - 'a') + 12;
    if( c >= '0' && c <= '9' )
        return (c - '0') + 2;
    if( c == '-' )
        return (c - '-') + 1;
    return 0;
}

uint128_t string_to_name( const char* str )
{
    uint128_t name = 0;
    int i = 0;
    for ( ; str[i] && i < 20; ++i) {
        uint128_t c = (char_to_symbol(str[i]) & 0x3f);
        c <<= (4 + (6 * i));
        name |= c;
    }

    return name;
}

template <typename Lambda>
void validate_name( const std::string& str, Lambda&& error_handler ) {
   const auto len = str.length();
   if ( len > 20 ) {
      std::cout << "Error, name {" << str << "} is more than 20 characters long\n";
      return error_handler();
   }
   uint128_t value = string_to_name( str.c_str() );

   static const char* charmap = ".-0123456789abcdefghijklmnopqrstuvwxyz_:<>[]{}()`~";
   std::string str2(20,'.');

    uint128_t tmp = value;
    tmp >>= 4;
    for( uint32_t i = 0; i < 20; ++i ) {
        char c = charmap[tmp & 0x3f];
        str2[i] = c;
        tmp >>= 6;
    }
   
   auto trim = [](std::string& s) {
      int i;
      for (i = s.length()-1; i >= 0; i--)
         if (s[i] != '.')
            break;
      s = s.substr(0,i+1);
   };
   trim(str2);

   if ( str2 != str ) {
      std::cout << "Error, name not properly normalized\n";
      return error_handler();
   }
}

std::string name_to_string( uint64_t name_v1, uint64_t name_v2, uint64_t name_v3, uint64_t name_v4 ) {
    static const char* charmap = ".-0123456789abcdefghijklmnopqrstuvwxyz_:<>[]{}()`~";
    std::string str(43,'.');

    uint128_t tmp = name_v2;
    tmp <<= 64;
    tmp |= name_v1;
    tmp >>= 4;
    uint32_t i = 0;
    for( ; i < 20; ++i ) {
        char c = charmap[tmp & 0x3f];
        str[i] = c;
        tmp >>= 6;
    }
    tmp |= (name_v3 & 0x3ull) << 4;
    str[i] = charmap[tmp & 0x3f];
    tmp = name_v4;
    tmp <<= 62;
    tmp |= name_v3 >> 2;
    for ( i = 22; i < 42; ++i ) {
       char c = charmap[tmp & 0x3f];
       str[i] = c;
       tmp >>= 6;
    }

   auto trim_right_dots = [](std::string& str) {
      const auto last = str.find_last_not_of('.');
      if (last != std::string::npos)
         str = str.substr(0, last+1);
   };
   trim_right_dots( str );
   return str;
}

struct environment {
   static llvm::ArrayRef<llvm::StringRef> get() {
      static std::vector<llvm::StringRef> env_table;
      std::vector<std::string>     env_storage;
       auto envp = []{
#if defined(_WIN32)
         _wgetenv(L"TMP"); // Populate _wenviron, initially is null
         return _wenviron;
#elif defined(__APPLE__)
         return *_NSGetEnviron();
#else
         return environ;
#endif
       }();

       auto prepareEnvVar = [&](decltype(*envp) var) -> llvm::StringRef {
#if defined(_WIN32)
         // On Windows convert UTF16 encoded variable to UTF8
         auto len = wcslen(var);
         llvm::ArrayRef<char> ref{reinterpret_cast<char const *>(var),
                            len * sizeof(*var)};
         env_storage.emplace_back();
         auto convStatus = convertUTF16ToUTF8String(ref, env_storage.back());
         return env_storage.back();
#else
         return llvm::StringRef(var);
#endif
       };

       while (*envp != nullptr) {
         env_table.emplace_back(prepareEnvVar(*envp));
         ++envp;
       }
     return env_table;
   }
   static bool exec_subprogram(const std::string prog, std::vector<std::string> options, bool root=false) {
      std::stringstream args;
      for (auto s : options)
         args << s << " ";
      std::string find_path = eosio::cdt::whereami::where();
      if (root)
         find_path = "/usr/bin";
      if ( auto path = llvm::sys::findProgramByName(prog.c_str(), {find_path}) )
         std::system((*path+" "+args.str()).c_str());
      else
         return false;
      return true; 
   }

};
}} // ns eosio::cdt
