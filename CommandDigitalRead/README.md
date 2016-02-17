## CommandDigitalRead

Device bonjour id is ```DIGITALREAD```

## Incoming

| Command ID | operand(s) | Action |
|:-:|---|---|
|```R```| | Request pin state <br> Send back ```S``` command|


## Outgoing

| Command ID | operand(s) | Action |
|:-:|---|---|
|```S```| ```bool``` status | State of the pin|
