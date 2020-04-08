# CommandPCA9548A

Device bonjour id is ```PCA9548A```

## Incoming

| Command ID | operand(s) | Action |
|:-:|---|---|
|```W```| ```int,...``` list of integers [1,8]| Set enabled channels|
|```R```| | Request list of enabled channels <br> Sends back ```C``` command|


## Outgoing

| Command ID | operand(s) | Action |
|:-:|---|---|
|```C```| ```int,...``` list of integers [1,8]| List of channels currently enabled|
