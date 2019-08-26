Token Contract
================

In previous articles, we have learned some basic syntax and data persistence of smart contracts. In this chapter, we will introduce token contract, which can publish different tokens.

Step1. Create token.hpp
----------------------------

In the header file, you need to include the necessary system header file and declare a token class.

.. code-block:: C

    #pragma once
    #include <ftllib/contract.hpp>

    using namespace ftl;
    using namespace std;

    class [[ftl::contract("token")]] token : public contract {

    public:

    private:
    }

Step2. Declare table
------------------------------

In the token contract, we need a table to record the contract information. First, the table stat is responsible for recording the information of token, including the name of token, the total amount and the dispatcher.

.. code-block:: C

    struct [[ftl::table]] stat {
        struct _key{
            uint64_t type;
        } key;
        uint64_t supply;
        uint64_t max_supply;
        address issuer;
    };

    typedef table<"stat"_n,stat> stats;

Table account is responsible for recording the number of tokens held by each user. This contract can publish multiple tokens, therefore, the combination of user address and token symbol, serves as the primary key of the account table.

.. code-block:: C

    struct [[ftl::table]] account{
        struct _key {
            address owner;
            uint64_t type;
        } key;
        uint64_t balance;
    };

    typedef table<"account"_n, account> accounts;

Step3. Declare action
---------------------------------

First, declare an action ``create`` to create tokens. Users call this action to publish their own token. The required parameter is a variable of type asset, which defines the token's label and total amount.

.. code-block:: c

    [[ftl::action]]
    void create(asset maximum_supply);

Then, declare an action ``issue`` to issue tokens. This action issues tokens to user. The required parameter is the recipient's address and the amount of tokens.

.. code-block:: C

    [[ftl::action]]
    void issue(vector<uint8_t> to,asset quantity);

Declare an action ``retire`` to retire user's tokens. This action retires tokens from some user and purges them.
The required parameter is the user's address and the amount of tokens.

.. code-block:: C

    [[ftl::action]]
    void retire(vector<uint8_t> from, asset quantity);

Declare an action ``transfer`` to transfer tokens to user. User calls this action to transfer tokens to designated recipient.
The required parameter is the recipient's address and the amount of tokens.

.. code-block:: C

    [[ftl::action]]
    void transfer (vector<uint8_t> to, asset quantity);

Now the complete header file shown is as follows:

.. code-block:: C

    #pragma once
    #include <ftllib/map.hpp>
    #include <ftllib/contract.hpp>
    #include <ftllib/asset.hpp>
    #include <ftllib/symbol.hpp>

    using namespace ftl;
    using namespace std;

    class [[ftl::contract("token")]] token : public contract {
    public:
        [[ftl::action]]
        void create(asset maximum_supply);

        [[ftl::action]]
        void issue(vector<uint8_t> to,asset quantity);

        [[ftl::action]]
        void retire(vector<uint8_t> from,asset quantity);

        [[ftl::action]]
        void transfer (vector<uint8_t> to,asset quantity);

    private:
        struct [[ftl::table]] account {
            struct _key {
                address owner;
                uint64_t type;
            } key;
            uint64_t balance;
        };

        struct [[ftl::table]] stat { 
            struct _key {
                uint64_t type;
            } key;
            uint64_t supply;
            uint64_t max_supply;
            address issuer;
        };

        typedef table<"account"_n, account > accounts;
        typedef table<"stat"_n,stat> stats;
    };


Step4. Perform actions
-----------------------
``create`` token.cpp and include token.cpp.

*create*

.. code-block:: CPP

    void token::create(asset maximum_supply) {
        address issuer = get_from_address();
        auto sym = maximum_supply.symbol;
        check(sym.is_valid(), "invalid symbol name");
        check(maximum_supply.is_valid(), "invalid supply");
        check(maximum_supply.amount > 0, "max-supply must be positive");

        stats statstable;
        bool existing = statstable.has_key(maximum_supply.symbol.code().raw());
        check(existing == false, "token with symbol already exists");

        statstable.emplace([&](auto &s) {
            s.key.type = maximum_supply.symbol.code().raw();
            s.max_supply = maximum_supply.amount;
            s.issuer = issuer;
        });
    }

