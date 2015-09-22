/*-------------------------------------------------------------------------
Compiler Generator Coco/R,
Copyright (c) 1990, 2004 Hanspeter Moessenboeck, University of Linz
extended by M. Loeberbauer & A. Woess, Univ. of Linz
ported to C++ by Csaba Balazs, University of Szeged
with improvements by Pat Terry, Rhodes University

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

As an exception, it is allowed to write an extension of Coco/R that is
used as a plugin in non-free software.

If not otherwise stated, any source code generated by Coco/R (other than
Coco/R itself) does not fall under the GNU General Public License.
-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------
  Trace output options
  0 | A: prints the states of the scanner automaton
  1 | F: prints the First and Follow sets of all nonterminals
  2 | G: prints the syntax graph of the productions
  3 | I: traces the computation of the First sets
  4 | J: prints the sets associated with ANYs and synchronisation sets
  6 | S: prints the symbol table (terminals, nonterminals, pragmas)
  7 | X: prints a cross reference list of all syntax symbols
  8 | P: prints statistics about the Coco run

  Trace output can be switched on by the pragma
    $ { digit | letter }
  in the attributed grammar or as a command-line option
  -------------------------------------------------------------------------*/


#include <stdio.h>
#include "Scanner.h"
#include "Parser.h"
#include "Tab.h"

using namespace Coco;

#ifdef _WIN32
int wmain(int argc, wchar_t *argv[]) {
#elif defined __GNUC__
int main(int argc, char *argv_[]) {
	wchar_t ** argv = new wchar_t*[argc];
	for (int i = 0; i < argc; ++i) {
		argv[i] = coco_string_create(argv_[i]);
	}
#else
#error unknown compiler!
#endif

	wprintf(L"Coco/R (Aug 03, 2011)\n");

	wchar_t *srcName = NULL, *nsName = NULL, *frameDir = NULL, *ddtString = NULL, *traceFileName = NULL;
	wchar_t *outDir = NULL;
	char *chTrFileName = NULL;
	bool emitLines = false;

	for (int i = 1; i < argc; i++) {
		if (coco_string_equal(argv[i], L"-namespace") && i < argc - 1) nsName = coco_string_create(argv[++i]);
		else if (coco_string_equal(argv[i], L"-frames") && i < argc - 1) frameDir = coco_string_create(argv[++i]);
		else if (coco_string_equal(argv[i], L"-trace") && i < argc - 1) ddtString = coco_string_create(argv[++i]);
		else if (coco_string_equal(argv[i], L"-o") && i < argc - 1) outDir = coco_string_create_append(argv[++i], L"/");
		else if (coco_string_equal(argv[i], L"-lines")) emitLines = true;
		else srcName = coco_string_create(argv[i]);
	}

#if defined __GNUC__
	for (int i = 0; i < argc; ++i) {
		coco_string_delete(argv[i]);
	}
	delete [] argv; argv = NULL;
#endif

	if (argc > 0 && srcName != NULL) {
		int pos = coco_string_lastindexof(srcName, '/');
		if (pos < 0) pos = coco_string_lastindexof(srcName, '\\');
		wchar_t* file = coco_string_create(srcName);
		wchar_t* srcDir = coco_string_create(srcName, 0, pos+1);

		Coco::Scanner *scanner = new Coco::Scanner(file);
		Coco::Parser  *parser  = new Coco::Parser(scanner);

		traceFileName = coco_string_create_append(srcDir, L"trace.txt");
		chTrFileName = coco_string_create_char(traceFileName);

		if ((parser->trace = fopen(chTrFileName, "w")) == NULL) {
			wprintf(L"-- could not open %ls\n", chTrFileName);
			exit(1);
		}

		parser->tab  = new Coco::Tab(parser);
		parser->dfa  = new Coco::DFA(parser);
		parser->pgen = new Coco::ParserGen(parser);

		parser->tab->srcName  = coco_string_create(srcName);
		parser->tab->srcDir   = coco_string_create(srcDir);
		parser->tab->nsName   = coco_string_create(nsName);
		parser->tab->frameDir = coco_string_create(frameDir);
		parser->tab->outDir   = coco_string_create(outDir != NULL ? outDir : srcDir);
		parser->tab->emitLines = emitLines;

		if (ddtString != NULL) parser->tab->SetDDT(ddtString);

		parser->Parse();

		fclose(parser->trace);

		// obtain the FileSize
		parser->trace = fopen(chTrFileName, "r");
		fseek(parser->trace, 0, SEEK_END);
		long fileSize = ftell(parser->trace);
		fclose(parser->trace);
		if (fileSize == 0)
			remove(chTrFileName);
		else
			wprintf(L"trace output is in %ls\n", chTrFileName);

		wprintf(L"%d errors detected\n", parser->errors->count);
		if (parser->errors->count != 0) {
			exit(1);
		}

		delete parser->pgen;
		delete parser->dfa;
		delete parser->tab;
		delete parser;
		delete scanner;
		coco_string_delete(file);
		coco_string_delete(srcDir);
	} else {
		wprintf(L"Usage: Coco Grammar.ATG {Option}\n");
		wprintf(L"Options:\n");
		wprintf(L"  -namespace <namespaceName>\n");
		wprintf(L"  -frames    <frameFilesDirectory>\n");
		wprintf(L"  -trace     <traceString>\n");
		wprintf(L"  -o         <outputDirectory>\n");
		wprintf(L"  -lines\n");
		wprintf(L"Valid characters in the trace string:\n");
		wprintf(L"  A  trace automaton\n");
		wprintf(L"  F  list first/follow sets\n");
		wprintf(L"  G  print syntax graph\n");
		wprintf(L"  I  trace computation of first sets\n");
		wprintf(L"  J  list ANY and SYNC sets\n");
		wprintf(L"  P  print statistics\n");
		wprintf(L"  S  list symbol table\n");
		wprintf(L"  X  list cross reference table\n");
		wprintf(L"Scanner.frame and Parser.frame files needed in ATG directory\n");
		wprintf(L"or in a directory specified in the -frames option.\n");
	}

	coco_string_delete(srcName);
	coco_string_delete(nsName);
	coco_string_delete(frameDir);
	coco_string_delete(ddtString);
	coco_string_delete(chTrFileName);
	coco_string_delete(traceFileName);

	return 0;
}
