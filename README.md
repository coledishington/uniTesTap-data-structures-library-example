# uniTesTap - testing examples with data structures

uniTesTap's (unit-test-tap) purpose is a standalone Test Anything Protocol (TAP) producer library for simplifying writing unit tests for automake.

* Test isolation via per process tests
* Concurrent test runs
* Plain c syntax for test functions
* Standalone library for reduced integration complexity

For more information go to the [uniTesTap GitHub repo](https://github.com/coledishington/uniTesTap).

This project has some basic usecases for using uniTesTap to write unit tests for consumption bya TAP test harness, specifically for automake.

# Building uniTesTap examples

For quickstart and most usecases, executing

    ./autogen.sh

will build the project outside of the source tree in <code>./build</code>. For building and running all the unit tests, execute

    ./autogen.sh --check

For further options check <code>./autogen --help</code>. If contributing, make sure to use the <code>--clean</code> and <code>--check</code> options of <code>autogen.sh</code>. For more complex use-cases, use the autotools toolset (e.g. <code>autoreconf</code>, <code>./configure</code>, and <code>make</code>).

# Contributors

Before submitting any patches, please run <code>./scripts/checkpatch.sh</code> and <code>./autogen.sh --check</code> over each commit.

# License

uniTesTap is licensed under [LGPLv3](COPYING.LESSER).