First, call ``get_from_address`` function to get the caller of the action. Next, check the name of the newly defined token, the validity of the amount and whether the name of token already exists in this contract. After all checks are passed, the new token information is recorded in the table stat and the caller is set to be the issuer.

*issue*

.. code-block:: CPP

    void token::issue(vector<uint8_t> to, asset quantity) {
        auto sym = quantity.symbol;
        check(sym.is_valid(), "invalid symbol name");
        stats statstable;
        auto existing = statstable.has_key(sym.code().raw());
        check(existing == true, "token with symbol does not exist, create token before issue");

        stat st = statstable.get(sym.code().raw());
        address issuer = get_from_address();
        check(issuer == st.issuer,"Wrong issuer");
        check(quantity.is_valid(), "invalid quantity");
        check(quantity.amount > 0, "must issue positive quantity");
        check(quantity.amount <= st.max_supply - st.supply, "quantity exceeds available supply");

        statstable.emplace([&](auto &s) {
            s.key.type = sym.code().raw();
            s.supply += quantity.amount;
            s.max_supply = st.max_supply;
            s.issuer = st.issuer;
        });
        add_balance(to, quantity);
    }


``issue`` verifies whether the name of the input asset is valid and exists in the table stat. Next, check whether the user calling the issue method has permission to issue the specified token. Check whether the input asset is valid and its amount is greater than zero. Check whether the name of the input asset is the same as the asset name found in stat. Check whether there is sufficient balance to issue token. After all the above verification, the amount of token issued in stat is increased, and the token is issued to the specified user using the `add_balance` function.

*retire*

.. code-block:: CPP

    void token::retire(vector<uint8_t> from, asset quantity) {
        auto sym = quantity.symbol;
        check(sym.is_valid(), "invalid symbol name");
        stats statstable;
        auto existing = statstable.has_key(sym.code().raw());
        check(existing == true, "token with symbol does not exist, create token before issue");
        
        stat st = statstable.get(sym.code().raw());
        address issuer = get_from_address();
        check(issuer == st.issuer,"Wrong issuer");
        check(quantity.is_valid(), "invalid quantity");
        check(quantity.amount > 0, "must retire positive quantity");
        check(quantity.amount <= st.max_supply, "quantity exceeds available supply");
        sub_balance(from, quantity);
    }

``retire`` checks the validity of the name of the input asset and whether the asset exists in the table stat. Check whether the contract caller has permission to call retire action. Check whether the input asset is valid and its amount is greater than zero. Check whether the name of the input asset is the same as the asset name found in stat. Check whether the amount of the input asset exceeds the maximum value when token is defined. After the inspection is passed, the token of the specified amount will be withdrawn from the designated account and burned.

*transfer*

.. code-block:: CPP

    void token::transfer(vector<uint8_t> to, asset quantity){
        vector from = get_from_address();
        check(from != to, "cannot transfer to self");

        auto sym = quantity.symbol.code();

        stats statstable;
        const auto &st = statstable.get(MapKey(sym.raw()));
        check(quantity.is_valid(), "invalid quantity");
        check(quantity.amount > 0, "must transfer positive quantity");

        sub_balance(from, quantity);
        add_balance(to, quantity);
    }

``transfer`` checks that the outbound account is not same as the inbound account. Check the validity of the input asset, whether the amount is greater than zero, and whether the name is the name found from the table of stat. After all the checks, outbound account deducts the corresponding amount and inbound account increases the corresponding amount.

To transfer tokens, we need to implement the account balance addition and subtraction functions, ``add_balance`` and ``sub_balance``.

*add_balance*

