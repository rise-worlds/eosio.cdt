// TODO: Make sure CHECK_EQUAL is changed REQUIRE_EQUAL
// TODO: Change: `REQUIRE_EQUAL( ds.tellp() == 9, true )` to `REQUIRE_EQUAL( ds.tellp(), 9 )`
// TODO: Organize tests to be in a logical order
// TODO: Swithc arrays with ints to arrays with chars; I think it makes it more readable
// TODO: Organize the `capi` tests and the respective `eosio` comments
// TODO: Double-check to make sure I get everything right
// TODO: Test a more complex `fixed_bytes`
// TODO: Test a more complex `key256`

/**
 *  @file
 *  @copyright defined in eosio.cdt/LICENSE.txt
 */

#include <eosio/native/tester.hpp>
#include <eosiolib/binary_extension.hpp>
#include <eosiolib/crypto.hpp>
#include <eosiolib/datastream.hpp>
#include <eosiolib/ignore.hpp>
#include <eosiolib/symbol.hpp>

#include <boost/container/flat_set.hpp>
#include <boost/container/flat_map.hpp>

#include <array>
#include <deque>
#include <list>
#include <set>
#include <string>
#include <vector>

using eosio::binary_extension;
using eosio::datastream;
using eosio::fixed_bytes;
using eosio::ignore;
using eosio::ignore_wrapper;
using eosio::key256;
using eosio::public_key;
using eosio::signature;
using eosio::symbol;
using eosio::symbol_code;
using eosio::unpack;

using boost::container::flat_map;
using boost::container::flat_set;

using std::array;
using std::begin;
using std::end;
using std::fill;
using std::list;
using std::map;
using std::set;
using std::string;
using std::tuple;
using std::vector;

// Definitions in `eosio.cdt/libraries/eosiolib/datastream.hpp`
EOSIO_TEST_BEGIN(datastream_test)
   silence_output(true);

   static constexpr uint16_t buffer_size{256};
   char datastream_buffer[buffer_size]{}; // Buffer for the datastream to point to
   char buffer[buffer_size]; // Buffer to compare `datastream_buffer` with

   // Fill the char array `datastream_buffer` with all 256 ASCII characters
   unsigned char j{0};
   for(int i{0}; i < buffer_size; ++i)
      datastream_buffer[i] = j++;

   // Why can't I initialize a data stream object of type `datastream<uint32_t> ds{0, 256}`?
   /// datastream(T, size_t)
   datastream<char*> ds{datastream_buffer, buffer_size};

   // inline void skip(size_t)
   ds.skip(1);
   CHECK_EQUAL( ds.pos() == datastream_buffer+1, true )
   ds.skip(-1);
   CHECK_EQUAL( ds.pos() == datastream_buffer, true )
   
   // inline bool read(char*, size_t)
   CHECK_EQUAL( ds.read(buffer, 256), true )
   CHECK_EQUAL( memcmp(buffer, datastream_buffer, 256), 0)

   CHECK_ASSERT( "read", ([&]() {ds.read(buffer, 1);}) )

   // T pos()const
   CHECK_EQUAL( ds.pos() == datastream_buffer+256, true )
   CHECK_EQUAL( ds.pos() == datastream_buffer, false )

   // inline bool seekp(size_t)
   ds.seekp(0);
   CHECK_EQUAL( ds.pos() == datastream_buffer, true )
   CHECK_EQUAL( ds.pos() == datastream_buffer+256, false )

   // inline bool write(const char*, size_t)
   // Fill `buffer` with a new set of values
   fill(begin(buffer), end(buffer), 1);

   CHECK_EQUAL( ds.write(buffer, 256), true )
   CHECK_EQUAL( memcmp(buffer, datastream_buffer, 256), 0 )

   CHECK_ASSERT( "write", ([&]() {ds.write(buffer, 1);}) )

   // inline bool put(char)
   ds.seekp(0);
   CHECK_EQUAL( ds.put('c'), true )
   *buffer = 'c';
   CHECK_EQUAL( memcmp(buffer, datastream_buffer, 256), 0 )

   ds.seekp(256);
   CHECK_ASSERT( "put", ([&]() {ds.put('c');}) )
  
   // inline bool get(unsigned char&)
   unsigned char uch{};

   ds.seekp(0);
   CHECK_EQUAL( ds.get(uch), true )
   CHECK_EQUAL( uch, 'c' )

   // inline bool get(char&)
   char ch{};

   ds.seekp(0);
   CHECK_EQUAL( ds.get(ch), true )
   CHECK_EQUAL( ch, 'c' )

   // inline bool valid()const
   ds.seekp(256);
   CHECK_EQUAL( ds.valid(), true )

   ds.seekp(257);
   CHECK_EQUAL( ds.valid(), false )

   // inline size_t tellp()const
   ds.seekp(0);
   CHECK_EQUAL( ds.tellp(), 0 )
   ds.seekp(256);
   CHECK_EQUAL( ds.tellp(), 256 )
   ds.seekp(257);
   CHECK_EQUAL( ds.tellp(), 257 )

   //inline size_t remaining()const
   ds.seekp(0);
   CHECK_EQUAL( ds.remaining(), 256 )
   ds.seekp(256);
   CHECK_EQUAL( ds.remaining(), 0 )
   ds.seekp(257);
   CHECK_EQUAL( ds.remaining(), -1)

   silence_output(false);
