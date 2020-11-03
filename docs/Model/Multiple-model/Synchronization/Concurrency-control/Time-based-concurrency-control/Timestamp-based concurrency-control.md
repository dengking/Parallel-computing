# Timestamp-based concurrency control



## wikipedia [Timestamp-based concurrency control](https://en.wikipedia.org/wiki/Timestamp-based_concurrency_control)

In [computer science](https://en.wikipedia.org/wiki/Computer_science), a **timestamp-based concurrency control** algorithm is a [non-lock concurrency control](https://en.wikipedia.org/wiki/Non-lock_concurrency_control) method. It is used in some [databases](https://en.wikipedia.org/wiki/Database) to safely handle transactions, using [timestamps](https://en.wikipedia.org/wiki/Timestamp).

### Operation

#### Formal

Each transaction ({\displaystyle T_{i}}![T_{i}](https://wikimedia.org/api/rest_v1/media/math/render/svg/a8dd1c50cb9436474f83624c3f679ccf3eebbfef)) is an ordered list of actions ({\displaystyle A_{ix}}![A_{{ix}}](https://wikimedia.org/api/rest_v1/media/math/render/svg/4958957970b4491369e7fac10e2d12b04f6948a9)). Before the transaction performs its first action ({\displaystyle A_{i1}}![A_{{i1}}](https://wikimedia.org/api/rest_v1/media/math/render/svg/8c8742e81c54fdaf86436ebeb6120fe9b927210f)), it is marked with the current [timestamp](https://en.wikipedia.org/wiki/Timestamp), or any other [strictly totally ordered](https://en.wikipedia.org/wiki/Total_order) sequence: {\displaystyle TS(T_{i})=NOW()}![TS(T_{i})=NOW()](https://wikimedia.org/api/rest_v1/media/math/render/svg/706a8416b672aa1fe7f3e82a33d7c9713d1b1dd1). Every transaction is also given an initially empty set of transactions upon which it depends, {\displaystyle DEP(T_{i})=[]}![DEP(T_{i})=[]](https://wikimedia.org/api/rest_v1/media/math/render/svg/7d815e65d336bc8cab691a1110a371787f6d11de), and an initially empty set of old objects which it updated, {\displaystyle OLD(T_{i})=[]}![OLD(T_{i})=[]](https://wikimedia.org/api/rest_v1/media/math/render/svg/911907b187ee4936ee803d14c7a0bbe3bd9ee233).

Each [object](https://en.wikipedia.org/wiki/Object_(computer_science)) {\displaystyle (O_{j})}![(O_{j})](https://wikimedia.org/api/rest_v1/media/math/render/svg/f8a7e89597614413f9f7a37034b92f25edb71a08) in the database is given two timestamp fields which are not used other than for concurrency control: {\displaystyle RTS(O_{j})}![RTS(O_{j})](https://wikimedia.org/api/rest_v1/media/math/render/svg/f3a766010928e2ea82314d7411938fa5e9f8673f) is the time at which the value of object was last used by a transaction, {\displaystyle WTS(O_{j})}![WTS(O_{j})](https://wikimedia.org/api/rest_v1/media/math/render/svg/5b84fa7a3d2d0cab8f7521728bb4708b24cfba32) is the time at which the value of the object was last updated by a transaction.

For all {\displaystyle T_{i}}![T_{i}](https://wikimedia.org/api/rest_v1/media/math/render/svg/a8dd1c50cb9436474f83624c3f679ccf3eebbfef):



To **abort**:

