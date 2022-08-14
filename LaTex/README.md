# LaTeX with MiKTeX compilier
**LaTeX** is a typesetting system that uses macro definitions, classes and packages written in TeX for document compilation and publication. In order to get a document from a LaTeX file with extension `.tex` you need a compiler.\
\
**MiKTeX** is such a TeX compiler. Once a system has the MiKTeX compiler installed on it, it can compile TeX projects and produce a `.pdf`.\
\
**Latexmk** is a **Perl** script built into MiKTeX. If you use cross-references, you will need to run the compiler more than once. If you use BibTeX for your bibliography or if you want to have a glossary you even need to run external programs in-between. Latexmk will manage all the cross-compiling automatically using MakeFiles, but your device will need a Perl environment to use Latexmk through MiKTeX.

## 1. Dependencies
Before setting up a text editor to produce a `.pdf` from a `.tex` the following software packages need to be installed on your system.

### Windows
1. [MiKTeX](https://miktex.org/howto/install-miktex/) : compiler
2. [Perl](https://strawberryperl.com/) : Perl Environment
3. [VS Code](https://code.visualstudio.com/download) : text editor

## 2. Setup

### Windows
1. Install the [Dependencies](#1-dependencies) above. Check your Environment Variables in windows and double check that the `Path` variable has a path to the MiKTeX folder with the executables. You may need to restart your device for the Path Variables to take effect.
2. Launch the **MiKTeX** console application and update (instructions for v4.7):
   - Click `Check for Updates`
   - Navigate to the `Updates` tab
   - Click `Update now` 
3. Launch **VS Code** and download the `LaTex Workshop` extension by navigating to `Extensions` on the left vertical bar, searching for `LaTex Workshop` and clicking `install`. You may need to restart VS Code.

## 3. Use

1. In **VS Code** open the project directory with the `.tex` file by clicking `File > Open Folder..` and navigating to the project folder.
2. In the file `explorer` pane that opens vertically to the left, choose the `.tex` file and open it in the text editor view by double-clicking. 
3. In the top right corner, click the green `play button` to compile the `tex` file, and then click the button next to it to open the `.pdf` associated with the TeX file. VS Code should automatically be set up to re-compile when you save with `CTRL + S` and the pdf view should be set to automatically grab the newly generated pdf compilation.