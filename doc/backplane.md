
Backplanes for inter-plugin communication
-----------------------------------------

The problem we've identified during the development of the promoted posts plugin is the following:

*How do plugins talk to each other?*

The answer is a design pattern which I (@theoreticalbts) have decided to call a *backplane*.  A backplane provides the following functionality:

- An interface (i.e. abstract base class) defining a method for updating an object (often a variant / variant_object)
- A registry by which other plugins (backplane client plugins) can push implementations of the interface
- An execution method which delegates to some implementation(s) provided by other plugins

The backplane design pattern is a powerful and flexible abstraction.

Use case: Web development plugin
--------------------------------

See #210.

- Replace the `state` object with a `variant_object`
- Each implementation plugin updates the `variant_object` in-place
- Execution method delegates to a chain of implementation objects provided by clients
- We could possibly move URL pattern matching to execution delegation if we have a C++ regex library

Use case: Rank plugin
---------------------

See #222.

- Rank plugin is a backplane for object sorting orders.
- Promoted is implemented alongside rank plugin as test case.
- Backplane is not responsible for creating/indexing peer objects, that's up to backplane client plugins
- With small amount of boilerplate, trending, hot, new, active, payout time, etc. provided by tags plugin can be turned into backplane-compatible ranking implementations
- For case of tags plugin, single plugin provides multiple ranking implementations backed by single peer object
- Keeps existing design, minimal modifications to tags plugin needed
- Backplane provides list of available ranking names
- Execution method delegates to implementation based on supplied ranking name
- Adding a new ranking to the website will be trivial -- frontend can query backend for list of available rankings to build drop-down
- Deprecate and remove functionality from database_api
