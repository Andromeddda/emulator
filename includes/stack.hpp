#ifndef HEADER_GUARD_STACK_HPP_INCLUDED
#define HEADER_GUARD_STACK_HPP_INCLUDED

#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <utils.hpp>

namespace stack_ns {
	
	template <typename T>
	class Stack {
		unsigned Length;
		unsigned Capacity;
		T* array;

		bool ok() const;
		void augment();
		void diminish();

	public:
		//////////////////////
		// The rule of five //
		//////////////////////

		Stack (); // Construct an empty stack with capacity = 2

		Stack (const Stack& s);	// Copy constructor
		Stack (Stack&& s);	// Move constructor

		Stack<T>& operator= (const Stack& s);	// Copy assignment
		Stack<T>& operator= (Stack&& s);	// Move assignment

		~Stack(); // Destructor (free the allocated memory)

		/////////////
		// Getters //
		/////////////

		unsigned size();
		unsigned capacity();

		/////////////
		// Methods //
		/////////////
		template <typename... Args>
		void emplace(Args&&...  args);

		void push(const T& value);
		void push(T&& value);
		void pop();
		T& top();
		T top() const;

	}; // class Stack

	///////////////////////
	// Memory management //
	///////////////////////
	template <typename T>
	void Stack<T>::augment() {
		VERIFY_CONTRACT(this->ok(), "ERROR: cannot allocate memory for invalid stack");

		Capacity *= 2;
		T* buffer;
		try {
			buffer = new T[Capacity];	
		}
		catch (const std::exception& exc) {
			TERMINATE("ERROR: unable to reallocate memory for push: " << exc.what());
		}
		
		for (unsigned i = 0; i < Length; i++) {
			buffer[i] = std::move(array[i]);
		}
		delete[] array;
		array = buffer;
		buffer = nullptr;
	}

	template <typename T>
	void Stack<T>::diminish() {
		VERIFY_CONTRACT(this->ok(), "ERROR: cannot allocate memory for invalid stack");

		Capacity /= 2;
		T* buffer;
		try {
			buffer = new T[Capacity];	
		}
		catch (const std::exception& exc) {
			TERMINATE("ERROR: unable to reallocate memory for push: " << exc.what());
		}
		
		for (unsigned i = 0; i < Length; i++) {
			buffer[i] = std::move(array[i]);
		}
		delete[] array;
		array = buffer;
		buffer = nullptr;
	}

	//////////////////////
	// The rule of five //
	//////////////////////

	// Check if stack is valid
	template <typename T>
	bool Stack<T>::ok() const {
		return (array != nullptr) && (Length <= Capacity) && (Capacity > 0U);
	}

	// Default constructor
	template <typename T>
	Stack<T>::Stack () : 
		Length(0), Capacity(2) 
	{
		VERIFY_CONTRACT(std::is_default_constructible_v<T>, "ERROR: the type cannot be default constructed");
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
		if (array != nullptr) delete[] array;
		array = nullptr;
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
		if (*this == s) return *this;

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
		if (*this == s) return *this;

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
			augment();
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
			augment();
		}

		array[Length] = std::move(value);
		++Length;
		VERIFY_CONTRACT(this->ok(), "ERROR: push failed, resulting stack is invalid");
	}

	// Constructing in-place
	template <typename T>
	template <typename... Args>
	void Stack<T>::emplace(Args&&... args) {
		VERIFY_CONTRACT(this->ok(), "ERROR: cannot emplace to invalid stack");
		// reallocate
		if (Length == Capacity) {
			augment();
		}
		array[Length] = T(std::forward<Args>(args)...);
	}

	// Pop
	template <typename T>
	void Stack<T>::pop() {
		VERIFY_CONTRACT(this->ok(), "ERROR: cannot pop from invalid stack");

		if (Length == 0U) return;

		//reallocate
		if (4*Length < Capacity) {
			diminish();
		}

		--Length;
		VERIFY_CONTRACT(this->ok(), "ERROR: pop failed, resulting stack is invalid");
	}

	// Top
	template <typename T>
	T& Stack<T>::top() {
		VERIFY_CONTRACT(this->ok(), "ERROR: cannot get top element from invalid stack");

		VERIFY_CONTRACT(Length > 0, "ERROR: cannot read top element of empty stack");

		return array[Length - 1];
	}

	// Top
	template <typename T>
	T Stack<T>::top() const {
		VERIFY_CONTRACT(this->ok(), "ERROR: cannot get top element from invalid stack");

		VERIFY_CONTRACT(Length > 0, "ERROR: cannot read top element of empty stack");

		return array[Length - 1];
	}

} // namespace stack_ns

#endif //HEADER_GUARD_STACK_HPP_INCLUDED
