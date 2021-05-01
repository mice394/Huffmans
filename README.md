# Huffmans
## How to compile and run encoder and decoder
1. In command-line type `g++ -std=c++17 <file name> -o <executable name>`
2. Run program `./<executable name> < <input file name>`

Note: To encoder produces 2 lines. First line is character frequency which is for decoder. Second line is actual encoding. 

If you input the output of the encoder into the decoder, you will get your original text back.
