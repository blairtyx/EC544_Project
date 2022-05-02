# Decode RS code in remote server


## Verify local codec and remote codec:

Related code: `rs_sqlite.c`

Compile with: 
```sh
# compile rs library
g++ -c rscombo.c -o rscombo.o
# compile rs_sqlite
g++  -o rs_sqlite rscombo.o rs_sqlite.c 
```

Here we compare the char array in hex between a) local generated codeword and b) transmited then concatenated codeword, to prove that using FRDM-K64F as the encoder then transmit both the encoded parity bit and original data to a remote decoder is feasible. 

rs_sqlite result: 

```
pi@raspberrypi:~/Workspace/Reed-Solomon-Encoder-Decoder- $ ./rs_sqlite "(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78)," "7D7056AFB0422F5F8B82E00EEADEE762"
ML: 241
Origin data is: (  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),
Encoded data is: (  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),
Try hex representation : 
size of msg: 225 size of codeword: 241
282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C00

282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C007D7056AFB0422F5F8B82E00EEADEE762

7D7056AFB0422F5F8B82E00EEADEE762
with erasures: "~~~~~~~~~~~~~~~~(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),"
Corrected codeword: "(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),"
7D7056AFB0422F5F8B82E00EEADEE762
282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C007D7056AFB0422F5F8B82E00EEADEE762

```

## Remote decoder Test

Related code: `rs_decoder.c`

Compile with: 
```sh
# compile rs library
g++ -c rscombo.c -o rscombo.o
# compile rs_decoder
g++  -o rs_decoder rscombo.o rs_decoder.c 
```

In this decoder, we pass two string parameters to the executable file, the first one is the concated original string, and the second one is the parity string in hex format. 



decode result, with build-in erasure simulation.

```sh
pi@raspberrypi:~/Workspace/Reed-Solomon-Encoder-Decoder- $ ./rs_decoder "(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78)," "7D7056AFB0422F5F8B82E00EEADEE762"
ML: 241

********************* Test Fake String *********************
7D7056AFB0422F5F8B82E00EEADEE762
282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C007D7056AFB0422F5F8B82E00EEADEE762

with erasures: "~~~~~~~~~~~~~~~~(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),"
Corrected codeword: "(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),"

```


## Remote decoder

Related code: `rs_decoder.c`

Compile with: 
```sh
# compile rs library
g++ -c rscombo.c -o rscombo.o
# compile rs_decoder
g++  -o rs_decoder rscombo.o rs_decoder.c 
```

In this decoder, we still pass two string parameters to the executable file, notice that the difference is: the first one is the erasure-simulated string. 

The original string is: 
```
(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),
```

to simulate the erasure, we manually put multiple `~` into this string
```
(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),~~~~~~~~~~~~~~~~(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),
```

then use this as the input string to test our decoder

Notice that this parity string `7D7056AFB0422F5F8B82E00EEADEE762` is generated by the encoder running on the FRDM board

decode result: 
```
pi@raspberrypi:~/Workspace/Reed-Solomon-Encoder-Decoder- $ ./rs_decoder "(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),~~~~~~~~~~~~~~~~(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78)," "7D7056AFB0422F5F8B82E00EEADEE762"
ML: 241

********************* Test Fake String *********************
7D7056AFB0422F5F8B82E00EEADEE762
282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C7E7E7E7E7E7E7E7E7E7E7E7E7E7E7E7E282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C282020342E30342C202D332E3738292C007D7056AFB0422F5F8B82E00EEADEE762

find - at 160
find - at 161
find - at 162
find - at 163
find - at 164
find - at 165
find - at 166
find - at 167
find - at 168
find - at 169
find - at 170
find - at 171
find - at 172
find - at 173
find - at 174
find - at 175
Corrected codeword: "(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),(  4.04, -3.78),"
```
