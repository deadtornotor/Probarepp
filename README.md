# Probarepp

A simple multithreaded C++ Testing library


## Building

Use cmake to create the solution files

```bash
mkdir build
cd build
cmake ..
make -j
```


## CMake Targets

Probare has the option PROBAREPP_TESTS set to ON by default, the following will explain what it does.

| Target | Usage |
| --- | --- |
| probare_test | Test target for probare (self test) |
| build_tests | Build target to define all required dependencies for building |
| tests | Build and run all tests via ctest |

> If you already have a target called tests/build_tests it will just skip defining those targets but still add probare_test to the build_tests and build_tests to the tests target


### Add test executables

```cmake
add_executable(my_tests tests/main.cpp)

target_link_libraries(my_tests PRIVATE probarepp)

add_test(NAME my_app COMMAND my_tests)
add_dependencies(build_tests my_tests)
```

If you have a setup like this

```
project/
->CMakeLists.txt
->tests/
```

You can use the function `add_probare_test(target)` which adds a test target named with **\<target\>_test** and globs all `.cpp/.c/.h/.hpp` files in `tests`

```cmake
add_probare_test(my_app)
```

> This function will also include the `src` and `include` directory automatically if they exist


## Functions

This is a list of defines used by this testing library

| Defines | Parameters | Explanation |
| --- | --- | --- |
| TEST | name | Create a Test |
| TEST_NO_PARALLEL | name | Create a Test that is forced to run serially and not parallel |
| ASSERT_FLOAT_EQ | expected, actual, epsilon | Assert a float value |
| ASSERT_STR_EQ | expected, actual | Assert strings are equal |
| ASSERT_MATCH | string, pattern | Assert that the string matches the regex pattern |
| ASSERT_TRUE | condition | Assert something is true |
| ASSERT_FALSE | condition | Assert something is false |
| ASSERT_EQ | expected, actual | Assert something is equal |
| ASSERT_NEQ | expected, actual | Assert something is not equal |
| ASSERT_GT | expected, actual | Assert something is greather than a value |
| ASSERT_LT | expected, actual | Assert something is smaller than a value |
| ASSERT_GT_EQ | expected, actual | Assert something is greather or equal to a value |
| ASSERT_LT_EQ | expected, actual | Assert something is lower or equal to a value |
| ASSERT_THROW | statement, exception_type | Assert something throws |
| ASSERT_NO_THROW | statement | Assert something does not throw |
| PROBARE_MESSAGE | message | Savely output message (thread save) |
| PROBARE_ERROR | message | Savely output error message (thread save) |


## Globals

Everything is found in the `probare` namespace

| Name | Type | Functionality |
| --- | --- | --- |
| tests() | `probare::TestEntryVec` | This variable contains all test entries |
| testing() | `probare::TestStats` | This variable contains the stats of the test |
| message_mutex() | `std::mutex` | This variable is the message mutex used by the library for thread save output |


## Add test manually

> The `TEST`/`TEST_NO_PARALLEL` define already does this automatically

You can manually add a test via the `make_test` function

A test needs a name, a function and the information if it is ran serial or parallel

```cpp
namespace probare {
    TestEntryPtr make_test(const std::string &name_,
                           std::function<void()> function_, bool serial_);
}
```

> It will return a shared pointer to the test struct if you need access to that information

### Example

```cpp
#include <probare>

void my_manual_test();

namespace {
    inline auto my_manual_test_info = ::probare::make_test("my_manual_test", my_manual_test, true);
}

void my_manual_test(){
    ASSERT_TRUE(true);
}
```

## Entrypoint

You can include `probare_entry` to use the entrypoint recommended

Alternatively you can define your own entry point and run tests via `probare::run_tests(bool parallel)` if `probare` is included

```cpp
#include <probare_entry>
```

### Defines

Probare lets you define some variables which have an effect on the way your tests run that you can declare

> These need to be declared before including `probare_entry`


| Name | Functionality |
| - | - |
| `NO_PARALLEL` | This runs all tests in serial instead of parallel |
| `PROBARE_EXPECTED_TESTS <int>` | This lets you validate the amount of tests you declared |
| `PROBARE_EXPECTED_PASSES <int>` | This lets you validate the amount of assertions/tests are completed |

#### NO_PARALLEL

This is usefull if you want to run all tests without them being executed parallel.

> You won't really need to wory about race conditions and thread safety,
> but it might slow down your tests significantly.

#### PROBARE_EXPECTED_TESTS

This lets you declare the total number of tests that you declared

This is usefull for validating the amount of tests you expect to run

> Needs to be updated manually if it changes

The benefit you get is that it will error if the tests don't match so that it catches not included tests that you expect to be ran

#### PROBARE_EXPECTED_PASSES

This lets you declare the total number of passes that are increased every time an assert succeeds

This is usefull vor validating the amount of passes that you expect from running the test

> Needs to be updated manually if it changes

The benefit you will get from this is that it will error at the end if you have more or less than expected passes

## Examples

You can find this in the tests directory

The following test is there to test the basic behaviour of the functionality of this library

```cpp
#include <probare_entry>

TEST(self_test_sample) // This will create a test named self_test_sample
{
    ASSERT_FLOAT_EQ(0.1 + 0.2, 0.3, 1e-9);
    ASSERT_STR_EQ("abc", std::string("abc"));
    ASSERT_MATCH("file_123.txt", "^file_\\d+\\.txt$");
    ASSERT_TRUE(true);
    ASSERT_FALSE(false);
    ASSERT_TRUE(!false);
    ASSERT_FALSE(!true);

    ASSERT_EQ(5, 2 + 3);
    ASSERT_NEQ(5, 3 + 3);

    ASSERT_THROW(throw std::runtime_error("oops"), std::runtime_error);
    ASSERT_NO_THROW(int x = 42; (void)x;);
}
```

The following test is an example of a non parallel test

```cpp
#include <probare>

TEST_NO_PARALLEL(serial_registration_verification)
{
    bool found = false;

    for (const auto &test : probare::tests) {
        if (test.no_parallel) {
            if (test.name == "serial_registration_verification") {
                found = true;
            }
        }
    }

    ASSERT_TRUE(found);
}
```
