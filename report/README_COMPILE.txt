========================================================================
MATSURI NIGHTS - ACADEMIC PROJECT REPORT COMPILATION INSTRUCTIONS
Course: CSE4102 - Computer Graphics and Image Processing Laboratory, KUET
Author: MD. Abu Hasanat Soykot (Roll No: 2107100, Group: B2)
========================================================================

PREREQUISITES:
- Any standard LaTeX distribution:
  * Windows: MiKTeX (recommended) or TeX Live
  * Linux: TeX Live (texlive-full or texlive-latex-extra)
  * macOS: MacTeX
- Python 3.x with matplotlib (for re-generating vector diagrams in report/figures/)

DIRECTORY STRUCTURE:
report/
|-- main.tex                     # Master LaTeX entry document
|-- README_COMPILE.txt           # This compilation guide
|-- figures/                     # 20 PNG screenshots + 6 PDF/PNG vector diagrams
|   |-- make_diagrams.py         # Diagram generation script
|   |-- fig_*.pdf / fig_*.png    # Vector PDF diagrams
|   |-- ss_*.png                 # 20 high-resolution viewport screenshots
|-- sections/                    # Modular chapter documents
    |-- 01_introduction.tex
    |-- 02_tech_stack.tex
    |-- 03_mathematics.tex
    |-- 04_geometry.tex
    |-- 05_scene_graph.tex
    |-- 06_lighting_shading.tex
    |-- 07_shadows.tex
    |-- 08_texturing.tex
    |-- 09_animations.tex
    |-- 10_ray_tracing.tex
    |-- 11_hud_interaction.tex
    |-- 12_testing_verification.tex
    |-- 13_controls_discrepancies.tex
    |-- 14_conclusion.tex

========================================================================
HOW TO COMPILE:
========================================================================

METHOD 1: Using pdflatex (Standard Command Line)
1. Open PowerShell, Command Prompt, or terminal and navigate to the report directory:
   cd path/to/Matsuri-Nights-A-Japanese-Festival-Street/report

2. Run pdflatex twice to resolve all cross-references, table of contents, and figure numbering:
   pdflatex main.tex
   pdflatex main.tex

3. The compiled document will be output as "main.pdf".

METHOD 2: Using latexmk (Automated Multi-Pass Build)
   latexmk -pdf main.tex

METHOD 3: Visual Studio Code / TeXstudio / TeXworks
- Open "report/main.tex".
- Ensure the build recipe is set to "pdflatex".
- Press Build / Compile (F5 in TeXstudio or Ctrl+Alt+B in VS Code LaTeX Workshop).

========================================================================
HOW TO RE-GENERATE FIGURES:
========================================================================
1. To regenerate the vector PDF and PNG diagrams:
   cd report/figures
   python make_diagrams.py

2. To regenerate all 20 viewport PNG screenshots from the OpenGL engine:
   cd "Matsuri Nights — A Japanese Festival Street"
   ".\x64\Debug\Matsuri Nights - A Japanese Festival Street.exe" --capture-report
========================================================================
