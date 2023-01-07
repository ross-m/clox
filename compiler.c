#include <stdio.h>
#include "common.h"
#include "compiler.h"
#include "scanner.h"
#include <stdlib.h>

typedef struct
{
	Token current;
	Token previous;
	bool hadError;
	bool panicMode;
} Parser;

Parser parser;

static void error(const char* message)
{
	errorAt(&parser.previous, message);
}

static void errorAt(Token* token, const char* message)
{
	if (parser.panicMode) return;
	fprintf(stderr, "[line %d] Error", token->line);

	if (token->type == TOKEN_EOF)
	{
		fprintf(stderr, " at end");
	} 
	else if (token->type == TOKEN_ERROR)
	{
		
	} 
	else 
	{
		fprintf(stderr, " at '%.*s'", token->length, token->start);
	}

	fprintf(stderr, ": %s\n", message);
	parser.hadError = true;
}

static void advance()
{
	parser.previous = parser.current;

	for (;;)
	{
		parser.current = scanToken();
		if (parser.current.type != TOKEN_ERROR) break;
	}

	errorAtCurrent(parser.current.start);
}

static void consume(TokenType type, const char* message)
{
	if (parser.current.type == type)
	{
		advance();
		return;
	}

	errorAtCurrent(message);
}

void compile(const char* source, Chunk* chunk)
{
	initScanner(source);
	parser.hadError = false;
	parser.panicMode = false;
	advance();
	expression();
	consume(TOKEN_EOF, "Expect end of expression.");
	return !parser.hadError;
}
