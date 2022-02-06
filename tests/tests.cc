#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif

#include "catch.hpp"

TEST_CASE("test", "[test1]") {
    REQUIRE(1 == 1);
    REQUIRE_FALSE(3 == 1);
}