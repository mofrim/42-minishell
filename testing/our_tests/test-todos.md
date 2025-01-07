# Test Todos

- check tokenization for `echo asdad 2>1`
- `&> 2>1` should generate `bash: syntax error near unexpected token 2`
- `ls bla>&2 bla | &> moep echo "hi" | <infile wc -w` or something similar
- `echo "Asdsd" 123>&34 2>err` should still echo `bash: 34: Bad file
  descriptor` to stdout because the redirection to file `err` is not processed!
  `echo "Asdsd" 2>err 123>&34` should instead print that stuff to file called
  `err`
