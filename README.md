## Require
* Visual Studio

## Structure project
* src - `source code`
* include - `header files from other libraries`
* lib - `required libraries`
* bin - `contains an executable file and the necessary dll files`

## Configure the project
1. Depois de criar um projeto do VS, mova a pasta do sdk do myo, para o diretório `"path:mymyo/mymyo/include"`.

2. Mouse botão direito click no MyoProjectpainel esquerdo e vá para propriedades.

3. In Configuration Properties > Directories VC ++

    * Include Directories: adicione o caminho do include/

    * clique no menu suspenso e clique em <Editar ...>. Uma caixa de diálogo é exibida. Clique na região branca na parte superior (uma linha deve destacar como azul). Clique no realce azul novamente e clique no botão `...` a direita

    * Library Directories: seguindo etapas semelhantes, como mostrado acima, inclua o `"path:mymyo/mymyo/lib"`

4. In Configuration Properties > C / C ++> General

    * Additional Include Directories: adicione o caminho ao include/novo. (Não tenho certeza se necessário, mas isso funciona para mim).

5. In Configuration Properties > Linker
    * > General
        * Additional Library Directories: Adicionar o caminho da lib `lib/`

    * > Input
        
        * Additional Dependencies: Adicionar o caminho `myo64.lib` or `myo32.lib` depende do seus sistema

6. Copie também `myo64.dll` ou `myo32.dll` para o local onde seu programa será executado. Por exemplo, `emDebug/`. Os dll estão no diretorio `"bin"`.

7. Última e muito importante etapa: vá para Build > Configuration Manager. Na coluna Plataforma, selecione <Novo ...> e escolha x64.

---