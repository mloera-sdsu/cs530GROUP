
# CANVAS INSTRUCTIONS (updated 3/29)

 1. All files are "text" files. For this project, please use the following filename extensions to help us understand the content of these files:
    * .sic -- SIC/XE assembly source
    * .lis -- SIC/XE listing file
    * .obj -- SIC/XE object file
    * .st -- SIC/XE ESTAB file

2. Output -- additional details explaining the project requirements; your project will generate SIC/XE object files, one for each control section and the ESTAB for the program. Each control section will be in its own assembly source / listing file and you will generate an object file for each listing file you are linking. Remember to hand "main" correctly, End records, and such. As such, this isn't a true linker which would combine all control sections into one object code file. This would actually be easy to do but not required for this project.

3.  Sample listing files -- several of you asked to have the text versions of the sample files provided. I will provide those here but please note, it's easy to accidentally change any content (delete/add a whitespace). Also, be careful if you move the file to other computers, some termio (terminal input/output controls) or programs alters the whitespace comments (replaces spaces with tabs, etc):
	>P2sampleAdder.lis
	>P2sampleWriter.lis


4. ESTAB format. Since this is an internal (run time) data structure, the number of spaces between fields isn't a hard requirement as long as there are enough for the symbol names (length) and the addresses. A suggested format could be like this:

	>Control Section (6 columns), 2 columns (spaces), Symbol Name (6 														columns), 2 columns (spaces), Address (6 columns), 2 columns (spaces), 			Length (6 columns).

    Figure 3.11 (a) on page 143 of our text has a good example.



5. SYMTAB. Don't confuse the SYMTAB with the ESTAB; I am not asking you to produce a SYMTAB but you will be doing some of that work to generate the Modification records. For expression evaluation, I will only be using expressions involving a local symbol and one outside of the control section. Or, two symbols outside of the control section. It is not hard to build a full expression evaluator, if you do that be sure to document that in your README.



6. You also asked for sample object files to correspond with these listing files:

	>P2sampleAdder.obj
	>P2sampleWriter.obj



	and to complete the set, the ESTAB: P2sampleESTAB.st


## ***This README will be replaced for the required one noted on the specs at end of project***