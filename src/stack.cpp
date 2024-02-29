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

///////////////////
// BASIC METHODS //
///////////////////

// Copy push
template <typename T>
void Stack<T>::push(const T& value) {
	VERIFY_CONTRACT(this->ok(), "ERROR: cannot push to invalid stack");

	// reallocate
	if (Length == Capacity) {
		Capacity *= 2;
		T* buffer;
		try {
			buffer = new T[Capacity];	
		}
		catch (const std::exception& exc) {
			TERMINATE("ERROR: unable to allocate memory for push: " << exc.what());
		}

		std::copy_n(array, buffer, Length);
		delete[] array;
		array = buffer;
		buffer = nullptr;
	}

	array[Length] = value;
	++Length;
	VERIFY_CONTRACT(this->ok(), "ERROR: push failed, resulting stack is invalid");
}

// Move push
template <typename T>
void Stack<T>::push(T&& value) {
	VERIFY_CONTRACT(this->ok(), "ERROR: cannot push to invalid stack");

	// reallocate
	if (Length == Capacity) {
		Capacity *= 2;
		T* buffer;
		try {
			buffer = new T[Capacity];	
		}
		catch (const std::exception& exc) {
			TERMINATE("ERROR: unable to reallocate memory for push: " << exc.what());
		}
		
		std::copy_n(array, buffer, Length);
		delete[] array;
		array = buffer;
		buffer = nullptr;
	}

	array[Length] = std::move(value);
	++Length;
	VERIFY_CONTRACT(this->ok(), "ERROR: push failed, resulting stack is invalid");
}

// Pop
template <typename T>
void Stack<T>::pop() {
	VERIFY_CONTRACT(this->ok(), "ERROR: cannot pop from invalid stack");

	if (Length == 0U) return;

	// reallocate
	if (4*Length < Capacity) {
		Capacity /= 2;
		T* buffer;
		try {
			buffer = new T[Capacity];	
		}
		catch (const std::exception& exc) {
			TERMINATE("ERROR: unable to reallocate memory for pop: " << exc.what());
		}

		std::copy_n(array, buffer, Length);
		delete[] array;
		array = buffer;
		buffer = nullptr;
	}

	--Length;
	VERIFY_CONTRACT(this->ok(), "ERROR: pop failed, resulting stack is invalid");
}

// Top
template <typename T>
T& Stack<T>::top() {
	VERIFY_CONTRACT(this->ok(), "ERROR: cannot get top element from invalid stack");

	return array[Length - 1];
}

// Comparison operator
bool Stack<T>::operator== (const Stack<T>& other) {
	VERIFY_CONTRACT(this->ok(), "ERROR: left side of comparison operator is invalid");
	VERIFY_CONTRACT(other.ok(), "ERROR: right side of comparison operator is invalid");

	if (Length != other.Length) return false;

	for (unsigned i = 0; i < Length; i++) {
		if (array[i] != other.array[i]) return false;
	}
	return true;
}
