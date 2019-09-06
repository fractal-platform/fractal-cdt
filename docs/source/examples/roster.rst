Data Persistence
================

To learn about data persistence, write a simple smart contract that functions as a roster. Fractal system use a map struct to store data.

Step 1. Write a fractal contract class
-------------------------------------------
Create a new directory called "roster", and create a new file named "roster.cpp".

.. code-block:: bash 

    mkdir roster
    cd roster
    touch roster.cpp

In a previous tutorial, you created a hello world contract and you learned the basics. You will be familiar with the structure below, the class has been named ``roster`` respectively.

.. code-block:: C 

    #include <ftllib/contract.hpp>

    using namespace ftl;

    class [[ftl::contract("roster")]] roster : public contract {
    public:
       
    private:
  
    };

Step 2. Create the data structure for the table
------------------------------------------------
Before a table can be configured and instantiated, a struct that represents the data structure of the roster needs to be written. Since it's an roster, the table will contain members, so create a ``struct`` called "member".

.. code-block:: CPP 

    struct member{};

When defining the structure of a map table, you will require a unique value to use as the primary key.

For this contract, use a field called "name" with type address. This contract will have one unique entry per user, so this key will be a consistent and guaranteed unique value based on the user's address.

.. code-block:: CPP 

    struct member {
        struct _key{
            address name;
        } key;
    }

Since this contract is a roster it probably should store some relevant details for each member.

.. code-block:: CPP 

    struct member {
        struct _key{
            address name;
        } key;
        std::string sex;
        std::string age;
        std::string birthday;
        std::string situation;
    };

Step 3. Configure the table
----------------------------------------
Now that the data structure of the table has been defined with a struct we need to configure the table. The ``ftl::table`` constructor needs to be named and configured to use the struct we previously defined.

.. code-block:: CPP 

    typedef table<"member"_n, member> member_list;

With the above configuration there is a table named member, that

  | 1. Uses the _n operator to define an ftl::name type and uses that to name the table. Note that the table's name should be same with the struct name.
 
  | 2. Pass in the singular member struct defined in the previous step.

  | 3. Declare this table's type. This type will be used to instantiate this table later.

So far, our file should look like this.

.. code-block:: C 

    #include <ftllib/contract.hpp>

    using namespace ftl;

    class [[ftl::contract("roster")]] roster : public contract {

        struct member {
            struct _key{
                address name;
            }key;
            std::string sex;
            std::string age;
            std::string birthday;
            std::string situation;
        };
       
        typedef table<"member"_n, member> member_list;
    };

Step 4. The constructor
-----------------------

When working with C++ classes, the first public method you should create is a constructor.

Our constructor will be responsible for initially setting up the contract.

ftl contracts extend the ``contract`` class. Initialize our parent contract class with the data stream which used to serialize data.

.. code-block:: C++

    roster(datastream<const char*> ds):contract(ds) {}

Step 5. Adding and modify the table
-----------------------------------

This section defines an action for the user to add or update a record. This action will need to accept any values that this action needs to be able to emplace (create) or modify.

Firstly, this method accepts an address type argument and asserts that the account executing the transaction equals the provided value.

.. code-block:: CPP
    
    void upsert(
        address user,
        std::string sex,
        std::string age,
        std::string birthday,
        std::string situation
    ) {}

Then instantiate the table defined above.

.. code-block:: CPP

    void upsert(address name, std::string sex, std::string age, std::string birthday, std::string situation) {
        member_list mems;
    }

Next, insert or update a row in table with the method ``emplace``. This function will 
update the row in the table if the element has existed otherwise insert it into table.

.. code-block:: CPP 

    void upsert(address user, std::string sex, std::string age, std::string birthday, std::string situation) {
        member_list mems;
        mems.emplace([&](auto& r){
            r.key.name = user;
            r.sex = sex;
            r.age = age;
            r.birthday = birthday;
            r.situation= situation;
        });
    }

The ``roster`` contract now has a functional action that will enable a user to create a row in the table if that record does not yet exist, and modify it if it already exists.

Step 6. Remove record from the table
-------------------------------------

This section will tell you how to remove a row from table. Similar to previous step. create a public method in the ``roster`` and instantiate the table.

.. code-block:: CPP 

    void erase(address user){
        member_list mems;
    }

Checkout if the record indeed exists in the table before erasing.

.. code-block:: CPP 

    void erase(address user){
        member_list mems;
        bool exist = mems.has_key(user);
        checkout(exist == false, "Record does not exist");
    }

Then,call the ``erase`` method to erase the record.

.. code-block:: CPP 

    void erase(std::vector<uint8_t> user){
        member_list mems;
        bool exist = mems.has_key(user);
        checkout(exist == false, "Record does not exist");
        mems.erase(user);
    }

Now you can create, modify and erase records form table.

Step 7. Preparing for the ABI
------------------------------

7.1 ABI action declarations
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To generate ABI file, it requires some declarations.

Above both the ``upsert`` and ``erase`` functions add the following C++ declaration:

.. code-block:: C++

    [[ftl::action]]

The above declaration will extract the arguments of the action and create necessary ABI ``struct`` descriptions in the generated ABI file.

7.2 ABI table declarations
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Add an ABI declaration to the table. Modify the following line defined in the private region of your contract:

.. code-block:: C++
    
    struct member{

To this:

.. code-block:: C++
    
    struct [[ftl::table]] member{

Now our contract is ready to be compiled.

Below is the final state of our ``roster`` contract:

.. code-block:: C 

    #include <ftllib/contract.hpp>
    #include <ftllib/dispatcher.hpp>
    #include <ftllib/map.hpp>


    using namespace ftl;
    using namespace std;

    class [[ftl::contract("roster")]] roster : public contract {
    public:

        roster(datastream<const char *> ds) : contract(ds) {}

        [[ftl::action]]
        void upsert(string name, string sex, string age, string birthday, string situation) {

            member_list mems;
            mems.emplace([&](auto &r) {
                r.key.name = name;
                r.sex = sex;
                r.age = age;
                r.birthday = birthday;
                r.situation = situation;
            });
        }

        [[ftl::action]]
        void erase(string name) {
            member_list mems;
            bool exist = mems.has_key(name);
            check(exist == false, "Record does not exist");
            mems.erase(name);
        }

    private:
        struct [[ftl::table]] member {
            struct _key {
                string name;
            } key;

            string sex;
            string age;
            string birthday;
            string situation;

        };

        typedef table<"member"_n, member> member_list;
    };

    FTL_DISPATCH(roster, (upsert)(erase))

Step 8. Compile the contract
-----------------------------
Execute the following command from your terminal.

.. code-block:: bash

    fractal-cpp -o roster.wasm roster.cpp


Step 9. Test the Contract
-----------------------------       

Insert or update a record:

.. code-block:: bash

    wasmtest --wasm roster.wasm --abi roster.abi --action upsert --args '["Wang","male","26","1993-7-1","student"]' exec

Romove a record:

.. code-block:: bash

    wasmtest --wasm roster.wasm --abi roster.abi --action erase --args '["Wang"]' exec

