Hello World!
================

Step 1. Setup build environment
--------------------------------

There are two ways to setup the build environment for fractal contract development.

 | 1. Download pre-build cdt zip-files from https://github.com/fractal-platform/fractal-cdt/releases

 | 2. Build cdt tools from source, reference http://www.github.com/fractal-platform/fractal-cdt for more details.

Setup env for build environment (for macos):

.. code-block:: bash 

    cd $HOME
    tar zxvf fractal-cdt-bin.macos.v0.1.0.tgz
    export PATH=$PATH:$HOME/fractal-cdt-bin/bin


Step 2. Write contract code
----------------------------

Create a new directory called "hello", and create a new file named "hello.cpp".

.. code-block:: bash 

    mkdir hello
    cd hello
    touch hello.cpp

Include the ``contract.hpp`` header file, which import several classes that are required to write a smart contract.

.. code-block:: CPP 

    #include <ftllib/contract.hpp>

Using the ftl namespace will de-clutter your code.

.. code-block:: CPP 

    using namespace ftl;

Create a standard C++ class. The contract class needs to extend ``ftl::contract`` class which is defined in the ``contract.hpp`` header file.

.. code-block:: C 

    #include <ftllib/contract.hpp>

    class [[ftl::contract]] hello : public ftl::contract {};

The ``using`` declaration allows us to write more concise code.

.. code-block:: C 

    #include <ftllib/contract.hpp>

    using namespace ftl;

    class [[ftl::contract]] hello : public contract {
    public:
    };

An empty contract wouldn't do any good. Add a public access specifier and a using-declaration.

.. code-block:: C 

    #include <ftllib/contract.hpp>

    using namespace ftl;

    class [[ftl::contract]] hello : public contract {
    public:
        [[ftl::action]]
        void hi( std::string user ) {
            print( "Hello, ", user);
        }
    };

The above action accepts a parameter called ``user`` that's a ``std:string`` type.

As is, the ABI GLOSSARY:ABI generator in ``fractal-cpp`` won't be able to call the hi() action without an attribute. Add a C++ style attribute above the action, this allows the abi generator to produce a more reliable output.

Finally, dispatch the action in smart contract.

Put everything together, here's the complete hello world contract. Header file dispatcher.hpp should be included and use ``FTL_DISPATCH`` to dispatch actions.

.. code-block:: C 

    #include <ftllib/contract.hpp>
    #include <ftllib/dispatcher.hpp>

    using namespace ftl;

    class [[ftl::contract("hello")]] hello : public contract {
    public:
        hello(datastream<const char*> ds):contract(ds) {}
        [[ftl::action]]
        void hi( std::string user ) {
            print( "Hello, ", user);
        }
    };
    FTL_DISPATCH(hello,(hi))

Step 3. Compile contract code
------------------------------

You can compile your code to web assembly (.wasm) as follows:

.. code-block:: bash 

    fractal-cpp -o hello.wasm hello.cpp

The above command generates two files, hello.wasm and hello.abi. hello.wasm is the web assembly byte code, and hello.abi is the interface for the contract.

Step 4. Test contract code
------------------------------

You can test the contract as follows:

.. code-block:: bash

    wasmtest --wasm hello.wasm --abi hello.abi --action hi --args '["Alice"]' exec



