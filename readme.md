# Cache Server Transfer Protocol (CSTP)

A simple (mostly) binary transmission protocol over sockets, to retrieve cached key-value paired information. 



# Protocol Design 

```
    | Bytes
    |(1)        (4)           (8)         not > value of byte(2,3,4,5)
    | 1         2345       6 ---> 13    14+    
    | =         ====       ========     ======================->
    | ^         ^--^       ^ -----^     ^  
    | |         |  |       |      |     |
    | Op Code   \  |       Key Hash     |
    | (operation)\ |                    |
    |             Size of Data          Data start, read in chunks of N bytes
    |
```
# Operation (Op) Codes

| Op Code | Description | Requirements                                                                    |
|---------|-------------|---------------------------------------------------------------------------------|
 | @       | Get         | Op Code, 4 empty bytes, 64 bit key hash                                         |
| I       | Insert      | op code, 4 byte value of data size to be inserted, 64 bit key hash, binary data |
 | !       | DELETE      | op code, 4 empty bytes, 64 bit key hash                                         |
| %       | UPDATE      | op code, 4 empty bytes, 64 bit key hash, binary data                            |

