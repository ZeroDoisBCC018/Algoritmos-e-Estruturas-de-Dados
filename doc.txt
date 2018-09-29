*Membros e números USP:
    -Joao Vitor Villaca - 10724239
    -Bruno Gazoni - 7585037
    -Bruno Baldissera - 10724351

*Compilador usado: 
    -GCC

*Plataformas de desenvolvimento: 
    -Foram utilizados GitHub para compartilhamento de código e GNU/Linux como SO

*Instruções de uso:
   Instruções de uso do programa:
	A versão teste do Googlebot, que está contida no arquivo de código-fonte
main.c está implementado com suas bibliotecas e estruturas de dados personalizadas.
Cada função tem por si mensagens de erro que são transmitidas diretamente no buffer
de erro padrão do sistema linux. Apesar disso, o programa só tem seu processo abortado
caso um erro fatal ocorra durante a execução destas funcões. A maior parte dos erros
deverá ser tratada pelo programa cliente.
	Algumas macros foram utilizadas, entre elas os retornos ERROR (-404), SUCCESS
(1), TRUE (1) e FALSE (0). Além disso, o tipo booleano 'boolean' também está definido.
O retorno booleano está reservado apenas para algumas funções.
	Não há nenhuma garantia de que as bibliotecas e o cliente deste programa
irão funcionar corretamente em algum sistema que não seja baseado em GNU/Linux,
utilizando um compilador diferente do GCC ou com um padrão da linguagem C que não
seja o ANSI C89.
	Este programa está sendo publicado sob o regime da licensa "The Unlicense".
Leia a licensa abaixo:

-----------------------------------------------------------------------

This is free and unencumbered software released into the public domain.

	Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

	In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org> 

-----------------------------------------------------------------------

SOBRE O USO DO PROGRAMA CLIENTE:
	O programa cliente deverá ser compilado em um diretório isolado com
os arquivos C e H que estão na mesma pasta zipada. O uso da makefile que vem
no arquivo zip facilita muito a compilação. As funções podem ser visualizadas
no arquivo de cabeçalho, juntamente com comentários pertinentes e seus retornos
e parâmetros.
	Ao inicializar o programa, um arquivo .csv deve ser referenciado em linha
de comando do Bash. Este será usado como o banco de dados principal. O padrão é
igual ao .csv que está na mesma pasta zipada. O menu do programa oferece algumas
opções com instruções. O menu está composto de duas seções diferentes. A segunda
seção é a mais simples: a base de dados é atualizada com a lista que roda no tempo
de execução do programa. A primeira seção permite que o usuário possa inserir sites,
removê-los, atualizar relevância e inserir tags (keywords). Para confirmar ações, apenas
o código 1 é aceito. Todas as opções de menu podem ser mostradas novamente no console
inserindo o respectivo código que é mostrado em cada seção.
	Caso seu computador não possua espaço de memória stack, heap ou de dados, o programa
será abortado na tentativa de criar arquivos ou listas para operar a base de dados. 
   
    
*Justificativas para as escolhas de estruturas:
    -O grupo tomou a decisão de usar listas ligadas simplesmente encadeadas como norte para o estruturamento dos dados. Também consideramos
     implementar listas circulares e/ou encadeadas com nó-cabeça, mas o início bem como o planejamento do projeto se deram em um momento
     anterior ao conhecimento mais aprofundado dessas estruturas. As listas são dinâmicas graças à natureza mutável das operações envolvidas
     no googlebot - vide inserção de novos sites em diferentes posições e remoção destes - e sua frequência num cenário realista.
     Listas duplamente encadeadas foram descartadas dado a ineficiência de operações como buscas e inserções ou remoções de elementos, já que 
     teria de haver muito manejamento de ponteiros e conexões. Já que tratamos de listas ordenadas por código, em que a maioria das atividades necessárias posteriormente à criação da lista a partir da leitura do CSV seria mais direta, a lista ligada simplesmente encadeada se mostrou simples e eficaz.
     É válido apontar que tratamos as funções envolvendo diretamente os sites e seus elementos e a lista com seus nós de forma mais abstrata como entidades com um certo grau de independência, inclusive em arquivos distintos de descrição. Isso facilita a legibilidade e garante elegância ao projeto graças à relação diferente de graus de abstração presente entre os sites (que usam por sua vez funções mais primitivas das listas) e das listas em si, tratadas como elementos abstratos como na definição.
     
* Justificativas para as escolhas dos métodos:

	A inserção e remoção de ítens foi feita  a partir de reassimilação de ponteiros auxiliares (através de funções iterativas),
de forma a não gerar necessidade de maiores acessos à memória em função de empilhamentos recursivos. Consideramos
que a inserção não é realizada de forma ordenada, adequando o programa a uma amplitude maior de experiências de usuário. Assim,
as operações de remoção e inserção foram planejadas desconsiderando a ordenação do vetor, tendo como parâmetros um código específico a ser atribuído a um site, ou ainda sua posição no primeiro ou último lugar da lista.
	A inserção de novos dados no CSV foi feita a partir da administração de mudanças em structs, forma mais maleável de lidar com os inputs do usuário, que sobrescrevem o arquivo original por completo à chamada da função de atualização da database. Isso permite maior flexibilidade de acesso e atualização dos dados originais sem gerar necessidade de navegação contínua do CSV.
	Já a busca foi baseada no algoritmo de merge sort readaptado para uma lista encadeada sem nó-cabeça. O merge sort e sua natureza nlogn se demonstrou especialmente apto para a ordenação de listas encadeadas, apesar da necessidade de uso substancial de memória auxiliar, que é compensado pelo processamento facilitado.