EOSIO_TEST_END

// Definitions in `eosio.cdt/libraries/eosiolib/datastream.hpp`
EOSIO_TEST_BEGIN(datastream_specialization_test)
   silence_output(true);

   static constexpr uint16_t buffer_size{256};
   char datastream_buffer[buffer_size]{}; // Buffer for the datastream to point to
   char buffer[buffer_size]; // Buffer to compare `datastream_buffer` with

   // Fill the char array `datastream_buffer` with all all 256 ASCII characters
   unsigned char j{0};
   for(int i{0}; i < buffer_size; ++i)
      datastream_buffer[i] = j++;
   
   // Why can't I initialize a data stream object of type `datastream<uint32_t> ds{0, 256}`?
   /// datastream(T, size_t)
   datastream<size_t> ds{buffer_size};

   // inline void skip(size_t)
   // inline size_t tellp()const
   CHECK_EQUAL( ds.skip(0), true)
   CHECK_EQUAL( ds.tellp(), 256)
   
   CHECK_EQUAL( ds.skip(1), true)
   CHECK_EQUAL( ds.tellp(), 257)
   
   CHECK_EQUAL( ds.skip(255), true)
   CHECK_EQUAL( ds.tellp(), 512)
   
   CHECK_EQUAL( ds.skip(1028), true)
   CHECK_EQUAL( ds.tellp(), 1540)
   
   // inline bool seekp(size_t)
   ds.seekp(0);
   CHECK_EQUAL( ds.tellp(), 0)

   // inline bool write(const char*,size_t)
   // inline size_t tellp()const
   CHECK_EQUAL( ds.write(buffer, 256), true )
   CHECK_EQUAL( ds.tellp(), 256 )

   CHECK_EQUAL( ds.write(buffer, 1), true )
   CHECK_EQUAL( ds.tellp(), 257 )

   // inline bool put(char)
   char ch{'c'};
   
   ds.seekp(0);
   CHECK_EQUAL( ds.put(ch), true )
   CHECK_EQUAL( ds.tellp(), 1 )

   // inline bool valid()
   ds.seekp(0);
   CHECK_EQUAL( ds.valid(), true )

   ds.seekp(1);
   CHECK_EQUAL( ds.valid(), true )

   ds.seekp(256);
   CHECK_EQUAL( ds.valid(), true )

   ds.seekp(257);
   CHECK_EQUAL( ds.valid(), true )

   // inline size_t remaining()
   ds.seekp(0);
   CHECK_EQUAL( ds.remaining(), 0 )

   ds.seekp(1);
   CHECK_EQUAL( ds.remaining(), 0 )

   ds.seekp(256);
   CHECK_EQUAL( ds.remaining(), 0 )

   ds.seekp(257);
   CHECK_EQUAL( ds.remaining(), 0 )

   silence_output(false);
EOSIO_TEST_END

