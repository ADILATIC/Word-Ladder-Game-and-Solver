# Word-Ladder-Game-and-Solver
This project is a C program that implements both an interactive Word Ladder game and a shortest-path Word Ladder solver. A word ladder is a sequence of words where each word differs from the previous by exactly one character.

Example:
cold → cord → card → ward → warm

The program allows the user to build a ladder manually or lets the solver compute the shortest possible ladder using a breadth-first search.

The program supports:

A fully interactive word ladder builder game

Random or user-selected start and final words

Validation of user-entered words, including:

Correct length

Must exist in dictionary

Must differ by exactly one character

A Word Ladder Solver that automatically finds the shortest ladder

Dictionary loading and filtering by word length

Binary search for word lookup

Linked list structures for ladders and BFS queueing

The program includes the following features:

WordNode: a linked list node storing one word in the ladder.

LadderNode: a linked list node storing an entire ladder for BFS solving.

Functions for:

Loading and filtering dictionary words

Binary search word lookup

Comparing word differences

Validating user input for the game

Building, copying, and freeing ladders

BFS solver to find the shortest ladder
