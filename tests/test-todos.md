# Test Todos

- check tokenization for `echo asdad 2>1`
- `&> 2>1` should generate `bash: syntax error near unexpected token 2`
- `ls bla>&2 bla | &> moep echo "hi" | <infile wc -w` or something similar
