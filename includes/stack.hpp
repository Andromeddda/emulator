template <typename T>
class Stack {
	unsigned Length;
	unsigned Capacity;
	T* array;

	bool ok();

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

	void push(const T& value);
	void push(T&& value);
	void pop();
	T& top();

	bool operator== (const Stack& other);
};
