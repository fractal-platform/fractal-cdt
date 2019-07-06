Hello World!
================

Step 1. Setup build environment
--------------------------------

You can find two ways to setup the build environment for fractal contract development.

 | 1. Download pre-build cdt zip-files from http://www.github.com/fractal-platform/.

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

Below the ``contract.hpp`` header file is included. The contract.hpp file includes a few classes required to write a smart contract.

.. code-block:: CPP 

    #include <ftllib/contract.hpp>

Using the ftl namespace will reduce clutter in your code.

.. code-block:: CPP 

    using namespace ftl;

Create a standard C++ class. The contract class needs to extend ``ftl::contract`` class which is defined in the ``contract.hpp`` file.

.. code-block:: C 

    #include <ftllib/contract.hpp>

    class [[ftl::contract]] hello : public ftl::contract {};

The ``using`` declaration will allow us to write more concise code.

.. code-block:: C 

    #include <ftllib/contract.hpp>

    using namespace ftl;

    class [[ftl::contract]] hello : public contract {
    public:
    };

An empty contract doesn't do much good. Add a public access specifier and a using-declaration. 

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

The above action accepts a parameter called ``user`` that's a ``std:string type``. 

As is, the ABI GLOSSARY:ABI generator in ``fractal-cpp`` won't know about the hi() action without an attribute. Add a C++ style attribute above the action, this way the abi generator can produce more reliable output.

Finally, dispatch the action in smart contract.

Everything together, here's the completed hello world contract head file dispatcher.hpp should be included in and use ``FTL_DISPATCH`` to dispatcher actions.

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

Above command generates two file hello.wasm and hello.abi. hello.wasm is the file for web assembly byte code, and hello.abi is the api description file for contract.

Step 4. Test contract code
------------------------------

You can test the contract as follows:

.. code-block:: bash

    wasmtest --wasm hello.wasm --abi hello.abi --action hi --args '["Alice"]' exec



