/*!
 * \file bar.h
 * \brief The doxygen style is Qt.
 *
 * Add an exclamation mark (!) after the opening of a C-style comment block.
 */

//!
//! BAR macro
//!
#define BAR "bar"

//!
//! bint typedef
//!
typedef int bint;

int bar1 = 1; /*!< bar1 with detailed description */

int bar2 = 2; //!< bar2 with detailed description
              //!<

int bar3 = 3; //!< bar3 with brief description

/*!
 * barFunc function with #BAR
 * \param b[in] input an integer
 * \return return an integer
 * \see bar1 bar2 bar3
 */
int barFunc(int b);

//!
//! bar namespace
//!
namespace bar {

/*!
 * BarClass class
 */
class BarClass
{

};

/*!
 * BarStruct struct
 */
struct BarStruct
{

};

/*!
 * BarEnum enum
 */
enum BarEnum
{

};

/*!
 * BarUnion union
 */
union BarUnion
{

};

}