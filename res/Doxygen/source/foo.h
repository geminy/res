/**
 * @file foo.h
 * @brief The doxygen style is JavaDoc.
 *
 * It consists of a C-style comment block starting with two *'s.
 */

///
/// FOO macro
///
#define FOO "foo"

///
/// fint typedef
///
typedef int fint;

int foo1 = 1; /**< foo1 with detailed description */

int foo2 = 2; ///< foo2 with detailed description
              ///<

int foo3 = 3; ///< foo3 with brief description

/**
 * fooFunc function #FOO
 * @param f[in] input an integer
 * @return return an integer
 * @sa foo1 foo2 foo3
 */
int fooFunc(int f);

///
/// foo namespace
///
namespace foo
{

/**
 * FooClass class
 */
class FooClass
{

};

/**
 * FooStruct struct
 */
struct FooStruct
{

};

/**
 * FooEnum enum
 */
enum FooEnum
{

};

/**
 * FooUnion union
 */
union FooUnion
{

};

}