# FEXTools

## Objetivos
FEXTools tem como objetivo diminuir a dificuldade na utilização das bibliotecas ROOT disponibilizadas pelo CERN nomeadamente no das cadeiras de física experimental no curso de Engenharia Física Tecnológica no Instituto Superior Técnico.

## Instalação
A instalação desta biblioteca requer a utilização do ROOT (https://root.cern/), sendo que, apesar de apenas ter sido testada na versão 3fa90959a1c874646e0074e09caf67be75316c0f (https://github.com/root-project/root/tree/3fa90959a1c874646e0074e09caf67be75316c0f), não utiliza as funcionalidades mais recentes deste software.

### Linux
1. Criar e ir para a diretoria onde se pertende guardar a biblioteca
2. Fazer o download da biblioteca (por exemplo '''git clone ''')

## Utilização
Todas as funcionalidades desta biblioteca encontram-se demonstradas no ExampleProject/ presente nesta directoria.

### Definir função a ajustar
1. Declarar e definir o valor inicial das variáveis necessárias
    - Uma variável por linha
    - Deixar pelo menos um espaço vazio entre a variável e o sinal de igualdade, assim como entre este e o valor incial
    - A keyword Const certifica-se que o valor da variável não irá ser alterado
    - Exemplo: '''Const Epsilon = 3'''
2. Definir função
    - Uma linha com a keyword '''#Function''' seguida por um espaço em branco e a declaração da função
    - Usar as expressões usuais da '''TFormula''' (x**n em vez de pow(x,n); ...)
    - A variável dependente é '''x'''
    - Exemplo: '''#Function Epsilon*x'''

### Compilação
Utilizar o cmake presente na pasta ExampleProject ('''mkdir build && cd build && cmake .. && make && ./$(PROJECTNAME)'''). Caso necesário alterar o nome do projeto o path da instalação.

## Bugs conhecidos
~~O DataStack não funciona, de todo.~~

## Planos futuros
1. Adicionar DataSets e funções 3D
2. Melhorar os CMakes
3. Limpar o código, em geral
4. ...