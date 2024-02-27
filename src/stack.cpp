#include <iostream>
#include <cstdlib>
#include <algorithm>

#include "utils.hpp"
#include "stack.hpp"


//////////////////////
// The rule of five //
//////////////////////

// Check if stack is valid
template <typename T>
bool Stack<T>::ok() {
	return (array != nullptr) && (Length <= Capacity) && (Capacity > 0U);
}

// Default constructor
template <typename T>
Stack<T>::Stack () : 
	Length(0), Capacity(2) 
{
	array = new T[Capacity]; // Allocate memory
	VERIFY_CONTRACT(this->ok(), "ERROR: cannot construct default stack (probable memory allocation fault)");
}

// Copy constructor
template <typename T>
Stack<T>::Stack (const Stack<T>& s) : 
	Length(s.Length), Capacity(s.Capacity) 
{
	VERIFY_CONTRACT(s.ok(), "ERROR: cannot copy stack from invalid origin");
	array = new T[s.Capacity]; // Allocate memory
	VERIFY_CONTRACT(array != nullptr, "ERROR: cannot allocate memory for stack");

	std::copy_n(s.array, s.Length, array); // Copy all elements

	VERIFY_CONTRACT(this->ok(), "ERROR: cannot construct stack by copying");
}

// Move constructor
template <typename T>
Stack<T>::Stack (Stack<T>&& s) {
	VERIFY_CONTRACT(s.ok(), "ERROR: cannot move stack from invalid origin");
	Length = s.Length;
	Capacity = s.Capacity;
	array = s.array;

	s.array = nullptr;
	s.Length = 0;
	s.Capacity = 0;

	VERIFY_CONTRACT(this->ok(), "ERROR: cannot construct stack by moving")
}

// Destructor
template <typename T>
Stack<T>::~Stack () {
	delete[] array;
	Length = 0;
	Capacity = 0;
	VERIFY_CONTRACT(!this->ok(), "ERROR: cannot destruct stack");
}

// Copy assignment
template <typename T>
Stack<T>& Stack<T>::operator= (const Stack<T>& s) {
	VERIFY_CONTRACT(this->ok(), "ERROR: left operand of copy assignment is invalid");
	VERIFY_CONTRACT(s.ok(), "ERROR: right operand of copy assignment is invalid");

	// Handle self-assignment
	if (*this == s) {
		return *this;
	}

	// Delete previous data
	delete[] array;

	// Allocate new memory
	array = new T[s.Capacity];

	// Copy
	Length = s.Length;
	Capacity = s.Capacity;

	std::copy_n(s.array, s.Length, array); // Copy all elements

	VERIFY_CONTRACT(this->ok(), "ERROR: cannot copy stack from assignment (probable memory allocation fault)");

	return *this;
}

// Move assignment
template <typename T>
Stack<T>& Stack<T>::operator= (Stack<T>&& s) {
	VERIFY_CONTRACT(this->ok(), "ERROR: left operand of move assignment is invalid");
	VERIFY_CONTRACT(s.ok(), "ERROR: right operand of move assignment is invalid");

	// Handle self-assignment
	if (*this == s) {
		return *this;
	}

	// Delete previous data
	delete[] array;

	// Move
	array = s,array;
	Length = s.Length;
	Capacity = s.Capacity;

	// Clear origin
	s.array = nullptr;
	s.Length = 0;
	s.Capacity = 0;

	VERIFY_CONTRACT(this->ok(), "ERROR: cannot move stack from assignment");
	VERIFY_CONTRACT(!s.ok(), "ERROR: move assignment is not destructive for origin");

	return *this;
}


/////////////
// Getters //
/////////////
template <typename T>
unsigned Stack<T>::size() {
	VERIFY_CONTRACT(this->ok(), "ERROR: cannot get size because stack is invalid");
	return Length;
}

template <typename T>
unsigned Stack<T>::capacity() {
	VERIFY_CONTRACT(this->ok(), "ERROR: cannot get capacity because stack is invalid");
	return Capacity;
}

