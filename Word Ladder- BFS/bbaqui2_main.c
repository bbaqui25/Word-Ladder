/*main.c*/

// Basic graph program to test Graph ADT.

//
// Word Ladders using Graphs
//
// Bushra Baqui
// Windows10
// Sublime Text using bert to test
// U. of Illinois, Chicago
// CS251, Fall 2016
// HW #12

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "graph.h"
#include "mymem.h"



int main()
{
	Graph *G;

	printf("** Starting Word Ladder App **\n\n");

	G = CreateGraph(1);

	InputFile(G, "merriam-webster-len4.txt");

	ProcessGraph(G);

	//
	// done:
	//
	printf("\n** Done **\n");
	mymem_stats();

	printf("\n");

	return 0;
}

