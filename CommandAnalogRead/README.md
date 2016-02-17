## CommandAnalogRead

Device bonjour id is ```ANALOGREAD```

## Incoming

| Command ID | operand(s) | Action |
|:-:|---|---|
|```R```| | Request analog pin level <br> Send back ```L``` command|


## Outgoing

| Command ID | operand(s) | Action |
|:-:|---|---|
|```L```| ```int``` level [0,1023] | Level of the analog pin|
