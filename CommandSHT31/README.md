# CommandServo

Device bonjour id is ```SHT31```

## Incoming

| Command ID | operand(s) | Action |
|:-:|---|---|
|```RC```| | Request temperature in Celsius <br> Send back ```C``` command|
|```RH```| | Request humidity in percent <br> Send back ```H``` command|


## Outgoing

| Command ID | operand(s) | Action |
|:-:|---|---|
|```C```| ```float``` temperature in Celsius| Actual temperature in Celsius|
|```H```| ```float``` humidity [0,100]| Actual humidity in percentage|