.. code-block:: CPP

    void token::add_balance(vector<uint8_t> owner, asset value) {
        check(value.amount > 0, "add negative assert");

        accounts account;
        bool has = account.has_key(owner, value.symbol.code().raw());
        if (!has) {
            account.emplace([&](auto &s) {
                s.key.owner = owner;
                s.key.type = value.symbol.code().raw();
                s.balance = value.amount;
            });
        } else {
            account.emplace([&](auto &s) {
                s.key.owner = owner;
                s.key.type = value.symbol.code().raw();
                s.balance += value.amount;
         	});
        }
    }

The ``add_balance`` function verifies that the amount added is positive. If the account exists then increases its balance else set the balance to be increased amount.

*sub_balance*


.. code-block:: CPP

    void token::sub_balance(vector <uint8_t> owner, asset value) {
        check(value.amount > 0, "sub negative assert");

        accounts account;
        bool has = account.has_key(owner, value.symbol.code().raw());
        check(has == true, "This is an empty account!");
        check(account.get(owner, value.symbol.code().raw()).balance >= value.amount,
            "account don not has enough tokens");

        account.emplace([&](auto &s) {
            s.key.owner = owner;
            s.key.type = value.symbol.code().raw();
            s.balance -= value.amount;
        });
    }

The ``sub_balance`` function verifies that the reduced balance is positive, the account exists and has sufficient balance. After the check is passed, the corresponding account balance is decreased.

See complete code in https://github.com/fractal-platform/fractal-contract/tree/v0.1.x.

Step5. Deploy and test smart contract
----------------------------------------

To deploy and test smart contract, you should construct a fractal chain locally or link it to the fractal test network.

Learn how to create private chain and link to fractal test network, `click here`_.

.. _`click here`: https://fractal-doc.readthedocs.io/en/latest/

Here we use gtool to deploy and call contract locally.

a). Deploy token contract. To ensure successful deployment, the address you used to deploy contract should have enough balance.

.. code-block:: bash

    gtool tx --rpc http://127.0.0.1:8545 --chainid 999 --keys data1/keys/ --pass 666 --wasm ./token.wasm deploy

Then you can get the contract address from the console. Check contract deployment status using following command.

.. code-block:: bash

    gtool state --rpc http://127.0.0.1:8545 --addr $CONTRACTADDR account

b). Create your own token ``CAT``

.. code-block:: bash

    gtool tx --rpc http://127.0.0.1:8545 --chainid 999 --keys data1/keys/ --pass 666 --to $CONTRACTADDR --abi token.abi --action create --args '["1000000000000.0000 CAT"]' call

Then verify whether you have successfully created token ``CAT``.

.. code-block:: bash

    gtool state --rpc http://127.0.0.1:8545 --addr $CONTRACTADDR --table stat --skey 0x46326ce0000000004341540000000000 storage

c). Issue 100.0000 ``CAT`` to your own address.

.. code-block:: bash

    gtool tx --rpc http://127.0.0.1:8545 --chainid 999 --keys data1/keys/ --pass 666 --to $CONTRACTADDR --abi token.abi --action issue --args '["$YOURADDR"，"100.0000 CAT"]' call

Verify that you have received the tokens successfully. Note that, here the key used to look up user's account is the combination of the address you are looking for encoded in the token's type.

.. code-block:: bash

    gtool state --rpc http://127.0.0.1:8545 --addr $CONTRACTADDR --table account --skey 0x${USERADDR}4341540000000000 storage

d). Retire 10.0000 ``CAT`` from your address.

.. code-block:: bash

    gtool tx --rpc http://10.1.1.169:8545 --chainid 999 --keys data1/keys/ --pass 666 --to $CONTRACTADDR --abi token.abi --action issue --args '["$YOURADDR","10.0000 CAT"]' call

e). Transfer 10.0000 ``CAT``.

.. code-block:: bash

    gtool tx --rpc http://10.1.1.169:8545 --chainid 999 --keys data1/keys/ --pass 666 --to $CONTRACTADDR --abi token.abi --action tranfer --args '["$USERADDR2"，"10.0000 CAT"]' call

Note that, the format of $CONTRACTADDR is 0xXXXXX, while user address does not have "0x".