// Definitions in `eosio.cdt/libraries/eosiolib/datastream.hpp`
EOSIO_TEST_BEGIN(datastream_stream_test)
   silence_output(false);

   // TODO: figure out why making this buffer and off number throws a segfault
   static constexpr uint8_t buffer_size{128};
   char datastream_buffer[buffer_size]; // Buffer for the datastream to point to

   datastream<char*> ds{datastream_buffer, buffer_size};

   // ---------
   // std::list
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const std::list<char> cl{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i' };
   std::list<char> l{};
   ds << cl;
   ds.seekp(0);
   ds >> l;
   CHECK_EQUAL( cl == l, true )

   // Note: uncomment once issue has been resolved
   // ----------
   // std::deque
   // ds.seekp(0);
   // fill(begin(datastream_buffer), end(datastream_buffer), 0);
   // const std::deque<char> cd{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i' };
   // std::deque<char> d{};
   // ds << cd;
   // ds.seekp(0);
   // ds >> d; // Fails here
   // CHECK_EQUAL( cd == d, true )

   // ----------------
   // binary_extension
   // ds.seekp(0);
   // fill(begin(datastream_buffer), end(datastream_buffer), 0);
   // const binary_extension<char> cbe_char{'c'};
   // binary_extension<char> cb_char{};
   // ds << cbe_char;
   // ds.seekp(0);
   // ds >> cb_char;
   // CHECK_EQUAL( cbe_char.value() == cb_char.value(), true )

   // ds.seekp(0);
   // fill(begin(datastream_buffer), end(datastream_buffer), 0);
   // binary_extension<int> be_int0{};
   // binary_extension<int> be_int1{};
   // ds << be_int0.value_or(42);
   // ds.seekp(0);
   // ds >> be_int1;
   // CHECK_EQUAL( be_int1.value() == 42, true )

   // I'm not understanding fully why this can't take data structures with
   // more constructors than just a default constructors
   struct be_test {
      be_test() : val{42} {}
      int val;
   };
   const be_test bet{};
   
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const binary_extension<be_test> cbe_btest{bet};
   binary_extension<be_test> cb_btest{};
   ds << cbe_btest;
   ds.seekp(0);
   ds >> cb_btest;
   CHECK_EQUAL( cbe_btest.value().val == cb_btest.value().val, true )
   
   const binary_extension<be_test> cbe_none{};
   CHECK_ASSERT( "cannot get value of empty binary_extension", [&](){cbe_none.value();} )

   // ------------
   // std::variant
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   std::variant<int, char> v0{1024};
   std::variant<int, char> v1{};
   ds << v0;
   ds.seekp(0);
   ds >> v1;
   CHECK_EQUAL( v0 == v1, true )

   // ---------
   // std::pair
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const std::pair<char, int> cp{'c', 0x43};
   std::pair<char, int> p{};
   ds << cp;
   ds.seekp(0);
   ds >> p;
   CHECK_EQUAL( cp == p, true )

   // -------------
   // std::optional
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const std::optional<char> co{'c'};
   std::optional<char> o{};
   ds << co;
   ds.seekp(0);
   ds >> o;
   CHECK_EQUAL( co == o, true )
   
   // -----------
   // eosio::symbol_code
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const symbol_code csc{"SYMBOLL"};
   symbol_code sc{};
   ds << csc;
   ds.seekp(0);
   ds >> sc;
   CHECK_EQUAL( csc == sc, true )

   // ------
   // eosio::symbol
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const symbol sym_no_prec{"SYMBOLL", 0};
   symbol sym{};
   ds << sym_no_prec;
   ds.seekp(0);
   ds >> sym;
   CHECK_EQUAL( sym_no_prec == sym, true )

   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const symbol sym_prec{"SYMBOLL", 255};
   ds << sym_prec;
   ds.seekp(0);
   ds >> sym;
   CHECK_EQUAL( sym_prec == sym, true )

   // --------------
   // eosio::ignore_wrapper
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const ignore_wrapper<char> ciw{'c'};
   char iw;
   ds << ciw;
   ds.seekp(0);
   ds >> iw;
   CHECK_EQUAL( ciw.value == iw, true )

   // ------
   // eosio::ignore
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const ignore<char> cig{};
   ignore<char> ig;
   ds << cig;
   CHECK_EQUAL( ds.tellp() == 0, true )
   ds >> ig;
   CHECK_EQUAL( ds.tellp() == 0, true )

   // ---------------
   // capi_public_key
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const capi_public_key c_cpubkey{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'};
   capi_public_key c_pubkey{};
   ds << c_cpubkey;
   ds.seekp(0);
   ds >> c_pubkey;
   CHECK_EQUAL( memcmp(c_cpubkey.data, c_cpubkey.data, 32), 0 )

   // ----------
   // eosio::public_key
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const public_key cpubkey{{}, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'};
   public_key pubkey{};
   ds << cpubkey;
   ds.seekp(0);
   ds >> pubkey;
   CHECK_EQUAL( cpubkey == pubkey, true )

   // ---------
   // signature
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const signature csig{{}, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'};
   signature sig{};
   ds << csig;
   ds.seekp(0);
   ds >> sig;
   CHECK_EQUAL( csig == sig, true )
   
   // // ------
   // // key256
   // ds.seekp(0);
   // fill(begin(datastream_buffer), end(datastream_buffer), 0);
   // const key256 ckey256{};
   // key256 key256{};
   // ds << ckey256;
   // ds.seekp(0);
   // ds >> key256;
   // CHECK_EQUAL( ckey256 == key256, true )
   
   // // -----------
   // // fixed_bytes
   // array<uint128_t,32> arr{0x01,0x02,0x03,0x04};
   
   // ds.seekp(0);
   // fill(begin(datastream_buffer), end(datastream_buffer), 0);
   // const fixed_bytes<32> cfb{arr};
   // fixed_bytes<32> fb{};
   // ds << cfb;
   // ds.seekp(0);
   // ds >> fb;
   // CHECK_EQUAL( cfb == fb, true )

   // ----
   // bool
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const bool cboolean{true};
   bool boolean{};
   ds << cboolean;
   ds.seekp(0);
   ds >> boolean;
   CHECK_EQUAL( cboolean == boolean, true )

   // ----------------
   // capi_checksum160

   // ----------------
   // capi_checksum512

   // -----------
   // checksum256
   
   // ------
   // string
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const string cstr {"abcdefghi"};
   string str{};
   ds << cstr;
   ds.seekp(0);
   ds >> str;
   CHECK_EQUAL( cstr == str, true )

   // ----------
   // std::array
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const array<char, 9> ca{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'};
   std::array<char, 9> a{};
   ds << ca;
   ds.seekp(0);
   ds >> a;
   CHECK_EQUAL( ca == a, true )

   // ---------------
   // T[] (primitive)
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const int cprim_array[10]{0,1,2,3,4,5,6,7,8,9};
   int prim_array[10]{};
   ds << cprim_array;
   REQUIRE_EQUAL( ds.tellp() == 41, true )

   ds.seekp(0); // Next three lines are testing an implementation detail ???
   ds >> prim_array;
   REQUIRE_EQUAL( memcmp(cprim_array, prim_array, 10), 0 )

   ds.seekp(1);
   fill(begin(prim_array), end(prim_array), 0);
   REQUIRE_ASSERT( "T[] size and unpacked size don't match", [&](){ds >> prim_array;} )

   // -------------------
   // T[] (non-primitive)
   struct non_prim_array_test {
      int val;
   };

   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const non_prim_array_test cnon_prim_array[10]{0,1,2,3,4,5,6,7,8,9};
   non_prim_array_test non_prim_array[10]{};
   ds << cnon_prim_array;
   REQUIRE_EQUAL( ds.tellp() == 41, true )

   ds.seekp(0); // Next three lines are testing an implementation detail ???
   ds >> non_prim_array;
   REQUIRE_EQUAL( memcmp(cnon_prim_array, non_prim_array, 10), 0 )

   ds.seekp(1);
   fill(begin(non_prim_array), end(non_prim_array), non_prim_array_test{});
   REQUIRE_ASSERT( "T[] size and unpacked size don't match", [&](){ds >> non_prim_array;} )

   // -----------
   // std::vector
   struct vec_test {
      char val;
   };

   // ds.seekp(0);
   // fill(begin(datastream_buffer), end(datastream_buffer), 0);
   // const vector<vec_test> ctest_vec{{'a'}, {'b'}, {'c'}, {'d'}, {'e'}, {'f'}, {'g'}, {'h'}, {'i'}};
   // std::vector<vec_test> test_vec{};
   // ds << ctest_vec;
   // ds.seekp(0);
   // ds >> test_vec;
   // CHECK_EQUAL( ctest_vec == test_vec, true )

   // -----------------
   // std::vector<char>
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const vector<char> cchar_vec{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'};
   vector<char> char_vec{};
   ds << cchar_vec;
   ds.seekp(0);
   ds >> char_vec;
   CHECK_EQUAL( cchar_vec == char_vec, true )

   // --------
   // std::set
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const set<char> cchar_set{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'};
   set<char> char_set{};
   ds << cchar_set;
   ds.seekp(0);
   ds >> char_set;
   CHECK_EQUAL( cchar_set == char_set, true )

   // --------------------------
   // boost::container::flat_set
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const flat_set<char> cchar_flat_set{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'};
   flat_set<char> char_flat_set{};
   ds << cchar_flat_set;
   ds.seekp(0);
   ds >> char_flat_set;
   CHECK_EQUAL( cchar_flat_set == char_flat_set, true )

   // --------
   // std::map
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const map<char,int> cchar_map{{'a',97}, {'b',98}, {'c',99}, {'d',100}};
   map<char,int> char_map{};
   ds << cchar_map;
   ds.seekp(0);
   ds >> char_map;
   CHECK_EQUAL( cchar_map == char_map, true )

   // --------------------------
   // boost::container::flat_map
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const flat_map<char,int> cchar_flat_map{{'a',97}, {'b',98}, {'c',99}, {'d',100}};
   flat_map<char,int> char_flat_map{};
   ds << cchar_flat_map;
   ds.seekp(0);
   ds >> char_flat_map;
   CHECK_EQUAL( cchar_flat_map == char_flat_map, true )

   // ----------
   // std::tuple
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const tuple<string,int,char> ctup{"abc123",42,'a'};
   tuple<string,int,char> tup{};
   ds << ctup;
   ds.seekp(0);
   ds >> tup;
   CHECK_EQUAL( ctup == tup, true )

   // ---------------
   // T (primitive)
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const int cprim{10};
   int prim{};
   ds << cprim;
   ds.seekp(0);
   ds >> prim;
   REQUIRE_EQUAL( cprim == prim, true )

   // -------------------
   // T (non-primitive)
   struct non_prim_test {
      int val;
   };

   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const non_prim_test cnonprim{10};
   non_prim_test nonprim{};
   ds << cnonprim;
   ds.seekp(0);
   ds >> nonprim;
   REQUIRE_EQUAL( cnonprim.val == nonprim.val, true )
   
   silence_output(false);
EOSIO_TEST_END

// Definitions in `eosio.cdt/libraries/eosiolib/datastream.hpp`
EOSIO_TEST_BEGIN(misc_datastream_test)
   silence_output(false);

   // -----------------------------
   // T unpack(const char*, size_t)
   const char unpack_source_buffer[9]{'a','b','c','d','e','f','g','h','i'};
   char unpack_ch{};
   for (uint8_t i = 0; i < 9; ++i) {
      unpack_ch = unpack<char>(unpack_source_buffer+i, 9);
      REQUIRE_EQUAL( unpack_source_buffer[i], unpack_ch )
   }

   // -----------------------------
   // T unpack(const vector<char>&)
   const vector<char> unpack_source_vec{'a','b','c','d','e','f','g','h','i'};
   for (uint8_t i = 0; i < 9; ++i) {
      unpack_ch = unpack<char>(unpack_source_buffer+i, 9);
      REQUIRE_EQUAL( unpack_source_buffer[i], unpack_ch )
   }

   // --------------------------
   // size_t pack_size(const T&)


   // ---------------------------
   // vector<char> pack(const T&)


   silence_output(false);
EOSIO_TEST_END

int main(int argc, char* argv[]) {
   // Note: uncomment once segfaulting during `memcpy` has been resolved
   // EOSIO_TEST(datastream_test);
   // EOSIO_TEST(datastream_specialization_test);
   EOSIO_TEST(datastream_stream_test);
   // EOSIO_TEST(misc_datastream_test);
   return has_failed();
}
