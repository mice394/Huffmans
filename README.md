# Huffmans
## How to compile and run encoder and decoder
1. In command-line, type `g++ -std=c++17 <file name> -o <executable name>` to compile
2. Run program `./<executable name> < <input file name>`

Note: To encoder produces 2 lines. First line is character frequency which is for decoder. Second line is actual encoding. 

If you input the output of the encoder into the decoder, you will get your original text back.

## Using the sample input and output files
1. Compile both programs
2. To use sample files, type `./encoder < sampleInput.txt > myOutput.txt`
3. Then, `./decoder < myOutput.txt > myDecodedText.txt`
4. `myDecodedText.txt` and `sampleInput.txt` should be the same
5. Similarly, you can do the same with `sampleOutput.txt` but with the decoder first
