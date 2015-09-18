\LoadClass[a4paper]{article}

\usepackage[left=2cm,right=2cm,top=2cm,bottom=2cm,headheight=15pt]{geometry}

\usepackage{pdfpages}
\AtBeginDocument{\includepdf[pages=-]{assignment.pdf}}

% Fancy headers and footers
\usepackage{fancyhdr}
\pagestyle{fancy}
\lhead{Algorithms Lab Solutions}
\rhead{\thepage}
\cfoot{Tom Sydney Kerckhove}
\renewcommand{\headrulewidth}{0.4pt}
\renewcommand{\footrulewidth}{0.4pt}

% Cooler section headers
\renewcommand\thesection{S \arabic{section}:}

\usepackage{minted}
% Inputting code
\newcommand\code[3]{
  \begin{figure}[H]
  \centering
  \begin{mdframed}
    \inputminted[firstline=#2, lastline=#3]{c++}{#1.cc}
  \end{mdframed}
  \end{figure}
}

% For frames
\usepackage{mdframed}

% Styles
\newenvironment{solutions}{}{}
\newenvironment{solution}[3]
{
  \section{#1}
  \subsection*{Time: $O\left(#2\right)$, Space: $O\left(#3\right)$}
}
{
}


