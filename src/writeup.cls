\LoadClass[a4paper]{article}

\usepackage[left=2cm,right=2cm,top=2cm,bottom=2cm,headheight=15pt]{geometry}

\usepackage{pdfpages}
\AtBeginDocument{\includepdf[pages=-]{assignment.pdf}}
\AtEndDocument{\newpage\inputminted[fontsize=\scriptsize]{c++}{main.cc}}

% \nointent everywhere
\setlength\parindent{0pt}

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
\newenvironment{solutions}
{}
{

}
\newenvironment{solution}[3]
{
  \section{#1}
  \subsection*{Time: $O\left(#2\right)$, Space: $O\left(#3\right)$}
}
{
}


% For maths
\usepackage{amsmath}
\usepackage{amsfonts}
\usepackage{amssymb}
\usepackage{amsthm}


% For tags
\newcommand{\bx}[1]{ \framebox[1.1\width]{#1} }
\newcommand{\tagged}[1]{\bx{#1}}

% For intervals
\usepackage{interval}

% Don't load the monospace font, otherwise verbatim will be messed up.
\usepackage[mono=false]{libertine}
\usepackage[libertine]{newtxmath}
