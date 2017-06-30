##### Instituto de Matemática e Estatística - Universidade de São Paulo

## EP3: Cálculo Distribuído do Conjunto de Mandelbrot

###### MAC 5742-0219 Introdução à Programação Concorrente, Paralela e Distribuída

Para compilar os códigos basta executar

```
make
```

Para execução do sequencial:

```
./mandelbrot_seq [coordenadas da região] [tamanho da imagem]
./mandelbrot_seq_io [coordenadas da região] [tamanho da imagem]
```

Para execução do distribuido:

```
mpirun -np [qtd de processos] mandelbrot_mpi [coordenadas da região] [tamanho da imagem]
mpirun -np [qtd de processos] mandelbrot_mpi_io [coordenadas da região] [tamanho da imagem]
```
