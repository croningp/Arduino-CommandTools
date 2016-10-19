# CommandServo

Device bonjour id is ```SHT1X```

## Incoming

| Command ID | operand(s) | Action |
|:-:|---|---|
|```RF```| | Request temperature in Fahrenheit <br> Send back ```F``` command|
|```RC```| | Request temperature in Celsius <br> Send back ```C``` command|
|```RH```| | Request humidity in percent <br> Send back ```H``` command|


## Outgoing

| Command ID | operand(s) | Action |
|:-:|---|---|
|```F```| ```float``` temperature in Fahrenheit| Actual temperature in Fahrenheit|
|```C```| ```float``` temperature in Celsius| Actual temperature in Celsius|
|```H```| ```float``` humidity [0,100]| Actual humidity in percentage|
