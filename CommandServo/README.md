# CommandServo

Device bonjour id is ```SERVO```

## Incoming

| Command ID | operand(s) | Action |
|:-:|---|---|
|```W```| ```int``` angle [0,180]| Set servo angle|
|```R```| | Request servo angle <br> Send back ```A``` command|


## Outgoing

| Command ID | operand(s) | Action |
|:-:|---|---|
|```A```| ```int``` angle [0,180]| Actual servo angle|
