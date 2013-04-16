#include <cassert>
#include <cmath>

#include <iostream>

#include "wire.hpp"

std::stringstream right, wrong;

#define test1(A) do { auto _A_ = (A); if( _A_ != decltype(A)(0) ) \
    right << "[ OK ] " #A " -> " << _A_ << std::endl; else \
    wrong << "[FAIL] " #A " -> " << _A_ << std::endl; \
} while(0)

#define test3(A,op,B) do { auto _A_ = (A); auto _B_ = (B); if( _A_ op _B_ ) \
    right << "[ OK ] " #A " " #op " " #B " -> " << _A_ << " " #op " " << _B_ << std::endl; else \
    wrong << "[FAIL] " #A " " #op " " #B " -> " << _A_ << " " #op " " << _B_ << std::endl; \
} while(0)

#define testNaN(NaN) test3(NaN,!=,NaN)

struct ctest_stream1
{
    operator std::string()
    {
        return "im b";
    }
};

struct ctest_stream2
{
    ctest_stream2()
    {}

    ctest_stream2( const std::string &t )
    {
        std::cout << t << std::endl;
    }

    void test()
    {}
};

int main( int argc, const char **argv )
{
    // tools
    test3( wire::format("%d %1.3f %s", 10, 3.14159f, "hello world"), ==, "10 3.142 hello world" );
    test3( wire::eval("(2+3)*2"), ==, 10 );

    test3( wire::string(99.95f), ==, "99.95" );
    test3( wire::string(999.9999f), ==, "999.9999" );
    test3( wire::precise(999.9999f), ==, "999.9999" );

    test3( wire::string(3.1415926535897932384626433832795L), ==, std::string("3.1415926535897932384626433832795").substr( 0, std::numeric_limits<long double>::digits10 + 1 + 1 ) );

    test3( wire::string(3.1415926535897932384626433832795L), ==, std::string("3.1415926535897932384626433832795").substr( 0, std::numeric_limits<long double>::digits10 + 1 + 1 ) );

    test3( wire::string().strip(), ==, wire::string() );
    test3( wire::string("").strip(), ==, wire::string() );
    test3( wire::string("abc").strip(), ==, "abc" );
    test3( wire::string("abc ").strip(), ==, "abc" );
    test3( wire::string(" abc").strip(), ==, "abc" );
    test3( wire::string(" abc ").strip(), ==, "abc" );
    test3( wire::string("a b c").strip(), ==, "a b c" );
    test3( wire::string(" a b c").strip(), ==, "a b c" );
    test3( wire::string("a b c ").strip(), ==, "a b c" );
    test3( wire::string(" a b c ").strip(), ==, "a b c" );

    test3( wire::string("abc").lstrip(), ==, "abc" );
    test3( wire::string("abc ").lstrip(), ==, "abc " );
    test3( wire::string(" abc").lstrip(), ==, "abc" );
    test3( wire::string(" abc ").lstrip(), ==, "abc " );
    test3( wire::string("a b c").lstrip(), ==, "a b c" );
    test3( wire::string(" a b c").lstrip(), ==, "a b c" );
    test3( wire::string("a b c ").lstrip(), ==, "a b c " );
    test3( wire::string(" a b c ").lstrip(), ==, "a b c " );

    test3( wire::string("abc").rstrip(), ==, "abc" );
    test3( wire::string("abc ").rstrip(), ==, "abc" );
    test3( wire::string(" abc").rstrip(), ==, " abc" );
    test3( wire::string(" abc ").rstrip(), ==, " abc" );
    test3( wire::string("a b c").rstrip(), ==, "a b c" );
    test3( wire::string(" a b c").rstrip(), ==, " a b c" );
    test3( wire::string("a b c ").rstrip(), ==, "a b c" );
    test3( wire::string(" a b c ").rstrip(), ==, " a b c" );

    //ptr to method
    if( 1 )
    {
        ctest_stream2 cts2;
        std::cout << wire::string( &ctest_stream2::test ) << std::endl;
    }

    test1( !wire::string("false").as<int>() );
    test1(  wire::string( "true").as<int>() );

    int a = wire::string();
    int b = int( wire::string() );

    test1( !a );
    test1( !b );

    //TEST
    std::string test_stream1a = ctest_stream1();
    //wire::string test_stream1b = ctest_stream1();

    test3( test_stream1a, ==, "im b" );
    //te3t1( test_stream1b, ==, "im b" );

    const char *str_constchar = "hello world";

    wire::string str_explicit("hello world");

    wire::string str_formatted  = wire::string("hello \1", "world");
    wire::string str_stdstring  = std::string("hello world");
    wire::string str_copyctor1  = wire::string( "hello world" );
    wire::string str_copyctor2  = wire::string( str_stdstring );

    wire::string str_assignop;  str_assignop = str_explicit;

    test3( str_explicit , ==, "hello world" );

    test3( str_formatted, ==, "hello world" );
    test3( str_stdstring, ==, "hello world" );
    test3( str_copyctor1, ==, "hello world" );
    test3( str_copyctor2, ==, "hello world" );

    test3( str_assignop , ==, "hello world" );

    test3( wire::string( 'a' ), ==, 'a' );
    test3( wire::string( "hi" ), ==, "hi" );
    test3( wire::string( true ), ==, true );
    test3( wire::string( 16384 ), ==, 16384 );
    test3( wire::string( 3.14159 ), ==, 3.14159 );
    test3( wire::string( 3.14159f ), ==, 3.14159f );

    test3( wire::string(), ==, 0 );
    test3( wire::string(), ==, 0.f );
    test3( wire::string(), ==, 0.0 );
    test3( wire::string(), ==, '\0' );
    test3( wire::string(), ==, "" );
    test3( wire::string(), ==, false );

    test3( wire::string( 'a' ), ==, 'a' );
    test3( wire::string( 'a' ), ==, "a" );
//  3 test1( wire::string( 'a' ), ==, 97 );
    test3( wire::string( "a" ), ==, 'a' );
    test3( wire::string( "a" ), ==, "a" );
//  3 test1( wire::string( "a" ), ==, 97 );
//  3 test1( wire::string(  97 ), ==, 'a' );
//  3 test1( wire::string(  97 ), ==, "a" );
    test3( wire::string(  97 ), ==, 97 );
    test3( wire::string(  97 ).as<int>(), ==, 97 );
    test3( wire::string(  97 ).as<char>(), ==, 'a' );

    test3( wire::string(         ).as<bool>(), ==, false );
    test3( wire::string(       0 ).as<bool>(), ==, false );
    test3( wire::string(       1 ).as<bool>(), ==,  true );
    test3( wire::string(       2 ).as<bool>(), ==,  true );
    test3( wire::string(     "0" ).as<bool>(), ==, false );
    test3( wire::string(     "1" ).as<bool>(), ==,  true );
    test3( wire::string(     "2" ).as<bool>(), ==,  true );
    test3( wire::string(   false ).as<bool>(), ==, false );
    test3( wire::string(    true ).as<bool>(), ==,  true );
    test3( wire::string( "false" ).as<bool>(), ==, false );
    test3( wire::string(  "true" ).as<bool>(), ==,  true );

    test3( wire::string(   'a' ).as<char>(), ==,    'a' );
    test3( wire::string(       ).as<char>(), ==,   '\0' );
    test3( wire::string(     0 ).as<char>(), ==,    '0' );
    test3( wire::string(     1 ).as<char>(), ==,    '1' );
    test3( wire::string(    33 ).as<char>(), ==,    '!' );
    test3( wire::string( false ).as<char>(), ==,   '\0' );
    test3( wire::string(  true ).as<char>(), ==, '\x01' );

    //short
    //long

    test3( wire::string(         ).as<int>(), ==,  0 );
    test3( wire::string(   false ).as<int>(), ==,  0 );
    test3( wire::string(    true ).as<int>(), ==,  1 );
    test3( wire::string( "false" ).as<int>(), ==,  0 );
    test3( wire::string(  "true" ).as<int>(), ==,  1 );
    test3( wire::string(       0 ).as<int>(), ==,  0 );
    test3( wire::string(       1 ).as<int>(), ==,  1 );
    test3( wire::string(      -1 ).as<int>(), ==, -1 );

    test3( wire::string(         ).as<unsigned>(), ==,             0 );
    test3( wire::string(   false ).as<unsigned>(), ==,             0 );
    test3( wire::string(    true ).as<unsigned>(), ==,             1 );
    test3( wire::string( "false" ).as<unsigned>(), ==,             0 );
    test3( wire::string(  "true" ).as<unsigned>(), ==,             1 );
    test3( wire::string(      -1 ).as<unsigned>(), ==, (unsigned)(-1) );

    test3( wire::string(         ).as<size_t>(), ==,           0  );
    test3( wire::string(   false ).as<size_t>(), ==,           0  );
    test3( wire::string(    true ).as<size_t>(), ==,           1  );
    test3( wire::string( "false" ).as<size_t>(), ==,           0  );
    test3( wire::string(  "true" ).as<size_t>(), ==,           1  );
    test3( wire::string(      -1 ).as<size_t>(), ==, (size_t)(-1) );

    // add limits float, double
    test3( wire::string(         ).as<float>(), ==, 0.f );
    test3( wire::string(   false ).as<float>(), ==, 0.f );
    test3( wire::string(    true ).as<float>(), ==, 1.f );
    test3( wire::string( "false" ).as<float>(), ==, 0.f );
    test3( wire::string(  "true" ).as<float>(), ==, 1.f );
    test3( wire::string(     3.f ).as<float>(), ==, 3.f );

    test3( wire::string(         ).as<double>(), ==, 0.0 );
    test3( wire::string(   false ).as<double>(), ==, 0.0 );
    test3( wire::string(    true ).as<double>(), ==, 1.0 );
    test3( wire::string( "false" ).as<double>(), ==, 0.f );
    test3( wire::string(  "true" ).as<double>(), ==, 1.f );
    test3( wire::string(     3.0 ).as<double>(), ==, 3.0 );

    //del replacement
    test3( wire::string("%25hello%25%25world%25").replace("%25",""), ==, "helloworld" );
    //same replacement
    test3( wire::string("%25hello%25%25world%25").replace("%25","%25"), ==, "%25hello%25%25world%25" );
    //longer replacement
    test3( wire::string("%25hello%25%25world%25").replace("%25","%255"), ==, "%255hello%255%255world%255" );
    //shorter replacement
    test3( wire::string("%25hello%25%25world%25").replace("%25","%2"), ==, "%2hello%2%2world%2" );

    test3( wire::string().size(), ==, 0 );
    test3( wire::string("").size(), ==, 0 );
    test3( wire::string(), ==, "" );
    test3( wire::string(""), ==, "" );

    // wire::string<<T and std::cout<<wire::string support

/*
    test3( ( wire::string() << false << std::endl ), ==, "false\n" );
    test3( ( wire::string() << '1' << std::endl ), ==, "1\n" );
    test3( ( wire::string() << "2" << std::endl ), ==, "2\n" );
    test3( ( wire::string() << 3 << std::endl ), ==, "3\n" );
    test3( ( wire::string() << 4.f << std::endl ), ==, "4.f\n" );
    test3( ( wire::string() << 5.0 << std::endl ), ==, "5.0\n" );
    test3( ( wire::string() << std::string("6") << std::endl ), ==, "6\n" );
    test3( ( wire::string() << wire::string("7") << std::endl ), ==, "7\n" );
*/

    test3( wire::string("Hi!").at(-6), ==, 'H' );
    test3( wire::string("Hi!").at(-5), ==, 'i' );
    test3( wire::string("Hi!").at(-4), ==, '!' );
    test3( wire::string("Hi!").at(-3), ==, 'H' );
    test3( wire::string("Hi!").at(-2), ==, 'i' );
    test3( wire::string("Hi!").at(-1), ==, '!' );
    test3( wire::string("Hi!").at( 0), ==, 'H' );
    test3( wire::string("Hi!").at( 1), ==, 'i' );
    test3( wire::string("Hi!").at( 2), ==, '!' );
    test3( wire::string("Hi!").at( 3), ==, 'H' );
    test3( wire::string("Hi!").at( 4), ==, 'i' );
    test3( wire::string("Hi!").at( 5), ==, '!' );

    test3( wire::string().at(-1), ==, '\0' );
    test3( wire::string().at( 0), ==, '\0' );
    test3( wire::string().at( 1), ==, '\0' );

    // Some simple expressions
    test3( wire::eval("1234"), ==, 1234 );
    test3( wire::eval("1+2*3"), ==, 7 );

    // Parenthesis
    test3( wire::eval("5*(4+4+1)"), ==, 45 );
    test3( wire::eval("5*(2*(1+3)+1)"), ==, 45 );
    test3( wire::eval("5*((1+3)*2+1)"), ==, 45 );

    // Spaces
    test3( wire::eval("5 * ((1 + 3) * 2 + 1)"), ==, 45 );
    test3( wire::eval("5 - 2 * ( 3 )"), ==, -1 );
    test3( wire::eval("5 - 2 * ( ( 4 )  - 1 )"), ==, -1 );

    // Sign before parenthesis
    test3( wire::eval("-(2+1)*4"), ==, -12 );
    test3( wire::eval("-4*(2+1)"), ==, -12 );

    // Fractional numbers
    test3( wire::eval("1.5/5"), ==, 0.3 );
    test3( wire::eval("1/5e10"), ==, 2e-11 );
    test3( wire::eval("(4-3)/(4*4)"), ==, 0.0625 );
    test3( wire::eval("1/2/2"), ==, 0.25 );
    test3( wire::eval("0.25 * .5 * 0.5"), ==, 0.0625 );
    test3( wire::eval(".25 / 2 * .5"), ==, 0.0625 );

    // Repeated operators
    test3( wire::eval("1+-2"), ==, -1 );
    test3( wire::eval("--2"), ==, 2 );
    test3( wire::eval("2---2"), ==, 0 );
    test3( wire::eval("2-+-2"), ==, 4 );

    // Check for parenthesis error
    testNaN( wire::eval("5*((1+3)*2+1") );
    testNaN( wire::eval("5*((1+3)*2)+1)") );

    // Check for repeated operators
    testNaN( wire::eval("5*/2") );

    // Check for wrong positions of an operator
    testNaN( wire::eval("*2") );
    testNaN( wire::eval("2+") );
    testNaN( wire::eval("2*") );

    // Check for divisions by zero
    testNaN( wire::eval("2/0") );
    testNaN( wire::eval("3+1/(5-5)+4") );
    testNaN( wire::eval("2/") ); // Erroneously detected as division by zero, but that's ok for us

    // Check for invalid characters
    testNaN( wire::eval("~5") );
    testNaN( wire::eval("5x") );

    // Check for multiply errors
    testNaN( wire::eval("3+1/0+4$") ); // Only one error will be detected (in this case, the last one)
    testNaN( wire::eval("3+1/0+4") );
    testNaN( wire::eval("q+1/0)") ); // ...or the first one
    testNaN( wire::eval("+1/0)") );
    testNaN( wire::eval("+1/0") );

    // Check for emtpy string
    testNaN( wire::eval("") );

    // End of tests. Show results.
    std::cout << right.str();
    std::cout << wrong.str();

    if( wrong.str().empty() )
        std::cout << "All ok." << std::endl;

    return 0;
}
